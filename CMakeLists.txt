#
# This file is a part of the Sharemind framework.
# Copyright (C) Cybernetica AS
#
# All rights are reserved. Reproduction in whole or part is prohibited
# without the written consent of the copyright owner. The usage of this
# code is subject to the appropriate license agreement.
#

CMAKE_MINIMUM_REQUIRED(VERSION "2.8.12")
PROJECT("SHAREMIND_FACILITY_EXECUTIONPROFILER" "CXX")
SET(SHAREMIND_FACILITY_EXECUTIONPROFILER_VERSION "0.1.0.0")
SET(SHAREMIND_FACILITY_EXECUTIONPROFILER_COMPAT_VERSION "0.1.0.0")

INCLUDE("${CMAKE_CURRENT_SOURCE_DIR}/config.local" OPTIONAL)
INCLUDE("${CMAKE_CURRENT_BINARY_DIR}/config.local" OPTIONAL)

FIND_PACKAGE(LogHard 0.1.0 REQUIRED)
FIND_PACKAGE(SharemindLibExecutionProfiler REQUIRED)
FIND_PACKAGE(SharemindLibfmodapi 0.1.0 REQUIRED)

ADD_COMPILE_OPTIONS(
  "-std=c++11" "-Wall" "-Wextra" "-O2"
  "$<$<NOT:$<STREQUAL:$<CONFIGURATION>,Release>>:-ggdb>"
  "$<$<NOT:$<STREQUAL:$<CONFIGURATION>,Release>>:-fno-omit-frame-pointer>"
  "$<$<STREQUAL:$<CONFIGURATION>,Release>:-DNDEBUG>"
  "$<$<STREQUAL:$<CONFIGURATION>,Release>:-fomit-frame-pointer>"
)

INCLUDE(CheckCXXCompilerFlag)
FUNCTION(CHECK_ADD_COMPILER_FLAG flag)
    STRING(SUBSTRING "${flag}" 1 -1 FlagName)
    STRING(REPLACE "+" "X" FlagName "${FlagName}")
    CHECK_CXX_COMPILER_FLAG("${flag}" C_COMPILER_HAS_${FlagName}_FLAG)
    IF(C_COMPILER_HAS_${FlagName}_FLAG)
        ADD_COMPILE_OPTIONS("${flag}")
    ENDIF()
ENDFUNCTION()
CHECK_ADD_COMPILER_FLAG("-Weverything")
CHECK_ADD_COMPILER_FLAG("-Wlogical-op")
CHECK_ADD_COMPILER_FLAG("-Wno-covered-switch-default")
CHECK_ADD_COMPILER_FLAG("-Wno-padded")
CHECK_ADD_COMPILER_FLAG("-Wno-c++98-compat")
CHECK_ADD_COMPILER_FLAG("-Wno-c++98-compat-pedantic")
CHECK_ADD_COMPILER_FLAG("-Wno-gnu-case-range")
CHECK_ADD_COMPILER_FLAG("-Wno-weak-vtables")

ADD_DEFINITIONS(
    ${LogHard_DEFINITIONS}
    ${SharemindLibExecutionProfiler_DEFINITIONS}
    ${SharemindLibfmodapi_DEFINITIONS}
    "-D__STDC_LIMIT_MACROS"
    "-D_XOPEN_SOURCE=700"
)

INCLUDE_DIRECTORIES(
    ${LogHard_INCLUDE_DIRS}
    ${SharemindLibExecutionProfiler_INCLUDE_DIRS}
    ${SharemindLibfmodapi_INCLUDE_DIRS}
)



################################################################################
# libExecutionProgilerFacility
################################################################################

FILE(GLOB_RECURSE SHAREMIND_FACILITY_EXECUTIONPROFILER_HEADERS
     "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h")
FILE(GLOB_RECURSE SHAREMIND_FACILITY_EXECUTIONPROFILER_SOURCES
     "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
ADD_LIBRARY("sharemind_facility_executionprofiler" MODULE
            ${SHAREMIND_FACILITY_EXECUTIONPROFILER_SOURCES}
            ${SHAREMIND_FACILITY_EXECUTIONPROFILER_HEADERS}
)
STRING(REGEX REPLACE "(([^\\.]+\\.[^\\.]+)\\.).*" "\\2"
       SHAREMIND_FACILITY_EXECUTIONPROFILER_SOVERSION "${SHAREMIND_FACILITY_EXECUTIONPROFILER_VERSION}")
SET_TARGET_PROPERTIES("sharemind_facility_executionprofiler" PROPERTIES
                      OUTPUT_NAME "sharemind_facility_executionprofiler"
                      VERSION "${SHAREMIND_FACILITY_EXECUTIONPROFILER_VERSION}"
                      SOVERSION "${SHAREMIND_FACILITY_EXECUTIONPROFILER_SOVERSION}")
TARGET_LINK_LIBRARIES("sharemind_facility_executionprofiler"
    ${LogHard_LIBRARIES}
    ${SharemindLibExecutionProfiler_LIBRARIES}
)

INSTALL(TARGETS "sharemind_facility_executionprofiler"
        LIBRARY DESTINATION "lib")
