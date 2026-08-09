cmake_minimum_required(VERSION 3.25)

project(HarlinnCommon
    VERSION 0.1.0
    LANGUAGES CXX
    DESCRIPTION "Header-only common utilities for the Harlinn ecosystem"
)

option(HARLINN_COMMON_CXX26 "Require C++26 instead of C++23" OFF)

if(HARLINN_COMMON_CXX26)
    set(HARLINN_COMMON_CXX_STANDARD 26)
else()
    set(HARLINN_COMMON_CXX_STANDARD 23)
endif()

# ── Library target ───────────────────────────────────────────────
add_library(HarlinnCommon INTERFACE)
add_library(Harlinn::Common ALIAS HarlinnCommon)

target_include_directories(HarlinnCommon INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_compile_features(HarlinnCommon INTERFACE cxx_std_${HARLINN_COMMON_CXX_STANDARD})

# ── Dependencies ─────────────────────────────────────────────────
include(cmake/Dependencies.cmake)

# ── Tests ────────────────────────────────────────────────────────
include(CTest)
if(BUILD_TESTING)
    add_subdirectory(tests)
endif()

# ── Examples ─────────────────────────────────────────────────────
option(BUILD_EXAMPLES "Build example targets" OFF)
if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

# ── Install & Export ─────────────────────────────────────────────
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

install(TARGETS HarlinnCommon EXPORT HarlinnCommonTargets)
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(EXPORT HarlinnCommonTargets
    FILE        HarlinnCommonTargets.cmake
    NAMESPACE   Harlinn::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/HarlinnCommon
)

configure_package_config_file(
    cmake/HarlinnCommonConfig.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/HarlinnCommonConfig.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/HarlinnCommon
)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/HarlinnCommonConfigVersion.cmake"
    COMPATIBILITY SameMajorVersion
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/HarlinnCommonConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/HarlinnCommonConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/HarlinnCommon
)
