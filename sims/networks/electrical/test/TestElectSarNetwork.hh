#ifndef TestElectSarNetwork_EXISTS
#define TestElectSarNetwork_EXISTS

/**
@file  TestElectSarNetwork.hh
@brief TestElectSarNetwork GUNNS Basic Network declarations.

@copyright  Copyright 2019 United States Government as represented by the Administrator of the
            National Aeronautics and Space Administration. All Rights Reserved.

@defgroup  GUNNS_TESTELECTSARNETWORK TestElectSarNetwork GUNNS Basic Network
@ingroup  GUNNS

@details
PURPOSE: (Provides classes for the TestElectSarNetwork GUNNS Basic Network.)

REFERENCES:
  ()

ASSUMPTIONS AND LIMITATIONS:
  ()

LIBRARY DEPENDENCY:
  ((TestElectSarNetwork.o))

PROGRAMMERS:
  ((Auto-generated by the GunnsDraw netexport script version 19.2.0) (2021-04-12 05:59:15.124280))

@{
*/

#include "software/SimCompatibility/TsSimCompatibility.hh"
#include "math/approximation/TsLinearInterpolator.hh"
#include "core/network/GunnsNetworkBase.hh"
#include "core/GunnsSensorAnalogWrapper.hh"
#include "aspects/electrical/Batt/GunnsElectBattery.hh"
#include "aspects/electrical/ConstantPowerLoad/EpsConstantPowerLoad.hh"
#include "aspects/electrical/SolarArray/SolarArray.hh"
#include "aspects/electrical/sar/GunnsSolarArrayRegulator.hh"
#include "core/GunnsBasicCapacitor.hh"
#include "core/GunnsBasicConductor.hh"
#include "core/GunnsBasicPotential.hh"

// Forward-declare the main network class for use in the config data.
class TestElectSarNetwork;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    TestElectSarNetwork GUNNS Network Config Data
///
/// @details  Configuration data class for the TestElectSarNetwork Network.
////////////////////////////////////////////////////////////////////////////////////////////////////
class TestElectSarNetworkConfigData
{
    public:
        // Solver configuration data
        GunnsConfigData netSolver;    /**< (--) trick_chkpnt_io(**) Network solver config data. */ 
        // Spotters configuration data
        GunnsSensorAnalogWrapperConfigData sarSensorVout;    /**< (--) trick_chkpnt_io(**) sarSensorVout config data. */
        GunnsSensorAnalogWrapperConfigData sarSensorIout;    /**< (--) trick_chkpnt_io(**) sarSensorIout config data. */
        // Links configuration data
        EpsConstantPowerLoadConfigData cpl;    /**< (--) trick_chkpnt_io(**) cpl config data. */
        GunnsBasicConductorConfigData conductor;    /**< (--) trick_chkpnt_io(**) conductor config data. */
        GunnsBasicPotentialConfigData potential;    /**< (--) trick_chkpnt_io(**) potential config data. */
        GunnsBasicCapacitorConfigData capacitor;    /**< (--) trick_chkpnt_io(**) capacitor config data. */
        SolarArrayConfigData sa;    /**< (--) trick_chkpnt_io(**) sa config data. */
        GunnsSolarArrayRegulatorConfigData sar;    /**< (--) trick_chkpnt_io(**) sar config data. */
        GunnsElectBatteryConfigData battery;    /**< (--) trick_chkpnt_io(**) battery config data. */
        /// @brief  Default constructs this network configuration data.
        TestElectSarNetworkConfigData(const std::string& name, TestElectSarNetwork* network);
        /// @brief  Default destructs this network configuration data.
        virtual ~TestElectSarNetworkConfigData();

    private:
        /// @details  Copy constructor unavailable since declared private and not implemented.
        TestElectSarNetworkConfigData(const TestElectSarNetworkConfigData&);
        /// @details  Assignment operator unavailable since declared private and not implemented.
        TestElectSarNetworkConfigData& operator =(const TestElectSarNetworkConfigData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    TestElectSarNetwork GUNNS Network Input Data
///
/// @details  Input data class for the TestElectSarNetwork Network.
////////////////////////////////////////////////////////////////////////////////////////////////////
class TestElectSarNetworkInputData
{
    public:
        // Spotters input data
        GunnsSensorAnalogWrapperInputData sarSensorVout;    /**< (--) trick_chkpnt_io(**) sarSensorVout input data. */
        GunnsSensorAnalogWrapperInputData sarSensorIout;    /**< (--) trick_chkpnt_io(**) sarSensorIout input data. */
        // Links input data
        EpsConstantPowerLoadInputData cpl;    /**< (--) trick_chkpnt_io(**) cpl input data. */
        GunnsBasicConductorInputData conductor;    /**< (--) trick_chkpnt_io(**) conductor input data. */
        GunnsBasicPotentialInputData potential;    /**< (--) trick_chkpnt_io(**) potential input data. */
        GunnsBasicCapacitorInputData capacitor;    /**< (--) trick_chkpnt_io(**) capacitor input data. */
        SolarArrayInputData sa;    /**< (--) trick_chkpnt_io(**) sa input data. */
        GunnsSolarArrayRegulatorInputData sar;    /**< (--) trick_chkpnt_io(**) sar input data. */
        GunnsElectBatteryInputData battery;    /**< (--) trick_chkpnt_io(**) battery input data. */
        /// @brief  Default constructs this network input data.
        TestElectSarNetworkInputData(TestElectSarNetwork* network);
        /// @brief  Default destructs this network input data.
        virtual ~TestElectSarNetworkInputData();

    private:
        /// @details  Copy constructor unavailable since declared private and not implemented.
        TestElectSarNetworkInputData(const TestElectSarNetworkInputData&);
        /// @details  Assignment operator unavailable since declared private and not implemented.
        TestElectSarNetworkInputData& operator =(const TestElectSarNetworkInputData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    TestElectSarNetwork GUNNS Network
///
/// @details  Main class for the TestElectSarNetwork Network.
////////////////////////////////////////////////////////////////////////////////////////////////////
class TestElectSarNetwork : public GunnsNetworkBase
{
    TS_MAKE_SIM_COMPATIBLE(TestElectSarNetwork);
    public:
        /// @brief  Enumeration of the TestElectSarNetwork Network nodes.
        enum Nodes
        {
            Node0 = 0,    ///< Node 0
            Node1 = 1,    ///< Node 1
            GROUND = 2,    ///< Ground Node
            N_NODES = 3    ///< Number of nodes including Ground
        };
        // Network declarations
        GunnsBasicNode netNodes[TestElectSarNetwork::N_NODES];    /**< (--) Network nodes array. */
        TestElectSarNetworkConfigData netConfig;    /**< (--) trick_chkpnt_io(**) Network config data. */
        TestElectSarNetworkInputData netInput;    /**< (--) trick_chkpnt_io(**) Network input data. */
        // Data Tables
        static const double BattSocVocTableSOC[3];    /**< (--) BattSocVocTable SOC (x) axis. */
        static const double BattSocVocTableVOC[3];    /**< (--) BattSocVocTable VOC (y) axis. */
        TsLinearInterpolator BattSocVocTable;    /**< (--) BattSocVocTable linear interpolator. */
        // Spotters
        GunnsSensorAnalogWrapper sarSensorVout;    /**< (--) sarSensorVout instance. */
        GunnsSensorAnalogWrapper sarSensorIout;    /**< (--) sarSensorIout instance. */
        // Links
        EpsConstantPowerLoad cpl;    /**< (--) cpl instance. */
        GunnsBasicConductor conductor;    /**< (--) conductor instance. */
        GunnsBasicPotential potential;    /**< (--) potential instance. */
        GunnsBasicCapacitor capacitor;    /**< (--) capacitor instance. */
        SolarArray sa;    /**< (--) sa instance. */
        GunnsSolarArrayRegulator sar;    /**< (--) sar instance. */
        GunnsElectBattery battery;    /**< (--) battery instance. */
        /// @brief  Default constructs this network.
        TestElectSarNetwork(const std::string& name = "");
        /// @brief  Default destructs this network.
        virtual ~TestElectSarNetwork();
        /// @brief  Network nodes initialization task.
        virtual void initNodes(const std::string& name);
        /// @brief  Network links & spotters initialization task.
        virtual void initNetwork();
        /// @brief  Update network spotters before the solver solution.
        virtual void stepSpottersPre(const double timeStep);
        /// @brief  Update network spotters after the solver solution.
        virtual void stepSpottersPost(const double timeStep);

    private:
        /// @details  Copy constructor unavailable since declared private and not implemented.
        TestElectSarNetwork(const TestElectSarNetwork&);
        /// @details  Assignment operator unavailable since declared private and not implemented.
        TestElectSarNetwork& operator =(const TestElectSarNetwork&);
};

/// @}  

#endif
