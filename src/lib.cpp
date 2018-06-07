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
#include <sharemind/Concat.h>
#include <sharemind/Exception.h>
#include <sharemind/ExceptionMacros.h>
#include <sharemind/compiler-support/GccPR54526.h>
#include <sharemind/ExecutionProfiler.h>
#include <sharemind/facility-module-apis/api.h>
#include <sharemind/facility-module-apis/api_0x2.h>
#include <sstream>


namespace  {

char const facilityName[] = "Profiler";

namespace V2 = sharemind::FacilityModuleApis::v2;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused"
#pragma GCC diagnostic ignored "-Wunused-function"
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-member-function"
#endif
SHAREMIND_DECLARE_EXCEPTION_CONST_MSG_NOINLINE(sharemind::Exception,
                                               InvalidConfigurationException);
SHAREMIND_DEFINE_EXCEPTION_CONST_MSG_NOINLINE(
        sharemind::Exception,,
        InvalidConfigurationException,
        "Invalid configuration string given!");
SHAREMIND_DECLARE_EXCEPTION_CONST_STDSTRING_NOINLINE(
        sharemind::Exception,
        MissingLoggerFacilityException);
SHAREMIND_DEFINE_EXCEPTION_CONST_STDSTRING_NOINLINE(
        sharemind::Exception,,
        MissingLoggerFacilityException);
#ifdef __clang__
#pragma clang diagnostic pop
#endif
#pragma GCC diagnostic pop

void initializeModule(V2::ModuleInitContext & context) {
    std::stringstream configuration(context.moduleConfigurationString());

    std::string loggerFacilityName;
    if (!(configuration >> loggerFacilityName) || loggerFacilityName.empty())
        throw InvalidConfigurationException();

    std::string profileFileName;
    if (!(configuration >> profileFileName) || profileFileName.empty())
        throw InvalidConfigurationException();

    auto const loggerFacilityPtr(
                context.findFacilityModuleFacility(loggerFacilityName));
    if (!loggerFacilityPtr)
        throw MissingLoggerFacilityException(
                sharemind::concat(
                    "Missing required facility module facility \"",
                    loggerFacilityName, "\"!"));

    auto facility(std::make_shared<sharemind::ExecutionProfiler>(
                      *static_cast<LogHard::Logger const *>(
                          loggerFacilityPtr.get())));
    facility->startLog(profileFileName);

    context.registerFacilityModuleFacility(facilityName, facility);
    context.registerModuleFacility(facilityName, facility);
    context.registerPdFacility(facilityName, facility);
    context.registerPdpiFacility(facilityName, facility);
    context.registerProcessFacility(facilityName, std::move(facility));
}

} // anonymous namespace

extern "C" {

SHAREMIND_FACILITY_MODULE_API_MODULE_INFO("Profiler", 2u, 2u);
extern V2::FacilityModuleInfo sharemindFacilityModuleInfo_v2;
V2::FacilityModuleInfo sharemindFacilityModuleInfo_v2{initializeModule};

} // extern "C" {
