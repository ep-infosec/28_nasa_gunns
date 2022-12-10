#ifndef UtGunnsElectUserLoadSwitch_EXISTS
#define UtGunnsElectUserLoadSwitch_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_SWITCH_USER_LOAD   Electrical User Load Switch Unit Tests
/// @ingroup  UT_GUNNS
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the GUNNS Electrical User Load Switch link model.
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "aspects/electrical/Switch/GunnsElectUserLoadSwitch.hh"
#include "aspects/electrical/UserLoad/GunnsElectResistiveUserLoad.hh"
#include "aspects/electrical/UserLoad/GunnsElectConstantPowerUserLoad.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsElectUserLoadSwitch and befriend UtGunnsElectUserLoadSwitch.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsElectUserLoadSwitch : public GunnsElectUserLoadSwitch
{
    public:
        FriendlyGunnsElectUserLoadSwitch();
        virtual ~FriendlyGunnsElectUserLoadSwitch();
        friend class UtGunnsElectUserLoadSwitch;
};
inline FriendlyGunnsElectUserLoadSwitch::FriendlyGunnsElectUserLoadSwitch() : GunnsElectUserLoadSwitch() {};
inline FriendlyGunnsElectUserLoadSwitch::~FriendlyGunnsElectUserLoadSwitch() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Valve unit tests.
////
/// @details  This class provides the unit tests for the GUNNS Electrical User Load Switch link
///           model within the CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsElectUserLoadSwitch: public CppUnit::TestFixture
{
    public:
        /// @brief    Default constructs this Check Valve unit test.
        UtGunnsElectUserLoadSwitch();
        /// @brief    Default constructs this Check Valve unit test.
        virtual ~UtGunnsElectUserLoadSwitch();
        /// @brief    Executes before each test.
        void setUp();
        /// @brief    Executes after each test.
        void tearDown();
        /// @brief    Tests config data.
        void testConfig();
        /// @brief    Tests input data.
        void testInput();
        /// @brief    Tests default construction.
        void testDefaultConstruction();
        /// @brief    Tests initialize method with user loads.
        void testNominalInitialization();
        /// @brief    Tests accessor methods.
        void testAccessors();
        /// @brief    Tests step method with user loads.
        void testStep();
        /// @brief    Tests step method without user loads.
        void testStepNoLoads();
        /// @brief    Tests step method without user loads, connected to Ground node.
        void testStepNoLoadsIsolated();
        /// @brief    Tests loads override in the step method.
        void testStepLoadsOverride();
        /// @brief    Tests computeFlows method for correct power value outputs.
        void testComputeFlows();
        /// @brief    Tests loads override in computeFlows with user loads.
        void testComputeFlowsOverrideLoads();
        /// @brief    Tests loads override in computeFlows to a non-Ground node.
        void testComputeFlowsOverrideNonGround();
        /// @brief    Tests dynamic port mapping and restrictions.
        void testPortRules();
        /// @brief    Tests minor steps, switch trips, and solution confirmation.
        void testTripLogic();
        /// @brief    Tests restart method.
        void testRestart();
        /// @brief    Tests initialize method exceptions.
        void testInitializationExceptions();

    private:
        CPPUNIT_TEST_SUITE(UtGunnsElectUserLoadSwitch);
        CPPUNIT_TEST(testConfig);
        CPPUNIT_TEST(testInput);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testNominalInitialization);
        CPPUNIT_TEST(testAccessors);
        CPPUNIT_TEST(testStep);
        CPPUNIT_TEST(testStepNoLoads);
        CPPUNIT_TEST(testStepNoLoadsIsolated);
        CPPUNIT_TEST(testStepLoadsOverride);
        CPPUNIT_TEST(testComputeFlows);
        CPPUNIT_TEST(testComputeFlowsOverrideLoads);
        CPPUNIT_TEST(testComputeFlowsOverrideNonGround);
        CPPUNIT_TEST(testPortRules);
        CPPUNIT_TEST(testTripLogic);
        CPPUNIT_TEST(testRestart);
        CPPUNIT_TEST(testInitializationExceptions);
        CPPUNIT_TEST_SUITE_END();
        ///  @brief   Enumeration for the number of nodes.
        enum {N_NODES = 3};
        std::vector<GunnsBasicLink*>              tLinks;              /**< (--)    Link vector. */
        std::string                               tName;               /**< (--)    Nominal name. */
        GunnsBasicNode                            tNodes[N_NODES];     /**< (--)    Nominal connected nodes. */
        GunnsNodeList                             tNodeList;           /**< (--)    Network node structure. */
        int                                       tPorts[2];           /**< (--)    Nominal ports assignment. */
        double                                    tSwitchResistance;   /**< (ohm)   Nominal config data. */
        int                                       tSwitchTripPriority; /**< (--)    Nominal config data. */
        double                                    tLoadsPowerRefV;     /**< (--)    Nominal config data. */
        GunnsElectUserLoadSwitchConfigData*       tConfigData;         /**< (--)    Pointer to nominal configuration data. */
        bool                                      tMalfBlockageFlag;   /**< (--)    Blockage malfunction flag. */
        double                                    tMalfBlockageValue;  /**< (--)    Blockage malfunction value. */
        bool                                      tSwitchIsClosed;     /**< (--)    Nominal input data. */
        double                                    tSwitchPosTripLimit; /**< (amp)   Nominal input data. */
        double                                    tSwitchNegTripLimit; /**< (amp)   Nominal input data. */
        bool                                      tLoadsOverrideActive; /**< (--)   Nominal input data. */
        double                                    tLoadsOverrideVoltage; /**< (V)   Nominal input data. */
        GunnsElectUserLoadSwitchInputData*        tInputData;          /**< (--)    Pointer to nominal input data. */
        FriendlyGunnsElectUserLoadSwitch*         tArticle;            /**< (--)    Pointer to the article under test. */
        GunnsElectResistiveUserLoadConfigData     tLoadRConfigData;    /**< (--)    A resistive user load config data. */
        GunnsElectResistiveUserLoadInputData      tLoadRInputData;     /**< (--)    A resistive user load input data. */
        GunnsElectResistiveUserLoad               tLoadR;              /**< (--)    A resistive user load. */
        GunnsElectResistiveUserLoadConfigData     tLoadR2ConfigData;   /**< (--)    A resistive user load config data. */
        GunnsElectResistiveUserLoadInputData      tLoadR2InputData;    /**< (--)    A resistive user load input data. */
        GunnsElectResistiveUserLoad               tLoadR2;             /**< (--)    A resistive user load. */
        GunnsElectConstantPowerUserLoadConfigData tLoadCpConfigData;   /**< (--)    A constant-power user load config data. */
        GunnsElectConstantPowerUserLoadInputData  tLoadCpInputData;    /**< (--)    A constant-power user load input data. */
        GunnsElectConstantPowerUserLoad           tLoadCp;             /**< (--)    A constant-power user load. */
        static int                                TEST_ID;             /**< (--)    Test identification number. */
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsElectUserLoadSwitch(const UtGunnsElectUserLoadSwitch&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsElectUserLoadSwitch& operator =(const UtGunnsElectUserLoadSwitch&);
};

///@}

#endif
