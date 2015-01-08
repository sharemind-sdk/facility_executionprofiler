#include <cassert>
#include <cctype>
#include <cstring>
#include <list>
#include <LogHard/Backend.h>
#include <LogHard/Logger.h>
#include <map>
#include <memory>
#include <sharemind/compiler-support/GccPR44436.h>
#include <sharemind/compiler-support/GccPR54526.h>
#include <sharemind/ExecutionProfiler.h>
#include <sharemind/libfmodapi/api_0x1.h>
#include <string>
#include <unordered_map>

extern "C" {

SHAREMIND_FACILITY_MODULE_API_MODULE_INFO("Profiler", 1u, 1u);

SHAREMIND_FACILITY_MODULE_API_0x1_INITIALIZER(c,errorStr);
SHAREMIND_FACILITY_MODULE_API_0x1_INITIALIZER(c,errorStr) {
    assert(c);
    try {
        const SharemindModuleApi0x1Facility * logger =
            c->findModuleFacility(c, c->conf);

        if (!logger || !logger->facility)
            return ::SHAREMIND_FACILITY_MODULE_API_0x1_MODULE_ERROR;

        ::LogHard::Logger * const b = static_cast<::LogHard::Logger *>(logger->facility);
        SharemindModuleApi0x1Facility * facility =
            new SharemindModuleApi0x1Facility{
                new sharemind::ExecutionProfiler{b->backend()},
                nullptr};
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
    SharemindModuleApi0x1Facility * facility =
        static_cast<SharemindModuleApi0x1Facility *>(c->moduleHandle);
    delete static_cast<sharemind::ExecutionProfiler *>(facility->facility);
    delete facility;
}

#define STUFF(name,NAME) \
    SHAREMIND_FACILITY_MODULE_API_0x1_FIND_ ## NAME ## _FACILITY(c, signature);\
    SHAREMIND_FACILITY_MODULE_API_0x1_FIND_ ## NAME ## _FACILITY(c, signature) \
    { \
        assert(c); \
        assert(c->moduleHandle); \
        return std::strcmp(signature, "Profiler") != 0 \
               ? nullptr \
               : static_cast<SharemindModuleApi0x1Facility *>(c->moduleHandle); \
    }
STUFF(module,MODULE)
STUFF(pd,PD)
STUFF(pdpi,PDPI)

} // extern "C" {
