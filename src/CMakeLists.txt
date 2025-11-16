# ---------------------------------
# Source Files Configuration
# ---------------------------------

set(SUBGRAPHS_LIB_SOURCES
    graph/multigraph.cpp
    graph/neighbor_iterator.cpp
    graph/neighbor_range.cpp
    graph/permutation_iterator.cpp
    graph/combination_iterator.cpp
    graph/sequence_iterator.cpp
    utils/graph_loader.cpp
    utils/graph_printer.cpp
    algorithms/subgraph_algorithm.cpp
)

set(SUBGRAPHS_SOURCES
    main.cpp
)

set(SUBGRAPHS_HEADERS
)

set(SUBGRAPHS_INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# ---------------------------------
# Create Library for Reuse in Tests
# ---------------------------------

add_library(subgraphs_lib STATIC ${SUBGRAPHS_LIB_SOURCES})
target_include_directories(subgraphs_lib PUBLIC ${SUBGRAPHS_INCLUDE_DIRS})
target_compile_features(subgraphs_lib PUBLIC cxx_std_20)

# ---------------------------------
# Create Executable
# ---------------------------------

add_executable(${PROJECT_NAME} ${SUBGRAPHS_SOURCES} ${SUBGRAPHS_HEADERS})
target_link_libraries(${PROJECT_NAME} PRIVATE subgraphs_lib)

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/bin/debug"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/bin/release"
)

# ---------------------------------
# Target Include Directories
# ---------------------------------

target_include_directories(${PROJECT_NAME} PRIVATE ${SUBGRAPHS_INCLUDE_DIRS})

# ---------------------------------
# Print Configuration Info
# ---------------------------------

message(STATUS "")
message(STATUS "=== ${PROJECT_NAME} Target Configuration ===")
message(STATUS "Executable: ${PROJECT_NAME}")
message(STATUS "Sources: ${SUBGRAPHS_SOURCES}")
message(STATUS "Include Directories: ${SUBGRAPHS_INCLUDE_DIRS}")
message(STATUS "C++ Standard: C++20")
message(STATUS "Output Directory: ${CMAKE_BINARY_DIR}/bin")
message(STATUS "")
