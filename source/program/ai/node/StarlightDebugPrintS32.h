#pragma once

#include "../Node.h"
#include <sead/prim/seadRuntimeTypeInfo.h>

namespace mod::ainb::node {

class StarlightDebugPrintS32 : public ai::Node {
    SEAD_RTTI_OVERRIDE(StarlightDebugPrintS32, ai::Node)

    ai::Input<int> iS32Value;

public:
    AI_NODE_NAME_CATEGORY("StarlightDebugPrintS32", "AI");

    static void RegisterNode();

    void parseParameters_(long Param1) override;
    void enterImpl_(long* Param1, long* Param2) override;
    void updateImpl_(long* Param1) override;
    void leaveImpl_() override;
};

} // namespace pe