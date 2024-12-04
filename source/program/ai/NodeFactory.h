#pragma once

#include "Node.h"
#include <cstddef>
#include <sead/container/seadOffsetList.h>
#include <sead/heap/seadHeap.h>

namespace ai {

struct NodeFactoryEntry {
    using CreateFunc = ai::Node* (*)(sead::Heap*);

    u32 hash;
    CreateFunc createFunc;
    sead::ListNode node;
};

namespace MainNodeFactory {
    extern sead::OffsetList<NodeFactoryEntry>* (*GetNodeCollector)();
    extern void* (*InsertFront)(long** Param1, long* Param2);

    void Initialize();
};

} // namespace ai