#pragma once

#include <basis/seadTypes.h>

namespace sead {

class Murmur3 {
public:
    static u32 calcHash(const char* str, u32 seed = 0);
};

} // namespace sead