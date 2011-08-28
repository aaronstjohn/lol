//
// Lol Engine
//
// Copyright: (c) 2010-2011 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://sam.zoy.org/projects/COPYING.WTFPL for more details.
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>

#include "core.h"

namespace lol
{

class HalfTest : public CppUnit::TestCase
{
    CPPUNIT_TEST_SUITE(HalfTest);
    CPPUNIT_TEST(test_half_makebits);
    CPPUNIT_TEST(test_half_makeslow);
    CPPUNIT_TEST(test_half_makefast);
    CPPUNIT_TEST(test_half_isnan);
    CPPUNIT_TEST(test_half_isinf);
    CPPUNIT_TEST(test_half_isfinite);
    CPPUNIT_TEST(test_half_isnormal);
    CPPUNIT_TEST(test_half_classify);
    CPPUNIT_TEST(test_half_to_float);
    CPPUNIT_TEST(test_half_to_int);
    CPPUNIT_TEST_SUITE_END();

public:
    HalfTest() : CppUnit::TestCase("Matrix Test") {}

    void setUp()
    {
    }

    void tearDown() {}

    void test_half_makebits()
    {
        for (unsigned int i = 0; i < 0x10000; i++)
        {
            half a = half::makebits(i);
            uint16_t b = i;
            CPPUNIT_ASSERT_EQUAL(a.bits(), b);
        }
    }

    void test_half_makeslow()
    {
        for (size_t i = 0; i < sizeof(pairs) / sizeof(*pairs); i++)
        {
            half a = half::makeslow(pairs[i].f);
            uint16_t b = pairs[i].x;
            CPPUNIT_ASSERT_EQUAL(a.bits(), b);
        }
    }

    void test_half_makefast()
    {
        for (size_t i = 0; i < sizeof(pairs) / sizeof(*pairs); i++)
        {
            half a = half::makefast(pairs[i].f);
            uint16_t b = pairs[i].x;
            CPPUNIT_ASSERT_EQUAL(a.bits(), b);
        }
    }

    void test_half_isnan()
    {
        CPPUNIT_ASSERT(half::makebits(0x7c01).isnan());
        CPPUNIT_ASSERT(half::makebits(0xfc01).isnan());
        CPPUNIT_ASSERT(half::makebits(0x7e00).isnan());
        CPPUNIT_ASSERT(half::makebits(0xfe00).isnan());

        CPPUNIT_ASSERT(!half::makebits(0x7c00).isnan());
        CPPUNIT_ASSERT(!half::makebits(0xfc00).isnan());

        CPPUNIT_ASSERT(!half(0.0f).isnan());
        CPPUNIT_ASSERT(!half(-0.0f).isnan());
        CPPUNIT_ASSERT(!half(2.0f).isnan());
        CPPUNIT_ASSERT(!half(-2.0f).isnan());
    }

    void test_half_isinf()
    {
        CPPUNIT_ASSERT(half(65536.0f).isinf());
        CPPUNIT_ASSERT(half(-65536.0f).isinf());

        CPPUNIT_ASSERT(!half(0.0f).isinf());
        CPPUNIT_ASSERT(!half(-0.0f).isinf());
        CPPUNIT_ASSERT(!half(65535.0f).isinf());
        CPPUNIT_ASSERT(!half(-65535.0f).isinf());

        CPPUNIT_ASSERT(half::makebits(0x7c00).isinf());
        CPPUNIT_ASSERT(half::makebits(0xfc00).isinf());

        CPPUNIT_ASSERT(!half::makebits(0x7e00).isinf());
        CPPUNIT_ASSERT(!half::makebits(0xfe00).isinf());
    }

    void test_half_isfinite()
    {
        CPPUNIT_ASSERT(half(0.0f).isfinite());
        CPPUNIT_ASSERT(half(-0.0f).isfinite());
        CPPUNIT_ASSERT(half(65535.0f).isfinite());
        CPPUNIT_ASSERT(half(-65535.0f).isfinite());

        CPPUNIT_ASSERT(!half(65536.0f).isfinite());
        CPPUNIT_ASSERT(!half(-65536.0f).isfinite());

        CPPUNIT_ASSERT(!half::makebits(0x7c00).isfinite());
        CPPUNIT_ASSERT(!half::makebits(0xfc00).isfinite());

        CPPUNIT_ASSERT(!half::makebits(0x7e00).isfinite());
        CPPUNIT_ASSERT(!half::makebits(0xfe00).isfinite());
    }

    void test_half_isnormal()
    {
        CPPUNIT_ASSERT(half(0.0f).isnormal());
        CPPUNIT_ASSERT(half(-0.0f).isnormal());
        CPPUNIT_ASSERT(half(65535.0f).isnormal());
        CPPUNIT_ASSERT(half(-65535.0f).isnormal());

        CPPUNIT_ASSERT(!half(65536.0f).isnormal());
        CPPUNIT_ASSERT(!half(-65536.0f).isnormal());

        CPPUNIT_ASSERT(!half::makebits(0x7c00).isnormal());
        CPPUNIT_ASSERT(!half::makebits(0xfc00).isnormal());

        CPPUNIT_ASSERT(!half::makebits(0x7e00).isnormal());
        CPPUNIT_ASSERT(!half::makebits(0xfe00).isnormal());
    }

    void test_half_classify()
    {
        for (uint32_t i = 0; i < 0x10000; i++)
        {
            half h = half::makebits(i);
            if (h.isnan())
            {
                CPPUNIT_ASSERT(!h.isinf());
                CPPUNIT_ASSERT(!h.isnormal());
                CPPUNIT_ASSERT(!h.isfinite());
            }
            else if (h.isinf())
            {
                CPPUNIT_ASSERT(!h.isnormal());
                CPPUNIT_ASSERT(!h.isfinite());
            }
            else
            {
                CPPUNIT_ASSERT(h.isfinite());
            }
        }
    }

    void test_half_to_float()
    {
        for (size_t i = 0; i < sizeof(pairs) / sizeof(*pairs); i++)
        {
            float a = (float)half::makebits(pairs[i].x);
            float b = pairs[i].f;
            CPPUNIT_ASSERT_EQUAL(a, b);
        }

        for (uint32_t i = 0; i < 0x10000; i++)
        {
            half h = half::makebits(i);
            float f = (float)h;
            half g = (half)f;
            if (h.isnan())
            {
                CPPUNIT_ASSERT(isnan(f));
                CPPUNIT_ASSERT(g.isnan());
            }
            else
            {
                CPPUNIT_ASSERT(!isnan(f));
                CPPUNIT_ASSERT_EQUAL(g.bits(), h.bits());
            }
        }
    }

    void test_half_to_int()
    {
        CPPUNIT_ASSERT_EQUAL((int)(half)(0.0f), 0);
        CPPUNIT_ASSERT_EQUAL((int)(half)(-0.0f), 0);
        CPPUNIT_ASSERT_EQUAL((int)(half)(0.9f), 0);
        CPPUNIT_ASSERT_EQUAL((int)(half)(-0.9f), 0);
        CPPUNIT_ASSERT_EQUAL((int)(half)(1.0f), 1);
        CPPUNIT_ASSERT_EQUAL((int)(half)(-1.0f), -1);
        CPPUNIT_ASSERT_EQUAL((int)(half)(1.9f), 1);
        CPPUNIT_ASSERT_EQUAL((int)(half)(-1.9f), -1);
        CPPUNIT_ASSERT_EQUAL((int)(half)(65504.0f), 65504);
        CPPUNIT_ASSERT_EQUAL((int)(half)(-65504.0f), -65504);
    }

private:
    struct TestPair { float f; uint16_t x; };

    static TestPair const pairs[11];
};

HalfTest::TestPair const HalfTest::pairs[] =
{
    /* All these values have exact half representations */
    { 0.0f,      0x0000 },
    { -0.0f,     0x8000 }, /* negative zero */
    { 1.0f,      0x3c00 },
    { -1.0f,     0xbc00 },
    { 2.0f,      0x4000 },
    { 0.5f,      0x3800 },
    { 0.125f,    0x3000 },
    { 15.9375f,  0x4bf8 },
    { 0x1.fp-10, 0x17c0 },
    { 0x1.fp-14, 0x07c0 }, /* denormal */
    { 0x1.fp-15, 0x03e0 }, /* denormal */
};

CPPUNIT_TEST_SUITE_REGISTRATION(HalfTest);

} /* namespace lol */

