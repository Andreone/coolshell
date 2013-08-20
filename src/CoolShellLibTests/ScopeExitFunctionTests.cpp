#include "stdafx.h"
#include "CoolshellLib/ScopeExit.h"

using namespace ScopeExit;

BOOST_AUTO_TEST_SUITE(ScopeExitFunctionTests)

BOOST_AUTO_TEST_CASE(Should_run_on_scope_exit)
{
    int i = 0;

    {
        auto f = AtScopeExit([&] { i++; });

    }

    BOOST_CHECK_EQUAL(1, i);
}

BOOST_AUTO_TEST_CASE(Should_only_run_when_exception_occurs)
{
    int i = 0;

    {
        auto f = AtScopeFailure([&] { i++; });
    }

    BOOST_CHECK_EQUAL(0, i);

    try
    {
        {
            auto f = AtScopeFailure([&] { i++; });
            throw std::exception();
        }
    }
    catch(std::exception&) { }

    BOOST_CHECK_EQUAL(1, i);
}

BOOST_AUTO_TEST_CASE(Should_only_run_when_no_exception_occurs)
{
    int i = 0;

    {
        auto f = AtScopeSuccess([&] { i++; });
    }

    BOOST_CHECK_EQUAL(1, i);

    try
    {
        {
            auto f = AtScopeSuccess([&] { i++; });
            throw std::exception();
        }
    }
    catch(std::exception&) { }

    BOOST_CHECK_EQUAL(1, i);
}

BOOST_AUTO_TEST_SUITE_END( )
