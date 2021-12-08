cmake_minimum_required(VERSION 3.21)
project(libuv_server C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

add_executable(libuv_server main.c)

find_package(uv REQUIRED)

target_include_directories(libuv_server PUBLIC ${UV_INCLUDE_DIR})
target_link_libraries(libuv_server PUBLIC ${UV_LIBRARY})
