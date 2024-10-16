/*
 * Copyright (c) 2020, Andreas Kling <andreas@ladybird.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include "AK/Assertions.h"
#include "AK/ByteBuffer.h"
#include "AK/Noncopyable.h"
#include <AK/DefaultDelete.h>
#include <AK/SinglyLinkedListSizePolicy.h>
#include <AK/Types.h>

namespace AK {

namespace Detail {
template<size_t inline_capacity>

class StringData;
}

enum class TrailingCodePointTransformation : u8;

class BigEndianInputBitStream;
class BigEndianOutputBitStream;
class Bitmap;
class CircularBuffer;
class ConstrainedStream;
class CountingStream;
class ByteString;
class DeprecatedStringCodePointIterator;
class Duration;
class Error;
class FlyString;
class GenericLexer;
class IPv4Address;
class IPv6Address;
class JsonArray;
class JsonObject;
class JsonValue;
class LexicalPath;
class LittleEndianInputBitStream;
class LittleEndianOutputBitStream;
class SearchableCircularBuffer;
class SeekableStream;
class StackInfo;
class Stream;
class String;
class StringBuilder;
class StringImpl;
class UnixDateTime;
class Utf16View;
class Utf32CodePointIterator;
class Utf32View;
class Utf8CodePointIterator;
class Utf8View;

template<typename T, typename TSizeCalculationPolicy = DefaultSizeCalculationPolicy>
class SinglyLinkedList;

template<typename T>
class DoublyLinkedList;

template<typename T, size_t capacity>
class CircularQueue;

template<typename T>
class Badge;

template<size_t precision, typename Underlying = i32>
class FixedPoint;

template<typename>
class Function;

template<typename Out, typename... In>
class Function<Out(In...)>;

template<typename T>
class NonnullRefPtr;

template<typename T>
class NonnullOwnPtr;

template<typename T>
class RefPtr;

template<typename T, typename TDeleter = DefaultDelete<T>>
class OwnPtr;

template<typename T>
class WeakPtr;

template<typename T, typename ErrorType = Error>
class [[nodiscard]] ErrorOr;

}

#if USING_AK_GLOBALLY
using AK::Badge;
using AK::BigEndianInputBitStream;
using AK::BigEndianOutputBitStream;
using AK::Bitmap;
using AK::ByteString;
using AK::CircularBuffer;
using AK::CircularQueue;
using AK::ConstrainedStream;
using AK::CountingStream;
using AK::DeprecatedStringCodePointIterator;
using AK::DoublyLinkedList;
using AK::Error;
using AK::ErrorOr;
using AK::FlyString;
using AK::Function;
using AK::GenericLexer;
using AK::IPv4Address;
using AK::IPv6Address;
using AK::JsonArray;
using AK::JsonObject;
using AK::JsonValue;
using AK::LexicalPath;
using AK::LittleEndianInputBitStream;
using AK::LittleEndianOutputBitStream;
using AK::NonnullOwnPtr;
using AK::NonnullRefPtr;
using AK::OwnPtr;
using AK::RefPtr;
using AK::SearchableCircularBuffer;
using AK::SeekableStream;
using AK::SinglyLinkedList;
using AK::StackInfo;
using AK::Stream;
using AK::String;
using AK::StringBuilder;
using AK::StringImpl;
using AK::TrailingCodePointTransformation;
using AK::UnixDateTime;
using AK::Utf16View;
using AK::Utf32CodePointIterator;
using AK::Utf32View;
using AK::Utf8CodePointIterator;
using AK::Utf8View;

#endif
