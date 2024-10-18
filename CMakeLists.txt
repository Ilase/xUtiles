cmake_minimum_required(VERSION 3.10)

project(
    xutiles
)



set(HEADER_DIR ${CMAKE_SOURCE_DIR}/include)
set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/src/)



#file(GLOB HEADERS "${HEADER_DIR}/*hpp")
file(GLOB SOURCES "${SOURCE_DIR}/*cpp")

include_directories(${HEADER_DIR})
find_package(X11 REQUIRED)

add_executable(
    ${PROJECT_NAME}
    ${SOURCES}
)
target_link_libraries(${PROJECT_NAME} ${X11_LIBRARIES})