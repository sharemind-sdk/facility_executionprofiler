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


namespace {

struct Facility: ::SharemindModuleApi0x1Facility {
    inline Facility(void * const facility, void * const context = nullptr)
        : ::SharemindModuleApi0x1Facility{facility, context}
    {}
    virtual ~Facility() noexcept {}
};
using FacilityPointer = ::std::shared_ptr<Facility>;

struct ExecutionProfilerFacility: Facility {
    inline ExecutionProfilerFacility(::LogHard::Backend & backend)
        : Facility{new ::sharemind::ExecutionProfiler(::LogHard::Logger(backend))}
        , executionProfiler{static_cast<SHAREMIND_GCCPR54526::sharemind::ExecutionProfiler *>(
                     this->facility)}
    {}
    ::std::unique_ptr<SHAREMIND_GCCPR54526::sharemind::ExecutionProfiler> executionProfiler;
};

using FacilityMap = ::std::unordered_map<SHAREMIND_GCCPR54526::std::string,
                                         FacilityPointer>;

struct ModuleData {
    inline ModuleData(const char* conf) {};
    FacilityMap moduleFacilities;
    FacilityMap pdFacilities;
    FacilityMap pdpiFacilities;
};

} // anonymous namespace

extern "C" {

SHAREMIND_FACILITY_MODULE_API_MODULE_INFO("Profiler", 1u, 1u);

SHAREMIND_FACILITY_MODULE_API_0x1_INITIALIZER(c,errorStr);
SHAREMIND_FACILITY_MODULE_API_0x1_INITIALIZER(c,errorStr) {
    assert(c);
    try {
        c->moduleHandle = new ModuleData{c->conf};
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
    delete static_cast<ModuleData *>(c->moduleHandle);
}

#define STUFF(name,NAME) \
    SHAREMIND_FACILITY_MODULE_API_0x1_FIND_ ## NAME ## _FACILITY(c, signature);\
    SHAREMIND_FACILITY_MODULE_API_0x1_FIND_ ## NAME ## _FACILITY(c, signature) \
    { \
        assert(c); \
        assert(c->moduleHandle); \
        auto const & map = \
                static_cast<ModuleData *>(c->moduleHandle)->name ## Facilities;\
        auto const it = map.find(signature); \
        return (it == map.cend()) ? nullptr : it->second.get(); \
    }
STUFF(module,MODULE)
STUFF(pd,PD)
STUFF(pdpi,PDPI)

} // extern "C" {
