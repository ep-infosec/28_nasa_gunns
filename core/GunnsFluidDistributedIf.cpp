/**
@file
@brief    GUNNS Fluid Distributed Interface Link implementation

@copyright Copyright 2022 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
   (
    (GunnsFluidCapacitor.o)
   )
*/

#include "GunnsFluidDistributedIf.hh"
#include "core/GunnsFluidUtils.hh"
#include "core/GunnsFluidCapacitor.hh"
#include "math/MsMath.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "software/exceptions/TsOutOfBoundsException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this Fluid Distributed Interface data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidDistributedIfData::GunnsFluidDistributedIfData()
    :
    mFrameCount(0),
    mFrameLoopback(0),
    mDemandMode(false),
    mCapacitance(0.0),
    mSource(0.0),
    mEnergy(0.0),
    mMoleFractions(0),
    mTcMoleFractions(0),
    mNumFluid(0),
    mNumTc(0),
    mNumFluidIf(0),
    mNumTcIf(0),
    mNumFluidCommon(0),
    mNumTcCommon(0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Fluid Distributed Interface data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidDistributedIfData::~GunnsFluidDistributedIfData()
{
    TS_DELETE_ARRAY(mTcMoleFractions);
    TS_DELETE_ARRAY(mMoleFractions);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  that  (--)  Object to be copied.
///
/// @details  Assigns values of this object's attributes to the given object's values.  The
///           mNumFluid and similar terms are not changed, and we assume that the two objects were
///           initialized identically; the array sizes and mNum* terms are the same.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidDistributedIfData& GunnsFluidDistributedIfData::operator =(const GunnsFluidDistributedIfData& that)
{
    if (this != &that) {
        mFrameCount    = that.mFrameCount;
        mFrameLoopback = that.mFrameLoopback;
        mDemandMode    = that.mDemandMode;
        mCapacitance   = that.mCapacitance;
        mSource        = that.mSource;
        mEnergy        = that.mEnergy;
        for (unsigned int i=0; i<mNumFluid; ++i) {
            mMoleFractions[i] = that.mMoleFractions[i];
        }
        for (unsigned int i=0; i<mNumTc; ++i) {
            mTcMoleFractions[i] = that.mTcMoleFractions[i];
        }
    }
    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] name               (--) Name of the instance for dynamic memory names for Trick MM.
/// @param[in] nFluids            (--) Number of fluid constituents.
/// @param[in] nTc                (--) Number of trace compounds.
/// @param[in] fluidSizesOverride (--) Override the interface fluid mixture sizes.
/// @param[in] nIfFluids          (--) Number of primary fluid compounds override value.
/// @param[in] nIfTc              (--) Number of trace compounds override value.
///
/// @details  Allocates dynamic arrays for mass and mole fractions.  By default, the mixture array
///           sizes will match the sizes in the GUNNS fluid network.  However, if the fluid sizes
///           override flag is set then the mixture array sizes are set to the given override sizes.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidDistributedIfData::initialize(const std::string& name,
                                             const unsigned int nFluids,
                                             const unsigned int nTc,
                                             const bool         fluidSizesOverride,
                                             const unsigned int nIfFluids,
                                             const unsigned int nIfTc)
{
    mNumFluid = nFluids;
    mNumTc    = nTc;
    if (fluidSizesOverride) {
        mNumFluidIf = nIfFluids;
        mNumTcIf    = nIfTc;
    } else {
        mNumFluidIf = mNumFluid;
        mNumTcIf    = mNumTc;
    }
    mNumFluidCommon = std::min(mNumFluid, mNumFluidIf);
    mNumTcCommon    = std::min(mNumTc,    mNumTcIf);
    if (mNumFluidIf > 0) {
        TS_DELETE_ARRAY(mMoleFractions);
        TS_NEW_PRIM_ARRAY_EXT(mMoleFractions, mNumFluidIf, double, name + ".mMoleFractions");
        for (unsigned int i=0; i<mNumFluidIf; ++i) {
            mMoleFractions[i] = 0.0;
        }
    }
    if (mNumTcIf > 0) {
        TS_DELETE_ARRAY(mTcMoleFractions);
        TS_NEW_PRIM_ARRAY_EXT(mTcMoleFractions, mNumTcIf, double, name + ".mTcMoleFractions");
        for (unsigned int i=0; i<mNumTcIf; ++i) {
            mTcMoleFractions[i] = 0.0;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  (--)  True if all data validation checks passed.
///
/// @details  Checks for all of the following conditions to be met:  Frame count > 0, energy > 0,
///           capacitance >= 0, pressure >= 0 (only in Supply mode), and all mixture fractions >= 0.
////////////////////////////////////////////////////////////////////////////////////////////////////
bool GunnsFluidDistributedIfData::hasValidData()
{
    if (mFrameCount < 1 or mEnergy <= 0.0 or mCapacitance < 0.0 or (mSource < 0.0 and not mDemandMode)) {
        return false;
    }
    for (unsigned int i=0; i<mNumFluid; ++i) {
        if (mMoleFractions[i] < 0.0) {
            return false;
        }
    }
    for (unsigned int i=0; i<mNumTc; ++i) {
        if (mTcMoleFractions[i] < 0.0) {
            return false;
        }
    }
    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] fractions (--) Array of bulk fluid mole fraction values to copy to the interface.
///
/// @details  Sets this interface's bulk fluid mole fractions to the given values.  If the
///           interface array is larger than the given array, then the remaining values in the
///           interface array are filled with zeroes.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidDistributedIfData::setMoleFractions(const double* fractions)
{
    for (unsigned int i=0; i<mNumFluidCommon; ++i) {
        mMoleFractions[i] = fractions[i];
    }
    for (unsigned int i=mNumFluidCommon; i<mNumFluidIf; ++i) {
        mMoleFractions[i] = 0.0;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] fractions (--) Array of trace compound mole fractions to copy to the interface.
///
/// @details  Sets this interface's trace compound mole fractions to the given values.  If the
///           interface array is larger than the given array, then the remaining values in the
///           interface array are filled with zeroes.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidDistributedIfData::setTcMoleFractions(const double* fractions)
{
    for (unsigned int i=0; i<mNumTcCommon; ++i) {
        mTcMoleFractions[i] = fractions[i];
    }
    for (unsigned int i=mNumTcCommon; i<mNumTcIf; ++i) {
        mTcMoleFractions[i] = 0.0;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[out] fractions (--) Array of bulk fluid mole fractions to copy from the interface.
///
/// @details  Sets the given builk fluid mole fractions to this interface's values.  If the given
///           array is larger than the interface array, then the remaining values in the given array
///           are filled with zeroes.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidDistributedIfData::getMoleFractions(double* fractions)
{
    for (unsigned int i=0; i<mNumFluidCommon; ++i) {
        fractions[i] = mMoleFractions[i];
    }
    for (unsigned int i=mNumFluidCommon; i<mNumFluid; ++i) {
        fractions[i] = 0.0;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[out] fractions (--) Array of trace compound mole fractions to copy from the interface.
///
/// @details  Sets the given builk trace compound fractions to this interface's values.  If the
///           given array is larger than the interface array, then the remaining values in the given
///           array are filled with zeroes.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidDistributedIfData::getTcMoleFractions(double* fractions)
{
    for (unsigned int i=0; i<mNumTcCommon; ++i) {
        fractions[i] = mTcMoleFractions[i];
    }
    for (unsigned int i=mNumTcCommon; i<mNumTc; ++i) {
        fractions[i] = 0.0;
    }
}

/// @details  This value is chosen to get reliable network capacitance calculations from the solver
///           for liquid and gas nodes.
const double GunnsFluidDistributedIf::mNetworkCapacitanceFlux = 1.0E-6;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] name          (--)  Link name.
/// @param[in] nodes         (--)  Network nodes array.
/// @param[in] isPairMaster  (--)  This is the master of the pair.
/// @param[in] useEnthalpy   (--)  Transport energy as specific enthalpy instead of temperature.
/// @param[in] demandOption  (--)  Demand mode option to trade stability for less restriction on flow rate.
/// @param[in] capacitorLink (--)  Pointer to the node capacitor link.
///
/// @details  Default GUNNS Fluid Distributed Interface Link Config Data Constructor.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidDistributedIfConfigData::GunnsFluidDistributedIfConfigData(
        const std::string&   name,
        GunnsNodeList*       nodes,
        const bool           isPairMaster,
        const bool           useEnthalpy,
        const bool           demandOption,
        GunnsFluidCapacitor* capacitorLink)
    :
    GunnsFluidLinkConfigData(name, nodes),
    mIsPairMaster(isPairMaster),
    mUseEnthalpy(useEnthalpy),
    mDemandOption(demandOption),
    mCapacitorLink(capacitorLink),
    mModingCapacitanceRatio(1.25),
    mDemandFilterConstA(1.5),
    mDemandFilterConstB(0.75),
    mFluidSizesOverride(false),
    mNumFluidOverride(0),
    mNumTcOverride(0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default GUNNS Fluid Distributed Interface Link Config Data Destructor.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidDistributedIfConfigData::~GunnsFluidDistributedIfConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] nFluids  (--)  Number of primary fluid compounds override value.
/// @param[in] nTc      (--)  Number of trace compounds override value.
///
/// @details  Sets the fluid mixture sizes override flag true, and sets the primary and trace fluid
///           compound mixture sizes to the given values.  This is used to force the interface
///           fluid mixture arrays to different sizes then the sizes in the GUNNS fluid network.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidDistributedIfConfigData::overrideInterfaceMixtureSizes(const unsigned int nFluids,
                                                                      const unsigned int nTc)
{
    mFluidSizesOverride = true;
    mNumFluidOverride   = nFluids;
    mNumTcOverride      = nTc;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] malfBlockageFlag   (--)  Blockage malfunction flag.
/// @param[in] malfBlockageValue  (--)  Blockage malfunction fractional value (0-1).
/// @param[in] forceDemandMode    (--)  Forces the link to always be in Demand mode.
/// @param[in] forceSupplyMode    (--)  Forces the link to always be in Supply mode.
///
/// @details  Default GUNNS Fluid Distributed Interface Link Input Data Constructor.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidDistributedIfInputData::GunnsFluidDistributedIfInputData(
        const bool   malfBlockageFlag,
        const double malfBlockageValue,
        const bool   forceDemandMode,
        const bool   forceSupplyMode)
    :
    GunnsFluidLinkInputData(malfBlockageFlag, malfBlockageValue),
    mForceDemandMode(forceDemandMode),
    mForceSupplyMode(forceSupplyMode)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default GUNNS Fluid Distributed Interface Link Input Data Destructor
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidDistributedIfInputData::~GunnsFluidDistributedIfInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default GUNNS Fluid Distributed Interface Link Constructor.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidDistributedIf::GunnsFluidDistributedIf()
    :
    GunnsFluidLink(NPORTS),
    mInData                (),
    mOutData               (),
    mIsPairMaster          (false),
    mUseEnthalpy           (false),
    mDemandOption          (false),
    mCapacitorLink         (0),
    mModingCapacitanceRatio(0.0),
    mDemandFilterConstA    (0.0),
    mDemandFilterConstB    (0.0),
    mForceDemandMode       (false),
    mForceSupplyMode       (false),
    mInDataLastDemandMode  (false),
    mFramesSinceFlip       (0),
    mSupplyVolume          (0.0),
    mEffectiveConductivity (0.0),
    mSourcePressure        (0.0),
    mDemandFlux            (0.0),
    mLoopLatency           (0),
    mDemandFluxGain        (0.0),
    mSuppliedCapacitance   (0.0),
    mTempMassFractions     (0),
    mTempMoleFractions     (0),
    mTempTcMoleFractions   (0),
    mOtherIfs              (),
    mFluidState            ()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default GUNNS Fluid Distributed Interface Link Destructor.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidDistributedIf::~GunnsFluidDistributedIf()
{
    {
        delete [] mTempTcMoleFractions;
        mTempTcMoleFractions = 0;
        delete [] mTempMoleFractions;
        mTempMoleFractions = 0;
        delete [] mTempMassFractions;
        mTempMassFractions = 0;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]     configData   (--) Reference to link Config Data.
/// @param[in]     inputData    (--) Reference to link Input Data.
/// @param[in,out] networkLinks (--) Network links vector.
/// @param[in]     port0        (--) Network port 0.
///
/// @throws   TsInitializationException
///
/// @details  Initializes this Fluid Distributed Interface link with configuration and input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidDistributedIf::initialize(const GunnsFluidDistributedIfConfigData& configData,
                                         const GunnsFluidDistributedIfInputData&  inputData,
                                         std::vector<GunnsBasicLink*>&            networkLinks,
                                         const int                                port0)
{
    /// - Initialize & validate parent.
    int ports[1] = {port0};
    GunnsFluidLink::initialize(configData, inputData, networkLinks, ports);

    /// - Reset init flag
    mInitFlag = false;

    /// - Initialize from config data.
    mIsPairMaster           = configData.mIsPairMaster;
    mUseEnthalpy            = configData.mUseEnthalpy;
    mDemandOption           = configData.mDemandOption;
    mCapacitorLink          = configData.mCapacitorLink;
    mModingCapacitanceRatio = configData.mModingCapacitanceRatio;
    mDemandFilterConstA     = configData.mDemandFilterConstA;
    mDemandFilterConstB     = configData.mDemandFilterConstB;

    /// - Initialize from input data.
    mForceDemandMode = inputData.mForceDemandMode;
    mForceSupplyMode = inputData.mForceSupplyMode;

    /// - Create the internal link fluid, allocate and load the fluid config map to translate the
    ///   external network's fluid to match our local network's config.
    createInternalFluid();

    /// - Both sides start out in Supply mode by default.
    mOutData.mDemandMode = false;

    /// - Allocate memory and build the temporary mass and mole fractions arrays.  We allocate
    ///   persistent arrays now to save allocation time during run.
    const unsigned int nTypes = mNodes[0]->getFluidConfig()->mNTypes;
    delete [] mTempTcMoleFractions;
    delete [] mTempMoleFractions;
    delete [] mTempMassFractions;
    mTempMassFractions = new double[nTypes];
    mTempMoleFractions = new double[nTypes];
    for (unsigned int i = 0; i < nTypes; ++i) {
        mTempMassFractions[i] = 0.0;
        mTempMoleFractions[i] = 0.0;
    }
    const GunnsFluidTraceCompoundsConfigData* tcConfig = mNodes[0]->getFluidConfig()->mTraceCompounds;
    unsigned int nTc = 0;
    if (tcConfig) {
        nTc = tcConfig->mNTypes;
        if (nTc > 0) {
            mTempTcMoleFractions = new double[nTc];
            for (unsigned int i = 0; i < nTc; ++i) {
                mTempTcMoleFractions[i] = 0.0;
            }
        }
    }

    /// - Initialize the interface data objects so they can allocate memory.
    mInData .initialize(mName + ".mInData",  nTypes, nTc, configData.mFluidSizesOverride, configData.mNumFluidOverride, configData.mNumTcOverride);
    mOutData.initialize(mName + ".mOutData", nTypes, nTc, configData.mFluidSizesOverride, configData.mNumFluidOverride, configData.mNumTcOverride);

    /// - Initialize remaining state variables.
    mSupplyVolume          = 0.0;
    mEffectiveConductivity = 0.0;
    mSourcePressure        = 0.0;
    mDemandFlux            = 0.0;
    mLoopLatency           = 0;
    mDemandFluxGain        = 1.0;
    mSuppliedCapacitance   = 0.0;

    try {
        mFluidState.initializeName(mName + ".mFluidState");
        double fractions[FluidProperties::NO_FLUID] = {1.0};
        // Use default values to satisfy fluid initialize, then set to node contents.
        PolyFluidInputData initFluidState(294.261,        // temperature
                                          101.325,        // pressure
                                          0.0,            // flowrate
                                          0.0,            // mass
                                          fractions);     // massFraction
        mFluidState.initialize(*mNodes[0]->getFluidConfig(), initFluidState);
        mFluidState.setState(mNodes[0]->getContent());
    } catch (...) {
        GUNNS_ERROR(TsInitializationException, "Invalid Initialization Data",
                "Caught exception from mFluidState initialization.");
    }

    /// - Validate initialization.
    validate();

    /// - Set init flag on successful validation.
    mInitFlag = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @throws   TsInitializationException
///
/// @details  Validates this GUNNS Fluid Distributed Interface initial state.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidDistributedIf::validate() const
{
    /// - Throw on null pointer to the node capacitor link.
    if (not mCapacitorLink) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Missing pointer to the node capacitor link.");
    }

    /// - Throw on invalid moding capacitance ratio range.
    if (mModingCapacitanceRatio <= 1.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "moding capacitance ration <= 1.");
    }

    /// - Throw if conflicting mode force flags.
    if (mForceDemandMode and mForceSupplyMode) {
        GUNNS_ERROR(TsInitializationException, "Invalid Input Data",
                    "both mode force flags are set.");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  otherIf  (--)  Address of another GunnsFluidDistributedIf link to store.
///
/// @details  Pushes the given GunnsFluidDistributedIf link pointer onto the mOtherIfs vector.
///           Duplicate object, including this, are quietly ignored.  This is a convenience to allow
///           the same list of links to be passed to all links in a network from the input file.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidDistributedIf::addOtherIf(GunnsFluidDistributedIf* otherIf)
{
    bool duplicate = (otherIf == this);
    for (unsigned int i=0; i<mOtherIfs.size(); ++i) {
        if (otherIf == mOtherIfs[i]) {
            duplicate = true;
        }
    }
    if (not duplicate) {
        mOtherIfs.push_back(otherIf);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Derived classes should call their base class implementation too.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidDistributedIf::restartModel()
{
    /// - Reset the base class.
    GunnsFluidLink::restartModel();

    /// - Reset non-config & non-checkpointed class attributes.
    mEffectiveConductivity = 0.0;
    mSourcePressure        = 0.0;
    mDemandFlux            = 0.0;
    mLoopLatency           = 0;
    mDemandFluxGain        = 1.0;
    mSuppliedCapacitance   = 0.0;
    for (int i = 0; i < mNodes[0]->getFluidConfig()->mNTypes; ++i) {
        mTempMassFractions[i] = 0.0;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Pre-network step calculations.  Processes the incoming data from the external
///           interface, flips modes and updates frame counters.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidDistributedIf::processInputs()
{
    /// - Mode changes and associated node volume update in response to incoming data.
    flipModesOnInput();

    /// - More processing of incoming data for resulting pairing mode.
    processInputsDemand();
    processInputsSupply();

    /// - Update frame counters and loop latency measurement.
    mOutData.mFrameCount   += 1;
    mLoopLatency            = mOutData.mFrameCount - mInData.mFrameLoopback;
    mOutData.mFrameLoopback = mInData.mFrameCount;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      pressure  (kPa)  Pressure to set the fluid to.
/// @param[in,out]  fluid     (--)   Pointer to the PolyFluid object to be set.
///
/// @returns  double (--) Sum of input bulk compound mole fractions, <= 1.
///
/// @details  Copies the incoming fluid state from the other side of the interface (mInData) into
///           the given fluid object and sets it to the given pressure.
////////////////////////////////////////////////////////////////////////////////////////////////////
double GunnsFluidDistributedIf::inputFluid(const double pressure, PolyFluid* fluid)
{
    /// - Normalize the incoming bulk mole fractions to sum to 1.  Internally, GUNNS sums the bulk
    ///   mole fractions to 1, and this doesn't include the trace compounds.  But the interface
    ///   data includes the TC's in the sum to 1.  Adjustment to the TC's is handled below.
    double inBulkFractionSum = 0.0;
    mInData.getMoleFractions(mTempMoleFractions);
    const PolyFluidConfigData* fluidConfig = mNodes[0]->getFluidConfig();
    for (int i = 0; i < fluidConfig->mNTypes; ++i) {
        inBulkFractionSum += mTempMoleFractions[i];
    }
    if (inBulkFractionSum < DBL_EPSILON) {
        GUNNS_ERROR(TsOutOfBoundsException, "Invalid Interface Data",
                    "incoming bulk mole fractions sum to zero.");
    }
    for (int i = 0; i < fluidConfig->mNTypes; ++i) {
        mTempMoleFractions[i] /= inBulkFractionSum;
    }

    /// - Convert incoming mole fractions to mass fractions.
    GunnsFluidUtils::convertMoleFractionToMassFraction(mTempMassFractions,
                                                       mTempMoleFractions,
                                                       mNodes[0]->getFluidConfig());

    fluid->setMassAndMassFractions(0.0, mTempMassFractions);
    fluid->setPressure(pressure);

    if (mUseEnthalpy) {
        fluid->setTemperature(fluid->computeTemperature(mInData.mEnergy));
    } else {
        fluid->setTemperature(mInData.mEnergy);
    }

    if (mInData.mTcMoleFractions) {
        GunnsFluidTraceCompounds* tc = fluid->getTraceCompounds();
        if (tc) {
            /// - As above, adjust the TC mole fractions to be relative to the internal GUNNS bulk
            ///   fluid.
            const GunnsFluidTraceCompoundsConfigData* tcConfig = tc->getConfig();
            if (tcConfig) {
                mInData.getTcMoleFractions(mTempTcMoleFractions);
                for (int i = 0; i < tcConfig->mNTypes; ++i) {
                    mTempTcMoleFractions[i] /= inBulkFractionSum;
                }
                tc->setMoleFractions(mTempTcMoleFractions);
            }
        }
    }
    return inBulkFractionSum;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Processes inputs from the other side of the interface when in supply mode.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidDistributedIf::processInputsSupply()
{
    /// - When we are Supply mode but have not yet received Demand data from the other side, we set
    ///   the demand flux to zero.
    /// - When in Demand mode, zero the demand flux.
    /// - When in Supply mode, zero the source pressure.
    mDemandFlux = 0.0;
    if (not mOutData.mDemandMode) {
        mSourcePressure = 0.0;
        if (mInData.hasValidData() and mInData.mDemandMode) {
            /// - Convert (mol/s) to (kmol/s), and external mole rate to internal GUNNS rate.  The
            ///   internal GUNNS rate does not include the mole rate of the trace compounds.  The
            ///   inputFluid function returns the fraction of the bulk fluid compunds in the total,
            ///   which is our adjustment.
            mDemandFlux = -mInData.mSource * UnitConversion::KILO_PER_UNIT
                        * inputFluid(1.0, mInternalFluid);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Processes inputs from the other side of the interface when in demand mode.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidDistributedIf::processInputsDemand()
{
    if (mOutData.mDemandMode) {
        if (mInData.hasValidData() and not mInData.mDemandMode) {
            /// - Convert (Pa) to (kPa).
            mSourcePressure = mInData.mSource * UnitConversion::KILO_PER_UNIT;
            inputFluid(mSourcePressure, mNodes[0]->getContent());
            mFluidState.setState(mNodes[0]->getContent());
        } else {
            /// - When we are in Demand mode but have not yet received Supply data from the other
            ///   side, we hold the node at its initial state.
            mSourcePressure = mNodes[0]->getPotential();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Handles mode flips in response to incoming data, and the initial mode flip at run
///           start.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidDistributedIf::flipModesOnInput()
{
    /// - Force mode swap based on the mode force flags.
    if (mForceDemandMode and not mOutData.mDemandMode) {
        flipToDemandMode();
    } else if (mForceSupplyMode and mOutData.mDemandMode) {
        flipToSupplyMode();
    } else if (mInData.hasValidData()) {
        /// - If in demand mode and the incoming data is also demand, then the other side has
        ///   initialized the demand/supply swap, so we flip to supply.
        if (mOutData.mDemandMode and mInData.mDemandMode and not mInDataLastDemandMode) {
            flipToSupplyMode();
        } else if (not mInData.mDemandMode and not mOutData.mDemandMode) {
            if ( (mOutData.mCapacitance < mInData.mCapacitance) or
                    (mIsPairMaster and mOutData.mCapacitance == mInData.mCapacitance) ) {
                /// - If in supply mode and the incoming data is also supply, then this is the start
                ///   of the run and the side with the smaller capacitance switches to demand mode,
                ///   and the master side is the tie-breaker.
                flipToDemandMode();
            }
        }
        mInDataLastDemandMode = mInData.mDemandMode;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Flips from supply to demand mode whenever the supply network capacitance drops below
///           some fraction of the demand side's capacitance.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidDistributedIf::flipModesOnCapacitance()
{
    /// - We do not check until we've been in supply mode for at least one full lag cycle.  This
    ///   prevents unwanted extra mode flips during large transients.
    if (mFramesSinceFlip > mLoopLatency and
            mOutData.mCapacitance * mModingCapacitanceRatio < mInData.mCapacitance) {
        flipToDemandMode();
        /// - Zero the output pressure/flow source term so the other side doesn't interpret our old
        ///   pressure value as a demand flux.  This will be set to a demand flux on the next full
        ///   pass in demand mode.
        mOutData.mSource = 0.0;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Processes flipping to Demand mode.  Since in Demand mode the node must have no
///           capacitance on its own, we zero it with the node's capacitor link's edit controls, and
///           save the volume value for restoration later.  This way the interfacing volume follows
///           the supply side during the flip.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidDistributedIf::flipToDemandMode()
{
    if (not mForceSupplyMode) {
        mOutData.mDemandMode = true;
        mSupplyVolume = mNodes[0]->getVolume();
        mCapacitorLink->editVolume(true, 0.0);
        mFramesSinceFlip = 0;
        GUNNS_INFO("switched to Demand mode.")
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Processes flipping to Supply mode.  Since in Demand mode the node's volume is zeroed,
///           we restore the node's original volume when entering Supply mode via the node's
///           capacitor link's edit controls.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidDistributedIf::flipToSupplyMode()
{
    if (not mForceDemandMode) {
        mOutData.mDemandMode = false;
        mCapacitorLink->editVolume(true, mSupplyVolume);
        mSupplyVolume = 0.0;
        mFramesSinceFlip = 0;
        GUNNS_INFO("switched to Supply mode.")
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  End-of-network calculations.  Sets outputs data based on our current mode.  Calls to
///           check if its time to flip to Demand node from Supply mode based on relative
///           capacitance, and updates the count of frames since the last mode flip.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidDistributedIf::processOutputs()
{
    if (mOutData.mDemandMode) {
        processOutputsDemand();
    } else {
        processOutputsSupply();
        flipModesOnCapacitance();
    }
    mFramesSinceFlip++;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  fluid  (--)  Fluid state to be output to the other side of the interface.
///
/// @returns  double (--)  Sum of all bulk and trace compound mole fractions, >= 1.
///
/// @details  Copies the given fluid state for output to the other side of the interface.
////////////////////////////////////////////////////////////////////////////////////////////////////
double GunnsFluidDistributedIf::outputFluid(PolyFluid* fluid)
{
    /// - Output energy as either temperature or specific enthalpy as configured.
    if (mUseEnthalpy) {
        mOutData.mEnergy = fluid->getSpecificEnthalpy();
    } else {
        mOutData.mEnergy = fluid->getTemperature();
    }

    /// - Convert outgoing mass fractions to mole fractions.
    const PolyFluidConfigData* fluidConfig = mNodes[0]->getFluidConfig();
    for (int i = 0; i < fluidConfig->mNTypes; ++i) {
        mTempMassFractions[i] = fluid->getMassFraction(i);
    }
    GunnsFluidUtils::convertMassFractionToMoleFraction(mTempMoleFractions,
                                                       mTempMassFractions,
                                                       fluidConfig);

    /// - Copy the trace compounds and sum their mole fractions for normalizing below.
    double moleFractionSum = 0.0;
    const GunnsFluidTraceCompounds* tc = fluid->getTraceCompounds();
    unsigned int nTc = 0;
    if (tc) {
        const GunnsFluidTraceCompoundsConfigData* tcConfig = tc->getConfig();
        if (tcConfig) {
            nTc = tcConfig->mNTypes;
        }
        for (unsigned int i = 0; i < nTc; ++i) {
            moleFractionSum += tc->getMoleFractions()[i];
        }
    }

    /// - Add bulk fluid mole fractions to the sum for normalizing.
    for (int i = 0; i < fluidConfig->mNTypes; ++i) {
        moleFractionSum += mTempMoleFractions[i];
    }

    /// - Normalize the bulk and trace compounds mole fractions so they all sum to 1.  Unlike
    ///   GUNNS fluids, where only the bulk fractions sum to 1 and TC's are tracked elsewhere,
    ///   this interface requires to the total sum of bulk + TC's to 1.
    for (int i = 0; i < fluidConfig->mNTypes; ++i) {
        mTempMoleFractions[i] /= moleFractionSum;
    }
    for (unsigned int i = 0; i < nTc; ++i) {
        mTempTcMoleFractions[i] = tc->getMoleFractions()[i] / moleFractionSum;
    }
    mOutData.setMoleFractions(mTempMoleFractions);
    mOutData.setTcMoleFractions(mTempTcMoleFractions);
    return moleFractionSum;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  End-of-network calculation of outputs to the other side of the interface when this
///           side is in Supply mode.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidDistributedIf::processOutputsSupply()
{
    outputCapacitance();

    /// - Convert (kPa) to (Pa).
    mOutData.mSource = mNodes[0]->getPotential() * UnitConversion::UNIT_PER_KILO;
    outputFluid(mNodes[0]->getContent());
    mFluidState.setState(mNodes[0]->getContent());
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  End-of-network calculation of outputs to the other side of the interface when this
///           side is in Demand mode.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidDistributedIf::processOutputsDemand()
{
    outputCapacitance();

    /// - If there is no inflow to the node then its inflow fluid has a reset state so we can't use
    ///   it.  Instead, use the node's contents.  Convert (kmol/s) to (mol/s).  Adjust mole flow
    ///   rate (mFlux only includes bulk compounds) to also include the trace compounds for total
    ///   flow rate to/from the interface.  The outputFluid function returns this scale factor.  We
    ///   also fall back to the node's contents if the node inflow fluid has any negative fluid
    ///   mixture fractions.
    PolyFluid* useFluid = mNodes[0]->getInflow();
    if (useFluid->getTemperature() > 0.0) {
        if (checkNegativeFluidFractions(useFluid)) {
            GUNNS_WARNING("demand node inflow has negative mixture fractions.");
            useFluid = mNodes[0]->getContent();
        }
    } else {
        useFluid = mNodes[0]->getContent();
    }
    mOutData.mSource = mFlux * UnitConversion::UNIT_PER_KILO * outputFluid(useFluid);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Calculates and sets the outgoing capacitance value.  This is the network capacitance
///           of the node, minus the effective capacitance added by this link in Demand mode (the
///           mSuppliedCapacitance), and minus the effective capacitance at our node added by other
///           links in Demand mode.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidDistributedIf::outputCapacitance()
{
    /// - Subtract the capacitance we supply in Demand mode.
    double capacitance = mNodes[0]->getNetworkCapacitance() - mSuppliedCapacitance;

    /// - For each other interface link that we know about, calculate and subtract its effective
    ///   capacitance at our node.  This is the capacitance that it supplied to its own node, 
    ///   reduced at our node by the resistances and capacitances in the network between the nodes.
    ///   The reduction is the increase in the other node potential over the increase in our node
    ///   potential when a flow source added at our node; these are intermediate values output by
    ///   the solver to the nodes in its network capacitance calculation.
    for (unsigned int i=0; i<mOtherIfs.size(); ++i) {
        const double otherSuppliedCap = mOtherIfs[i]->getSuppliedCapacitance();
        if (otherSuppliedCap > DBL_EPSILON) {    // they are in Demand mode
            const double* ourNetCapDp = getNetCapDeltaPotential();
            const double  otherDp     = ourNetCapDp[mOtherIfs[i]->getNodeMap()[0]];
            if (otherDp > DBL_EPSILON) {         // they affect us thru the conductive network
                const double ourDp = ourNetCapDp[mNodeMap[0]];
                const double ratio = otherDp / std::max(ourDp, DBL_EPSILON);
                capacitance       -= (otherSuppliedCap * ratio);
            }
        }
    }

    /// - Limit the outgoing capacitance to positive values, just in case something goes wrong
    ///   in our calculation.
    mOutData.mCapacitance = std::max(0.0, capacitance);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  dt  (s)  Integration time step.
///
/// @details  Calculates this link's contributions to the network system of equations.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidDistributedIf::step(const double dt)
{
    /// - In Demand mode, conductance mirrors the Supply network capacitance: G = C/dt.  In Supply
    ///   mode, zero conductance blocks the Demand mode potential source effect.
    if (mOutData.mDemandMode and dt > DBL_EPSILON) {
        /// - Comparison to FLT_EPSILON avoids chatter caused by mSuppliedCapacitance not exactly
        ///   equaling network capacitance.
        if (mOutData.mCapacitance > FLT_EPSILON and mInData.mCapacitance > FLT_EPSILON) {
            /// - In Demand mode, update demand flux gain as a function of Cs/Cd.  For Cs/Cd < 1,
            ///   lower gain based on latency.  For > 1, approach gain of 1.
            const double csOverCd  = MsMath::limitRange(1.0, mInData.mCapacitance / mOutData.mCapacitance, mModingCapacitanceRatio);
            const int    exponent  = MsMath::limitRange(1, mLoopLatency, 100);
            const double gainLimit = std::min(1.0, mDemandFilterConstA * powf(mDemandFilterConstB, exponent));
            mDemandFluxGain = gainLimit + (1.0 - gainLimit) * (csOverCd - 1.0) * 4.0;
            const double conductance = mDemandFluxGain * mInData.mCapacitance / dt;
            /// - The default for this option = false follows the interface design standard, but our
            ///   GUNNS implementation sometimes restricts the resulting flow rate too much.  Use
            ///   this option = true to relax the stability in favor of increased flow rate.  You
            ///   can safely use this when Cs/Cd >> 1 and for small lags <= 4.
            if (mDemandOption) {
                mEffectiveConductivity = conductance;
            } else {
                mEffectiveConductivity = 1.0 / std::max(1.0/conductance + dt/mOutData.mCapacitance, DBL_EPSILON);
            }
        } else {
            mDemandFluxGain = 1.0;
            mEffectiveConductivity = mDemandFluxGain * mInData.mCapacitance / dt;
        }
        /// - Reduce the effective conductance from the blockage malfunction.
        if (mMalfBlockageFlag) {
            mEffectiveConductivity *= (1.0 - mMalfBlockageValue);
        }
    } else {
        mEffectiveConductivity = 0.0;
    }

    /// - Build admittance matrix.
    const double systemConductance = MsMath::limitRange(0.0, mEffectiveConductivity, mConductanceLimit);
    if (fabs(mAdmittanceMatrix[0] - systemConductance) > 0.0) {
        mAdmittanceMatrix[0] = systemConductance;
        mAdmittanceUpdate    = true;
    }

    if (mOutData.mDemandMode) {
        mSuppliedCapacitance = mAdmittanceMatrix[0] * dt;
    } else {
        mSuppliedCapacitance = 0.0;
    }

    /// - Build source vector, including the potential source effect in Demand mode, and the flow
    ///   source effect to the demand side in Supply mode.
    mSourceVector[0] = mSourcePressure * mAdmittanceMatrix[0] + mDemandFlux;

    /// - Flag the node to have its network capacitance calculated by the solver.
    mNodes[0]->setNetworkCapacitanceRequest(mNetworkCapacitanceFlux);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  dt  (s)  Integration time step (unused).
///
/// @details  Computes the flux across the link, sets port flow directions and schedules outflux
///           from the source node.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidDistributedIf::computeFlows(const double dt __attribute__((unused)))
{
    mPotentialDrop = -mPotentialVector[0];
    computeFlux();

    /// - Set port flow directions and schedule flow from source nodes.
    if (mFlux > DBL_EPSILON) {
        mPortDirections[0] = SINK;
    } else if (mFlux < -DBL_EPSILON) {
        if (mOutData.mDemandMode) {
            mPortDirections[0] = SOURCE;
            mNodes[0]->scheduleOutflux(-mFlux);
        } else {
            mPortDirections[0] = SINK;
        }
    } else {
        mPortDirections[0] = NONE;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt (s) Integration time step (unused).
///
/// @details  Updates mass flow rate and transports fluid from or to the node.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidDistributedIf::transportFlows(const double dt __attribute__((unused)))
{
    /// - Calculate mass flow rate (mFlowRate) from molar rate (mFlux).
    if (mOutData.mDemandMode) {
        /// - In Demand mode, we use the node's MW because the node's fluid contents have already
        ///   taken the properties of the Supply fluid (from mInData).  This is true for both flow
        ///   directions for the fluid transport to/from the node.  However for negative flow (out
        ///   of the node to Ground), this is not the actual mass flow rate to the supply network -
        ///   that is handled in processOutputs.
        mFlowRate = mFlux * mNodes[0]->getContent()->getMWeight();
    } else {
        /// - In Supply mode, we use the MW of the Demand network fluid (from mInternalFluid, which
        ///   came from mInData).
        mFlowRate = mFlux * mInternalFluid->getMWeight();
    }

    /// - Transport fluid:
    if (mOutData.mDemandMode) {
        if (mFlowRate > m100EpsilonLimit) {
            mNodes[0]->collectInflux(mFlowRate, mNodes[0]->getContent());
        } else if (mFlowRate < -m100EpsilonLimit) {
            mNodes[0]->collectOutflux(-mFlowRate);
        }
    } else if (fabs(mFlowRate) > m100EpsilonLimit) {
        mNodes[0]->collectInflux(mFlowRate, mInternalFluid);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] port (--) The port to be assigned.
/// @param[in] node (--) The desired node to assign the port to.
///
/// @returns  bool  (--) Returns true if all rules checks pass
///
/// @details  Checks the requested port & node arguments for validity against rules that apply to
///           this specific class.  These are:
///           - A GunnsFluidDistributedIf must not map port 0 to the network vacuum node.
////////////////////////////////////////////////////////////////////////////////////////////////////
bool GunnsFluidDistributedIf::checkSpecificPortRules(const int port, const int node) const
{
    bool result = true;

    /// - Fail if port 0 is the vacuum node.
    if ((0 == port) && (node == getGroundNodeIndex())) {
        GUNNS_WARNING("aborted setting a port: cannot assign port 0 to the boundary node.");
        result = false;
    }
    return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] fluid (--) Pointer to the fluid object to check.
///
/// @returns  bool (--)  True if any fractions are negative.
///
/// @details  Checks all the bulk and trace compound mixture fractions in the given fluid for any
///           negative values.
////////////////////////////////////////////////////////////////////////////////////////////////////
bool GunnsFluidDistributedIf::checkNegativeFluidFractions(const PolyFluid* fluid) const
{
    const PolyFluidConfigData* fluidConfig = mNodes[0]->getFluidConfig();
    for (int i = 0; i < fluidConfig->mNTypes; ++i) {
        if (fluid->getMoleFraction(i) < 0.0) {
            return true;
        }
    }

    const GunnsFluidTraceCompounds* tc = fluid->getTraceCompounds();
    if (tc) {
        const GunnsFluidTraceCompoundsConfigData* tcConfig = tc->getConfig();
        for (int i = 0; i < tcConfig->mNTypes; ++i) {
            if (tc->getMoleFractions()[i] < 0.0) {
                return true;
            }
        }
    }

    return false;
}
