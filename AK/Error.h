/*
 * Copyright (c) 2021, Andreas Kling <andreas@ladybird.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/Try.h>
#include <errno.h>
#include <string.h>
#include <AK/FlyString.h>

namespace AK {

class [[nodiscard]] Error {
public:
    ALWAYS_INLINE Error(Error&&) = default;
    ALWAYS_INLINE Error& operator=(Error&&) = default;

    static Error from_errno(int code)
    {
        VERIFY(code != 0);
        return Error(code);
    }

    // NOTE: For calling this method from within kernel code, we will simply print
    // the error message and return the errno code.
    // For calling this method from userspace programs, we will simply return from
    // the Error::from_string_view method!
    static Error from_string_view_or_print_error_and_return_errno(StringView string_literal, int code);

    static Error from_syscall(StringView syscall_name, int rc)
    {
        return Error(syscall_name, rc);
    }
    static Error from_string_view(StringView string_literal) { return Error(string_literal); }

    template<OneOf<ByteString, FlyString, String, FlyString> T>
    static Error from_string_view(T)
    {
        // `Error::from_string_view(ByteString::formatted(...))` is a somewhat common mistake, which leads to a UAF situation.
        // If your string outlives this error and _isn't_ a temporary being passed to this function, explicitly call .view() on it to resolve to the StringView overload.
        static_assert(DependentFalse<T>, "Error::from_string_view(String) is almost always a use-after-free");
        VERIFY_NOT_REACHED();
    }

    static Error copy(Error const& error)
    {
        return Error(error);
    }

    // NOTE: Prefer `from_string_literal` when directly typing out an error message:
    //
    //     return Error::from_string_literal("Class: Some failure");
    //
    // If you need to return a static string based on a dynamic condition (like
    // picking an error from an array), then prefer `from_string_view` instead.
    template<size_t N>
    ALWAYS_INLINE static Error from_string_literal(char const (&string_literal)[N])
    {
        return from_string_view(StringView { string_literal, N - 1 });
    }

    // Note: Don't call this from C++; it's here for Jakt interop (as the name suggests).
    template<SameAs<StringView> T>
    ALWAYS_INLINE static Error __jakt_from_string_literal(T string)
    {
        return from_string_view(string);
    }

    bool operator==(Error const& other) const
    {
        return m_code == other.m_code && m_string_literal == other.m_string_literal && m_syscall == other.m_syscall;
    }

    int code() const { return m_code; }
    bool is_errno() const
    {
        return m_code != 0;
    }
    bool is_syscall() const
    {
        return m_syscall;
    }
    StringView string_literal() const
    {
        return m_string_literal;
    }

protected:
    Error(int code)
        : m_code(code)
    {
    }

private:
    Error(StringView string_literal)
        : m_string_literal(string_literal)
    {
    }

    Error(StringView syscall_name, int rc)
        : m_string_literal(syscall_name)
        , m_code(-rc)
        , m_syscall(true)
    {
    }

    Error(Error const&) = default;
    Error& operator=(Error const&) = default;

    StringView m_string_literal;

    int m_code { 0 };

    bool m_syscall { false };
};

#if USING_AK_GLOBALLY
using AK::Error;
using AK::ErrorOr;
#endif
