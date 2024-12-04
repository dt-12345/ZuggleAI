#include "StarlightDebugPrintS32.h"
#include "../RegisterNode.h"
#include "../../util/Logger.h"

namespace mod::ainb::node {

void StarlightDebugPrintS32::RegisterNode()
{
    ai::RegisterNode<StarlightDebugPrintS32> reg("StarlightDebugPrintS32", "AI");
}

void StarlightDebugPrintS32::parseParameters_(long Param1)
{
    mod::util::Logger::Log("Parse Param");
    parseInput("S32Value", &iS32Value);
}

void StarlightDebugPrintS32::enterImpl_(long* Param1, long* Param2)
{
    mod::util::Logger::Log("Debug message from StarlightDebugPrintS32: %i", *iS32Value.getPtr());

    mCalcFlags = (mCalcFlags & 0x3fffffff) | 0x80000000;
}

void StarlightDebugPrintS32::updateImpl_(long* Param1) { }

void StarlightDebugPrintS32::leaveImpl_() { }

} // namespace pe