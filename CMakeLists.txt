CMAKE_MINIMUM_REQUIRED(VERSION 3.3)
PROJECT(parafem)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
SET(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
SET(CMAKE_CXX_FLAGS_RELEASE "-O3")

# OPTIONS
OPTION(BUILD_TESTS "test require vtk" ON)
OPTION(BUILD_PYTHON_BINDINGS "create python bindings" ON)
OPTION(BUILD_JSON "json in out" ON)

# DEPENDENCIES
FIND_PACKAGE(OpenMP)

## EIGEN
FIND_PACKAGE(Eigen3)
MESSAGE(${EIGEN3_VERSION})
FIND_PACKAGE(OpenMP)
INCLUDE_DIRECTORIES( ${EIGEN3_INCLUDE_DIR})     # eigen3

IF(OPENMP_FOUND)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    SET(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   ${OpenMP_C_FLAGS}")
ENDIF(OPENMP_FOUND)

ADD_SUBDIRECTORY("src/")

IF (BUILD_TESTS)
    ADD_SUBDIRECTORY("tests/")
ENDIF(BUILD_TESTS)