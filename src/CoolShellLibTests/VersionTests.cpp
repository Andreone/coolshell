#include "stdafx.h"
#include "CoolShellLib\Version.h"

BOOST_AUTO_TEST_SUITE(TestVersion)

BOOST_AUTO_TEST_CASE( Test_parsing )
{
    CString expected(_T("1.2.3.4"));
    Version v1(expected);
    BOOST_CHECK(expected == v1.ToString<CString>());
}

BOOST_AUTO_TEST_CASE( Test_CString_conversion )
{
    CString expected(_T("1.2.3.4"));
    Version v1(1, 2, 3, 4);
    BOOST_CHECK(expected == v1.ToString<CString>());
}

BOOST_AUTO_TEST_CASE( Test_string_conversion )
{
    std::string expected("1.2.3.4");
    Version v1(1, 2, 3, 4);
    BOOST_CHECK(expected == v1.ToString<std::string>());
}

BOOST_AUTO_TEST_CASE( TestCopy )
{
    Version v1(1, 2, 3, 4);
    Version v2 = v1;
    BOOST_CHECK(v1 == v2);
}

BOOST_AUTO_TEST_CASE( TestEqualityOperator )
{
    BOOST_CHECK(Version(1, 2, 3, 4) == Version(1, 2, 3, 4));
    BOOST_CHECK(Version(1, 0) == Version(1));
    BOOST_CHECK(Version(1, 1) == Version(1, 1, 0, 0));
}

BOOST_AUTO_TEST_CASE( TestDifferenceOperator )
{
    BOOST_CHECK(Version(1, 2, 3, 4) != Version(1, 2, 3));
    BOOST_CHECK(!(Version(1, 2, 3, 4) != Version(1, 2, 3, 4)));
}

BOOST_AUTO_TEST_CASE( TestLowerThanOperator )
{
    BOOST_CHECK(Version(1, 0) < Version(2, 0));
    BOOST_CHECK(Version(_T("1.09")) < Version(1, 20));
    BOOST_CHECK(!(Version(1, 1) < Version(1, 1)));
}

BOOST_AUTO_TEST_CASE( TestGreaterThanOperator )
{
    BOOST_CHECK(Version(2, 0, 0, 0) >= Version(1, 9, 9, 9));
    BOOST_CHECK(Version(3, 9, 0, 0) >= Version(3, 8, 0, 5));
    BOOST_CHECK(Version(1, 9, 2, 1) >= Version(1, 9, 2, 0));
    BOOST_CHECK(Version(1, 9, 2, 1) >= Version(1, 9, 2, 1));
    BOOST_CHECK(!(Version(1, 9, 0) >= Version(1, 9, 1)));
}

BOOST_AUTO_TEST_CASE( TestLowerThanOrEqualOperator )
{
    BOOST_CHECK(Version(1, 9) <= Version(1, 10));
    BOOST_CHECK(Version(1, 9, 0) <= Version(1, 10, 1, 2));
    BOOST_CHECK(Version(1, 9) <= Version(1, 9));
}

BOOST_AUTO_TEST_CASE( TestGreaterThanOrEqualOperator )
{
    BOOST_CHECK(Version(2, 0, 0, 0) >= Version(1, 9, 9, 9));
    BOOST_CHECK(Version(3, 9, 0, 0) >= Version(3, 8, 0, 5));
    BOOST_CHECK(Version(1, 9, 2, 1) >= Version(1, 9, 2, 0));
    BOOST_CHECK(Version(1, 9, 2, 1) >= Version(1, 9, 2, 1));
    BOOST_CHECK(!(Version(1, 9, 0) >= Version(1, 9, 1)));
}

BOOST_AUTO_TEST_SUITE_END( )
