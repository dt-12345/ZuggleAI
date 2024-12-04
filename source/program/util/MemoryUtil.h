#pragma once

#include <cstdint>

namespace mod::util
{
    namespace MemoryUtil
    {
        extern void* (*mAllocate)(uint64_t Size);
        extern void (*mFree)(void* Address);
        extern void* (*mReallocate)(void* Address, uint64_t Size);

        void Initialize();
    } // namespace MemoryUtil
} // namespace mod
