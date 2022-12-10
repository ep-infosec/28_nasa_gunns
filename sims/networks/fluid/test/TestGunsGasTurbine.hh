#ifndef TestGunsGasTurbine_EXISTS
#define TestGunsGasTurbine_EXISTS

#include "core/Gunns.hh"
#include "software/SimCompatibility/TsSimCompatibility.hh"
#include "aspects/fluid/conductor/GunnsDriveShaftSpotter.hh"
#include "core/GunnsFluidPotential.hh"
#include "aspects/fluid/conductor/GunnsGasTurbine.hh"
#include "aspects/fluid/potential/GunnsGasFan.hh"
#include "aspects/fluid/source/GunnsFluidHeater.hh"

/**
@file
@brief TestGunsGasTurbine GUNNS Network declarations.

@defgroup  ISS_ORCH_FLUID_TESTGUNSGASTURBINE TestGunsGasTurbine GUNNS Network
@ingroup   ISS_ORCH_FLUID

 PURPOSE: (Provides a manager class for the TestGunsGasTurbine GUNNS Network.)
@details
 REFERENCE:
- ()

 ASSUMPTIONS AND LIMITATIONS:
- ()

 LIBRARY DEPENDENCY:
- (
   (TestGunsGasTurbine.o)
  )

 PROGRAMMERS:
- (
    (AutoGenerated by GunnShow version 14.5.0 autogeneration template.)    (9/29/2016)
  )

@{
*/

////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief   Data structure for TestGunsGasTurbine GUNNS network mass fractions.
////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct TestGunsGasTurbineFractions {
    double GUNNS_N2;
} TestGunsGasTurbineFractions;

/////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Union for TestGunsGasTurbine GUNNS network mass fractions as scalars and as an array.
///           The array is used as an argument in the PolyFluidInputData constructor. 
/////////////////////////////////////////////////////////////////////////////////////////////////
union TestGunsGasTurbineMassFractions {
    TestGunsGasTurbineFractions scalar;
    double array[sizeof(TestGunsGasTurbineFractions)/sizeof(double)];
    TestGunsGasTurbineMassFractions(
                const double iGUNNS_N2);
};

// Forward-declare the main network class for use in the config data
class TestGunsGasTurbine;

/////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    TestGunsGasTurbine GUNNS Network Config Data 
///
/// @details  GUNNS Manager configuration data class for the TestGunsGasTurbine Network. 
/////////////////////////////////////////////////////////////////////////////////////////////////
class TestGunsGasTurbineConfigData
{
    public:
	    /// @brief   Enumeration of the number of internal constituent fluids in the TestGunsGasTurbine Network.
        enum {N_INTERNAL_FLUIDS = 1};
        // Network fluid configurations
        static FluidProperties::FluidType netInternalFluidTypes[TestGunsGasTurbineConfigData::N_INTERNAL_FLUIDS]; /**< (--) trick_chkpnt_io(**) Network internal fluid types array */
        // Solver configuration data
        GunnsConfigData                   netSolver;        /**< (--) trick_chkpnt_io(**) Network solver config data */ 
        // Spotter configuration data
        GunnsDriveShaftSpotterConfigData    DriveShaft; /**< (--)  DriveShaft config data */
        // Links configuration data
        GunnsFluidPotentialConfigData    SourcePotential; /**< (--) trick_chkpnt_io(**) SourcePotential  link config data */
        GunnsFluidPotentialConfigData    ExitPotential; /**< (--) trick_chkpnt_io(**) ExitPotential  link config data */
        GunnsGasTurbineConfigData    Turbine1; /**< (--) trick_chkpnt_io(**) Turbine1  link config data */
        GunnsGasFanConfigData    Fan1; /**< (--) trick_chkpnt_io(**) Fan1  link config data */
        GunnsFluidHeaterConfigData    Heater; /**< (--) trick_chkpnt_io(**) Heater  link config data */
        GunnsGasFanConfigData    LoadFan; /**< (--) trick_chkpnt_io(**) LoadFan  link config data */

        /// @brief  Default constructs this network configuration data.
        TestGunsGasTurbineConfigData(const std::string& name, TestGunsGasTurbine* network);
        /// @brief  Default destructs this network configuration data.
        virtual ~TestGunsGasTurbineConfigData();
    private:
        bool stubBool; /**< (--) trick_chkpnt_io(**) Added to simplify gunnshow auto code generation logic */
        /// @details  Copy constructor unavailable since declared private and not implemented.
        TestGunsGasTurbineConfigData(const TestGunsGasTurbineConfigData& rhs);
        /// @details  Assignment operator unavailable since declared private and not implemented.
        TestGunsGasTurbineConfigData& operator =(const TestGunsGasTurbineConfigData& rhs);
};



/////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    TestGunsGasTurbine GUNNS Network Input Data 
///
/// @details  GUNNS Manager input data class for the TestGunsGasTurbine Network. 
/////////////////////////////////////////////////////////////////////////////////////////////////
class TestGunsGasTurbineInputData
{
	public:
        // Internal network fluids
        TestGunsGasTurbineMassFractions  DefaultFluidStateFractions; /**< (--) trick_chkpnt_io(**) Fluid mass fractions */
        PolyFluidInputData                DefaultFluidState; /**< (--) trick_chkpnt_io(**) Fluid input data */
        // Spotter input data
       GunnsDriveShaftSpotterInputData    DriveShaft; /**< (--)  DriveShaft input data */
        // Links input data
        GunnsFluidPotentialInputData    SourcePotential; /**< (--)  trick_chkpnt_io(**) SourcePotential  link input data */
        GunnsFluidPotentialInputData    ExitPotential; /**< (--)  trick_chkpnt_io(**) ExitPotential  link input data */
        GunnsGasTurbineInputData    Turbine1; /**< (--)  trick_chkpnt_io(**) Turbine1  link input data */
        GunnsGasFanInputData    Fan1; /**< (--)  trick_chkpnt_io(**) Fan1  link input data */
        GunnsFluidHeaterInputData    Heater; /**< (--)  trick_chkpnt_io(**) Heater  link input data */
        GunnsGasFanInputData    LoadFan; /**< (--)  trick_chkpnt_io(**) LoadFan  link input data */
        /// @brief  Default constructs this network input data.
        TestGunsGasTurbineInputData(TestGunsGasTurbine* network);
        /// @brief  Default destructs this network input data.
        virtual ~TestGunsGasTurbineInputData();
    private:
        bool stubBool; /**< (--) trick_chkpnt_io(**) Added to simplify gunnshow auto code generation logic */
        /// @details  Copy constructor unavailable since declared private and not implemented.
        TestGunsGasTurbineInputData(const TestGunsGasTurbineInputData& rhs);
        /// @details  Assignment operator unavailable since declared private and not implemented.
        TestGunsGasTurbineInputData& operator =(const TestGunsGasTurbineInputData& rhs);
};


/////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    TestGunsGasTurbine Network
///
/// @details  GUNNS Manager class for the TestGunsGasTurbine Network. 
/////////////////////////////////////////////////////////////////////////////////////////////////
class TestGunsGasTurbine
{
        TS_MAKE_SIM_COMPATIBLE(TestGunsGasTurbine);
	public:
        /// @brief   Enumeration of the TestGunsGasTurbine Network nodes.
        enum Nodes
        {
        Node0 = 0,
        Node1 = 1,
        Node2 = 2,
        Node3 = 3,
        VACUUM = 4,
        N_NODES = 5
        };

        /// @brief    Default constructs this network.
        TestGunsGasTurbine(const std::string& name = "");
        /// @brief    Default destructs this network.
        virtual ~TestGunsGasTurbine();
        /// @brief    Network update task.
        virtual void update(const double timeStep);
        /// @brief    Network initialization task.
        virtual void initialize(const std::string& name = "");
        /// @brief    Network restart task.
        virtual void restart();

        ///    TestGunsGasTurbine Network declarations.
        Gunns                             netSolver;                                           /**<    (--) Network solver object */
        GunnsFluidNode                    netNodes[TestGunsGasTurbine::N_NODES]; /**<    (--) Network nodes array */
        GunnsNodeList                     netNodeList;                                         /**<    (--) Network node list structure */
        std::vector<GunnsBasicLink*>      netLinks;                                            /**<    (--) trick_chkpnt_io(**) Network links vector */
        TestGunsGasTurbineConfigData          netConfig;                         /**<    (--) trick_chkpnt_io(**) Network config data */
        TestGunsGasTurbineInputData           netInput;                          /**<    (--) trick_chkpnt_io(**) Network input data */
        const DefinedFluidProperties      netFluidProperties;                                  /**<    (--) trick_chkpnt_io(**) Network defined fluid properties */
        PolyFluidConfigData               netInternalFluidConfig;                              /**<    (--) trick_chkpnt_io(**) Network internal fluid config */
        // Network Spotters
        GunnsDriveShaftSpotter    DriveShaft; /**< (--)  DriveShaft instance */
	    // Network links
        GunnsFluidPotential SourcePotential; /**< (--) SourcePotential link instance */
        GunnsFluidPotential ExitPotential; /**< (--) ExitPotential link instance */
        GunnsGasTurbine Turbine1; /**< (--) Turbine1 link instance */
        GunnsGasFan Fan1; /**< (--) Fan1 link instance */
        GunnsFluidHeater Heater; /**< (--) Heater link instance */
        GunnsGasFan LoadFan; /**< (--) LoadFan link instance */
    private:
        bool stubBool; /**<  (--) trick_chkpnt_io(**) Added to simplify gunnshow auto code generation logic. */
        /// @details  Copy constructor unavailable since declared private and not implemented.
        TestGunsGasTurbine(const TestGunsGasTurbine& rhs);
        /// @details  Assignment operator unavailable since declared private and not implemented.
        TestGunsGasTurbine& operator =(const TestGunsGasTurbine& rhs);
};

/// @}  

////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param   iGUNNS_N2   (--) Mass fraction of the GUNNS_N2 constituent in the composite fluid
///
/// @details  Constructs the mass fraction union from the scalar mass fraction arguments.
////////////////////////////////////////////////////////////////////////////////////////////////////////
inline TestGunsGasTurbineMassFractions::TestGunsGasTurbineMassFractions(
            const double iGUNNS_N2)
{
    scalar.GUNNS_N2 = iGUNNS_N2;
}

#endif