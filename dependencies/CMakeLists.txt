include(FetchContent)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.17.0
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/gtest
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
    # boostgraph
    # eigen
    # discreture
)
