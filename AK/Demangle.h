/*
 * Copyright (c) 2018-2020, Andreas Kling <andreas@ladybird.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/ByteString.h>

#include <cxxabi.h>

namespace AK {

inline ByteString demangle(std::string_view name)
{
    int status = 0;
    auto* demangled_name = abi::__cxa_demangle(name.to_byte_string().characters(), nullptr, nullptr, &status);
    auto string = ByteString(status == 0 ? std::string_view { demangled_name, strlen(demangled_name) } : name);
    if (status == 0)
        free(demangled_name);
    return string;
}

}

#if USING_AK_GLOBALLY
using AK::demangle;
#endif
