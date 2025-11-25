include(FetchContent)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.17.0
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/gtest
)

FetchContent_Declare(
    hungarian_algorithm
    GIT_REPOSITORY https://github.com/mcximing/hungarian-algorithm-cpp.git
    GIT_TAG master
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/hungarian-algorithm-cpp
)

# FetchContent_Declare(
#     boostgraph
#     GIT_REPOSITORY https://github.com/boostorg/graph.git
#     GIT_TAG boost-1.89.0
#     SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/boostgraph
# )

# FetchContent_Declare(
#     eigen
#     GIT_REPOSITORY https://gitlab.com/libeigen/eigen.git
#     GIT_TAG 3.3.4
#     SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/eigen
# )

# FetchContent_Declare(
#     discreture
#     GIT_REPOSITORY https://github.com/mraggi/discreture.git
#     GIT_TAG v1.2
#     SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/discreture
# )

FetchContent_MakeAvailable(
    googletest
    hungarian_algorithm
    # boostgraph
    # eigen
    # discreture
)

# ---------------------------------
# Create hungarian_algorithm library target
# ---------------------------------
# The hungarian-algorithm-cpp repo doesn't provide CMake config,
# so we need to create the library target manually

add_library(hungarian_algorithm STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/hungarian-algorithm-cpp/Hungarian.cpp
)

target_include_directories(hungarian_algorithm PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/hungarian-algorithm-cpp
)

target_compile_features(hungarian_algorithm PUBLIC cxx_std_11)
