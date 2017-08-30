#
# Copyright (C) 2015 Cybernetica
#
# Research/Commercial License Usage
# Licensees holding a valid Research License or Commercial License
# for the Software may use this file according to the written
# agreement between you and Cybernetica.
#
# GNU General Public License Usage
# Alternatively, this file may be used under the terms of the GNU
# General Public License version 3.0 as published by the Free Software
# Foundation and appearing in the file LICENSE.GPL included in the
# packaging of this file.  Please review the following information to
# ensure the GNU General Public License version 3.0 requirements will be
# met: http://www.gnu.org/copyleft/gpl-3.0.html.
#
# For further information, please contact us at sharemind@cyber.ee.
#

CMAKE_MINIMUM_REQUIRED(VERSION 3.0)
PROJECT(SharemindFacilityExecutionProfiler VERSION 0.1.0 LANGUAGES CXX)

INCLUDE("${CMAKE_CURRENT_SOURCE_DIR}/config.local" OPTIONAL)
INCLUDE("${CMAKE_CURRENT_BINARY_DIR}/config.local" OPTIONAL)

FIND_PACKAGE(SharemindCMakeHelpers REQUIRED)

SharemindSetProjectVersion()


FIND_PACKAGE(LogHard 0.1.0 REQUIRED)
FIND_PACKAGE(SharemindCxxHeaders REQUIRED)
FIND_PACKAGE(SharemindLibExecutionProfiler REQUIRED)
FIND_PACKAGE(SharemindLibFModapi 0.1.0 REQUIRED)


# The facility:
SharemindSetCxx11CompileOptions()
FILE(GLOB_RECURSE SharemindFacilityExecutionProfiler_HEADERS
     "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h")
FILE(GLOB_RECURSE SharemindFacilityExecutionProfiler_SOURCES
     "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
SharemindAddSharedLibrary("sharemind_facility_executionprofiler"
    MODULE
    OUTPUT_NAME "sharemind_facility_executionprofiler"
    SOURCES
        ${SharemindFacilityExecutionProfiler_SOURCES}
        ${SharemindFacilityExecutionProfiler_HEADERS}
    INCLUDE_DIRECTORIES
        ${LogHard_INCLUDE_DIRS}
        ${SharemindCxxHeaders_INCLUDE_DIRS}
        ${SharemindLibExecutionProfiler_INCLUDE_DIRS}
        ${SharemindLibFModapi_INCLUDE_DIRS}
    COMPILE_DEFINITIONS
        "_XOPEN_SOURCE=700"
    LEGACY_DEFINITIONS
        ${LogHard_DEFINITIONS}
        ${SharemindCxxHeaders_DEFINITIONS}
        ${SharemindLibExecutionProfiler_DEFINITIONS}
        ${SharemindLibFModapi_DEFINITIONS}
    LINK_LIBRARIES
        ${LogHard_LIBRARIES}
        ${SharemindLibExecutionProfiler_LIBRARIES}
)


# Packaging:
SharemindSetupPackaging()
SharemindAddComponentPackage("lib"
    NAME "libsharemind-facility-executionprofiler"
    DESCRIPTION "Sharemind Execution Profiler facility"
    DEB_SECTION "libs"
    DEB_DEPENDS
        "libsharemind-executionprofiler (>= 0.1.0)"
        "libloghard (>= 0.1.0)"
        "libstdc++6 (>= 4.8.0)"
        "libc6 (>= 2.19)"
)
SharemindAddComponentPackage("debug"
    NAME "libsharemind-facility-executionprofiler-dbg"
    DESCRIPTION "Sharemind Execution Profiler facility debug symbols"
    DEB_SECTION "debug"
    DEB_DEPENDS
        "libsharemind-facility-executionprofiler (= ${SharemindFacilityExecutionProfiler_VERSION})"
)
SharemindPackagingFinalize()
