#pragma once

#include "engine/AI.h"

#include <container/seadPtrArray.h>

class OneShotZuggle : public engine::ai::OneShotBase {
    SEAD_RTTI_OVERRIDE(OneShotZuggle, engine::ai::OneShotBase)

public:
    AI_NODE_NAME_CATEGORY("OneShotZuggle", "AI");
    
    void parseParameters_() override;
    void updateImpl_(const ai::NodeCalcArg&) override;
    void oneshot_(const ai::NodeCalcArg&) override;

private:
    ai::BoolInput mIsDynamic;
    ai::BoolInput mIsReequip;
    ai::BoolInput mIsShield;
    ai::BoolInput mIsWeapon;
    ai::BoolInput mIsBow;
    int mShieldIndex = -1;
    int mWeaponIndex = -1;
    int mBowIndex = -1;
    int mTimer = 0;
};