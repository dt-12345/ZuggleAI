#pragma once

#include <nn/os.h>
#include <sead/heap/seadDisposer.h>

namespace ai {

class AIFactoryMgr {
    SEAD_SINGLETON_DISPOSER(AIFactoryMgr)
public:
    u8 _20[0x34];
    nn::os::TlsSlot mTls;

    void tryPulseTLS()
    {
        union {
            u64 (*a)(nn::os::TlsSlot);
            u32 (*b)(nn::os::TlsSlot);
        } get(nn::os::GetTlsValue);
        union {
            void (*a)(nn::os::TlsSlot, u64 value);
            void (*b)(nn::os::TlsSlot, u32 value);
        } set(nn::os::SetTlsValue);

        if (get.b(mTls) & 1 << 31) {
            set.b(mTls, (1 << 31) + 1);
        }
    }
};

} // namespace ai