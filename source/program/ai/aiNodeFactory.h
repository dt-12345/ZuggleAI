#pragma once

#include "aiNode.h"
#include <codec/seadMurMur3.h>
#include <container/seadOffsetList.h>
#include <heap/seadHeap.h>

#include <type_traits>

extern "C" {
void* malloc(size_t size);
}

namespace ai {

struct NodeFactoryEntry {
    using CreateFunc = ai::Node* (*)(sead::Heap*);

    u32 hash;
    CreateFunc createFunc;
    sead::ListNode node;
};

using NodeFactoryList = sead::OffsetList<NodeFactoryEntry>;

NodeFactoryList* GetNodeFactoryList();

template <typename T>
struct RegisterNode {
    static_assert(std::is_base_of<ai::Node, T>::value, "T must derive from ai::Node");

    RegisterNode(const char* name, const char* category = "AI") {
        NodeFactoryEntry* entry = reinterpret_cast<NodeFactoryEntry*>(malloc(sizeof(NodeFactoryEntry)));
        entry->createFunc = Create;
        entry->hash = sead::Murmur3::calcHash(name) + sead::Murmur3::calcHash(category);
        entry->node = {};
        
        ai::GetNodeFactoryList()->pushBack(entry);
    }

    static ai::Node* Create(sead::Heap* heap) {
        return new (heap) T;
    }
};

#define REGISTER_AI_NODE(NAME) \
    static ::ai::RegisterNode<NAME> reg(#NAME);

} // namespace ai