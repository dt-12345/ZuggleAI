#include "NodeFactory.h"

#include "lib.hpp"

namespace ai {
    sead::OffsetList<NodeFactoryEntry>* (*MainNodeFactory::GetNodeCollector)() = nullptr;
    void* (*MainNodeFactory::InsertFront)(long** Param1, long* Param2) = nullptr;

    void MainNodeFactory::Initialize()
    {
        GetNodeCollector = reinterpret_cast<sead::OffsetList<NodeFactoryEntry>*(*)()>(exl::util::modules::GetTargetOffset(0x0076c224));
        InsertFront = reinterpret_cast<void*(*)(long**, long*)>(exl::util::modules::GetTargetOffset(0x00750854));
    }
}