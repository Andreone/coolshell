#include "stdafx.h"
#include "CoolShellLib\ScopeExitFunction.h"

BOOST_AUTO_TEST_SUITE(TestScopeExitFunction)

BOOST_AUTO_TEST_CASE(Test_that_the_set_function_is_called_when_out_of_scope)
{
    int i = 0;

    {
        ScopeExitFunction f([&] { i++; });
    }

    BOOST_CHECK_EQUAL(1, i);
}

BOOST_AUTO_TEST_CASE(Test_that_reset_with_a_new_function_will_call_the_previously_set_function_immediately)
{
    int i = 0;

    ScopeExitFunction f([&] { i++; });
    f.reset([&] { });
    BOOST_CHECK_EQUAL(1, i);
}

BOOST_AUTO_TEST_CASE(Test_that_release_clears_and_that_the_previously_set_function_is_not_called_when_out_of_scope)
{
    int i = 0;

    {
        ScopeExitFunction f([&] { i++; });
        f.release();
    }

    BOOST_CHECK_EQUAL(0, i);
}

BOOST_AUTO_TEST_SUITE_END( )