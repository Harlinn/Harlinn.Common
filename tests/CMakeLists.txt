add_executable(HarlinnCommonTests
    test_common.cpp
    test_basic_types_concepts.cpp
    test_utils_bits.cpp
    test_utils_memory.cpp
    test_utils_arithmetic.cpp
)

target_link_libraries(HarlinnCommonTests PRIVATE
    HarlinnCommon
    Catch2::Catch2WithMain
)

list(APPEND CMAKE_MODULE_PATH ${Catch2_SOURCE_DIR}/extras)
include(Catch)
catch_discover_tests(HarlinnCommonTests)
