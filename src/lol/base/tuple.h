//
// Lol Engine
//
// Copyright: (c) 2010-2014 Sam Hocevar <sam@hocevar.net>
//            (c) 2013-2014 Benjamin "Touky" Huet <huet.benjamin@gmail.com>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

//
// The tuple class
// ---------------
// A very simple tuple class.
//

#if !defined __LOL_BASE_TUPLE_H__
#define __LOL_BASE_TUPLE_H__

#include <lol/base/assert.h>

namespace lol
{

template<typename T1, typename T2 = void, typename T3 = void,
         typename T4 = void, typename T5 = void, typename T6 = void,
         typename T7 = void, typename T8 = void>
class tuple
{
public:
    T1 m1; T2 m2; T3 m3; T4 m4; T5 m5; T6 m6; T7 m7; T8 m8;
};

template<typename T1, typename T2, typename T3, typename T4, typename T5,
         typename T6, typename T7>
class tuple<T1, T2, T3, T4, T5, T6, T7, void>
{
public:
    T1 m1; T2 m2; T3 m3; T4 m4; T5 m5; T6 m6; T7 m7;
};

template<typename T1, typename T2, typename T3, typename T4, typename T5,
         typename T6>
class tuple<T1, T2, T3, T4, T5, T6, void, void>
{
public:
    T1 m1; T2 m2; T3 m3; T4 m4; T5 m5; T6 m6;
};

template<typename T1, typename T2, typename T3, typename T4, typename T5>
class tuple<T1, T2, T3, T4, T5, void, void, void>
{
public:
    T1 m1; T2 m2; T3 m3; T4 m4; T5 m5;
};

template<typename T1, typename T2, typename T3, typename T4>
class tuple<T1, T2, T3, T4, void, void, void, void>
{
public:
    T1 m1; T2 m2; T3 m3; T4 m4;
};

template<typename T1, typename T2, typename T3>
class tuple<T1, T2, T3, void, void, void, void, void>
{
public:
    T1 m1; T2 m2; T3 m3;
};

template<typename T1, typename T2>
class tuple<T1, T2, void, void, void, void, void, void>
{
public:
    T1 m1; T2 m2;
};

template<typename T1>
class tuple<T1, void, void, void, void, void, void, void>
{
public:
    T1 m1;
};

} /* namespace lol */

#endif // __LOL_BASE_TUPLE_H__

