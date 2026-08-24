cmake_minimum_required(VERSION 3.28)

project(
    avernal-rhi
    VERSION 0.1.0
    DESCRIPTION "Rendering hardware interface for the Avernal game engine"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)

include(FetchContent)

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../avernal-core/CMakeLists.txt")
    set(AVERNAL_CORE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../avernal-core")
else()
    set(AVERNAL_CORE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../avernal-core")
endif()

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../avernal-window/CMakeLists.txt")
    set(AVERNAL_WINDOW_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../avernal-window")
else()
    set(AVERNAL_WINDOW_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../avernal-window")
endif()

if(NOT EXISTS "${AVERNAL_CORE_SOURCE_DIR}/CMakeLists.txt")
    message(FATAL_ERROR
        "avernal-core not found at:\n"
        "  ${AVERNAL_CORE_SOURCE_DIR}\n"
        "Place avernal-core as a sibling of this project, or set "
        "FETCHCONTENT_SOURCE_DIR_AVERNAL_CORE.")
endif()

if(NOT EXISTS "${AVERNAL_WINDOW_SOURCE_DIR}/CMakeLists.txt")
    message(FATAL_ERROR
        "avernal-window not found at:\n"
        "  ${AVERNAL_WINDOW_SOURCE_DIR}\n"
        "Place avernal-window as a sibling of this project.")
endif()

FetchContent_Declare(
    avernal_core
    SOURCE_DIR "${AVERNAL_CORE_SOURCE_DIR}"
    EXCLUDE_FROM_ALL
)

FetchContent_Declare(
    avernal_window
    SOURCE_DIR "${AVERNAL_WINDOW_SOURCE_DIR}"
    EXCLUDE_FROM_ALL
)

set(AVERNAL_CORE_BUILD_DEMO OFF)
set(AVERNAL_WINDOW_BUILD_DEMO OFF)

FetchContent_MakeAvailable(avernal_core avernal_window)

add_library(avernal-rhi)
add_library(avernal::rhi ALIAS avernal-rhi)

target_sources(
    avernal-rhi
    PRIVATE src/device.cpp
    PUBLIC
        FILE_SET HEADERS
        BASE_DIRS include
        FILES
            include/avernal/rhi/rhi.hpp
)

target_link_libraries(avernal-rhi PUBLIC avernal::core avernal::window)
target_compile_features(avernal-rhi PUBLIC cxx_std_23)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    target_compile_options(avernal-rhi PRIVATE -Wall -Wextra -Wpedantic)
elseif(MSVC)
    target_compile_options(avernal-rhi PRIVATE /W4)
endif()

option(AVERNAL_RHI_BUILD_DEMO "Build the Avernal RHI demo executable" ON)

if(AVERNAL_RHI_BUILD_DEMO)
    add_executable(avernal-rhi-demo apps/demo/main.cpp)
    target_link_libraries(avernal-rhi-demo PRIVATE avernal::rhi)
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
        target_compile_options(avernal-rhi-demo PRIVATE -Wall -Wextra -Wpedantic)
    elseif(MSVC)
        target_compile_options(avernal-rhi-demo PRIVATE /W4)
    endif()
endif()

if(PROJECT_IS_TOP_LEVEL)
    option(AVERNAL_RHI_BUILD_TESTS "Build Avernal RHI unit tests" ON)
else()
    option(AVERNAL_RHI_BUILD_TESTS "Build Avernal RHI unit tests" OFF)
endif()

if(AVERNAL_RHI_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
