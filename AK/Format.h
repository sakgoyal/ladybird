/*
 * Copyright (c) 2020, the SerenityOS developers.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/CheckedFormatString.h>
#include <AK/Error.h>
#include <AK/Forward.h>
#include <limits>
#include <stdio.h>
#include <string.h>
#include <type_traits>
#include <variant>

namespace AK {
enum AllowDebugOnlyFormatters {
    No,
    Yes
};

template<typename Formatter>
inline constexpr bool is_debug_only_formatter()
{
    constexpr bool has_is_debug_only = requires(Formatter const& formatter) { formatter.is_debug_only(); };
    if constexpr (has_is_debug_only)
        return Formatter::is_debug_only();
    return false;
}

} // namespace AK

#if USING_AK_GLOBALLY
#    define dbgln_if(flag, fmt, ...)       \
        do {                               \
            if constexpr (flag)            \
                dbgln(fmt, ##__VA_ARGS__); \
        } while (0)

#endif
