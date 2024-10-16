/*
 * Copyright (c) 2020, the SerenityOS developers.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <AK/CharacterTypes.h>
#include <AK/Format.h>
#include <AK/GenericLexer.h>
#include <AK/IntegralMath.h>
#include <AK/LexicalPath.h>


#include <AK/StringFloatingPointConversions.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#if defined(AK_OS_SERENITY)
#    include <serenity.h>
#endif

#if defined(AK_OS_ANDROID)
#    include <android/log.h>
#endif

#if defined(AK_OS_WINDOWS)
#    include <io.h>
#endif

namespace AK {

static std::string_view process_name_for_logging()
{
    // NOTE: We use AK::Format in the DynamicLoader and LibC, which cannot use thread-safe statics
    // Also go to extraordinary lengths here to avoid strlen() on the process name every call to dbgln
    static char process_name_buf[256] = {};
    static std::string_view process_name;
    static bool process_name_retrieved = false;
    if (!process_name_retrieved) {
        auto path = LexicalPath(process_name_helper());
        process_name_retrieved = true;
        (void)path.basename().copy_characters_to_buffer(process_name_buf, sizeof(process_name_buf));
        process_name = { process_name_buf, strlen(process_name_buf) };
    }
    return process_name;
}

static bool is_debug_enabled = true;

void set_debug_enabled(bool value)
{
    is_debug_enabled = value;
}

// On Serenity, dbgln goes to a non-stderr output
static bool is_rich_debug_enabled =
#if defined(AK_OS_SERENITY)
    true;
#else
    false;
#endif

void set_rich_debug_enabled(bool value)
{
    is_rich_debug_enabled = value;
}

} // namespace AK
