#define BOOST_AUTO_TEST_MAIN

#include <boost/test/unit_test_suite.hpp>
#include <boost/test/unit_test_log.hpp>
#include <boost/test/results_reporter.hpp>

#include "vld.h"

struct InitTests
{
    InitTests()
    {
        // all passed test names are printed to the output
        // boost::unit_test::unit_test_log.set_threshold_level(boost::unit_test::log_test_units);
        
        // a very small report is printed to the output (passed tests count / total tests count)
        boost::unit_test::results_reporter::set_level(boost::unit_test::SHORT_REPORT);
    }
};

BOOST_GLOBAL_FIXTURE( InitTests );