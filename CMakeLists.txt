cmake_minimum_required(VERSION 3.11)
project(Fractals LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

find_package(raylib 5.0 QUIET)
if (NOT raylib_FOUND)
    include(FetchContent)

    set(CUSTOMIZE_BUILD ON CACHE BOOL "" FORCE)
    set(SUPPORT_MODULE_RMODELS OFF CACHE BOOL "" FORCE)
    set(SUPPORT_MODULE_RAUDIO OFF CACHE BOOL "" FORCE)

    FetchContent_Declare(
        raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib.git
        GIT_TAG 5.5
    )

    FetchContent_MakeAvailable(raylib)
endif()

include_directories(
    Resources
    Constants
    XORShift32
    Welcome
    Multithread
    Tree
)

set(SOURCES
    main.c
    "Welcome/welcome.c"
    "Multithread/multithread.c"
    "Tree/tree.c"
)

if(WIN32)
    list(APPEND SOURCES "${CMAKE_SOURCE_DIR}/Resources/resources.rc")
endif()

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} raylib)

if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif()

#Release/Debug properties
set_target_properties(${PROJECT_NAME} PROPERTIES 
    WIN32_EXECUTABLE $<$<CONFIG:Release>:TRUE> #Disable console in Release build
    OUTPUT_NAME_RELEASE "${PROJECT_NAME}"
    OUTPUT_NAME_DEBUG   "${PROJECT_NAME} (DEBUG)"
)

set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE)

#Compiler flags
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE 
        /W4
        $<$<CONFIG:Release>:
            /Os
        >
    )
else()
    target_compile_options(${PROJECT_NAME} PRIVATE 
        -Wall
        $<$<CONFIG:Release>:
            -Os
        >
    )
endif()