/*
 * Copyright (c) 2018-2020, Andreas Kling <andreas@ladybird.org>
 * Copyright (c) 2020, Fei Wu <f.eiwu@yahoo.com>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once


#include <AK/EnumBits.h>
#include <AK/Forward.h>

namespace AK {

namespace Detail {
template<Concepts::AnyString T, Concepts::AnyString U>
inline constexpr bool IsHashCompatible<T, U> = true;
}

enum class CaseSensitivity {
    CaseInsensitive,
    CaseSensitive,
};

enum class ReplaceMode {
    All,
    FirstOnly,
};

enum class TrimMode {
    Left,
    Right,
    Both
};

enum class TrimWhitespace {
    Yes,
    No,
};

enum class SplitBehavior : unsigned {
    // Neither keep empty substrings nor keep the trailing separator.
    // This is the default behavior if unspecified.
    Nothing = 0,

    // If two separators follow each other without any characters
    // in between, keep a "" in the resulting vector. (or only the
    // separator if KeepTrailingSeparator is used)
    KeepEmpty = 1,

    // Do not strip off the separator at the end of the string.
    KeepTrailingSeparator = 2,
};
AK_ENUM_BITWISE_OPERATORS(SplitBehavior);

enum class TrailingCodePointTransformation : u8 {
    // Default behaviour; Puts the first typographic letter unit of each word, if lowercase, in titlecase; the other characters in lowercase.
    Lowercase,
    // Puts the first typographic letter unit of each word, if lowercase, in titlecase; other characters are unaffected. (https://drafts.csswg.org/css-text/#valdef-text-transform-capitalize)
    PreserveExisting,
};

struct MaskSpan {
    size_t start;
    size_t length;

    bool operator==(MaskSpan const& other) const
    {
        return start == other.start && length == other.length;
    }
};

namespace StringUtils {

bool matches(std::string_view str, std::string_view mask, CaseSensitivity = CaseSensitivity::CaseInsensitive, Vector<MaskSpan>* match_spans = nullptr);
template<typename T = int>
Optional<T> convert_to_int(std::string_view, TrimWhitespace = TrimWhitespace::Yes);
template<typename T = unsigned>
Optional<T> convert_to_uint(std::string_view, TrimWhitespace = TrimWhitespace::Yes);
template<typename T = unsigned>
Optional<T> convert_to_uint_from_hex(std::string_view, TrimWhitespace = TrimWhitespace::Yes);
template<typename T = unsigned>
Optional<T> convert_to_uint_from_octal(std::string_view, TrimWhitespace = TrimWhitespace::Yes);
template<typename T>
Optional<T> convert_to_floating_point(std::string_view, TrimWhitespace = TrimWhitespace::Yes);
bool equals_ignoring_ascii_case(std::string_view, std::string_view);
bool ends_with(std::string_view a, std::string_view b, CaseSensitivity);
bool starts_with(std::string_view, std::string_view, CaseSensitivity);
bool contains(std::string_view, std::string_view, CaseSensitivity);
bool is_whitespace(std::string_view);
std::string_view trim(std::string_view string, std::string_view characters, TrimMode mode);
std::string_view trim_whitespace(std::string_view string, TrimMode mode);

Optional<size_t> find(std::string_view haystack, char needle, size_t start = 0);
Optional<size_t> find(std::string_view haystack, std::string_view needle, size_t start = 0);
Optional<size_t> find_last(std::string_view haystack, char needle);
Optional<size_t> find_last(std::string_view haystack, std::string_view needle);
Optional<size_t> find_last_not(std::string_view haystack, char needle);
Vector<size_t> find_all(std::string_view haystack, std::string_view needle);
enum class SearchDirection {
    Forward,
    Backward
};
Optional<size_t> find_any_of(std::string_view haystack, std::string_view needles, SearchDirection);

ByteString to_snakecase(std::string_view);
ByteString to_titlecase(std::string_view);
ByteString invert_case(std::string_view);

ByteString replace(std::string_view, std::string_view needle, std::string_view replacement, ReplaceMode);
ErrorOr<String> replace(String const&, std::string_view needle, std::string_view replacement, ReplaceMode);

size_t count(std::string_view, std::string_view needle);
size_t count(std::string_view, char needle);

}

}

#if USING_AK_GLOBALLY
using AK::CaseSensitivity;
using AK::ReplaceMode;
using AK::SplitBehavior;
using AK::TrailingCodePointTransformation;
using AK::TrimMode;
using AK::TrimWhitespace;
#endif
