#include <array>
#include <cstdint>
#include <limits>
#include <catch2/catch_test_macros.hpp>
#include <harlinn/common/BasicTypes.h>
#include <harlinn/common/Concepts.h>
#include <harlinn/common/Utils.h>

TEST_CASE("Harlinn.Common memory range overlap", "[common]")
{
    std::array<char, 16> buffer{};

    REQUIRE(Harlinn::Common::Overlaps(buffer.data(), 8, buffer.data() + 4, 8));
    REQUIRE_FALSE(Harlinn::Common::Overlaps(buffer.data(), 4, buffer.data() + 4, 4));
    REQUIRE_FALSE(Harlinn::Common::Overlaps(buffer.data(), 0, buffer.data(), 1));
    REQUIRE_FALSE(Harlinn::Common::Overlaps(nullptr, 1, buffer.data(), 1));
    REQUIRE(Harlinn::Common::AlignTo<8>(buffer.data()) == buffer.data() + Harlinn::Common::AlignedPaddingFor<8>(buffer.data()));
}

TEST_CASE("Harlinn.Common IndexOfBitFromLSB accepts nullptr output index", "[common]")
{
    REQUIRE(Harlinn::Common::IndexOfBitFromLSB(nullptr, Harlinn::Common::UInt32{1}));
    REQUIRE_FALSE(Harlinn::Common::IndexOfBitFromLSB(nullptr, Harlinn::Common::UInt32{0}));

    Harlinn::Common::UInt32 index = 99;
    REQUIRE(Harlinn::Common::IndexOfBitFromLSB(&index, Harlinn::Common::UInt32{8}));
    REQUIRE(index == 3);
}

TEST_CASE("Harlinn.Common Finally accepts lvalue callables", "[common]")
{
    int calls = 0;
    auto cleanup = [&calls] { ++calls; };

    {
        auto guard = Harlinn::Common::Finally(cleanup);
        REQUIRE(calls == 0);
    }

    REQUIRE(calls == 1);
}

TEST_CASE("Harlinn.Common IsPowerOfTwo rejects negative signed values", "[common]")
{
    using Int32 = Harlinn::Common::Int32;

    REQUIRE_FALSE(Harlinn::Common::IsPowerOfTwo(Int32{-1}));
    REQUIRE_FALSE(Harlinn::Common::IsPowerOfTwo(std::numeric_limits<Int32>::min()));
    REQUIRE(Harlinn::Common::IsPowerOfTwo(Int32{1}));
    REQUIRE(Harlinn::Common::IsPowerOfTwo(Int32{8}));
    REQUIRE_FALSE(Harlinn::Common::IsPowerOfTwo(Int32{0}));
}

TEST_CASE("Harlinn.Common::BitsXX bit views handles representation aliasing", "[common]")
{
    Harlinn::Common::Bits16 bits16{};
    bits16.SetLow(0x34);
    bits16.SetHigh(0x12);
    REQUIRE(bits16.Value == 0x1234);

    Harlinn::Common::Bits32 bits32{};
    bits32.SetLow(bits16);
    bits32.SetHigh({0x5678});
    REQUIRE(bits32.Value == 0x56781234);
    REQUIRE(bits32.Low().Value == 0x1234);
    REQUIRE(bits32.High().Value == 0x5678);

    Harlinn::Common::Bits64 bits64{};
    bits64.SetLow(bits32);
    bits64.SetHigh({0x9ABCDEF0});
    REQUIRE(bits64.Value == 0x9ABCDEF056781234ULL);
    REQUIRE(bits64.Low().Value == 0x56781234);
    REQUIRE(bits64.High().Value == 0x9ABCDEF0);
}

TEST_CASE("Harlinn.Common ReverseBits supports long types", "[common]")
{
    REQUIRE(Harlinn::Common::ReverseBits(1L) == std::numeric_limits<long>::min());
    REQUIRE(Harlinn::Common::ReverseBits(1UL) == (1UL << (std::numeric_limits<unsigned long>::digits - 1)));
}

enum class TestFlags : std::uint32_t
{
    First = 1,
    Second = 2
};
HCC_DEFINE_ENUM_FLAG_OPERATORS(TestFlags, std::uint32_t);

TEST_CASE("Harlinn.Common enum flags preserve aliasing rules", "[common]")
{
    TestFlags flags = TestFlags::First;
    flags |= TestFlags::Second;
    REQUIRE(flags == (TestFlags::First | TestFlags::Second));
    REQUIRE(static_cast<std::uint32_t>(flags) == 3);
    REQUIRE(Harlinn::Common::ContainsAllOf(flags, TestFlags::First, TestFlags::Second));
}

TEST_CASE("Verify that Harlinn.Common checked arithmetic handles signed limits", "[common]")
{
    using Int32 = Harlinn::Common::Int32;
    constexpr auto min = std::numeric_limits<Int32>::min();
    constexpr auto max = std::numeric_limits<Int32>::max();

    REQUIRE_FALSE(Harlinn::Common::IsAdditionSafe(max, Int32{1}));
    REQUIRE(Harlinn::Common::SafeAddition(max, Int32{1}) == max);
    REQUIRE_FALSE(Harlinn::Common::IsMultiplicationSafe(min, Int32{-1}));
    REQUIRE(Harlinn::Common::SafeMultiplication(min, Int32{-1}) == max);
    REQUIRE_FALSE(Harlinn::Common::IsDivisionSafe(min, Int32{-1}));
    REQUIRE(Harlinn::Common::SafeDivision(min, Int32{-1}) == max);
}

static_assert(sizeof(Harlinn::Common::Int32) == 4);
static_assert(sizeof(Harlinn::Common::UInt32) == 4);
static_assert(sizeof(Harlinn::Common::Long32) == 4);
static_assert(sizeof(Harlinn::Common::DWord64) == 8);
