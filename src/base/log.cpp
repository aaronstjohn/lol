//
// Lol Engine
//
// Copyright: (c) 2010-2011 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include <cstdio>

#ifdef WIN32
#   define WIN32_LEAN_AND_MEAN
#   include <windows.h>
#endif

#if defined __ANDROID__
#   include <android/log.h>
#   include <unistd.h> /* for gettid() */
#else
#   include <cstdarg>
#endif

#include "core.h"

namespace lol
{

/*
 * Public Log class
 */

void Log::Debug(char const *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    Helper(DebugMessage, fmt, ap);
    va_end(ap);
}

void Log::Info(char const *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    Helper(InfoMessage, fmt, ap);
    va_end(ap);
}

void Log::Warn(char const *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    Helper(WarnMessage, fmt, ap);
    va_end(ap);
}

void Log::Error(char const *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    Helper(ErrorMessage, fmt, ap);
    va_end(ap);
}

/*
 * Private helper function
 */

void Log::Helper(MessageType type, char const *fmt, va_list ap)
{
#if defined __ANDROID__
    int const prio[] =
    {
        ANDROID_LOG_DEBUG,
        ANDROID_LOG_INFO,
        ANDROID_LOG_WARN,
        ANDROID_LOG_ERROR
    };

    String buf = String::Printf(fmt, ap);
    __android_log_print(prio[type], "LOL", "[%d] %s", (int)gettid(), &buf[0]);

#else
    char const *prefix[] =
    {
        "DEBUG",
        "INFO",
        "WARN",
        "ERROR",
    };

#   if defined _WIN32
    String buf = String(prefix[type]) + ": " + String::Printf(fmt, ap);
    OutputDebugString(&buf[0]);
#   else
    fprintf(stderr, "%s: ", prefix[type]);
    vfprintf(stderr, fmt, ap);
#   endif
#endif
}

} /* namespace lol */
