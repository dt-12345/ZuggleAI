#pragma once

#include "prim/seadRuntimeTypeInfo.h"
#include "prim/seadSafeString.h"
#include <sead/basis/seadTypes.h>

namespace ai {

class AinbHolder;
class ParameterHandler;

template <typename T>
class Input {
protected:
    uintptr_t mPtr = 0;

    constexpr static u64 sPtrMask = 0b1111111111111111111111111111111111111111111111111111111111111100;

public:
    bool isTaggedToPulseTLS() const { return mPtr >> 1 & 1; }

    T* getPtr()
    {
        return reinterpret_cast<T*>(mPtr & sPtrMask);
    }
};

template <typename T>
class Output : public Input<T> {
public:
    void setOutput(T value);
};

class Node {
    SEAD_RTTI_BASE(Node)

    AinbHolder* mAinbHolder = nullptr;
    ParameterHandler* mParameterHolder = nullptr;
    u32 mCalcFlags = 0;
    struct {
        u8 _14 : 5 = 0;
        u16 mNodeIndex : 11 = 0;
    };
    u8 mChildNodeParamIndex = 255;
    u8 mPauseState = 0;

public:
    virtual ~Node();
    //virtual ~Node(void* Param1);
    virtual void intialize(long* Param1, long** Param2);
    virtual void intializeParameters(long* Param1, void* Param2);
    virtual void enter(long* Param1);
    virtual void update(long* Param1);
    virtual void leave(long* Param1);
    virtual void postUpdateChildren();
    virtual bool hasOutputs(long* Param1);
    virtual void* isDisableEnterInRootUpdate();
    virtual void* unk0();
    virtual void isNoCalcChildren(long* Param1);
    virtual void* isStateEndForModuleCaller();
    virtual unsigned int getResult(long Param1);
    virtual void updateRoot(long* Param1);
    virtual const char* getName();
    virtual const char* getCategory();
    virtual long getLocalParameters(long Param1);
    virtual void unk1();
    virtual void* isNoCalcChildrenImpl_();
    virtual void* isChildNotRequired();
    virtual void calcExb();
    virtual void parseParameters_(long Param1);
    virtual void enterImpl_(long* Param1, long* Param2);
    virtual void updateImpl_(long* Param1);
    virtual void leaveImpl_();
    virtual void initializeImpl_();
    virtual void intializeParametersImpl_(long* Param1);
    virtual void unk2();
    virtual void unk3();
    virtual long unk4();
    virtual long unk5();
    virtual void unk6(long Param1);

    template <typename T>
    void parseInput(const sead::SafeString& outputName, ai::Input<T>* out);
    template <typename T>
    void parseOutput(const sead::SafeString& outputName, ai::Output<T>* out);
};

#define AI_NODE_NAME_CATEGORY(NAME, CATEGORY)             \
    const char* getName() override { return NAME; } \
    const char* getCategory() override { return CATEGORY; }

} // namespace ai