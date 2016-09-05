/*
 * Copyright (C) 2015 Cybernetica
 *
 * Research/Commercial License Usage
 * Licensees holding a valid Research License or Commercial License
 * for the Software may use this file according to the written
 * agreement between you and Cybernetica.
 *
 * GNU General Public License Usage
 * Alternatively, this file may be used under the terms of the GNU
 * General Public License version 3.0 as published by the Free Software
 * Foundation and appearing in the file LICENSE.GPL included in the
 * packaging of this file.  Please review the following information to
 * ensure the GNU General Public License version 3.0 requirements will be
 * met: http://www.gnu.org/copyleft/gpl-3.0.html.
 *
 * For further information, please contact us at sharemind@cyber.ee.
 */

#include <cassert>
#include <cstring>
#include <LogHard/Logger.h>
#include <new>
#include <sharemind/compiler-support/GccPR54526.h>
#include <sharemind/ExecutionProfiler.h>
#include <sharemind/libfmodapi/api_0x1.h>
#include <sstream>

extern "C" {

SHAREMIND_FACILITY_MODULE_API_MODULE_INFO("Profiler", 1u, 1u);

SHAREMIND_FACILITY_MODULE_API_0x1_INITIALIZER(c,errorStr);
SHAREMIND_FACILITY_MODULE_API_0x1_INITIALIZER(c,errorStr) {
    (void) errorStr;
    assert(c);

    if (!c->conf)
        return ::SHAREMIND_FACILITY_MODULE_API_0x1_INVALID_CONFIGURATION;

    try {
        // Parse configuration
        /* Configuration string:
         * <nameOfLoggerFacility> <fileNameForProfilingResults>
         */
        // If we want to handle quoted strings, then facility_loghard has a tokenizer
        ::std::stringstream configuration(c->conf);
        ::std::string loggerFacilityName, profileFileName;
        configuration >> loggerFacilityName >> profileFileName;

        if (loggerFacilityName.empty() || profileFileName.empty())
            return ::SHAREMIND_FACILITY_MODULE_API_0x1_INVALID_CONFIGURATION;

        ::SharemindModuleApi0x1Facility const * const flogger =
            c->findModuleFacility(c, loggerFacilityName.c_str());

        if (!flogger || !flogger->facility)
            return ::SHAREMIND_FACILITY_MODULE_API_0x1_INVALID_CONFIGURATION;

        ::LogHard::Logger const & logger =
            *static_cast<SHAREMIND_GCCPR54526_WORKAROUND::LogHard::Logger *>(
                    flogger->facility);

        ::sharemind::ExecutionProfiler * const profiler =
            new ::sharemind::ExecutionProfiler{logger};
        profiler->startLog(profileFileName);

        ::SharemindModuleApi0x1Facility * const facility =
            new SharemindModuleApi0x1Facility{profiler, nullptr};
        c->moduleHandle = facility;
        return ::SHAREMIND_FACILITY_MODULE_API_0x1_OK;
    } catch (::std::bad_alloc const &) {
        return ::SHAREMIND_FACILITY_MODULE_API_0x1_OUT_OF_MEMORY;
    } catch (...) {
        return ::SHAREMIND_FACILITY_MODULE_API_0x1_MODULE_ERROR;
    }
}

SHAREMIND_FACILITY_MODULE_API_0x1_DEINITIALIZER(c);
SHAREMIND_FACILITY_MODULE_API_0x1_DEINITIALIZER(c) {
    assert(c);
    assert(c->moduleHandle);
    ::SharemindModuleApi0x1Facility * const facility =
        static_cast<
            SHAREMIND_GCCPR54526_WORKAROUND::SharemindModuleApi0x1Facility *>(
                c->moduleHandle);
    assert(facility->facility);
    delete static_cast<
               SHAREMIND_GCCPR54526_WORKAROUND::sharemind::ExecutionProfiler *>(
                       facility->facility);
    delete facility;
}

#define FIND_FACILITY(NAME) \
    SHAREMIND_FACILITY_MODULE_API_0x1_FIND_ ## NAME ## _FACILITY(c, signature);\
    SHAREMIND_FACILITY_MODULE_API_0x1_FIND_ ## NAME ## _FACILITY(c, signature) \
    { \
        assert(c); \
        assert(c->moduleHandle); \
        using WORKAROUND_GCCPR54526 = ::SharemindModuleApi0x1Facility; \
        return std::strcmp(signature, "Profiler") != 0 \
               ? nullptr \
               : static_cast<WORKAROUND_GCCPR54526 *>(c->moduleHandle); \
    }
FIND_FACILITY(MODULE)
FIND_FACILITY(PD)
FIND_FACILITY(PDPI)

} // extern "C" {
