#pragma once
#ifndef HARLINN_COMMON_BASICTYPES_H_
#define HARLINN_COMMON_BASICTYPES_H_

/*
   Copyright 2024-2026 Espen Harlinn

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/
#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES 1
#endif

#include <bit>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <type_traits>

namespace Harlinn::Common
{

    template<typename EnumT, typename ... Args >
        requires std::is_enum_v<EnumT>&& std::conjunction_v<std::is_same<std::remove_cvref_t<EnumT>, std::remove_cvref_t<Args>>...> && ( sizeof...( Args ) > 0 )
    constexpr bool ContainsAllOf( EnumT flags, Args&& ... args )
    {
        using IntType = std::underlying_type_t<EnumT>;
        IntType mask = std::bit_cast< IntType >( ( args | ... ) );
        return ( std::bit_cast< IntType >( flags ) & mask ) == mask;
    }

    template<typename EnumT, typename ... Args >
        requires std::is_enum_v<EnumT>&& std::conjunction_v<std::is_same<std::remove_cvref_t<EnumT>, std::remove_cvref_t<Args>>...> && ( sizeof...( Args ) > 0 )
    constexpr bool ContainsNoneOf( EnumT flags, Args ... args )
    {
        using IntType = std::underlying_type_t<EnumT>;
        IntType mask = std::bit_cast< IntType >( ( args | ... ) );
        return ( std::bit_cast< IntType >( flags ) & mask ) == static_cast< IntType >( 0 );
    }
    template<typename EnumT, typename ... Args >
        requires std::is_enum_v<EnumT>&& std::conjunction_v<std::is_same<std::remove_cvref_t<EnumT>, std::remove_cvref_t<Args>>...> && ( sizeof...( Args ) > 0 )
    constexpr bool ContainsAnyOf( EnumT flags, Args ... args )
    {
        using IntType = std::underlying_type_t<EnumT>;
        IntType mask = std::bit_cast< IntType >( ( args | ... ) );
        return ( std::bit_cast< IntType >( flags ) & mask ) != static_cast< IntType >( 0 );
    }
    template<typename EnumT, typename ... Args >
        requires std::is_enum_v<EnumT>&& std::conjunction_v<std::is_same<std::remove_cvref_t<EnumT>, std::remove_cvref_t<Args>>...> && ( sizeof...( Args ) > 0 )
    constexpr bool IsSet( EnumT flags, Args&& ... args )
    {
        using IntType = std::underlying_type_t<EnumT>;
        IntType mask = std::bit_cast< IntType >( ( args | ... ) );
        return ( std::bit_cast< IntType >( flags ) & mask ) == mask;
    }
    template<typename EnumT, typename ... Args >
        requires std::is_enum_v<EnumT>&& std::conjunction_v<std::is_same<std::remove_cvref_t<EnumT>, std::remove_cvref_t<Args>>...> && ( sizeof...( Args ) > 0 )
    constexpr bool IsNotSet( EnumT flags, Args ... args )
    {
        using IntType = std::underlying_type_t<EnumT>;
        IntType mask = std::bit_cast< IntType >( ( args | ... ) );
        return ( std::bit_cast< IntType >( flags ) & mask ) == static_cast< IntType >( 0 );
    }
    

#define HCC_DEFINE_ENUM_FLAG_OPERATORS(ENUMTYPE,INTTYPE) \
  inline constexpr ENUMTYPE operator | (ENUMTYPE a, ENUMTYPE b) noexcept { return static_cast<ENUMTYPE>(static_cast<INTTYPE>(a) | static_cast<INTTYPE>(b)); } \
  inline constexpr ENUMTYPE &operator |= (ENUMTYPE &a, ENUMTYPE b) noexcept { a = static_cast<ENUMTYPE>(static_cast<INTTYPE>(a) | static_cast<INTTYPE>(b)); return a; } \
  inline constexpr ENUMTYPE operator & (ENUMTYPE a, ENUMTYPE b) noexcept { return static_cast<ENUMTYPE>(static_cast<INTTYPE>(a) & static_cast<INTTYPE>(b)); } \
  inline constexpr ENUMTYPE &operator &= (ENUMTYPE &a, ENUMTYPE b) noexcept { a = static_cast<ENUMTYPE>(static_cast<INTTYPE>(a) & static_cast<INTTYPE>(b)); return a; } \
  inline constexpr ENUMTYPE operator ~ (ENUMTYPE a) noexcept { return static_cast<ENUMTYPE>(~static_cast<INTTYPE>(a)); } \
  inline constexpr ENUMTYPE operator ^ (ENUMTYPE a, ENUMTYPE b) noexcept { return static_cast<ENUMTYPE>(static_cast<INTTYPE>(a) ^ static_cast<INTTYPE>(b)); } \
  inline constexpr ENUMTYPE &operator ^= (ENUMTYPE &a, ENUMTYPE b) noexcept { a = static_cast<ENUMTYPE>(static_cast<INTTYPE>(a) ^ static_cast<INTTYPE>(b)); return a; } \
  inline constexpr bool operator == (ENUMTYPE a, ENUMTYPE b) noexcept { return static_cast<INTTYPE>(a) == static_cast<INTTYPE>(b); } \
  inline constexpr bool operator == (INTTYPE a, ENUMTYPE b) noexcept { return static_cast<INTTYPE>(a) == static_cast<INTTYPE>(b); } \
  inline constexpr bool operator == (ENUMTYPE a, INTTYPE b) noexcept { return static_cast<INTTYPE>(a) == static_cast<INTTYPE>(b); } \
  inline constexpr bool operator != (ENUMTYPE a, ENUMTYPE b) noexcept { return static_cast<INTTYPE>(a) != static_cast<INTTYPE>(b); } \
  inline constexpr bool operator != (INTTYPE a, ENUMTYPE b) noexcept { return static_cast<INTTYPE>(a) != static_cast<INTTYPE>(b); } \
  inline constexpr bool operator != (ENUMTYPE a, INTTYPE b) noexcept { return static_cast<INTTYPE>(a) != static_cast<INTTYPE>(b); } \
  static_assert(std::is_same_v<std::underlying_type_t<ENUMTYPE>, INTTYPE> )

    

    constexpr unsigned long long operator""_gb( unsigned long long value )
    {
        return value * 1024ULL * 1024ULL * 1024ULL;
    }
    constexpr unsigned long long operator""_mb( unsigned long long value )
    {
        return value * 1024ULL * 1024ULL;
    }
    constexpr unsigned long long operator""_kb( unsigned long long value )
    {
        return value * 1024ULL;
    }

    using Boolean = bool;
    using Int8 = std::int8_t;
    using SByte = std::int8_t;
    using UInt8 = std::uint8_t;
    using Byte = std::uint8_t;
    using Int16 = std::int16_t;
    using UInt16 = std::uint16_t;
    using ushort = unsigned short;

    using Int32 = std::int32_t;
    using Int = int;
    using UInt32 = std::uint32_t;
    using UInt = unsigned int;
    using ULong32 = std::uint32_t;
    using Long32 = std::int32_t;
    using uint = unsigned int;
    using DWord32 = std::uint32_t;
    using DWord = std::uint32_t;

    using Int64 = std::int64_t;
    using Long64 = std::int64_t;
    using LongLong = long long;
    using UInt64 = std::uint64_t;
    using ULong64 = std::uint64_t;
    using ULongLong = unsigned long long;
    using DWord64 = std::uint64_t;

    using ssize_t = std::make_signed_t<std::size_t>;
    using SSizeT = ssize_t;
    using SizeT = std::size_t;


    using Char = char;
    using WideChar = wchar_t;
    using Char8 = char8_t;
    using Char16 = char16_t;
    using Char32 = char32_t;

    using Single = float;
    using Double = double;


    constexpr UInt8 MaxUInt8 = ( ( UInt8 )~( ( UInt8 )0 ) );
    constexpr Int8 MaxInt8 = ( ( Int8 )( MaxUInt8 >> 1 ) );
    constexpr Int8 MinInt8 = ( ( Int8 )~MaxInt8 );

    constexpr UInt16 MaxUInt16 = ( ( UInt16 )~( ( UInt16 )0 ) );
    constexpr Int16  MaxInt16 = ( ( Int16 )( MaxUInt16 >> 1 ) );
    constexpr Int16  MinInt16 = ( ( Int16 )~MaxInt16 );

    constexpr UInt32 MaxUInt32 = ( ( UInt32 )~( ( UInt32 )0 ) );
    constexpr Int32  MaxInt32 = ( ( Int32 )( MaxUInt32 >> 1 ) );
    constexpr Int32  MinInt32 = ( ( Int32 )~MaxInt32 );

    constexpr UInt64 MaxUInt64 = ( ( UInt64 )~( ( UInt64 )0 ) );
    constexpr Int64  MaxInt64 = ( ( Int64 )( MaxUInt64 >> 1 ) );
    constexpr Int64  MinInt64 = ( ( Int64 )~MaxInt64 );

    constexpr ULong32 MaxULong32 = ( ( ULong32 )~( ( ULong32 )0 ) );
    constexpr Long32  MaxLong32 = ( ( Long32 )( MaxULong32 >> 1 ) );
    constexpr Long32  MinLong32 = ( ( Long32 )~MaxLong32 );

    constexpr ULong64 MaxULong64 = ( ( ULong64 )~( ( ULong64 )0 ) );
    constexpr Long64 MaxLong64 = ( ( Long64 )( MaxULong64 >> 1 ) );
    constexpr Long64 MinLong64 = ( ( Long64 )~MaxLong64 );

    constexpr ULongLong MaxULongLong = ( ( ULongLong )~( ( ULongLong )0 ) );
    constexpr LongLong MaxLongLong = ( ( LongLong )( MaxULongLong >> 1 ) );
    constexpr LongLong MinLongLong = ( ( LongLong )~MaxLongLong );

    constexpr SizeT MaxSizeT = ( ( SizeT )~( ( SizeT )0 ) );
    constexpr SSizeT MaxSSizeT = ( ( SSizeT )( MaxSizeT >> 1 ) );
    constexpr SSizeT MinSSizeT = ( ( SSizeT )~MaxSSizeT );

    constexpr UInt MaxUInt = ( ( UInt )~( ( UInt )0 ) );
    constexpr Int MaxInt = ( ( Int )( MaxUInt >> 1 ) );
    constexpr Int  MinInt = ( ( Int )~MaxInt );

    constexpr DWord32 MaxDWord32 = ( ( DWord32 )~( ( DWord32 )0 ) );
    constexpr DWord64 MaxDWord64 = ( ( DWord64 )~( ( DWord64 )0 ) );

    template<typename T>
    using Nullable = std::optional<T>;

}

#endif