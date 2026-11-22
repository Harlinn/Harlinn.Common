#include <array>
#include <bit>
#include <cstdint>
#include <cstring>
#include <cwchar>
#include <string>
#include <vector>

#include <catch2/catch_test_macros.hpp>
#include <harlinn/common/Utils.h>

namespace
{
    struct CompareToValue
    {
        int value;
        int CompareTo(const CompareToValue& other) const { return value - other.value; }
    };
}

static_assert(Harlinn::Common::GetHigh32Bits<Harlinn::Common::UInt32>(0x123456789ABCDEF0ULL) == 0x12345678u);
static_assert(Harlinn::Common::GetLow32Bits<Harlinn::Common::UInt32>(0x123456789ABCDEF0ULL) == 0x9ABCDEF0u);
static_assert(Harlinn::Common::SetHigh32Bits(0x000000009ABCDEF0ULL, Harlinn::Common::UInt32{0x12345678}) == 0x123456789ABCDEF0ULL);
static_assert(Harlinn::Common::SetLow32Bits(0x1234567800000000ULL, Harlinn::Common::UInt32{0x9ABCDEF0}) == 0x123456789ABCDEF0ULL);
static_assert(Harlinn::Common::From32BitsTo64Bits<Harlinn::Common::UInt64>(Harlinn::Common::UInt32{0x12345678}, Harlinn::Common::UInt32{0x9ABCDEF0}) == 0x123456789ABCDEF0ULL);
static_assert(Harlinn::Common::LengthOf(std::array<int, 3>{}) == 3);
static_assert(Harlinn::Common::LengthOf("hello") == 5);
static_assert(Harlinn::Common::Compare("abc", "abd") < 0);
static_assert(Harlinn::Common::Compare(nullptr, "x") < 0);
static_assert(Harlinn::Common::Compare("", nullptr) == 0);
static_assert(Harlinn::Common::Compare(2, 3) < 0);
static_assert(Harlinn::Common::AreNearlyEqual(1.0, 1.00001));
static_assert(!Harlinn::Common::AreNearlyEqual(1.0, 1.01));

TEST_CASE("Utils alignment handles power-of-two and arbitrary boundaries", "[utils][memory]")
{
    using namespace Harlinn::Common;
    alignas(16) std::array<Byte, 40> storage{};
    for (std::size_t offset = 0; offset < 16; ++offset)
    {
        auto* address = storage.data() + offset;
        auto* aligned8 = AlignTo<8>(address);
        REQUIRE(reinterpret_cast<std::uintptr_t>(aligned8) % 8 == 0);
        REQUIRE(aligned8 >= address);
        REQUIRE(static_cast<std::size_t>(aligned8 - address) == AlignedPaddingFor<8>(address));

        auto* aligned3 = AlignTo<3>(address);
        REQUIRE(reinterpret_cast<std::uintptr_t>(aligned3) % 3 == 0);
        REQUIRE(aligned3 >= address);
        REQUIRE(static_cast<std::size_t>(aligned3 - address) == AlignedPaddingFor<3>(address));
    }
    REQUIRE(AlignTo<8>(static_cast<Byte*>(nullptr)) == nullptr);
    REQUIRE(AlignedPaddingFor<3>(static_cast<const Byte*>(nullptr)) == 0);
}

TEST_CASE("Utils split and combine high and low 32-bit words", "[utils][memory]")
{
    using namespace Harlinn::Common;
    constexpr UInt64 original = 0x123456789ABCDEF0ULL;
    REQUIRE(GetHigh32Bits<UInt32>(original) == 0x12345678u);
    REQUIRE(GetLow32Bits<UInt32>(original) == 0x9ABCDEF0u);
    REQUIRE(GetHigh32Bits<Int32>(original) == std::bit_cast<Int32>(UInt32{0x12345678}));
    REQUIRE(SetHigh32Bits(UInt64{0x000000009ABCDEF0ULL}, UInt32{0x12345678}) == original);
    REQUIRE(SetLow32Bits(UInt64{0x1234567800000000ULL}, UInt32{0x9ABCDEF0}) == original);
    REQUIRE(From32BitsTo64Bits<UInt64>(UInt32{0x12345678}, UInt32{0x9ABCDEF0}) == original);

    const double value = std::bit_cast<double>(original);
    REQUIRE(std::bit_cast<UInt64>(SetHigh32Bits(value, UInt32{0x12345678})) == original);
    REQUIRE(std::bit_cast<UInt64>(SetLow32Bits(value, UInt32{0x9ABCDEF0})) == original);
}

TEST_CASE("Utils overlap detection uses half-open ranges", "[utils][memory]")
{
    using namespace Harlinn::Common;
    std::array<Byte, 32> data{};
    REQUIRE(Overlaps(data.data(), 8, data.data() + 4, 8));
    REQUIRE(Overlaps(data.data() + 4, 8, data.data(), 8));
    REQUIRE(Overlaps(data.data(), 8, data.data(), 8));
    REQUIRE_FALSE(Overlaps(data.data(), 8, data.data() + 8, 8));
    REQUIRE_FALSE(Overlaps(data.data(), 0, data.data(), 1));
    REQUIRE_FALSE(Overlaps(nullptr, 1, data.data(), 1));
    REQUIRE_FALSE(Overlaps(data.data(), 1, nullptr, 1));
}

TEST_CASE("Utils character and wide-character memory helpers honor their contracts", "[utils][memory]")
{
    using namespace Harlinn::Common;
    char source[] = "abcdef";
    char destination[8]{};
    MemCopy(static_cast<char*>(nullptr), static_cast<const char*>(nullptr), 0);
    MemCopyOrZero(static_cast<char*>(nullptr), static_cast<const char*>(nullptr), 0);
    MemMove(static_cast<char*>(nullptr), static_cast<const char*>(nullptr), 0);
    MemMoveOrZero(static_cast<char*>(nullptr), static_cast<const char*>(nullptr), 0);
    MemSet(nullptr, 'x', 0);
    REQUIRE(MemCmp(static_cast<const char*>(nullptr), static_cast<const char*>(nullptr), 0) == 0);
    REQUIRE(MemICmp(static_cast<const char*>(nullptr), static_cast<const char*>(nullptr), 0) == 0);
    MemCopy(destination, source, 6);
    REQUIRE(std::memcmp(destination, source, 6) == 0);
    MemCopyOrZero(destination, nullptr, 3);
    REQUIRE(destination[0] == 0);
    REQUIRE(destination[1] == 0);
    REQUIRE(destination[2] == 0);
    MemCopyOrZero(destination, source, 3);
    REQUIRE(std::memcmp(destination, source, 3) == 0);
    MemSet(destination, 'x', 4);
    REQUIRE(std::string(destination, 4) == "xxxx");
    char overlap[] = "abcdef";
    MemMove(overlap + 2, overlap, 4);
    REQUIRE(std::string(overlap, 6) == "ababcd");
    MemMove(destination + 1, destination, 3);
    REQUIRE(std::string(destination, 4) == "xxxx");
    MemMoveOrZero(destination, nullptr, 4);
    REQUIRE((destination[0] == 0 && destination[3] == 0));
    REQUIRE(MemCmp("abc", "abd", 3) < 0);
    REQUIRE(MemCmp("abc", "abc", 3) == 0);
    REQUIRE(MemCmp("", "", 0) == 0);
    REQUIRE(MemICmp("AbC", "aBc", 3) == 0);
    REQUIRE(MemChr(source, 'd', 6) == source + 3);
    const char* constSource = source;
    REQUIRE(MemChr(constSource, 'd', 6) == constSource + 3);
    REQUIRE(MemChr(source, 'z', 6) == nullptr);
    REQUIRE(MemChr(static_cast<const char*>(nullptr), 'z', 0) == nullptr);

    wchar_t wideSource[] = L"abcdef";
    wchar_t wideDestination[8]{};
    MemCopy(static_cast<wchar_t*>(nullptr), static_cast<const wchar_t*>(nullptr), 0);
    MemCopyOrZero(static_cast<wchar_t*>(nullptr), static_cast<const wchar_t*>(nullptr), 0);
    MemMove(static_cast<wchar_t*>(nullptr), static_cast<const wchar_t*>(nullptr), 0);
    MemMoveOrZero(static_cast<wchar_t*>(nullptr), static_cast<const wchar_t*>(nullptr), 0);
    MemSet(nullptr, L'x', 0);
    REQUIRE(MemCmp(static_cast<const wchar_t*>(nullptr), nullptr, 0) == 0);
    REQUIRE(MemICmp(static_cast<const wchar_t*>(nullptr), nullptr, 0) == 0);
    MemCopy(wideDestination, wideSource, 6);
    REQUIRE(wmemcmp(wideDestination, wideSource, 6) == 0);
    MemCopyOrZero(wideDestination, nullptr, 3);
    REQUIRE((wideDestination[0] == 0 && wideDestination[2] == 0));
    MemCopyOrZero(wideDestination, wideSource, 3);
    REQUIRE(wmemcmp(wideDestination, wideSource, 3) == 0);
    MemSet(wideDestination, L'Q', 4);
    REQUIRE(wideDestination[3] == L'Q');
    MemMove(wideDestination + 1, wideDestination, 3);
    REQUIRE((wideDestination[1] == L'Q' && wideDestination[3] == L'Q'));
    MemMoveOrZero(wideDestination, nullptr, 4);
    REQUIRE((wideDestination[0] == 0 && wideDestination[3] == 0));
    REQUIRE(MemCmp(L"abc", L"abd", 3) < 0);
    REQUIRE(MemCmp(L"", L"", 0) == 0);
    REQUIRE(MemICmp(L"AbC", L"aBc", 3) == 0);
    const wchar_t* constWideSource = wideSource;
    REQUIRE(MemChr(wideSource, L'd', 6) == wideSource + 3);
    REQUIRE(MemChr(constWideSource, L'd', 6) == constWideSource + 3);
    REQUIRE(MemChr(wideSource, L'z', 6) == nullptr);
}

TEST_CASE("Utils length helpers support arrays, strings, and containers", "[utils][memory]")
{
    using namespace Harlinn::Common;
    int array[] = {1, 2, 3, 4};
    char text[] = "hello";
    wchar_t wideText[] = L"world";
    REQUIRE(LengthOf(array) == 4);
    REQUIRE(LengthOf(text) == 5);
    REQUIRE(LengthOf(wideText) == 5);
    REQUIRE(LengthOf(static_cast<const char*>(nullptr)) == 0);
    REQUIRE(LengthOf(static_cast<const wchar_t*>(nullptr)) == 0);
    REQUIRE(LengthOf("") == 0);
    REQUIRE(LengthOf(L"") == 0);
    REQUIRE(LengthOf(std::string("hello")) == 5);
    REQUIRE(LengthOf(std::vector<int>{1, 2, 3}) == 3);
}

TEST_CASE("Utils Compare overloads normalize ordering and null string values", "[utils][memory]")
{
    using namespace Harlinn::Common;
    REQUIRE(Compare(nullptr, nullptr) == 0);
    REQUIRE(Compare(nullptr, "") == 0);
    REQUIRE(Compare(nullptr, "x") < 0);
    REQUIRE(Compare("x", nullptr) > 0);
    REQUIRE(Compare("", nullptr) == 0);
    REQUIRE(Compare("same", "same") == 0);
    REQUIRE(Compare("abc", "abd") < 0);
    REQUIRE(Compare("abd", "abc") > 0);
    REQUIRE(Compare(L"abc", L"abd") < 0);
    REQUIRE(Compare(1, 2) < 0);
    REQUIRE(Compare(2, 1) > 0);
    REQUIRE(Compare(2.0, 2.0) == 0);
    REQUIRE(Compare(CompareToValue{1}, CompareToValue{4}) < 0);
    REQUIRE(Compare(std::string("a"), std::string("b")) < 0);
    REQUIRE(AreNearlyEqual(1.0, 1.00005, 0.0001));
    REQUIRE_FALSE(AreNearlyEqual(1.0, 1.001, 0.0001));
    REQUIRE(AreNearlyEqual(-1.0f, -1.00001f, 0.0001f));
}
