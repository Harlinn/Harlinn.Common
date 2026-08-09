add_executable(HarlinnCommonTests
    test_common.cpp
)

target_link_libraries(HarlinnCommonTests PRIVATE
    HarlinnCommon
    Catch2::Catch2WithMain
)

list(APPEND CMAKE_MODULE_PATH ${Catch2_SOURCE_DIR}/extras)
include(Catch)
catch_discover_tests(HarlinnCommonTests)
