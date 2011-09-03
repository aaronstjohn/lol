//
// Lol Engine - Benchmark program
//
// Copyright: (c) 2005-2011 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://sam.zoy.org/projects/COPYING.WTFPL for more details.
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include <cstdio>

#if defined HAVE_FASTMATH_H
#   include <fastmath.h>
#endif

#include "core.h"
#include "loldebug.h"

using namespace std;
using namespace lol;

static size_t const TRIG_TABLE_SIZE = 128 * 1024;
static size_t const TRIG_RUNS = 50;

static size_t const MATRIX_TABLE_SIZE = 64 * 1024;
static size_t const MATRIX_RUNS = 100;

static size_t const HALF_TABLE_SIZE = 1024 * 1024;
static size_t const HALF_RUNS = 50;

static void bench_trig(int mode);
static void bench_matrix(int mode);
static void bench_half(int mode);

int main(int argc, char **argv)
{
    Log::Info("--------------------------\n");
    Log::Info(" Trigonometry [-1e5, 1e5]\n");
    Log::Info("--------------------------\n");
    bench_trig(1);

    Log::Info("------------------------\n");
    Log::Info(" Trigonometry [-pi, pi]\n");
    Log::Info("------------------------\n");
    bench_trig(2);

    Log::Info("----------------------------\n");
    Log::Info(" Trigonometry [-1e-2, 1e-2]\n");
    Log::Info("----------------------------\n");
    bench_trig(3);

    Log::Info("----------------------------\n");
    Log::Info(" Float matrices [-2.0, 2.0]\n");
    Log::Info("----------------------------\n");
    bench_matrix(1);

    Log::Info("-------------------------------------\n");
    Log::Info(" Half precision floats (random bits)\n");
    Log::Info("-------------------------------------\n");
    bench_half(1);

    Log::Info("-----------------------------------\n");
    Log::Info(" Half precision floats [-2.0, 2.0]\n");
    Log::Info("-----------------------------------\n");
    bench_half(2);

    return EXIT_SUCCESS;
}

static void bench_trig(int mode)
{
    float result[7] = { 0.0f };
    Timer timer;

    /* Set up tables */
    float *pf = new float[TRIG_TABLE_SIZE];
    float *pf2 = new float[TRIG_TABLE_SIZE];

    for (size_t run = 0; run < TRIG_RUNS; run++)
    {
        switch (mode)
        {
        case 1:
            for (size_t i = 0; i < TRIG_TABLE_SIZE; i++)
                pf[i] = RandF(-1e5f, 1e5f);
            break;
        case 2:
            for (size_t i = 0; i < TRIG_TABLE_SIZE; i++)
                pf[i] = RandF(-M_PI, M_PI);
            break;
        case 3:
            for (size_t i = 0; i < TRIG_TABLE_SIZE; i++)
                pf[i] = RandF(-1e-2f, 1e-2f);
            break;
        }

        /* Sin */
        timer.GetMs();
        for (size_t i = 0; i < TRIG_TABLE_SIZE; i++)
            pf2[i] = __builtin_sinf(pf[i]);
        result[0] += timer.GetMs();

        /* Fast sin */
        timer.GetMs();
        for (size_t i = 0; i < TRIG_TABLE_SIZE; i++)
#if defined HAVE_FASTMATH_H
            pf2[i] = f_sinf(pf[i]);
#else
            pf2[i] = sinf(pf[i]);
#endif
        result[1] += timer.GetMs();

        /* Lol sin */
        timer.GetMs();
        for (size_t i = 0; i < TRIG_TABLE_SIZE; i++)
            pf2[i] = lol_sin(pf[i]);
        result[2] += timer.GetMs();

        /* Cos */
        timer.GetMs();
        for (size_t i = 0; i < TRIG_TABLE_SIZE; i++)
            pf2[i] = __builtin_cosf(pf[i]);
        result[3] += timer.GetMs();

        /* Fast cos */
        timer.GetMs();
        for (size_t i = 0; i < TRIG_TABLE_SIZE; i++)
#if defined HAVE_FASTMATH_H
            pf2[i] = f_cosf(pf[i]);
#else
            pf2[i] = cosf(pf[i]);
#endif
        result[4] += timer.GetMs();

        /* Lol cos */
        timer.GetMs();
        for (size_t i = 0; i < TRIG_TABLE_SIZE; i++)
            pf2[i] = lol_cos(pf[i]);
        result[5] += timer.GetMs();

        /* Tan */
        timer.GetMs();
        for (size_t i = 0; i < TRIG_TABLE_SIZE; i++)
            pf2[i] = __builtin_tanf(pf[i]);
        result[6] += timer.GetMs();
    }

    delete[] pf;
    delete[] pf2;

    for (size_t i = 0; i < sizeof(result) / sizeof(*result); i++)
        result[i] *= 1000000.0f / (TRIG_TABLE_SIZE * TRIG_RUNS);

    Log::Info("                          ns/elem\n");
    Log::Info("float = sinf(float)      %7.3f\n", result[0]);
    Log::Info("float = fastsinf(float)  %7.3f\n", result[1]);
    Log::Info("float = lol_sinf(float)  %7.3f\n", result[2]);
    Log::Info("float = cosf(float)      %7.3f\n", result[3]);
    Log::Info("float = fastcosf(float)  %7.3f\n", result[4]);
    Log::Info("float = lol_cosf(float)  %7.3f\n", result[5]);
    Log::Info("float = tanf(float)      %7.3f\n", result[6]);
}

static void bench_matrix(int mode)
{
    float result[5] = { 0.0f };
    Timer timer;

    /* Set up tables */
    mat4 *pm = new mat4[MATRIX_TABLE_SIZE + 1];
    float *pf = new float[MATRIX_TABLE_SIZE];

    for (size_t run = 0; run < MATRIX_RUNS; run++)
    {
        switch (mode)
        {
        case 1:
            for (size_t i = 0; i < MATRIX_TABLE_SIZE; i++)
                for (int j = 0; j < 4; j++)
                    for (int k = 0; k < 4; k++)
                        pm[i][j][k] = RandF(-2.0f, 2.0f);
            break;
        }

        /* Copy matrices */
        timer.GetMs();
        for (size_t i = 0; i < MATRIX_TABLE_SIZE; i++)
            pm[i] = pm[i + 1];
        result[0] += timer.GetMs();

        /* Determinant */
        timer.GetMs();
        for (size_t i = 0; i < MATRIX_TABLE_SIZE; i++)
            pf[i] = pm[i].det();
        result[1] += timer.GetMs();

        /* Multiply matrices */
        timer.GetMs();
        for (size_t i = 0; i < MATRIX_TABLE_SIZE; i++)
            pm[i] *= pm[i + 1];
        result[2] += timer.GetMs();

        /* Add matrices */
        timer.GetMs();
        for (size_t i = 0; i < MATRIX_TABLE_SIZE; i++)
            pm[i] += pm[i + 1];
        result[3] += timer.GetMs();

        /* Invert matrix */
        timer.GetMs();
        for (size_t i = 0; i < MATRIX_TABLE_SIZE; i++)
            pm[i] = pm[i].invert();
        result[4] += timer.GetMs();
    }

    delete[] pm;
    delete[] pf;

    for (size_t i = 0; i < sizeof(result) / sizeof(*result); i++)
        result[i] *= 1000000.0f / (MATRIX_TABLE_SIZE * MATRIX_RUNS);

    Log::Info("                          ns/elem\n");
    Log::Info("mat4 = mat4              %7.3f\n", result[0]);
    Log::Info("float = mat4.det()       %7.3f\n", result[1]);
    Log::Info("mat4 *= mat4             %7.3f\n", result[2]);
    Log::Info("mat4 += mat4             %7.3f\n", result[3]);
    Log::Info("mat4 = mat4.invert()     %7.3f\n", result[4]);
}

static void bench_half(int mode)
{
    float result[10] = { 0.0f };
    Timer timer;

    /* Set up tables */
    float *pf = new float[HALF_TABLE_SIZE + 1];
    half *ph = new half[HALF_TABLE_SIZE + 1];

    for (size_t run = 0; run < HALF_RUNS; run++)
    {
        switch (mode)
        {
        case 1:
            for (size_t i = 0; i < HALF_TABLE_SIZE + 1; i++)
                ph[i] = half::makebits(rand());
            break;
        case 2:
            for (size_t i = 0; i < HALF_TABLE_SIZE + 1; i++)
                ph[i] = RandF(-2.0f, 2.0f);
            break;
        }

        /* Copy float */
        timer.GetMs();
        for (size_t i = 0; i < HALF_TABLE_SIZE; i++)
            pf[i] = pf[i + 1];
        result[0] += timer.GetMs();

        /* Convert half to float (array) */
        timer.GetMs();
        half::convert(pf, ph, HALF_TABLE_SIZE);
        result[1] += timer.GetMs();

        /* Convert half to float (fast) */
        timer.GetMs();
        for (size_t i = 0; i < HALF_TABLE_SIZE; i++)
            pf[i] = (float)ph[i];
        result[2] += timer.GetMs();

        /* Add a half to every float */
        timer.GetMs();
        for (size_t i = 0; i < HALF_TABLE_SIZE; i++)
            pf[i] += ph[i];
        result[3] += timer.GetMs();

        /* Copy half */
        timer.GetMs();
        for (size_t i = 0; i < HALF_TABLE_SIZE; i++)
            ph[i] = ph[i + 1];
        result[4] += timer.GetMs();

        /* Change sign of every half */
        timer.GetMs();
        for (size_t i = 0; i < HALF_TABLE_SIZE; i++)
            ph[i] = -ph[i];
        result[5] += timer.GetMs();

        /* Convert float to half (array) */
        timer.GetMs();
        half::convert(ph, pf, HALF_TABLE_SIZE);
        result[6] += timer.GetMs();

        /* Convert float to half (fast) */
        timer.GetMs();
        for (size_t i = 0; i < HALF_TABLE_SIZE; i++)
            ph[i] = (half)pf[i];
        result[7] += timer.GetMs();

        /* Convert float to half (accurate) */
        timer.GetMs();
        for (size_t i = 0; i < HALF_TABLE_SIZE; i++)
            ph[i] = half::makeaccurate(pf[i]);
        result[8] += timer.GetMs();

        /* Add a float to every half */
        timer.GetMs();
        for (size_t i = 0; i < HALF_TABLE_SIZE; i++)
            ph[i] += pf[i];
        result[9] += timer.GetMs();
    }

    delete[] pf;
    delete[] ph;

    for (size_t i = 0; i < sizeof(result) / sizeof(*result); i++)
        result[i] *= 1000000.0f / (HALF_TABLE_SIZE * HALF_RUNS);

    Log::Info("                          ns/elem\n");
    Log::Info("float = float            %7.3f\n", result[0]);
    Log::Info("float = half (array)     %7.3f\n", result[1]);
    Log::Info("float = half (fast)      %7.3f\n", result[2]);
    Log::Info("float += half            %7.3f\n", result[3]);
    Log::Info("half = half              %7.3f\n", result[4]);
    Log::Info("half = -half             %7.3f\n", result[5]);
    Log::Info("half = float (array)     %7.3f\n", result[6]);
    Log::Info("half = float (fast)      %7.3f\n", result[7]);
    Log::Info("half = float (accurate)  %7.3f\n", result[8]);
    Log::Info("half += float            %7.3f\n", result[9]);
}

