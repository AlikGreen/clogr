cmake_minimum_required(VERSION 3.29)
project(clogr)

set(CMAKE_CXX_STANDARD 23)


# cpptrace requires ninja or visual studio
if(NOT CMAKE_GENERATOR MATCHES "Ninja|Visual Studio")
    message(FATAL_ERROR
            "Generator '${CMAKE_GENERATOR}' is not supported. "
            "Please use Ninja or Visual Studio 17.4+.\n"
            "  CLion:   Settings > Build, Execution, Deployment > CMake > Generator\n"
            "  Command line: cmake -G \"Ninja\" .."
    )
endif()

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/dependencies/fmtlib)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/dependencies/cpptrace)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/dependencies/catch2)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/dependencies/grl)


file(GLOB_RECURSE
        SRC_FILES
        CONFIGURE_DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h
)

message(STATUS "Found source files: ${SRC_FILES}")


add_library(clogr STATIC ${SRC_FILES}
        include/clogr/clogr.h)

target_include_directories(clogr PUBLIC include/ src/ dependencies/backward)
target_link_libraries(clogr PUBLIC fmt::fmt cpptrace::cpptrace grl)

add_library(clogr::clogr ALIAS clogr)

### Testing

file(GLOB_RECURSE
        TEST_SRC_FILES
        CONFIGURE_DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.h
)

add_executable(clogr_tests ${TEST_SRC_FILES})
target_link_libraries(clogr_tests PUBLIC clogr::clogr Catch2::Catch2WithMain)