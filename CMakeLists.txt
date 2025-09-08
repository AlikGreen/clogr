cmake_minimum_required(VERSION 3.29)
project(neonLog)

set(CMAKE_CXX_STANDARD 23)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/dependencies/spdlog)

file(GLOB_RECURSE
        SRC_FILES
        CONFIGURE_DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h
)

message(STATUS "Found source files: ${SRC_FILES}")


add_library(neonLog STATIC ${SRC_FILES}
        include/neonLog/neonLog.h)

target_include_directories(neonLog PRIVATE src/)
target_include_directories(neonLog PUBLIC include/)
target_link_libraries(neonLog PRIVATE spdlog)