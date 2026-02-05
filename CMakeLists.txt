cmake_minimum_required(VERSION 3.29)
project(neonLog)

set(CMAKE_CXX_STANDARD 23)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/dependencies/fmtlib)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/dependencies/cpptrace)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/dependencies/catch2)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/dependencies/neoncore)


file(GLOB_RECURSE
        SRC_FILES
        CONFIGURE_DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h
)

message(STATUS "Found source files: ${SRC_FILES}")


add_library(neonLog STATIC ${SRC_FILES}
        include/neonLog/neonLog.h)

target_include_directories(neonLog PUBLIC include/ src/ dependencies/backward)
target_link_libraries(neonLog PUBLIC fmt::fmt cpptrace::cpptrace neonCore)

### Testing

file(GLOB_RECURSE
        TEST_SRC_FILES
        CONFIGURE_DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.h
)

add_executable(neonLog_Test ${TEST_SRC_FILES})
target_link_libraries(neonLog_Test PUBLIC neonLog Catch2::Catch2WithMain)