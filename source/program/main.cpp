#include "lib.hpp"

#include "util/Logger.h"
#include "util/MemoryUtil.h"
#include "ai/node/StarlightDebugPrintS32.h"
#include "ai/NodeFactory.h"

bool RegisteredNodes = false;

HOOK_DEFINE_INLINE(Whistle) {
    static void Callback(exl::hook::InlineCtx* ctx) {
        mod::util::Logger::Log("AINB Node Reg size: %u", ai::MainNodeFactory::GetNodeCollector()->size());
    }
};

HOOK_DEFINE_TRAMPOLINE(InitializeNodeFactory)
{
    static void Callback(void* Param1)
    {
        mod::util::Logger::Log("NodeFactoryInit AINB Node Reg size 1: %u", ai::MainNodeFactory::GetNodeCollector()->size());
        Orig(Param1);
        mod::util::Logger::Log("NodeFactoryInit AINB Node Reg size 2: %u", ai::MainNodeFactory::GetNodeCollector()->size());
    }
};

HOOK_DEFINE_TRAMPOLINE(RegisterNodes)
{ 
    static void Callback()
    {
        Orig();
        //Reg AI Nodes
        if(!RegisteredNodes)
        {
            mod::ainb::node::StarlightDebugPrintS32::RegisterNode();
            RegisteredNodes = true;
        }
    }
};

extern "C" void exl_main(void* x0, void* x1) {
    exl::hook::Initialize();

    mod::util::MemoryUtil::Initialize();

    mod::util::Logger::Log("Test Log Num: %u", 19);

    ai::MainNodeFactory::Initialize();
    RegisterNodes::InstallAtOffset(0x018df8a0);

    Whistle::InstallAtOffset(0x01d8fecc);
    InitializeNodeFactory::InstallAtOffset(0x011b6f20);

    mod::ainb::node::StarlightDebugPrintS32 Class;
    Class.leaveImpl_();
}

extern "C" NORETURN void exl_exception_entry() {
    /* TODO: exception handling */
    EXL_ABORT(0x420);
}