#include "MemoryUtil.h"

#include "lib.hpp"

void* (*mod::util::MemoryUtil::mAllocate)(uint64_t Size) = nullptr;
void (*mod::util::MemoryUtil::mFree)(void* Address) = nullptr;
void* (*mod::util::MemoryUtil::mReallocate)(void* Address, uint64_t Size) = nullptr;

void mod::util::MemoryUtil::Initialize()
{
    mAllocate = reinterpret_cast<void*(*)(uint64_t)>(exl::util::modules::GetTargetOffset(0x010c54b0));
    mFree = reinterpret_cast<void(*)(void*)>(exl::util::modules::GetTargetOffset(0x0074aec0));
    mReallocate = reinterpret_cast<void*(*)(void*, uint64_t)>(exl::util::modules::GetTargetOffset(0x00754e30));
}