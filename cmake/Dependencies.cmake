include(FetchContent)

# ── Catch2 (tests only) ─────────────────────────────────────────
if(BUILD_TESTING)
    if(NOT TARGET Catch2::Catch2WithMain)
        FetchContent_Declare(Catch2
            GIT_REPOSITORY https://github.com/catchorg/Catch2.git
            GIT_TAG        v3.7.1
        )
        FetchContent_MakeAvailable(Catch2)
    endif()
endif()
