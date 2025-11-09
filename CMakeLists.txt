cmake_minimum_required(VERSION 3.10)

project(subgraphs CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(EXECUTABLE_NAME subgraphs)

file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS "src/*.cpp")

add_executable(${EXECUTABLE_NAME} main.cpp ${SOURCE_FILES})

target_include_directories(${EXECUTABLE_NAME} PRIVATE src)

if(MSVC)
  target_compile_options(${EXECUTABLE_NAME} PRIVATE /W4)
else()
  target_compile_options(${EXECUTABLE_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  if(MSVC)
    target_compile_options(${EXECUTABLE_NAME} PRIVATE /Zi /Od)
  else()
    target_compile_options(${EXECUTABLE_NAME} PRIVATE -g -O0)
  endif()
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
  if(MSVC)
    target_compile_options(${EXECUTABLE_NAME} PRIVATE /O2)
  else()
    target_compile_options(${EXECUTABLE_NAME} PRIVATE -O3)
  endif()
endif()

message(STATUS "Project: ${PROJECT_NAME}")
message(STATUS "Executable: ${EXECUTABLE_NAME}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
