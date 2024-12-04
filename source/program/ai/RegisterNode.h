#pragma once

#include "Node.h"
#include "NodeFactory.h"
#include "../util/Math.h"
#include "../util/Logger.h"
#include "../util/MemoryUtil.h"
#include <container/seadOffsetList.h>
#include <type_traits>
#include <prim/seadPtrUtil.h>
#include "lib.hpp"

namespace ai {

template <typename T>
struct RegisterNode {
    static_assert(std::is_base_of<ai::Node, T>::value, "T must derive from ai::Node");

    RegisterNode(const char* nodeName, const char* nodeCategory = "AI")
    {
        mod::util::Logger::Log("Alloc NodeFactoryEntry");
        NodeFactoryEntry* entry = (NodeFactoryEntry*)mod::util::MemoryUtil::mAllocate(sizeof(NodeFactoryEntry));
        mod::util::Logger::Log("CreateFunc");
        entry->createFunc = reinterpret_cast<ai::Node*(*)(sead::Heap*)>(exl::util::modules::GetTargetOffset(0x018df8fc));
        mod::util::Logger::Log("Gen Hash");
        entry->hash = mod::util::Math::HashMurmur32(nodeName, 0) + mod::util::Math::HashMurmur32(nodeCategory, 0);
        mod::util::Logger::Log("Empty node");
        entry->node = {};

        mod::util::Logger::Log("Init collector");
        mod::util::Logger::Log("Getting collector");
        sead::OffsetList<NodeFactoryEntry>* collector = MainNodeFactory::GetNodeCollector();
        mod::util::Logger::Log("Init offset");
        //collector->initOffset(offsetof(NodeFactoryEntry, node));
        mod::util::Logger::Log("Pushing back entry, prev size: %u", collector->size());
        //collector->pushBack(entry);
        MainNodeFactory::InsertFront((long**)collector, (long*)collector->objToListNode(entry));
        collector->mCount++;
        //collector->mCount++;
        mod::util::Logger::Log("New size: %u", collector->size());
    }
};

#define REGISTER_AI_NODE(NAME) \
    static ::ai::RegisterNode<NAME> reg(#NAME);

} // namespace ai