/*
 * Copyright (c) 2018-2020, Andreas Kling <andreas@ladybird.org>
 * Copyright (c) 2021, Max Wipfli <max.wipfli@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/ByteString.h>


// On Linux distros that use mlibc `basename` is defined as a macro that expands to `__mlibc_gnu_basename` or `__mlibc_gnu_basename_c`, so we undefine it.
#if defined(AK_OS_LINUX) && defined(basename)
#    undef basename
#endif

namespace AK {

class LexicalPath {
public:
    enum StripExtension {
        No,
        Yes
    };

    explicit LexicalPath(ByteString);

    bool is_absolute() const { return !m_string.is_empty() && m_string[0] == '/'; }
    ByteString const& string() const { return m_string; }

    std::string_view dirname() const { return m_dirname; }
    std::string_view basename(StripExtension s = StripExtension::No) const { return s == StripExtension::No ? m_basename : m_basename.substring_view(0, m_basename.length() - m_extension.length() - 1); }
    std::string_view title() const { return m_title; }
    std::string_view extension() const { return m_extension; }

    Vector<std::string_view> const& parts_view() const { return m_parts; }
    [[nodiscard]] Vector<ByteString> parts() const;

    bool has_extension(std::string_view) const;
    bool is_child_of(LexicalPath const& possible_parent) const;

    [[nodiscard]] LexicalPath append(std::string_view) const;
    [[nodiscard]] LexicalPath prepend(std::string_view) const;
    [[nodiscard]] LexicalPath parent() const;

    [[nodiscard]] static ByteString canonicalized_path(ByteString);
    [[nodiscard]] static ByteString absolute_path(ByteString dir_path, ByteString target);
    [[nodiscard]] static ByteString relative_path(std::string_view absolute_path, std::string_view prefix);

    template<typename... S>
    [[nodiscard]] static LexicalPath join(std::string_view first, S&&... rest)
    {
        StringBuilder builder;
        builder.append(first);
        ((builder.append('/'), builder.append(forward<S>(rest))), ...);

        return LexicalPath { builder.to_byte_string() };
    }

    [[nodiscard]] static ByteString dirname(ByteString path)
    {
        auto lexical_path = LexicalPath(move(path));
        return lexical_path.dirname();
    }

    [[nodiscard]] static ByteString basename(ByteString path, StripExtension s = StripExtension::No)
    {
        auto lexical_path = LexicalPath(move(path));
        return lexical_path.basename(s);
    }

    [[nodiscard]] static ByteString title(ByteString path)
    {
        auto lexical_path = LexicalPath(move(path));
        return lexical_path.title();
    }

    [[nodiscard]] static ByteString extension(ByteString path)
    {
        auto lexical_path = LexicalPath(move(path));
        return lexical_path.extension();
    }

private:
    Vector<std::string_view> m_parts;
    ByteString m_string;
    std::string_view m_dirname;
    std::string_view m_basename;
    std::string_view m_title;
    std::string_view m_extension;
};

template<>
struct Formatter<LexicalPath> : Formatter<std::string_view> {
    ErrorOr<void> format(FormatBuilder& builder, LexicalPath const& value)
    {
        return Formatter<std::string_view>::format(builder, value.string());
    }
};

};

#if USING_AK_GLOBALLY
using AK::LexicalPath;
#endif
