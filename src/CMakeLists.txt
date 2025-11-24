# ---------------------------------
# Source Files Configuration
# ---------------------------------

set(SUBGRAPHS_SOURCES
    main.cpp
)

set(SUBGRAPHS_INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# ---------------------------------
# Create Header-Only Library for Reuse in Tests
# ---------------------------------

add_library(subgraphs_lib INTERFACE)
target_include_directories(subgraphs_lib INTERFACE ${SUBGRAPHS_INCLUDE_DIRS})
target_compile_features(subgraphs_lib INTERFACE cxx_std_20)
target_link_libraries(subgraphs_lib INTERFACE hungarian_algorithm)

# ---------------------------------
# Create Executable
# ---------------------------------

add_executable(${PROJECT_NAME} ${SUBGRAPHS_SOURCES})
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
