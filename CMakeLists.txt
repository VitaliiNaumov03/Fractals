cmake_minimum_required(VERSION 3.10)
project(Fractals LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

set(RAYLIB_PATH "${CMAKE_SOURCE_DIR}/Raylib/")
include_directories(${RAYLIB_PATH} Compiling)
add_executable(${PROJECT_NAME} main.c Compiling/icon48.rc)

#Compiler flags
if(MSVC)
    set(CMAKE_C_FLAGS_RELEASE "/O2 /Gy")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "/INCREMENTAL:NO")
elseif(CMAKE_C_COMPILER_ID MATCHES "Clang|GNU")
    set(CMAKE_C_FLAGS_RELEASE "-s -O2 -Wall")
    set(CMAKE_C_FLAGS_DEBUG "-Og -g")
endif()

#Exe name & console disabling
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        OUTPUT_NAME "${PROJECT_NAME}"
        WIN32_EXECUTABLE TRUE
    )
else()
    set_target_properties(${PROJECT_NAME} PROPERTIES
        OUTPUT_NAME "${PROJECT_NAME} (DEBUG)"
        WIN32_EXECUTABLE FALSE
    )
endif()

#Linking raylib
find_library(RAYLIB_LIBRARY
    NAMES raylib libraylib
    PATHS ${RAYLIB_PATH}
    NO_DEFAULT_PATH
)

if(NOT RAYLIB_LIBRARY)
    message(FATAL_ERROR "Raylib was not found in \"${RAYLIB_PATH}\"")
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE ${RAYLIB_LIBRARY} winmm)