#include <array>
#include <bit>
#include <cstdint>
#include <limits>
#include <type_traits>

#include <catch2/catch_test_macros.hpp>
#include <harlinn/common/Utils.h>

namespace
{
    using namespace Harlinn::Common;

    template<typename T>
    constexpr T ReverseBitsReference(T value)
    {
        using U = std::make_unsigned_t<T>;
        U remaining = std::bit_cast<U>(value);
        U reversed = 0;
        for (unsigned i = 0; i < sizeof(T) * 8; ++i)
        {
            reversed = static_cast<U>((reversed << 1) | (remaining & U{1}));
            remaining >>= 1;
        }
        return std::bit_cast<T>(reversed);
    }

    template<typename T>
    concept CanCreateIntegerMask = requires { MaskTrailingOnes<T, 2>(); };

    template<typename T>
    concept CanUseIntegerByteSwap = requires(T value) { ByteSwap(value); };

    template<typename T>
    void CheckBitScans(T value)
    {
        using U = std::make_unsigned_t<T>;
        const U bits = std::bit_cast<U>(value);
        UInt32 expectedLsb = 0;
        UInt32 expectedMsb = 0;
        bool found = false;
        for (UInt32 bit = 0; bit < sizeof(T) * 8; ++bit)
        {
            if ((bits & (U{1} << bit)) != 0)
            {
                if (!found)
                {
                    expectedLsb = bit;
                    found = true;
                }
                expectedMsb = bit;
            }
        }

        UInt32 index = 0xFFFFFFFFu;
        REQUIRE(IndexOfBitFromLSB(&index, value) == found);
        if (found) REQUIRE(index == expectedLsb);
        index = 0xFFFFFFFFu;
        REQUIRE(IndexOfBitFromMSB(&index, value) == found);
        if (found) REQUIRE(index == expectedMsb);
        REQUIRE(IndexOfBitFromLSB(value).has_value() == found);
        REQUIRE(IndexOfBitFromMSB(value).has_value() == found);
        if (found)
        {
            REQUIRE(*IndexOfBitFromLSB(value) == expectedLsb);
            REQUIRE(*IndexOfBitFromMSB(value) == expectedMsb);
        }
        REQUIRE(IndexOfBitFromLSB(nullptr, value) == found);
        REQUIRE(IndexOfBitFromMSB(nullptr, value) == found);
    }
}

static_assert(std::is_same_v<Harlinn::Common::MakeUnsigned<Harlinn::Common::Int8>, Harlinn::Common::Byte>);
static_assert(std::is_same_v<Harlinn::Common::MakeUnsigned<Harlinn::Common::Int16>, Harlinn::Common::UInt16>);
static_assert(std::is_same_v<Harlinn::Common::MakeUnsigned<Harlinn::Common::Int32>, Harlinn::Common::UInt32>);
static_assert(std::is_same_v<Harlinn::Common::MakeUnsigned<Harlinn::Common::Int64>, Harlinn::Common::UInt64>);
static_assert(Harlinn::Common::HasSameSizeAndAlignment_v<float, Harlinn::Common::UInt32>);
static_assert(!Harlinn::Common::HasSameSizeAndAlignment_v<float, Harlinn::Common::UInt64>);
static_assert(CanCreateIntegerMask<Harlinn::Common::Int32>);
static_assert(!CanCreateIntegerMask<float>);
static_assert(CanUseIntegerByteSwap<Harlinn::Common::UInt32>);
static_assert(CanUseIntegerByteSwap<float>);
static_assert(!CanUseIntegerByteSwap<bool>);
static_assert(Harlinn::Common::FractionMask<int, Harlinn::Common::UInt32> == 0xFFFFFFFFu);
static_assert(Harlinn::Common::ExponentMask<int, Harlinn::Common::UInt32> == 0u);
static_assert(Harlinn::Common::FractionMask<float> == 0x007FFFFFu);
static_assert(Harlinn::Common::ExponentMask<float> == 0x7F800000u);
static_assert(Harlinn::Common::FractionMask<double> == 0x000FFFFFFFFFFFFFULL);
static_assert(Harlinn::Common::ExponentMask<double> == 0x7FF0000000000000ULL);
static_assert(sizeof(Harlinn::Common::Bits8) == 1);
static_assert(sizeof(Harlinn::Common::Bits16) == 2);
static_assert(sizeof(Harlinn::Common::Bits32) == 4);
static_assert(sizeof(Harlinn::Common::Bits64) == 8);
static_assert(Harlinn::Common::ByteSwap(Harlinn::Common::UInt16{0x1234}) == 0x3412);
static_assert(Harlinn::Common::ByteSwap(Harlinn::Common::UInt32{0x12345678}) == 0x78563412);
static_assert(Harlinn::Common::ByteSwap(Harlinn::Common::UInt64{0x0123456789ABCDEFULL}) == 0xEFCDAB8967452301ULL);
static_assert(Harlinn::Common::BitMask<0>::value == 0);
static_assert(Harlinn::Common::BitMask<1>::value == 1);
static_assert(Harlinn::Common::BitMask<8>::value == 0xFF);
static_assert(Harlinn::Common::BitMask<9>::value == 0x1FF);
static_assert(Harlinn::Common::BitMask<16>::value == 0xFFFF);
static_assert(Harlinn::Common::BitMask<32>::value == 0xFFFFFFFFu);
static_assert(Harlinn::Common::BitMask<64>::value == 0xFFFFFFFFFFFFFFFFULL);
static_assert(std::is_same_v<Harlinn::Common::BitMask_t<0>, Harlinn::Common::Byte>);
static_assert(std::is_same_v<Harlinn::Common::BitMask_t<8>, Harlinn::Common::Byte>);
static_assert(std::is_same_v<Harlinn::Common::BitMask_t<9>, Harlinn::Common::UInt16>);
static_assert(std::is_same_v<Harlinn::Common::BitMask_t<17>, Harlinn::Common::UInt32>);
static_assert(std::is_same_v<Harlinn::Common::BitMask_t<33>, Harlinn::Common::UInt64>);
static_assert(Harlinn::Common::MaskTrailingOnes<Harlinn::Common::UInt8, 0>() == 0);
static_assert(Harlinn::Common::MaskTrailingOnes<Harlinn::Common::UInt8, 8>() == 0xFF);
static_assert(Harlinn::Common::MaskLeadingOnes<Harlinn::Common::UInt16, 5>() == 0xF800);
static_assert(Harlinn::Common::MaskTrailingZeros<Harlinn::Common::UInt32, 4>() == 0xFFFFFFF0u);
static_assert(Harlinn::Common::MaskLeadingZeros<Harlinn::Common::UInt64, 8>() == 0x00FFFFFFFFFFFFFFULL);
static_assert(Harlinn::Common::ReverseBits(Harlinn::Common::Byte{0x01}) == 0x80);
static_assert(Harlinn::Common::IndexOfBitFromLSB(Harlinn::Common::UInt32{0x80000000u}).value() == 31);
static_assert(Harlinn::Common::IndexOfBitFromMSB(Harlinn::Common::UInt64{0x8000000000000001ULL}).value() == 63);
static_assert(Harlinn::Common::CreateBitMask32(0) == 0);
static_assert(Harlinn::Common::CreateBitMask32(32) == 0xFFFFFFFFu);
static_assert(Harlinn::Common::CreateBitMask64(64) == 0xFFFFFFFFFFFFFFFFULL);
static_assert(Harlinn::Common::ExtractBits32(0xFEDCBA98u, 4, 8) == 0xA9u);
static_assert(Harlinn::Common::ExtractBits64(0xFEDCBA9876543210ULL, 60, 16) == 0xFULL);

TEST_CASE("Utils bit swap handles integer and representation types", "[utils][bits]")
{
    using namespace Harlinn::Common;
    REQUIRE(ByteSwap(Byte{0xA5}) == 0xA5);
    REQUIRE(ByteSwap(UInt16{0x1234}) == 0x3412);
    REQUIRE(ByteSwap(UInt32{0x12345678}) == 0x78563412u);
    REQUIRE(ByteSwap(UInt64{0x0123456789ABCDEFULL}) == 0xEFCDAB8967452301ULL);
    REQUIRE(ByteSwap(Int32{0x12345678}) == 0x78563412);
    REQUIRE(ByteSwap(1.0f) == std::bit_cast<float>(UInt32{0x0000803Fu}));
    REQUIRE(ByteSwap(1.0) == std::bit_cast<double>(UInt64{0x000000000000F03FULL}));

    enum class ByteOrder : UInt16 { Value = 0x1234 };
    REQUIRE(ByteSwap(ByteOrder::Value) == static_cast<ByteOrder>(0x3412));
}

TEST_CASE("Utils bit masks cover widths and edge boundaries", "[utils][bits]")
{
    using namespace Harlinn::Common;
    REQUIRE(CreateBitMask32(0) == 0);
    REQUIRE(CreateBitMask32(1) == 1);
    REQUIRE(CreateBitMask32(31) == 0x7FFFFFFFu);
    REQUIRE(CreateBitMask32(32) == 0xFFFFFFFFu);
    REQUIRE(CreateBitMask64(0) == 0);
    REQUIRE(CreateBitMask64(1) == 1);
    REQUIRE(CreateBitMask64(63) == 0x7FFFFFFFFFFFFFFFULL);
    REQUIRE(CreateBitMask64(64) == 0xFFFFFFFFFFFFFFFFULL);

    REQUIRE(MaskTrailingOnes<UInt16, 12>() == 0x0FFF);
    REQUIRE(MaskLeadingOnes<UInt16, 4>() == 0xF000);
    REQUIRE(MaskTrailingZeros<UInt32, 12>() == 0xFFFFF000u);
    REQUIRE(MaskLeadingZeros<UInt32, 12>() == 0x000FFFFFu);
}

TEST_CASE("Utils bit reversal agrees with a bit-by-bit oracle", "[utils][bits]")
{
    using namespace Harlinn::Common;
    const std::array<UInt64, 8> values{
        0, 1, 2, 0xFF, 0x123456789ABCDEF0ULL,
        0x8000000000000000ULL, 0xFFFFFFFFFFFFFFFFULL, 0x0123456789ABCDEFULL
    };
    for (UInt64 value : values)
    {
        REQUIRE(ReverseBits(value) == ReverseBitsReference(value));
        REQUIRE(ReverseBits(std::bit_cast<Int64>(value)) == ReverseBitsReference(std::bit_cast<Int64>(value)));
    }
    REQUIRE(ReverseBits(UInt16{0x1234}) == ReverseBitsReference(UInt16{0x1234}));
    REQUIRE(ReverseBits(Int16{-1234}) == ReverseBitsReference(Int16{-1234}));
    REQUIRE(ReverseBits(UInt32{0x12345678u}) == ReverseBitsReference(UInt32{0x12345678u}));
    REQUIRE(ReverseBits(Int32{-1234567}) == ReverseBitsReference(Int32{-1234567}));
    REQUIRE(ReverseBits(SByte{-128}) == SByte{1});
    REQUIRE(ReverseBits(char{1}) == std::bit_cast<char>(Byte{0x80}));
    REQUIRE(ReverseBits(wchar_t{1}) == ReverseBitsReference(wchar_t{1}));
    REQUIRE(std::bit_cast<UInt32>(ReverseBits(1.0f)) == ReverseBits(std::bit_cast<UInt32>(1.0f)));
    REQUIRE(std::bit_cast<UInt64>(ReverseBits(1.0)) == ReverseBits(std::bit_cast<UInt64>(1.0)));
}

TEST_CASE("Utils bit scans find both ends across integer widths", "[utils][bits]")
{
    CheckBitScans(Harlinn::Common::UInt8{0});
    CheckBitScans(Harlinn::Common::UInt8{0x80});
    CheckBitScans(Harlinn::Common::UInt16{0});
    CheckBitScans(Harlinn::Common::UInt16{0x8100});
    CheckBitScans(Harlinn::Common::UInt32{0});
    CheckBitScans(Harlinn::Common::UInt32{0x80000001u});
    CheckBitScans(Harlinn::Common::UInt64{0});
    CheckBitScans(Harlinn::Common::UInt64{0x8000000000000001ULL});
    CheckBitScans(Harlinn::Common::Int32{-1});
    CheckBitScans(Harlinn::Common::Int64{std::numeric_limits<Harlinn::Common::Int64>::min()});
    CheckBitScans(std::numeric_limits<unsigned long>::max());
}

TEST_CASE("Utils bit extraction clamps to available bits", "[utils][bits]")
{
    using namespace Harlinn::Common;
    REQUIRE(ExtractBits32(0xFEDCBA98u, 0, 0) == 0);
    REQUIRE(ExtractBits32(0xFEDCBA98u, 0, 32) == 0xFEDCBA98u);
    REQUIRE(ExtractBits32(0xFEDCBA98u, 4, 8) == 0xA9u);
    REQUIRE(ExtractBits32(0xFFFFFFFFu, 28, 32) == 0xFu);
    REQUIRE(ExtractBits32(0xFFFFFFFFu, 32, 1) == 0);
    REQUIRE(ExtractBits64(0xFEDCBA9876543210ULL, 0, 64) == 0xFEDCBA9876543210ULL);
    REQUIRE(ExtractBits64(0xFEDCBA9876543210ULL, 60, 16) == 0xFULL);
    REQUIRE(ExtractBits64(0xFFFFFFFFFFFFFFFFULL, 63, 64) == 1);
    REQUIRE(ExtractBits64(0xFFFFFFFFFFFFFFFFULL, 64, 1) == 0);
}

TEST_CASE("Bits views preserve component and scalar bit patterns", "[utils][bits]")
{
    using namespace Harlinn::Common;
    Bits16 b16{0xABCD};
    REQUIRE(b16.Low() == 0xCD);
    REQUIRE(b16.High() == 0xAB);
    b16.SetLow(0x12);
    b16.SetHigh(0x34);
    REQUIRE(b16.Value == 0x3412);

    Bits32 b32{};
    b32.SetLow(Bits16{0x5678});
    b32.SetHigh(Bits16{0x1234});
    REQUIRE(b32.Value == 0x12345678u);
    REQUIRE(b32.Low().Value == 0x5678);
    REQUIRE(b32.High().Value == 0x1234);
    b32.SetInt32Value(-1);
    REQUIRE(b32.GetInt32Value() == -1);
    b32.SetSingleValue(1.25f);
    REQUIRE(b32.GetSingleValue() == 1.25f);

    Bits64 b64{};
    b64.SetLow(Bits32{0x89ABCDEFu});
    b64.SetHigh(Bits32{0x01234567u});
    REQUIRE(b64.Value == 0x0123456789ABCDEFULL);
    REQUIRE(b64.Low().Value == 0x89ABCDEFu);
    REQUIRE(b64.High().Value == 0x01234567u);
    b64.SetInt64Value(std::numeric_limits<Int64>::min());
    REQUIRE(b64.GetInt64Value() == std::numeric_limits<Int64>::min());
    b64.SetDoubleValue(-3.5);
    REQUIRE(b64.GetDoubleValue() == -3.5);
}
