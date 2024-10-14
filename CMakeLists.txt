cmake_minimum_required(VERSION 3.10)

project(
    xdrive
    VERSION 1.0
)

set(HEADER_DIR ${CMAKE_SOURCE_DIR}/include)
set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)

set(HEADERS 
    ${HEADER_DIR}/xdrivers.hpp
)

set(SOURCE
    ${SOURCE_DIR}/xdr_main.cpp
    ${SOURCE_DIR}/xdrivers.cpp
)

add_executable(
    ${PROJECT_NAME}
    ${SOURCE}
    ${HEADERS}
)