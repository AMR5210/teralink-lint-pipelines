cmake_minimum_required(VERSION 3.16)
project(teralink_lint_demo LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(teralink_checks src/checksum.c src/crc16.cpp)
target_include_directories(teralink_checks PUBLIC include)

add_executable(checks_demo src/main.cpp)
target_link_libraries(checks_demo PRIVATE teralink_checks)
