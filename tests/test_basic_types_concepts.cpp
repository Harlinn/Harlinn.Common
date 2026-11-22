#include <array>
#include <cstddef>
#include <cstdint>
#include <forward_list>
#include <limits>
#include <list>
#include <map>
#include <optional>
#include <ranges>
#include <span>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

#include <catch2/catch_test_macros.hpp>
#include <harlinn/common/BasicTypes.h>
#include <harlinn/common/Concepts.h>
#include <harlinn/common/Utils.h>

namespace
{
    using namespace Harlinn::Common;

    enum class TestFlags : UInt32
    {
        None = 0,
        Read = 1,
        Write = 2,
        Execute = 4
    };
    HCC_DEFINE_ENUM_FLAG_OPERATORS(TestFlags, UInt32);

    struct ComLike
    {
        UInt32 AddRef( ) { return 1; }
        UInt32 Release( ) { return 0; }
    };

    struct NotComLike {};

    struct EmptyType {};

    struct ValueTypeOnly
    {
        using value_type = int;
    };

    struct KeyTypeOnly
    {
        using key_type = int;
    };

    struct MappedTypeOnly
    {
        using mapped_type = int;
    };
}

using namespace Harlinn::Common;

static_assert(std::is_same_v<Harlinn::Common::Nullable<int>, std::optional<int>>);
static_assert(std::is_same_v<Harlinn::Common::Boolean, bool>);
static_assert(std::is_same_v<Harlinn::Common::Int8, std::int8_t>);
static_assert(std::is_same_v<Harlinn::Common::SByte, std::int8_t>);
static_assert(std::is_same_v<Harlinn::Common::UInt8, std::uint8_t>);
static_assert(std::is_same_v<Harlinn::Common::Byte, std::uint8_t>);
static_assert(std::is_same_v<Harlinn::Common::Int16, std::int16_t>);
static_assert(std::is_same_v<Harlinn::Common::UInt16, std::uint16_t>);
static_assert(std::is_same_v<Harlinn::Common::ushort, unsigned short>);
static_assert(std::is_same_v<Harlinn::Common::Int32, std::int32_t>);
static_assert(std::is_same_v<Harlinn::Common::Int, int>);
static_assert(std::is_same_v<Harlinn::Common::UInt32, std::uint32_t>);
static_assert(std::is_same_v<Harlinn::Common::UInt, unsigned int>);
static_assert(std::is_same_v<Harlinn::Common::ULong32, std::uint32_t>);
static_assert(std::is_same_v<Harlinn::Common::Long32, std::int32_t>);
static_assert(std::is_same_v<Harlinn::Common::uint, unsigned int>);
static_assert(std::is_same_v<Harlinn::Common::DWord32, std::uint32_t>);
static_assert(std::is_same_v<Harlinn::Common::DWord, std::uint32_t>);
static_assert(std::is_same_v<Harlinn::Common::Int64, std::int64_t>);
static_assert(std::is_same_v<Harlinn::Common::Long64, std::int64_t>);
static_assert(std::is_same_v<Harlinn::Common::LongLong, long long>);
static_assert(std::is_same_v<Harlinn::Common::UInt64, std::uint64_t>);
static_assert(std::is_same_v<Harlinn::Common::ULong64, std::uint64_t>);
static_assert(std::is_same_v<Harlinn::Common::ULongLong, unsigned long long>);
static_assert(std::is_same_v<Harlinn::Common::DWord64, std::uint64_t>);
static_assert(std::is_same_v<Harlinn::Common::ssize_t, std::make_signed_t<std::size_t>>);
static_assert(std::is_same_v<Harlinn::Common::SSizeT, Harlinn::Common::ssize_t>);
static_assert(std::is_same_v<Harlinn::Common::SizeT, std::size_t>);
static_assert(std::is_same_v<Harlinn::Common::Char, char>);
static_assert(std::is_same_v<Harlinn::Common::WideChar, wchar_t>);
static_assert(std::is_same_v<Harlinn::Common::Char8, char8_t>);
static_assert(std::is_same_v<Harlinn::Common::Char16, char16_t>);
static_assert(std::is_same_v<Harlinn::Common::Char32, char32_t>);
static_assert(std::is_same_v<Harlinn::Common::Single, float>);
static_assert(std::is_same_v<Harlinn::Common::Double, double>);
static_assert(Harlinn::Common::MaxUInt8 == std::numeric_limits<Harlinn::Common::UInt8>::max());
static_assert(Harlinn::Common::MinInt8 == std::numeric_limits<Harlinn::Common::Int8>::min());
static_assert(Harlinn::Common::MaxUInt16 == std::numeric_limits<Harlinn::Common::UInt16>::max());
static_assert(Harlinn::Common::MinInt16 == std::numeric_limits<Harlinn::Common::Int16>::min());
static_assert(Harlinn::Common::MaxUInt32 == std::numeric_limits<Harlinn::Common::UInt32>::max());
static_assert(Harlinn::Common::MinInt32 == std::numeric_limits<Harlinn::Common::Int32>::min());
static_assert(Harlinn::Common::MaxUInt64 == std::numeric_limits<Harlinn::Common::UInt64>::max());
static_assert(Harlinn::Common::MinInt64 == std::numeric_limits<Harlinn::Common::Int64>::min());
static_assert(Harlinn::Common::MaxULong32 == Harlinn::Common::MaxUInt32);
static_assert(Harlinn::Common::MaxLong32 == Harlinn::Common::MaxInt32);
static_assert(Harlinn::Common::MinLong32 == Harlinn::Common::MinInt32);
static_assert(Harlinn::Common::MaxULong64 == Harlinn::Common::MaxUInt64);
static_assert(Harlinn::Common::MaxLong64 == Harlinn::Common::MaxInt64);
static_assert(Harlinn::Common::MinLong64 == Harlinn::Common::MinInt64);
static_assert(Harlinn::Common::MaxULongLong == std::numeric_limits<unsigned long long>::max());
static_assert(Harlinn::Common::MaxLongLong == std::numeric_limits<long long>::max());
static_assert(Harlinn::Common::MinLongLong == std::numeric_limits<long long>::min());
static_assert(Harlinn::Common::MaxSizeT == std::numeric_limits<std::size_t>::max());
static_assert(Harlinn::Common::MaxSSizeT == std::numeric_limits<Harlinn::Common::SSizeT>::max());
static_assert(Harlinn::Common::MinSSizeT == std::numeric_limits<Harlinn::Common::SSizeT>::min());
static_assert(Harlinn::Common::MaxUInt == std::numeric_limits<unsigned int>::max());
static_assert(Harlinn::Common::MaxInt == std::numeric_limits<int>::max());
static_assert(Harlinn::Common::MinInt == std::numeric_limits<int>::min());
static_assert(Harlinn::Common::MaxDWord32 == Harlinn::Common::MaxUInt32);
static_assert(Harlinn::Common::MaxDWord64 == Harlinn::Common::MaxUInt64);
static_assert(1_kb == 1024ULL);
static_assert(2_mb == 2ULL * 1024 * 1024);
static_assert(3_gb == 3ULL * 1024 * 1024 * 1024);

static_assert(CharType<char> && CharType<wchar_t> && CharType<char8_t> && CharType<char16_t> && CharType<char32_t>);
static_assert(!CharType<int>);
static_assert(ApiCharType<char> && ApiCharType<wchar_t> && !ApiCharType<char8_t>);
static_assert(InputRange<std::vector<int>> && !InputRange<int>);
static_assert(OutputRange<std::vector<int>&, int> && !OutputRange<const std::vector<int>&, int>);
static_assert(ForwardRange<std::forward_list<int>>);
static_assert(BidirectionalRange<std::list<int>>);
static_assert(RandomAccessRange<std::vector<int>>);
static_assert(ContiguousRange<std::array<int, 4>>);
static_assert(Range<std::vector<int>> && !Range<int>);
static_assert(BorrowedRange<std::span<int>> && !BorrowedRange<std::vector<int>>);
static_assert(SizedRange<std::array<int, 4>> && !SizedRange<std::ranges::subrange<int*, std::unreachable_sentinel_t>>);
static_assert(View<std::span<int>> && !View<std::vector<int>>);
static_assert(CommonRange<std::vector<int>>);
static_assert(ViewableRange<std::vector<int>> && !ViewableRange<int>);
static_assert(ConstantRange<std::span<const int>> && !ConstantRange<std::span<int>>);
static_assert(ContiguousContainerLike<std::vector<int>> && !ContiguousContainerLike<std::list<int>>);
static_assert(HasValueType<std::vector<int>> && !HasValueType<EmptyType>);
static_assert(ValueTypeIs<std::vector<int>, int> && !ValueTypeIs<std::vector<int>, double>);
static_assert(HasKeyType<std::map<int, double>> && !HasKeyType<EmptyType>);
static_assert(KeyTypeIs<std::map<int, double>, int> && !KeyTypeIs<std::map<int, double>, double>);
static_assert(HasMappedType<std::map<int, double>> && !HasMappedType<EmptyType>);
static_assert(MappedTypeIs<std::map<int, double>, double> && !MappedTypeIs<std::map<int, double>, int>);
static_assert(SimpleSpanLike<std::vector<int>>);
static_assert(SimpleCharSpanLike<std::string> && SimpleAnsiCharSpanLike<std::string>);
static_assert(SimpleWideCharSpanLike<std::wstring>);
static_assert(SimpleByteSpanLike<std::vector<Byte>>);
static_assert(SpanLike<std::vector<int>>);
static_assert(CharSpanLike<std::string> && AnsiCharSpanLike<std::string>);
static_assert(WideCharSpanLike<std::wstring>);
static_assert(SimpleStringLike<std::string> && SimpleAnsiStringLike<std::string>);
static_assert(SimpleWideStringLike<std::wstring>);
static_assert(StringLike<std::string> && AnsiStringLike<std::string>);
static_assert(WideStringLike<std::wstring>);
static_assert(SimpleComLike<ComLike> && !SimpleComLike<NotComLike>);
static_assert(BinaryLike<std::vector<Byte>> && !BinaryLike<std::vector<int>>);
static_assert(BooleanType<bool> && !BooleanType<int>);
static_assert(FloatingPointType<float> && !FloatingPointType<int>);
static_assert(IntegerType<int> && !IntegerType<bool> && !IntegerType<float>);
static_assert(SignedIntegerType<int> && !SignedIntegerType<unsigned>);
static_assert(UnsignedIntegerType<unsigned> && !UnsignedIntegerType<int>);
static_assert(UnsignedIntegerOrBooleanType<unsigned> && UnsignedIntegerOrBooleanType<bool> && !UnsignedIntegerOrBooleanType<int>);
static_assert(ArithmeticType<int> && ArithmeticType<double> && !ArithmeticType<bool>);

TEST_CASE("Verify functionality implemented by the HCC_DEFINE_ENUM_FLAG_OPERATORS macro that adds enum flags support operators and queries", "[basic-types]")
{
    using namespace Harlinn::Common;
    using ::TestFlags;

    constexpr auto combined = TestFlags::Read | TestFlags::Write;
    STATIC_REQUIRE((combined & TestFlags::Read) == TestFlags::Read);
    STATIC_REQUIRE((combined ^ TestFlags::Read) == TestFlags::Write);
    STATIC_REQUIRE((~TestFlags::None) == static_cast<TestFlags>(~UInt32{0}));
    STATIC_REQUIRE(TestFlags::Read == UInt32{1});
    STATIC_REQUIRE(UInt32{1} == TestFlags::Read);
    STATIC_REQUIRE(TestFlags::Read != UInt32{2});
    STATIC_REQUIRE(UInt32{2} != TestFlags::Read);

    auto flags = TestFlags::Read;
    flags |= TestFlags::Write;
    REQUIRE(flags == combined);
    flags &= TestFlags::Read;
    REQUIRE(flags == TestFlags::Read);
    flags ^= TestFlags::Execute;
    REQUIRE(flags == (TestFlags::Read | TestFlags::Execute));

    REQUIRE(ContainsAllOf(combined, TestFlags::Read, TestFlags::Write));
    REQUIRE_FALSE(ContainsAllOf(combined, TestFlags::Read, TestFlags::Execute));
    REQUIRE(ContainsAnyOf(combined, TestFlags::Write, TestFlags::Execute));
    REQUIRE_FALSE(ContainsAnyOf(combined, TestFlags::Execute, TestFlags::None));
    REQUIRE(ContainsNoneOf(combined, TestFlags::Execute, TestFlags::None));
    REQUIRE_FALSE(ContainsNoneOf(combined, TestFlags::Read, TestFlags::Execute));
    REQUIRE(IsSet(combined, TestFlags::Read, TestFlags::Write));
    REQUIRE_FALSE(IsSet(combined, TestFlags::Read, TestFlags::Execute));
    REQUIRE(IsNotSet(combined, TestFlags::Execute, TestFlags::None));
    REQUIRE_FALSE(IsNotSet(combined, TestFlags::Write, TestFlags::Execute));
}

TEST_CASE("Verify the concepts that recognizes ranges, containers and strings together with pointers to COM-like objects", "[concepts]")
{
    STATIC_REQUIRE(InputRange<std::string_view>);
    STATIC_REQUIRE(ForwardRange<std::array<int, 2>>);
    STATIC_REQUIRE(RandomAccessRange<std::span<int>>);
    STATIC_REQUIRE(ContiguousRange<std::string>);
    STATIC_REQUIRE(SizedRange<std::span<int>>);
    STATIC_REQUIRE(ContiguousContainerLike<std::string>);
    STATIC_REQUIRE(SimpleSpanLike<std::wstring>);
    STATIC_REQUIRE(SpanLike<std::wstring>);
    STATIC_REQUIRE(BinaryLike<std::vector<Byte>>);
    STATIC_REQUIRE(SimpleComLike<ComLike>);
}
