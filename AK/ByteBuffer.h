/*
 * Copyright (c) 2018-2021, Andreas Kling <andreas@ladybird.org>
 * Copyright (c) 2021, Gunnar Beutner <gbeutner@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/Badge.h>
#include <AK/Error.h>

#include <AK/Types.h>


namespace AK {
namespace Detail {

template<size_t inline_capacity>
using ByteBuffer = std::array<char, inline_capacity>;

// template<>
// struct Traits<ByteBuffer> : public DefaultTraits<ByteBuffer> {
//     static unsigned hash(ByteBuffer const& byte_buffer)
//     {
//         return Traits<ReadonlyBytes>::hash(byte_buffer.span());
//     }
//     static bool equals(ByteBuffer const& byte_buffer, Bytes const& other)
//     {
//         return byte_buffer.bytes() == other;
//     }
//     static bool equals(ByteBuffer const& byte_buffer, ReadonlyBytes const& other)
//     {
//         return byte_buffer.bytes() == other;
//     }
// };
}
}
