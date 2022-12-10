////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup  UT_GUNNS_ELECT_POWERBUS   Power Bus Unit tests
/// @ingroup   UT_GUNNS_ELECT
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
/// @details   Unit test class for Power Bus Elect.
////////////////////////////////////////////////////////////////////////////////////////////////////


#include <cppunit/ui/text/TestRunner.h>
#include "UtPowerBusElect.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param    argc  int     --  not used
/// @param    argv  char**  --  not used
///
/// @return    --  status (always 0)
///
/// @details  Main for unit tests in the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char** argv) {
    CppUnit::TextTestRunner runner;

    runner.addTest(UtPowerBusElect::suite());

    runner.run();

    return 0;
}

/// @}
