# ---------------------------------
# Setup subgraphs project
# ---------------------------------

cmake_minimum_required(VERSION 4.0)
project(subgraphs VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD 17)

# ---------------------------------
# Add source directory
# ---------------------------------

add_subdirectory(src)

# ---------------------------------
# Enable testing
# ---------------------------------

enable_testing()

# ---------------------------------
# Add dependencies
# ---------------------------------

add_subdirectory(dependencies)

# ---------------------------------
# Add tests directory
# ---------------------------------

add_subdirectory(tests)

# ---------------------------------
# Setup compile options
# ---------------------------------

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build type (Debug/Release)" FORCE)
    message(STATUS "Build type not specified, defaulting to Debug")
endif()

message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID}")

# ---------------------------------
# Common compile flags for all compilers
# ---------------------------------

set(COMMON_CXX_FLAGS "")
set(COMMON_C_FLAGS "")

# ---------------------------------
# GCC/Clang Common Flags
# ---------------------------------

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    list(APPEND COMMON_CXX_FLAGS
        -Wall                   # Enable all common warnings
        -Wextra                 # Enable extra warnings
        -Wpedantic              # Enable pedantic warnings
        -Wshadow                # Warn about shadowed variables
        -Wunused                # Warn about unused variables and functions
        -Wconversion            # Warn about implicit conversions
        -Wdouble-promotion      # Warn about float to double promotion
        -Wformat=2              # Better format string checking
        -Wlogical-op            # Warn about suspicious logical operations
        -Wundef                 # Warn about undefined preprocessor symbols
        -fstrict-aliasing       # Enable strict aliasing optimization
    )
    list(APPEND COMMON_C_FLAGS
        -Wall
        -Wextra
        -Wpedantic
        -Wshadow
        -Wunused
        -Wconversion
        -Wformat=2
        -Wundef
    )
endif()

# ---------------------------------
# GCC Specific Flags
# ---------------------------------

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    message(STATUS "Configuring for GCC compiler")

    list(APPEND COMMON_CXX_FLAGS
        -Wduplicated-cond       # Warn about duplicate conditions
        -Wduplicated-branches   # Warn about duplicate branches
        -Wrestrict              # Warn about certain restrict violations
        -Wnull-dereference      # Warn about null dereferences
    )
    list(APPEND COMMON_C_FLAGS
        -Wduplicated-cond
        -Wduplicated-branches
        -Wrestrict
        -Wnull-dereference
    )
endif()

# ---------------------------------
# Clang Specific Flags
# ---------------------------------

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    message(STATUS "Configuring for Clang compiler")

    list(APPEND COMMON_CXX_FLAGS
        -Winconsistent-missing-override  # Warn about inconsistent override
        -Wno-c++98-compat                # Don't warn about C++98 compatibility
    )
    list(APPEND COMMON_C_FLAGS
        -Wno-c99-extensions
    )
endif()

# ---------------------------------
# MSVC Specific Flags
# ---------------------------------

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    message(STATUS "Configuring for MSVC compiler")

    list(APPEND COMMON_CXX_FLAGS
        /W4                     # Warning level 4 (highest)
        /WX                     # Treat warnings as errors (optional, can remove)
        /permissive-            # Standards conformance mode
        /Za                     # Disable extensions
    )
    list(APPEND COMMON_C_FLAGS
        /W4
        /permissive-
    )
endif()

# ---------------------------------
# Debug Build Configuration
# ---------------------------------

string(TOUPPER "${CMAKE_BUILD_TYPE}" BUILD_TYPE_UPPER)
if(BUILD_TYPE_UPPER STREQUAL "DEBUG")
    message(STATUS "Configuring Debug build with maximum error checking")

    add_compile_definitions(DEBUG_MODE=1)

    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        set(DEBUG_CXX_FLAGS
            -g                  # Generate debug symbols
            -O0                 # No optimization
            -fno-omit-frame-pointer  # Keep frame pointers for debugging
            -fno-inline         # Disable inlining for easier debugging
            -DDEBUG_BUILD       # Define debug build macro
        )

        if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
            list(APPEND DEBUG_CXX_FLAGS
                -fsanitize=address              # Address sanitizer
                -fsanitize=undefined            # Undefined behavior sanitizer
                -fsanitize=bounds               # Bounds checking
                -fsanitize=leak                 # Leak detection
                -fno-sanitize-recover=all       # Stop on first error
            )
        endif()

        set(DEBUG_C_FLAGS
            -g
            -O0
            -fno-omit-frame-pointer
            -DDEBUG_BUILD
        )

    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        set(DEBUG_CXX_FLAGS
            /ZI                 # Edit and continue debugging
            /Od                 # Disable optimization
            /RTC1               # Runtime error checking
            /D"DEBUG_BUILD"     # Define debug build macro
        )
        set(DEBUG_C_FLAGS
            /ZI
            /Od
            /RTC1
            /D"DEBUG_BUILD"
        )
    endif()

    set(CMAKE_CXX_FLAGS "${COMMON_CXX_FLAGS} ${DEBUG_CXX_FLAGS}")
    set(CMAKE_C_FLAGS "${COMMON_C_FLAGS} ${DEBUG_C_FLAGS}")

# ---------------------------------
# Release Build Configuration
# ---------------------------------

elseif(BUILD_TYPE_UPPER STREQUAL "RELEASE")
    message(STATUS "Configuring Release build with maximum optimizations")

    add_compile_definitions(RELEASE_MODE=1)

    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        set(RELEASE_CXX_FLAGS
            -O3                 # Maximum optimization
            -march=native       # Optimize for native architecture
            -flto               # Link-time optimization
            -DNDEBUG            # Disable assertions
            -ffast-math         # Fast math operations (use with caution)
        )
        set(RELEASE_C_FLAGS
            -O3
            -march=native
            -flto
            -DNDEBUG
        )

    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        set(RELEASE_CXX_FLAGS
            /O2                 # Maximum optimization
            /Oi                 # Intrinsic functions
            /Ot                 # Favor fast code
            /GL                 # Whole program optimization
            /D"NDEBUG"          # Disable assertions
        )
        set(RELEASE_C_FLAGS
            /O2
            /Oi
            /Ot
            /GL
            /D"NDEBUG"
        )

        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /LTCG")
        set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /LTCG")
        set(CMAKE_STATIC_LINKER_FLAGS "${CMAKE_STATIC_LINKER_FLAGS} /LTCG")
    endif()

    set(CMAKE_CXX_FLAGS "${COMMON_CXX_FLAGS} ${RELEASE_CXX_FLAGS}")
    set(CMAKE_C_FLAGS "${COMMON_C_FLAGS} ${RELEASE_C_FLAGS}")

else()
    message(STATUS "Configuring for custom build type: ${CMAKE_BUILD_TYPE}")
    set(CMAKE_CXX_FLAGS "${COMMON_CXX_FLAGS}")
    set(CMAKE_C_FLAGS "${COMMON_C_FLAGS}")
endif()

# ---------------------------------
# Print configured flags
# ---------------------------------

message(STATUS "C++ Compiler Flags: ${CMAKE_CXX_FLAGS}")
message(STATUS "C Compiler Flags: ${CMAKE_C_FLAGS}")
