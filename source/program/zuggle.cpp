#include "zuggle.h"

#include "ai/aiContext.h"
#include "ai/aiNodeFactory.h"

// this registers the node factory;
static ai::RegisterNode<ExecuteZuggle> reg("ExecuteZuggle", "AI");

void ExecuteZuggle::parseParameters_() {
    parseBoolInput("IsDynamic", mIsDynamic);
    parseBoolInput("IsReequip", mIsReequip);
    parseBoolInput("IsShield", mIsShield);
    parseBoolInput("IsWeapon", mIsWeapon);
    parseBoolInput("IsBow", mIsBow);
    parseS32Input("Count", mCount);
}

static void set_pending_drop(GameActor* actor) {
    if (actor == nullptr) return;
    // cursed but it's the laziest solution to the dependency ring buffer being smaller on 1.0.0 than other versions
    // we don't access any members before the ring buffer so it works in this case
    actor = reinterpret_cast<GameActor*>(reinterpret_cast<uintptr_t>(actor) - 0x8);
    DynamicEquipmentComponent* cmp = actor->get_dynamic_equipment_component();
    // sets the flag that marks the equipment as waiting to be dropped (meaning it ignores all state changes except dropping)
    setPendingDrop(&cmp);
}

// function is inlined so we have to recreate it
static void setEquipedDrawn(DynamicEquipmentComponent* cmp) {
    float delta_frame = 1.f;
    if (s_VFRMgrPtr && *s_VFRMgrPtr) {
        CoreCounter* counter = getCoreCounter(*s_VFRMgrPtr);
        delta_frame = counter->raw_delta_frame;
    }

    if (cmp->state_controller.current_state) {
        cmp->state_controller.current_state->_14 = 0;
        cmp->state_controller.current_state->f09(&delta_frame);
        cmp->state_controller.previous_state = cmp->state_controller.state;
        cmp->state_controller.flags |= 2;
    }
    if (cmp->state_controller.states[EquipedDrawn]) {
        cmp->state_controller.states[EquipedDrawn]->f07(&delta_frame);
        cmp->state_controller.states[EquipedDrawn]->timer.time = 0.f;
        cmp->state_controller.states[EquipedDrawn]->timer.last_time = 0.f;
        cmp->state_controller.states[EquipedDrawn]->timer.rate = 1.f;
        cmp->state_controller.state = EquipedDrawn;
        cmp->state_controller.flags |= 1;
    }
};

static void equip_and_draw(GameActor* actor) {
    if (actor == nullptr) return;
    actor = reinterpret_cast<GameActor*>(reinterpret_cast<uintptr_t>(actor) - 0x8);
    DynamicEquipmentComponent* cmp = actor->get_dynamic_equipment_component();
    if (!cmp) return;
    setEquipedDrawn(cmp);
}

EquipmentUserComponent* ExecuteZuggle::getEquipmentUserComponent() {
    GameActor* actor = getActor();
    if (!actor) 
        return nullptr;
    actor = reinterpret_cast<GameActor*>(reinterpret_cast<uintptr_t>(actor) - 0x8); // cursed but whatever
    return actor->get_equipment_user_component();
}

void ExecuteZuggle::enterImpl_(const ai::NodeCalcArg&) {
    mNumZuggled = 0;
    mState = Zuggle;
}

void ExecuteZuggle::updateImpl_(const ai::NodeCalcArg& arg) {
    if (mNumZuggled >= mCount.get()) {
        setResult(ai::Result::Success);
        return;
    }

    switch (mState) {
        case Zuggle: {
            EquipmentUserComponent* cmp = getEquipmentUserComponent();
            if (!cmp) {
                setResult(ai::Result::Failure);
                break;
            }
            
            if (mIsShield.get()) {
                equip_and_draw(cmp->dynamic_equipment[4].get_actor());

                if (!mIsDynamic.get())
                    set_pending_drop(cmp->dynamic_equipment[4].get_actor());
                
                ActorLinkDtor(&cmp->dynamic_equipment[4]);

                getInt(*s_GmdMgrPtr, &mShieldIndex, shield_index);
                setInt(*s_GmdMgrPtr, -1, shield_index);
            }
            if (mIsWeapon.get()) {
                equip_and_draw(cmp->dynamic_equipment[0].get_actor());

                if (!mIsDynamic.get())
                    set_pending_drop(cmp->dynamic_equipment[0].get_actor());

                ActorLinkDtor(&cmp->dynamic_equipment[0]);

                getInt(*s_GmdMgrPtr, &mWeaponIndex, weapon_index);
                setInt(*s_GmdMgrPtr, -1, weapon_index);
            }
            if (mIsBow.get()) {
                equip_and_draw(cmp->dynamic_equipment[3].get_actor());

                if (!mIsDynamic.get())
                    set_pending_drop(cmp->dynamic_equipment[3].get_actor());
                
                ActorLinkDtor(&cmp->dynamic_equipment[3]);

                getInt(*s_GmdMgrPtr, &mBowIndex, bow_index);
                setInt(*s_GmdMgrPtr, -1, bow_index);
            }
            mState = WaitZuggle;
            setResult(ai::Result::Busy);
            break;
        }
        case WaitZuggle: {
            // one frame buffer to allow gamedata + equipment to update
            if (!mIsReequip.get()) {
                mState = Zuggle;
                ++mNumZuggled;
            } else {
                mState = Reequip;
            }
            break;
        }
        case Reequip: {
            EquipmentUserComponent* cmp = getEquipmentUserComponent();
            if (!cmp) {
                setResult(ai::Result::Failure);
                break;
            }
            
            if (mIsShield.get())
                equipItem(*s_PouchMgrPtr, Shield, mShieldIndex, (*s_PouchMgrPtr)->current_index);
            if (mIsWeapon.get())
                equipItem(*s_PouchMgrPtr, Weapon, mWeaponIndex, (*s_PouchMgrPtr)->current_index);
            if (mIsBow.get())
                equipItem(*s_PouchMgrPtr, Bow, mBowIndex, (*s_PouchMgrPtr)->current_index);
            mState = WaitReequip;
            setResult(ai::Result::Busy);
            break;
        }
        case WaitReequip: {
            EquipmentUserComponent* cmp = getEquipmentUserComponent();
            if (!cmp) {
                setResult(ai::Result::Failure);
                break;
            }
            
            bool ready = true;
            if (mIsShield.get())
                ready = ready && cmp->slots[Shield].state != 1;
            if (mIsWeapon.get())
                ready = ready && cmp->slots[Weapon].state != 1;
            if (mIsBow.get())
                ready = ready && cmp->slots[Bow].state != 1;
            if (!ready) {
                setResult(ai::Result::Busy);
            } else {
                mState = Zuggle;
                ++mNumZuggled;
            }
        }
    }
}

void ExecuteZuggle::leaveImpl_(const ai::NodeCalcArg&) {
    mNumZuggled = 0;
    mState = Zuggle;
}