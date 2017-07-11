cmake_minimum_required(VERSION 2.6)
project(meeh)

set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -pedantic -std=c++14 -fno-rtti")
set(CMAKE_C_FLAGS "-Wall -Wextra -pedantic")

set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
set(CMAKE_C_FLAGSS_DEBUG "-g -O0")

set(CMAKE_CXX_FLAGS_RELEASE "-O2 -flto -DNDEBUG -march=native")
set(CMAKE_C_FLAGS_RELEASE "-O2 -flto -DNDEBUG -march=native")

set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g -fno-omit-frame-pointer")
set(CMAKE_C_FLAGS_RELWITHDEBINFO "-O2 -g -fno-omit-frame-pointer")

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

add_definitions(-DGLM_FORCE_NO_CTOR_INIT)

include_directories(./include)

file(GLOB_RECURSE headers "./include/*")
add_custom_target(headers SOURCES ${headers})

add_subdirectory(src/meeh)
add_subdirectory(src/apps)
