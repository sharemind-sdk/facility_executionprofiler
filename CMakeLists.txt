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
PROJECT(SharemindFacilityExecutionProfiler VERSION 0.3.0 LANGUAGES CXX)

INCLUDE("${CMAKE_CURRENT_SOURCE_DIR}/config.local" OPTIONAL)
INCLUDE("${CMAKE_CURRENT_BINARY_DIR}/config.local" OPTIONAL)

FIND_PACKAGE(SharemindCMakeHelpers 1.6 REQUIRED)


FIND_PACKAGE(LogHard 0.6.0 REQUIRED)
FIND_PACKAGE(SharemindCxxHeaders 0.9.0 REQUIRED)
FIND_PACKAGE(SharemindFacilityModuleApis 2.1.0 REQUIRED)
FIND_PACKAGE(SharemindLibExecutionProfiler 0.4.0 REQUIRED)


# The facility:
FILE(GLOB_RECURSE SharemindFacilityExecutionProfiler_HEADERS
     "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h")
FILE(GLOB_RECURSE SharemindFacilityExecutionProfiler_SOURCES
     "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
SharemindAddSharedLibrary(FacilityExecutionProfiler
    MODULE
    OUTPUT_NAME "sharemind_facility_executionprofiler"
    SOURCES
        ${SharemindFacilityExecutionProfiler_SOURCES}
        ${SharemindFacilityExecutionProfiler_HEADERS}
)
TARGET_COMPILE_DEFINITIONS(FacilityExecutionProfiler
    PRIVATE "_XOPEN_SOURCE=700")
TARGET_LINK_LIBRARIES(FacilityExecutionProfiler
    PRIVATE
        LogHard::LogHard
        Sharemind::CxxHeaders
        Sharemind::FacilityModuleApis
        Sharemind::LibExecutionProfiler
    )

# Configuration files:
INSTALL(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/packaging/configs/sharemind/"
        DESTINATION "/etc/sharemind/"
        COMPONENT "lib"
        EXCLUDE_FROM_ALL)

# Packaging:
SharemindSetupPackaging()
SharemindAddComponentPackage("lib"
    NAME "libsharemind-facility-executionprofiler"
    DESCRIPTION "Sharemind Execution Profiler facility"
    DEB_SECTION "libs"
    DEB_DEPENDS
        "libsharemind-executionprofiler (>= 0.4.0)"
        "libloghard (>= 0.5.0)"
        "libstdc++6 (>= 4.8.0)"
        "libc6 (>= 2.19)"
    DEB_EXTRA_CONTROL_FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/packaging/debian/conffiles"
)
SharemindAddComponentPackage("debug"
    NAME "libsharemind-facility-executionprofiler-dbg"
    DESCRIPTION "Sharemind Execution Profiler facility debug symbols"
    DEB_SECTION "debug"
    DEB_DEPENDS
        "libsharemind-facility-executionprofiler (= ${SharemindFacilityExecutionProfiler_DEB_lib_PACKAGE_VERSION})"
)
SharemindPackagingFinalize()
