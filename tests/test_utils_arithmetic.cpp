#include <array>
#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <limits>
#include <memory>
#include <type_traits>

#include <catch2/catch_test_macros.hpp>
#include <harlinn/common/Utils.h>

namespace
{
    using namespace Harlinn::Common;

    struct ReleaseTracked
    {
        int* releases;

        void Release()
        {
            ++*releases;
            delete this;
        }
    };

    template<typename T>
    void CheckSignedArithmeticBoundaries()
    {
        constexpr T min = std::numeric_limits<T>::min();
        constexpr T max = std::numeric_limits<T>::max();
        REQUIRE(!IsAdditionSafe(max, T{1}));
        REQUIRE(SafeAddition(max, T{1}) == max);
        REQUIRE(!IsAdditionSafe(min, T{-1}));
        REQUIRE(SafeAddition(min, T{-1}) == min);
        REQUIRE(IsAdditionSafe(min, max));
        REQUIRE(SafeAddition(min, max) == T{-1});
        REQUIRE(IsSubtractionSafe(min, T{0}));
        REQUIRE(!IsSubtractionSafe(min, T{1}));
        REQUIRE(SafeSubtraction(min, T{1}) == min);
        REQUIRE(!IsSubtractionSafe(max, T{-1}));
        REQUIRE(SafeSubtraction(max, T{-1}) == max);
        REQUIRE(IsSubtractionSafe(T{7}, T{7}));
        REQUIRE(SafeSubtraction(T{7}, T{7}) == T{0});
        REQUIRE(!IsMultiplicationSafe(min, T{-1}));
        REQUIRE(SafeMultiplication(min, T{-1}) == max);
        REQUIRE(!IsMultiplicationSafe(max, T{2}));
        REQUIRE(SafeMultiplication(max, T{2}) == max);
        REQUIRE(!IsMultiplicationSafe(min, T{2}));
        REQUIRE(SafeMultiplication(min, T{2}) == min);
        REQUIRE(IsMultiplicationSafe(T{-3}, T{-4}));
        REQUIRE(SafeMultiplication(T{-3}, T{-4}) == T{12});
        REQUIRE(IsMultiplicationSafe(T{0}, min));
        REQUIRE(SafeMultiplication(T{0}, min) == T{0});
        REQUIRE(IsDivisionSafe(T{12}, T{3}));
        REQUIRE(SafeDivision(T{12}, T{3}) == T{4});
        REQUIRE(!IsDivisionSafe(T{1}, T{0}));
        REQUIRE(SafeDivision(T{1}, T{0}) == max);
        REQUIRE(SafeDivision(T{-1}, T{0}) == min);
        REQUIRE(!IsDivisionSafe(min, T{-1}));
        REQUIRE(SafeDivision(min, T{-1}) == max);
    }

    template<typename T>
    void CheckUnsignedArithmeticBoundaries()
    {
        constexpr T max = std::numeric_limits<T>::max();
        REQUIRE(!IsAdditionSafe(max, T{1}));
        REQUIRE(SafeAddition(max, T{1}) == max);
        REQUIRE(IsAdditionSafe(T{0}, max));
        REQUIRE(SafeAddition(T{0}, max) == max);
        REQUIRE(IsSubtractionSafe(T{9}, T{9}));
        REQUIRE(SafeSubtraction(T{9}, T{9}) == T{0});
        REQUIRE(!IsSubtractionSafe(T{0}, T{1}));
        REQUIRE(SafeSubtraction(T{0}, T{1}) == T{0});
        REQUIRE(IsMultiplicationSafe(T{0}, max));
        REQUIRE(SafeMultiplication(T{0}, max) == T{0});
        REQUIRE(IsMultiplicationSafe(T{3}, T{7}));
        REQUIRE(SafeMultiplication(T{3}, T{7}) == T{21});
        REQUIRE(!IsMultiplicationSafe(max, T{2}));
        REQUIRE(SafeMultiplication(max, T{2}) == max);
        REQUIRE(IsDivisionSafe(T{1}, T{1}));
        REQUIRE(SafeDivision(T{9}, T{2}) == T{4});
        REQUIRE(!IsDivisionSafe(T{1}, T{0}));
        REQUIRE(SafeDivision(T{1}, T{0}) == max);
    }
}

static_assert(Harlinn::Common::IsNotBoolean<int>);
static_assert(!Harlinn::Common::IsNotBoolean<const bool&>);
static_assert(Harlinn::Common::IsBoolean<const bool&>);
static_assert(Harlinn::Common::IsInteger<const int&>);
static_assert(Harlinn::Common::IsSignedInteger<const int&>);
static_assert(Harlinn::Common::IsUnsignedInteger<const unsigned&>);
static_assert(Harlinn::Common::IsFloatingPoint<const float&>);
static_assert(Harlinn::Common::IsArithmetic<const double&>);
static_assert(Harlinn::Common::IsStdChronoDuration<std::chrono::milliseconds>);
static_assert(Harlinn::Common::IsStdChronoDuration<const std::chrono::seconds&>);
static_assert(!Harlinn::Common::IsStdChronoDuration<int>);
static_assert(Harlinn::Common::IsPowerOfTwo(Harlinn::Common::UInt8{1}));
static_assert(Harlinn::Common::IsPowerOfTwo(Harlinn::Common::UInt16{16}));
static_assert(Harlinn::Common::IsPowerOfTwo(Harlinn::Common::UInt32{1024}));
static_assert(Harlinn::Common::IsPowerOfTwo(Harlinn::Common::UInt64{1} << 63));
static_assert(!Harlinn::Common::IsPowerOfTwo(Harlinn::Common::Int32{-1}));
static_assert(!Harlinn::Common::IsPowerOfTwo(std::numeric_limits<Harlinn::Common::Int32>::min()));
static_assert(!Harlinn::Common::IsPowerOfTwo(Harlinn::Common::UInt32{0}));
static_assert(Harlinn::Common::NextPowerOfTwo(Harlinn::Common::UInt8{3}) == 4);
static_assert(Harlinn::Common::NextPowerOfTwo(Harlinn::Common::UInt16{257}) == 512);
static_assert(Harlinn::Common::NextPowerOfTwo(Harlinn::Common::UInt32{0}) == 0);
static_assert(Harlinn::Common::NextPowerOfTwo(Harlinn::Common::UInt64{0x8000000000000001ULL}) == 0);
static_assert(Harlinn::Common::NextPowerOfTwo(Harlinn::Common::Int32{-1}) == 0);
static_assert(Harlinn::Common::NextPowerOfTwo(Harlinn::Common::Int32{0}) == 0);
static_assert(Harlinn::Common::NextPowerOfTwo(Harlinn::Common::Int32{17}) == 32);
static_assert(Harlinn::Common::NextPowerOfTwo(std::numeric_limits<Harlinn::Common::Int32>::max()) == std::numeric_limits<Harlinn::Common::Int32>::max());
static_assert(Harlinn::Common::IsAdditionSafe(Harlinn::Common::Int32{1}, Harlinn::Common::Int32{2}));
static_assert(!Harlinn::Common::IsAdditionSafe(std::numeric_limits<Harlinn::Common::Int32>::max(), Harlinn::Common::Int32{1}));
static_assert(Harlinn::Common::SafeAddition(std::numeric_limits<Harlinn::Common::Int32>::max(), Harlinn::Common::Int32{1}) == std::numeric_limits<Harlinn::Common::Int32>::max());
static_assert(Harlinn::Common::IsSubtractionSafe(Harlinn::Common::UInt32{3}, Harlinn::Common::UInt32{2}));
static_assert(Harlinn::Common::SafeSubtraction(Harlinn::Common::UInt32{1}, Harlinn::Common::UInt32{2}) == 0);
static_assert(Harlinn::Common::IsMultiplicationSafe(Harlinn::Common::UInt32{7}, Harlinn::Common::UInt32{8}));
static_assert(Harlinn::Common::SafeMultiplication(Harlinn::Common::UInt32{7}, Harlinn::Common::UInt32{8}) == 56);
static_assert(Harlinn::Common::IsDivisionSafe(Harlinn::Common::UInt32{9}, Harlinn::Common::UInt32{2}));
static_assert(Harlinn::Common::SafeDivision(Harlinn::Common::UInt32{9}, Harlinn::Common::UInt32{2}) == 4);

TEST_CASE("Utils checked signed arithmetic handles limits for every width", "[utils][arithmetic]")
{
    CheckSignedArithmeticBoundaries<Harlinn::Common::Int8>();
    CheckSignedArithmeticBoundaries<Harlinn::Common::Int16>();
    CheckSignedArithmeticBoundaries<Harlinn::Common::Int32>();
    CheckSignedArithmeticBoundaries<Harlinn::Common::Int64>();
}

TEST_CASE("Utils checked unsigned arithmetic handles limits for every width", "[utils][arithmetic]")
{
    CheckUnsignedArithmeticBoundaries<Harlinn::Common::UInt8>();
    CheckUnsignedArithmeticBoundaries<Harlinn::Common::UInt16>();
    CheckUnsignedArithmeticBoundaries<Harlinn::Common::UInt32>();
    CheckUnsignedArithmeticBoundaries<Harlinn::Common::UInt64>();
}

TEST_CASE("Utils power-of-two helpers cover enum and integer boundaries", "[utils][arithmetic]")
{
    using namespace Harlinn::Common;
    enum class Flags : UInt32 { None = 0, One = 1, Eight = 8, Mixed = 10 };
    REQUIRE(IsPowerOfTwo(Flags::One));
    REQUIRE(IsPowerOfTwo(Flags::Eight));
    REQUIRE_FALSE(IsPowerOfTwo(Flags::None));
    REQUIRE_FALSE(IsPowerOfTwo(Flags::Mixed));
    REQUIRE(IsPowerOf2(UInt32{16}));
    REQUIRE(IsPowerOf2(Int32{16}));
    REQUIRE_FALSE(IsPowerOf2(Int32{-16}));

    REQUIRE(NextPowerOfTwo(SByte{0}) == 0);
    REQUIRE(NextPowerOfTwo(SByte{1}) == 1);
    REQUIRE(NextPowerOfTwo(SByte{3}) == 4);
    REQUIRE(NextPowerOfTwo(SByte{-1}) == 0);
    REQUIRE(NextPowerOfTwo(Int16{32767}) == 32767);
    REQUIRE(NextPowerOfTwo(UInt8{255}) == 0);
    REQUIRE(NextPowerOfTwo(UInt16{65535}) == 0);
    REQUIRE(NextPowerOfTwo(UInt32{0x80000000u}) == 0x80000000u);
    REQUIRE(NextPowerOfTwo(UInt64{0x4000000000000000ULL}) == 0x4000000000000000ULL);
}

TEST_CASE("Utils floating-point bit helpers cover special representations", "[utils][arithmetic]")
{
    using namespace Harlinn::Common;
    const float floatSubnormal = std::numeric_limits<float>::denorm_min();
    const double doubleSubnormal = std::numeric_limits<double>::denorm_min();
    REQUIRE(IsSubnormal(floatSubnormal));
    REQUIRE(IsSubnormal(-floatSubnormal));
    REQUIRE(IsSubnormal(doubleSubnormal));
    REQUIRE(IsSubnormal(-doubleSubnormal));
    REQUIRE_FALSE(IsSubnormal(0.0f));
    REQUIRE_FALSE(IsSubnormal(-0.0));
    REQUIRE_FALSE(IsSubnormal(1.0f));
    REQUIRE_FALSE(IsSubnormal(std::numeric_limits<double>::infinity()));

    REQUIRE(Exponent(1.0f) == 0);
    REQUIRE(Exponent(8.0f) == 3);
    REQUIRE(Exponent(floatSubnormal) == -126);
    REQUIRE(Exponent(0.0) == -1022);
    REQUIRE(Exponent(8.0) == 3);
    REQUIRE(Exponent(std::numeric_limits<double>::infinity()) == 1024);
    REQUIRE(Significand(1.0f) == 0);
    REQUIRE(Significand(std::bit_cast<float>(UInt32{0x3FC00000u})) == 0x400000u);
    REQUIRE(Significand(1.0) == 0);
    REQUIRE(Significand(std::bit_cast<double>(UInt64{0x3FF8000000000000ULL})) == 0x8000000000000ULL);
    REQUIRE(FastSignBit(1.0f) == 0);
    REQUIRE(FastSignBit(-0.0f) == 0x80000000u);
    REQUIRE(FastSignBit(1.0) == 0);
    REQUIRE(FastSignBit(-0.0) == 0x8000000000000000ULL);
}

TEST_CASE("Utils RAII deleters and Finally invoke cleanup exactly once", "[utils][raii]")
{
    using namespace Harlinn::Common;
    int lvalueCalls = 0;
    auto cleanup = [&lvalueCalls] { ++lvalueCalls; };
    {
        auto guard = Finally(cleanup);
        REQUIRE(lvalueCalls == 0);
    }
    REQUIRE(lvalueCalls == 1);

    int temporaryCalls = 0;
    {
        auto guard = Finally([&temporaryCalls] { ++temporaryCalls; });
        REQUIRE(temporaryCalls == 0);
    }
    REQUIRE(temporaryCalls == 1);

    {
        auto releaseGuard = Finally([&temporaryCalls] { ++temporaryCalls; });
        static_assert(!std::is_copy_constructible_v<decltype(releaseGuard)>);
        static_assert(!std::is_move_constructible_v<decltype(releaseGuard)>);
    }
    REQUIRE(temporaryCalls == 2);

    auto moveOnlyCleanup = [state = std::make_unique<int>(0), &temporaryCalls]() mutable
    {
        ++*state;
        ++temporaryCalls;
    };
    {
        auto guard = Finally(std::move(moveOnlyCleanup));
    }
    REQUIRE(temporaryCalls == 3);

    int releases = 0;
    std::unique_ptr<ReleaseTracked, ReleaseDeleter<ReleaseTracked>> comObject(new ReleaseTracked{&releases});
    REQUIRE(releases == 0);
    comObject.reset();
    REQUIRE(releases == 1);

    auto* allocated = static_cast<int*>(std::malloc(sizeof(int)));
    REQUIRE(allocated != nullptr);
    *allocated = 42;
    {
        std::unique_ptr<int, FreeDeleter<int>> owned(allocated);
        REQUIRE(*owned == 42);
    }
    FreeDeleter<int>{}(nullptr);
#if defined(_MSC_VER)
    auto* allocaMemory = static_cast<int*>(_malloca(sizeof(int)));
    REQUIRE(allocaMemory != nullptr);
    *allocaMemory = 24;
    {
        std::unique_ptr<int, FreeADeleter<int>> ownedWithFreeA(allocaMemory);
        REQUIRE(*ownedWithFreeA == 24);
    }
#endif
}
