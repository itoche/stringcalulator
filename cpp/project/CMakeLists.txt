cmake_minimum_required(VERSION 2.8)

set(PROJECT_NAME stringCalculator)

project(${PROJECT_NAME} CXX)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/contrib/cmake)

if(WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

if(CMAKE_COMPILER_IS_GNUCXX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
endif()

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()


set(GMOCK_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/contrib/gmock-1.7.0/include ${PROJECT_SOURCE_DIR}/contrib/gmock-1.7.0)
set(GTEST_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/contrib/gmock-1.7.0/gtest/include ${PROJECT_SOURCE_DIR}/contrib/gmock-1.7.0/gtest)
set(GOOGLE_MOCK_FILES  ${PROJECT_SOURCE_DIR}/contrib/gmock-1.7.0/src/gmock-all.cc ${PROJECT_SOURCE_DIR}/contrib/gmock-1.7.0/src/gmock_main.cc ${PROJECT_SOURCE_DIR}/contrib/gmock-1.7.0/gtest/src/gtest-all.cc)


file(GLOB_RECURSE SRC src/*.cpp)
file(GLOB_RECURSE TEST_SRC test/*.cpp)
include_directories(src ${GMOCK_INCLUDE_DIRS} ${GTEST_INCLUDE_DIRS})

add_library(${PROJECT_NAME} ${SRC})

add_executable(test_${PROJECT_NAME} ${TEST_SRC} ${GOOGLE_MOCK_FILES})
target_link_libraries(test_${PROJECT_NAME} ${PROJECT_NAME})

if(UNIX)
    set_target_properties(test_${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-Wno-missing-field-initializers")
    if(CMAKE_COMPILER_IS_GNUCXX)
        target_link_libraries(test_${PROJECT_NAME} pthread)
    endif()
endif()

enable_testing()

add_test(test_${PROJECT_NAME} test_${PROJECT_NAME})


# Code coverage
if(NOT WIN32 AND NOT DISABLE_COVERAGE)
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        if(CMAKE_COMPILER_IS_GNUCXX)
            include(CodeCoverage)
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_COVERAGE}")
            set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${CMAKE_C_FLAGS_COVERAGE}")
            set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${CMAKE_EXE_LINKER_FLAGS_COVERAGE}")
            set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${CMAKE_SHARED_LINKER_FLAGS_COVERAGE}")
            setup_coverage()
        endif()
    endif()
endif()
