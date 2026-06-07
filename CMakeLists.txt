cmake_minimum_required(VERSION 3.16)
project(uni-update VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(uni-update
    src/main.cpp
    src/distro.cpp
    src/runner.cpp
    src/extras.cpp
    src/cli.cpp
)

target_include_directories(uni-update PRIVATE src)

install(TARGETS uni-update DESTINATION bin)
