cmake_minimum_required(VERSION 3.20)

set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/external/vcpkg/scripts/buildsystems/vcpkg.cmake"
CACHE STRING "Vcpkg toolchain file")

# use C++20
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED)

project(
  apbr
  LANGUAGES C CXX
  DESCRIPTION "A Renderer Based on the book 'Physically Based Rendering'(https://www.pbr-book.org/)"
  VERSION 0.1.1)

find_package(glfw3 CONFIG REQUIRED)
add_library(glad STATIC external/glad/src/glad.c)
include_directories(external/glad/include external/include)

if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
  message(STATUS "Exporting compile_commands.json")
  set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
endif()

set(apbr "${PROJECT_NAME}-${PROJECT_VERSION}")
add_executable(${apbr})

add_subdirectory(config)
add_subdirectory(src)

target_link_libraries(${apbr} PRIVATE glfw glad)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang" AND MSVC)
  target_compile_options(
    ${apbr}
    PRIVATE 
    /clang:-Wall
    -Wextra
    $<$<CONFIG:Debug>:-Werror>)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL MSVC)
  target_compile_options(${apbr}
                         PRIVATE /MP $<$<CONFIG:Debug>:/WX>)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL Clang OR CMAKE_CXX_COMPILER_ID STREQUAL
                                               GNU)
  target_compile_options(
    ${apbr}
    PRIVATE 
    -Wall
    -Wextra
    $<$<CONFIG:Debug>:-Werror>)
endif()

# copy resources while configuring the project. 
# An example: `resources/shaders` => `shaders`.
if(NOT CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_CURRENT_BINARY_DIR)
    message("Copying resource folders from ${CMAKE_SOURCE_DIR}/resources/ to ${CMAKE_BINARY_DIR}")
    FILE(COPY ${CMAKE_SOURCE_DIR}/resources/shaders DESTINATION "${CMAKE_BINARY_DIR}")
    FILE(COPY ${CMAKE_SOURCE_DIR}/resources/textures DESTINATION "${CMAKE_BINARY_DIR}")
endif()