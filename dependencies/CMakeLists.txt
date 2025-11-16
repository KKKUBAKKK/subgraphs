# Google Test dependency
# This file downloads and makes Google Test available to the project

include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.14.0
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/gtest
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Disable installation of googletest
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)

# Fetch the content
FetchContent_MakeAvailable(googletest)
