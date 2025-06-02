#include "lib.hpp"

#include "zuggle.h"

// just ignore these
// TODO: port these offsets to other versions
static constexpr u64 s_WhistleOffsets[] = {
    0x01d2e1dc, 0, 0, 0, 0, 0x01d8fecc,
};
static constexpr u64 s_DtorOffsets[] = {
    0x009665d0, 0, 0, 0, 0, 0x0076eb88,
};
static constexpr u64 s_MgrOffsets[] = {
    0x04646c60, 0, 0, 0, 0, 0x04721b98,
};
static constexpr u64 s_SetOffsets[] = {
    0x00b90968, 0, 0, 0, 0, 0x00b51b08,
};
static constexpr u64 s_SetDropOffsets[] = {
    0x0153e984, 0, 0, 0, 0, 0x0156ac04,
};
static constexpr u64 s_CoreCountOffsets[] = {
    0x00850d7c, 0, 0, 0, 0, 0x0065a508,
};
static constexpr u64 s_VFRMgrOffsets[] = {
    0x04661598, 0, 0, 0, 0, 0x04725bb8,
};
static constexpr u64 s_PauseOffsets[] = {
    0x00745e6c, 0, 0, 0, 0, 0x00786948,
};

extern "C" void exl_main(void* x0, void* x1) {
    exl::hook::Initialize();

    ActorLinkDtor = reinterpret_cast<Dtor*>(exl::util::modules::GetTargetOffset(s_DtorOffsets[0]));
    setInt = reinterpret_cast<SetInt*>(exl::util::modules::GetTargetOffset(s_SetOffsets[0]));
    getInt = reinterpret_cast<GetInt*>(exl::util::modules::GetTargetOffset(0x00996230));
    setPendingDrop = reinterpret_cast<SetPendingDrop*>(exl::util::modules::GetTargetOffset(s_SetDropOffsets[0]));
    getCoreCounter = reinterpret_cast<GetCounter*>(exl::util::modules::GetTargetOffset(s_CoreCountOffsets[0]));
    requestPause = reinterpret_cast<RequestPause*>(exl::util::modules::GetTargetOffset(s_PauseOffsets[0]));
    equipItem = reinterpret_cast<EquipFunc*>(exl::util::modules::GetTargetOffset(0x01a15c78));
    s_GmdMgrPtr = reinterpret_cast<void**>(exl::util::modules::GetTargetOffset(s_MgrOffsets[0]));
    s_VFRMgrPtr = reinterpret_cast<void**>(exl::util::modules::GetTargetOffset(s_VFRMgrOffsets[0]));
    s_PouchMgrPtr = reinterpret_cast<PouchContentMgr**>(exl::util::modules::GetTargetOffset(0x045f28a8));
}

extern "C" NORETURN void exl_exception_entry() {
    /* TODO: exception handling */
    EXL_ABORT(0x420);
}