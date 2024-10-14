cmake_minimum_required(VERSION 3.10)

project(
    xdrive
)

# set(PROJECT_VERSION 1.2)
# set(BUILD_DIR 
#     "builds/v${PROJECT_VERSION}"
# )
# file(MAKE_DIRECTORY ${BUILD_DIR})

# execute_process(
#     COMMAND ${CMAKE_COMMAND} ..
#     WORKING_DIRECTORY ${BUILD_DIR}
# )

set(HEADER_DIR ${CMAKE_SOURCE_DIR}/include)
set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)

set(HEADERS 
    ${HEADER_DIR}/xdrivers.hpp
)

set(SOURCE
    ${SOURCE_DIR}/xdr_main.cpp
    ${SOURCE_DIR}/xdrivers.cpp
)

include_directories(${HEADER_DIR})
find_package(X11 REQUIRED)

add_executable(
    ${PROJECT_NAME}
    ${SOURCE}
    ${HEADERS}
)
target_link_libraries(${PROJECT_NAME} ${X11_LIBRARIES})