#pragma once

#include "misc.h"

#include "lib.hpp"

HOOK_DEFINE_INLINE(FabsFix) {
    static void Callback(exl::hook::InlineCtx* ctx) {
        const PouchActorInfoRow* pai = reinterpret_cast<const PouchActorInfoRow*>(ctx->X[8]);

        if (!pai->mPouchStockable) {
            return;
        }

        const StructHandle* slot_handle = reinterpret_cast<const StructHandle*>(ctx->X[2]);

        StructHandle effect_handle;
        if (!getStructStruct(*s_GmdMgrPtr, effect_handle, *slot_handle, 0xecff4f2a)) {
            return;
        }

        setStructEnum(*s_GmdMgrPtr, calcHash(pai->mpCureEffectType, 0), effect_handle, 0x746585e5);
        setStructInt(*s_GmdMgrPtr, pai->mCureEffectLevel, effect_handle, 0x195a67f5);
        setStructInt(*s_GmdMgrPtr, pai->mCureEffectiveTime < 1 ? 0 : pai->mCureEffectiveTime / 30, effect_handle, 0xaa1f82b8);
    }
};