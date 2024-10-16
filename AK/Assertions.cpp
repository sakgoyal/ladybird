/*
 * Copyright (c) 2018-2020, Andreas Kling <andreas@ladybird.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <AK/Assertions.h>
#include <AK/Backtrace.h>
#include <AK/Format.h>
#include <AK/Platform.h>
#include <stacktrace>


#if defined(AK_OS_ANDROID) && (__ANDROID_API__ >= 33)
#    include <android/log.h>
#    define EXECINFO_BACKTRACE
#    define PRINT_ERROR(s) __android_log_write(ANDROID_LOG_WARN, "AK", (s))
#else
#    define PRINT_ERROR(s) (void)fputs((s), stderr)
#endif

#if defined(AK_OS_SERENITY)
#    define ERRORLN dbgln
#else
#    define ERRORLN warnln
#endif

#if defined(AK_HAS_BACKTRACE_HEADER)
ALWAYS_INLINE void dump_backtrace()
{
        auto st = std::stacktrace::current().to_string();
        PRINT_ERROR(st);
}
#endif

extern "C" {
void ak_verification_failed(char const* message)
{
#if defined(AK_OS_SERENITY) || defined(AK_OS_ANDROID)
    bool colorize_output = true;
#elif defined(AK_OS_WINDOWS)
    bool colorize_output = false;
#else
    bool colorize_output = isatty(STDERR_FILENO) == 1;
#endif

    if (colorize_output)
        ERRORLN("\033[31;1mVERIFICATION FAILED\033[0m: {}", message);
    else
        ERRORLN("VERIFICATION FAILED: {}", message);

#if defined(AK_HAS_BACKTRACE_HEADER)
    dump_backtrace();
#endif
    __builtin_trap();
}

void ak_assertion_failed(char const* message)
{
#if defined(AK_OS_SERENITY) || defined(AK_OS_ANDROID)
    bool colorize_output = true;
#elif defined(AK_OS_WINDOWS)
    bool colorize_output = false;
#else
    bool colorize_output = isatty(STDERR_FILENO) == 1;
#endif

    if (colorize_output)
        ERRORLN("\033[31;1mASSERTION FAILED\033[0m: {}", message);
    else
        ERRORLN("ASSERTION FAILED: {}", message);

#if defined(AK_HAS_BACKTRACE_HEADER)
    dump_backtrace();
#endif
    __builtin_trap();
}
}
