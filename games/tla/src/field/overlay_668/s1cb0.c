#include "types.h"
#include "object_runtime.h"
#include "party_state.h"
#include "ram_buffer.h"
#include "eventwrk.h"
#include "scene_ids.h"
#include "call.h"
#include "text/msg_ids.h"

extern s32 Data_020054f4[];
extern s32 Data_0200556c[];
extern u32 Data_0200d5d0[];
extern u32 Data_02005620[];

struct ObjectRuntime *Object_GetById(s32 id);
s32 GameFlag_Test();
s32 Inventory_PromptAndSetObjectMode();
void Func_02004b6c(s32 actor, s32 a, s32 b);
void UiText_OpenMessageAtObject(s32 actor, s32 flags);
void Battle_WaitMode0();
void Event_FinishScreenTransition();
void Event_WaitTransitionFrames();
void Func_02004a94();
void Func_02004a9c();
void Func_02004b04();
void Func_02004b3c();
void Func_02004b4c();
void Func_02004b74();
void Func_02004b84();
void Func_02004b94();
void Func_02004ba4();
void Func_02004be4();
void Func_02004bec();
void GameFlag_SetBit();
void Motion_CamBounds();
void Motion_SetModeAndWaitAnimation();
void Motion_SetVarCbAndRefresh();
void ObjectMotion_ArmCallback();
void ObjectMotion_CommitCurrentPositionAndActivate();
void ObjectMotion_CommitPositionAndActivate();
void ObjectMotion_EnableActionAndResetMotion();
void ObjectMotion_EnableActionAndSetCallback();
void ObjectMotion_Launch();
void ObjectMotion_OffsetPositionAndReset();
void ObjectMotion_ResetAndSetPositionInMode2();
void ObjectMotion_SetPositionAndReset();
void ObjectMotion_SetSpeedParameters();
void ObjectMotion_SetVariantCallback();
void Object_AttachWorkTargetToObject();
void Object_LinkObjectAndSetCallback();
void Object_RefreshSelectorById();
void Object_SetModeById();

static __inline__ void Func_02004b6c_(s32 actor, s32 a, s32 b)
{
    /* FAKEMATCH: pinned, the second and third arguments load before the actor, as the native does at every call here (one reordered row per call without). */
    register s32 second asm("r1") = a;
    /* FAKEMATCH: the third argument, pinned with the second. */
    register s32 third asm("r2") = b;

    Func_02004b6c(actor, second, third);
}

static __inline__ void UiText_OpenMessageAtObject_(s32 actor, s32 flags)
{
    /* FAKEMATCH: pinned, the flags load before the actor, as the native does (one reordered row without). */
    register s32 f asm("r1") = flags;

    UiText_OpenMessageAtObject(actor, f);
}

void Func_02001cb0(void)
{
    ((void (*)())Func_02004a94)();
    Call1((void (*)())Func_02004be4, 0);
    Call1((void (*)())Func_02004b3c, (s32)MsgHeyThatBlock);
    Call3((void (*)())ObjectMotion_SetSpeedParameters, 0x1c, 0x2cccc, 0x16666);
    Call3((void (*)())ObjectMotion_SetSpeedParameters, 0x1d, 0x2cccc, 0x16666);
    Call3((void (*)())ObjectMotion_SetSpeedParameters, 0x17, 0x1cccc, 0xe666);
    Call3((void (*)())ObjectMotion_SetSpeedParameters, 0x18, 0x1cccc, 0xe666);
    Call3((void (*)())Func_02004b04, 0x1c, 0x1480000, 0x2680000);
    Call3((void (*)())ObjectMotion_ArmCallback, 0x1c, 0xe000, 0);
    Call3((void (*)())Func_02004b04, 0x1d, 0x1280000, 0x2680000);
    Call3((void (*)())ObjectMotion_ArmCallback, 0x1d, 0xe000, 0);
    Call3((void (*)())Func_02004b4c, 0x1c, 0, 0xa);
    Call3((void (*)())ObjectMotion_ArmCallback, 4, 0x4000, 0);
    {
        /* FAKEMATCH: pinned, the mode loads before the z bound is shifted (one reordered row without). */
        register s32 mode asm("r3") = 1;

        Call4((void (*)())Motion_CamBounds, 0x1480000, -1, 0x2780000, mode);
    }
    ((void (*)())Func_02004b94)();
    Call1((void (*)())Battle_WaitMode0, 0x14);
    Call2((void (*)())ObjectMotion_SetVariantCallback, 0x1c, 3);
    Call2((void (*)())Func_02004b74, 0x1c, 0x102);
    Call1((void (*)())Battle_WaitMode0, 0x32);
    Func_02004b6c_(0x1d, 0x102, 0x1e);
    Call3((void (*)())Func_02004b4c, 0x1c, 0, 0xa);
    Call3((void (*)())ObjectMotion_ArmCallback, 0x1c, 0x8000, 0);
    Call1((void (*)())Battle_WaitMode0, 0x14);
    Call2((void (*)())Motion_SetModeAndWaitAnimation, 0x1c, 3);
    Call1((void (*)())Battle_WaitMode0, 0xa);
    Call3((void (*)())Func_02004b4c, 0x1c, 0, 0xa);
    Call3((void (*)())ObjectMotion_ArmCallback, 0x1d, 0, 0);
    Call1((void (*)())Battle_WaitMode0, 0x14);
    Call2((void (*)())Motion_SetModeAndWaitAnimation, 0x1d, 3);
    Call1((void (*)())Battle_WaitMode0, 0xa);
    Call3((void (*)())Func_02004b4c, 0x1c, 0, 0xa);
    Call3((void (*)())ObjectMotion_ArmCallback, 0x1d, 0x8000, 0);
    Call1((void (*)())Battle_WaitMode0, 0xa);
    Call3((void (*)())ObjectMotion_OffsetPositionAndReset, 0x1c, -0xa0, 0);
    Call3((void (*)())ObjectMotion_CommitPositionAndActivate, 0x1d, -0xa0, 0);
    Call3((void (*)())Func_02004b04, 0x1c, 0, 0);
    Call3((void (*)())Func_02004b04, 0x1d, 0, 0);
    Call1((void (*)())Battle_WaitMode0, 0x1e);
    Call3((void (*)())Func_02004b04, 0x17, 0x1080000, 0x1800000);
    Call3((void (*)())ObjectMotion_ArmCallback, 0x17, 0, 0);
    Call3((void (*)())Func_02004b04, 0x18, 0x1180000, 0x1880000);
    Call3((void (*)())ObjectMotion_ArmCallback, 0x18, 0, 0);
    Call3((void (*)())Func_02004b04, 4, 0xe80000, 0x1800000);
    Call3((void (*)())ObjectMotion_ArmCallback, 4, 0, 0);
    Call3((void (*)())Func_02004b4c, 0x18, 0, 0xa);
    Call3((void (*)())Func_02004b4c, 0x17, 0, 0xa);
    Call3((void (*)())Func_02004b4c, 0x18, 0, 0xa);
    Call3((void (*)())Func_02004b4c, 0x17, 0, 0xa);
    Call4((void (*)())Motion_CamBounds, 0x1080000, -1, 0x1880000, 1);
    ((void (*)())Func_02004b94)();
    Call1((void (*)())Battle_WaitMode0, 0x14);
    Call3((void (*)())ObjectMotion_CommitPositionAndActivate, 0x17, -0x10, 0);
    Call1((void (*)())Battle_WaitMode0, 0x14);
    UiText_OpenMessageAtObject_(0x17, 0);
    if (Value2(Inventory_PromptAndSetObjectMode, 4, 0) == 0) {
        Call1((void (*)())Battle_WaitMode0, 0x14);
        Call2((void (*)())Motion_SetModeAndWaitAnimation, 0x17, 3);
        Call1((void (*)())Battle_WaitMode0, 0xa);
        Call3((void (*)())Func_02004b4c, 0x17, 0, 0xa);
        *(u16 *)((u8 *)Ram_HeapSlots->event_work + 0x1c4) += 1;
    } else {
        Call1((void (*)())Battle_WaitMode0, 0x28);
        Call2((void (*)())Motion_SetModeAndWaitAnimation, 0x17, 4);
        Call1((void (*)())Battle_WaitMode0, 0xa);
        *(u16 *)((u8 *)Ram_HeapSlots->event_work + 0x1c4) += 1;
        Call3((void (*)())Func_02004b4c, 0x17, 0, 0xa);
    }
    Call3((void (*)())ObjectMotion_ArmCallback, 0x17, 0, 0);
    Call1((void (*)())Battle_WaitMode0, 0x14);
    Call3((void (*)())ObjectMotion_ArmCallback, 0x18, 0x8000, 0);
    Call1((void (*)())Battle_WaitMode0, 0x1e);
    Call3((void (*)())Func_02004b4c, 0x17, 0, 0xa);
    Func_02004b6c_(0x18, 0x108, 0x2d);
    Call3((void (*)())ObjectMotion_Launch, 0x18, 6, 0xf);
    Call3((void (*)())ObjectMotion_Launch, 0x18, 6, 0x17);
    Call1((void (*)())Battle_WaitMode0, 0xa);
    Call2((void (*)())Motion_SetModeAndWaitAnimation, 0x17, 3);
    Call1((void (*)())Battle_WaitMode0, 0xa);
    Call3((void (*)())Func_02004b4c, 0x17, 0, 0xa);
    Call2((void (*)())Motion_SetModeAndWaitAnimation, 0x18, 3);
    Call1((void (*)())Battle_WaitMode0, 0xa);
    Call2((void (*)())Motion_SetModeAndWaitAnimation, 0x18, 3);
    Call1((void (*)())Battle_WaitMode0, 0x14);
    Call3((void (*)())ObjectMotion_CommitPositionAndActivate, 0x17, 0, 8);
    Call2((void (*)())Object_LinkObjectAndSetCallback, 4, 0x17);
    Call3((void (*)())ObjectMotion_OffsetPositionAndReset, 0x18, -0x60, 0);
    Call3((void (*)())ObjectMotion_CommitPositionAndActivate, 0x17, -0x60, 0);
    Call3((void (*)())ObjectMotion_OffsetPositionAndReset, 0x17, 0, 0x50);
    Call3((void (*)())ObjectMotion_CommitPositionAndActivate, 0x18, -0x20, 0);
    Call3((void (*)())ObjectMotion_CommitPositionAndActivate, 0x18, 0, 0x50);
    Call1((void (*)())ObjectMotion_EnableActionAndResetMotion, 4);
    Call3((void (*)())Func_02004b04, 0x18, 0, 0);
    Call3((void (*)())Func_02004b04, 0x17, 0, 0);
    Call2((void (*)())Object_AttachWorkTargetToObject, 4, 1);
    ((void (*)())Func_02004b94)();
    ((void (*)())Func_02004a9c)();
}

void Func_02002048(void)
{
    if (Value1((s32 (*)())GameFlag_Test, 0x8a7) == 0)
        return;
    Call1((void (*)())GameFlag_SetBit, 0x905);
    ((void (*)())Func_02004a94)();
    Call1((void (*)())Func_02004be4, 0);
    Call1((void (*)())Func_02004b3c, (s32)MsgIStillJust);
    Call2((void (*)())Func_02004b84, 0x40000, 0x8000);
    Call3((void (*)())Func_02004b04, 0x19, 0x380000, 0x2780000);
    Call3((void (*)())ObjectMotion_ArmCallback, 0x19, 0, 0);
    Call3((void (*)())Func_02004b04, 0x1b, 0x380000, 0x2880000);
    Call3((void (*)())ObjectMotion_ArmCallback, 0x1b, 0, 0);
    Call3((void (*)())Func_02004b04, 0x1c, 0x300000, 0x2680000);
    Call3((void (*)())ObjectMotion_ArmCallback, 0x1c, 0, 0);
    Call3((void (*)())Func_02004b04, 0x1d, 0x200000, 0x2600000);
    Call3((void (*)())ObjectMotion_ArmCallback, 0x1d, 0, 0);
    Call1((void (*)())Battle_WaitMode0, 0xa);
    Call3((void (*)())Func_02004b4c, 0x19, 0, 0xa);
    Call4((void (*)())Motion_CamBounds, 0x900000, -1, 0x2980000, 1);
    Call1((void (*)())Battle_WaitMode0, 0xa);
    Call3((void (*)())ObjectMotion_SetSpeedParameters, 0x1c, 0x10000, 0x8000);
    Call3((void (*)())ObjectMotion_SetSpeedParameters, 0x1d, 0x10000, 0x8000);
    Call3((void (*)())ObjectMotion_SetSpeedParameters, 0x19, 0x10000, 0x8000);
    Call3((void (*)())ObjectMotion_SetSpeedParameters, 0x1b, 0x10000, 0x8000);
    Call3((void (*)())ObjectMotion_OffsetPositionAndReset, 0x19, 0x20, 0);
    Call3((void (*)())ObjectMotion_OffsetPositionAndReset, 0x1b, 0x20, 0);
    Call3((void (*)())ObjectMotion_OffsetPositionAndReset, 0x1c, 0x18, 0x18);
    Call3((void (*)())ObjectMotion_OffsetPositionAndReset, 0x1d, 0x18, 0x18);
    Call1((void (*)())ObjectMotion_CommitCurrentPositionAndActivate, 0x1c);
    Call1((void (*)())ObjectMotion_CommitCurrentPositionAndActivate, 0x1d);
    Call3((void (*)())ObjectMotion_ArmCallback, 0x1c, 0, 0);
    Call3((void (*)())ObjectMotion_ArmCallback, 0x1d, 0, 0);
    ((void (*)())Func_02004b94)();
    Call1((void (*)())Battle_WaitMode0, 0xa);
    Func_02004b6c_(0x1c, 0x107, 0);
    Func_02004b6c_(0x1c, 0x107, 0x28);
    Call3((void (*)())ObjectMotion_SetSpeedParameters, 0x1c, 0x30000, 0x18000);
    Call3((void (*)())ObjectMotion_SetSpeedParameters, 0x1d, 0x30000, 0x18000);
    Call2((void (*)())ObjectMotion_EnableActionAndSetCallback, 0x1c, (s32)Data_020054f4);
    Call2((void (*)())ObjectMotion_EnableActionAndSetCallback, 0x1d, (s32)Data_0200556c);
    Call1((void (*)())Object_RefreshSelectorById, 0x1c);
    Call1((void (*)())Object_RefreshSelectorById, 0x1d);
    Call1((void (*)())Battle_WaitMode0, 0xa);
    Call2((void (*)())Motion_SetVarCbAndRefresh, 0x1c, 2);
    Call3((void (*)())Func_02004b4c, 0x1c, 0, 0xa);
    Call3((void (*)())ObjectMotion_Launch, 0x1d, 6, 0xf);
    Call3((void (*)())ObjectMotion_Launch, 0x1d, 6, 0x17);
    Call3((void (*)())Func_02004b4c, 0x1d, 0, 0xa);
    Call3((void (*)())ObjectMotion_SetSpeedParameters, 0x1c, 0x19999, 0xcccc);
    Call3((void (*)())ObjectMotion_SetSpeedParameters, 0x1d, 0x19999, 0xcccc);
    Call3((void (*)())ObjectMotion_SetSpeedParameters, 0x19, 0x19999, 0xcccc);
    Call3((void (*)())ObjectMotion_SetSpeedParameters, 0x1b, 0x19999, 0xcccc);
    Call2((void (*)())ObjectMotion_SetVariantCallback, 0x19, 3);
    Call2((void (*)())Func_02004b74, 0x19, 0x102);
    Call2((void (*)())ObjectMotion_SetVariantCallback, 0x1b, 3);
    Call2((void (*)())Func_02004b74, 0x1b, 0x102);
    Call1((void (*)())Battle_WaitMode0, 0x28);
    Call2((void (*)())ObjectMotion_EnableActionAndSetCallback, 0x1c, (s32)Data_0200d5d0);
    Call2((void (*)())ObjectMotion_EnableActionAndSetCallback, 0x1d, (s32)Data_02005620);
    Call1((void (*)())Battle_WaitMode0, 0x14);
    Call2((void (*)())Object_AttachWorkTargetToObject, 0x19, 1);
    Call3((void (*)())ObjectMotion_OffsetPositionAndReset, 0x19, 0x40, 0);
    Call3((void (*)())ObjectMotion_CommitPositionAndActivate, 0x1b, 0x40, 0);
    Call3((void (*)())ObjectMotion_ResetAndSetPositionInMode2, 0x19, 0xa8, 0x268);
    Call3((void (*)())ObjectMotion_SetPositionAndReset, 0x1b, 0xb8, 0x268);
    Call1((void (*)())Object_RefreshSelectorById, 0x19);
    Call2((void (*)())Object_SetModeById, 0x19, 1);
    Call3((void (*)())ObjectMotion_ArmCallback, 0x19, 0xe000, 0);
    Call3((void (*)())ObjectMotion_ArmCallback, 0x1b, 0xe000, 0);
    Call1((void (*)())Object_RefreshSelectorById, 0x1c);
    Call1((void (*)())Object_RefreshSelectorById, 0x1d);
    Call1((void (*)())Battle_WaitMode0, 0x14);
    Call2((void (*)())ObjectMotion_SetVariantCallback, 0x19, 3);
    Call2((void (*)())Func_02004b74, 0x19, 0x102);
    Call2((void (*)())ObjectMotion_SetVariantCallback, 0x1b, 3);
    Call2((void (*)())Func_02004b74, 0x1b, 0x102);
    Call1((void (*)())Battle_WaitMode0, 0x28);
    Call2((void (*)())Func_02004b84, 0x60000, 0xc000);
    Call4((void (*)())Motion_CamBounds, 0x1800000, -1, 0x1e00000, 1);
    ((void (*)())Func_02004b94)();
    Call1((void (*)())Battle_WaitMode0, 0xa);
    Call3((void (*)())Func_02004b4c, 0x19, 0, 0xa);
    Call2((void (*)())Func_02004b84, 0x60000, 0xc000);
    Call4((void (*)())Motion_CamBounds, 0x1e00000, -1, 0x1580000, 1);
    ((void (*)())Func_02004b94)();
    Call1((void (*)())Battle_WaitMode0, 0xa);
    Call3((void (*)())Func_02004b4c, 0x19, 0, 0xa);
    Call2((void (*)())Func_02004b84, 0x60000, 0xc000);
    Call4((void (*)())Motion_CamBounds, 0x1e80000, -1, 0x1c80000, 1);
    ((void (*)())Func_02004b94)();
    Call1((void (*)())Battle_WaitMode0, 0xa);
    Call3((void (*)())Func_02004b4c, 0x19, 0, 0xa);
    Call2((void (*)())Func_02004b84, 0x60000, 0xc000);
    Call4((void (*)())Motion_CamBounds, 0xc00000, -1, 0x2680000, 1);
    ((void (*)())Func_02004b94)();
    Call1((void (*)())Battle_WaitMode0, 0x14);
    Func_02004b6c_(0x19, 0x102, 0x1e);
    Call3((void (*)())ObjectMotion_ArmCallback, 0x19, 0xc000, 0);
    Call1((void (*)())Battle_WaitMode0, 0x14);
    Call3((void (*)())Func_02004b4c, 0x19, 0, 0xa);
    Call3((void (*)())ObjectMotion_ArmCallback, 0x1c, 0x3000, 0);
    Call3((void (*)())ObjectMotion_ArmCallback, 0x1d, 0x5000, 0);
    Call1((void (*)())Battle_WaitMode0, 0x1e);
    Call2((void (*)())Object_SetModeById, 0x1c, 3);
    Call2((void (*)())Motion_SetModeAndWaitAnimation, 0x1d, 3);
    Call1((void (*)())Battle_WaitMode0, 0x14);
    Call3((void (*)())ObjectMotion_ArmCallback, 0x1b, 0x8000, 0);
    Call1((void (*)())Battle_WaitMode0, 0x1e);
    Func_02004b6c_(0x1b, 0x108, 0x1e);
    Call3((void (*)())Func_02004b4c, 0x1b, 0, 0xa);
    Call3((void (*)())ObjectMotion_ArmCallback, 0x19, 0, 0);
    Call1((void (*)())Battle_WaitMode0, 0x14);
    Call2((void (*)())Motion_SetModeAndWaitAnimation, 0x19, 3);
    Call1((void (*)())Battle_WaitMode0, 0xa);
    Call3((void (*)())Func_02004b4c, 0x19, 0, 0xa);
    Call2((void (*)())Object_SetModeById, 0x1c, 3);
    Call2((void (*)())Motion_SetModeAndWaitAnimation, 0x1d, 3);
    Call1((void (*)())Battle_WaitMode0, 0x14);
    Call3((void (*)())ObjectMotion_ArmCallback, 0x19, 0xd000, 0);
    Call3((void (*)())ObjectMotion_ArmCallback, 0x1b, 0xb000, 0);
    Call1((void (*)())Battle_WaitMode0, 0x28);
    Func_02004b6c_(0x19, 0x101, 0x1e);
    Call3((void (*)())Func_02004b4c, 0x19, 0, 0xa);
    Call2((void (*)())ObjectMotion_SetVariantCallback, 0x1c, 3);
    Call2((void (*)())Func_02004b74, 0x1c, 0x102);
    Call2((void (*)())ObjectMotion_SetVariantCallback, 0x1d, 3);
    Call2((void (*)())Func_02004b74, 0x1d, 0x102);
    Call1((void (*)())Battle_WaitMode0, 0x32);
    Call3((void (*)())ObjectMotion_ArmCallback, 0x1c, 0xb000, 0);
    Call3((void (*)())ObjectMotion_ArmCallback, 0x1d, 0xb000, 0);
    Call1((void (*)())Battle_WaitMode0, 0x28);
    Func_02004b6c_(0x1c, 0x100, 0);
    Func_02004b6c_(0x1d, 0x100, 0x28);
    Call3((void (*)())ObjectMotion_ArmCallback, 0x19, 0xb000, 0);
    Call3((void (*)())ObjectMotion_ArmCallback, 0x1b, 0xb000, 0);
    Call1((void (*)())Battle_WaitMode0, 0x14);
    Call4((void (*)())Motion_CamBounds, 0x900000, -1, 0x2200000, 1);
    ((void (*)())Func_02004b94)();
    Call1((void (*)())Battle_WaitMode0, 0x14);
    Call3((void (*)())Func_02004b4c, 0x19, 0, 0xa);
    Func_02004b6c_(4, 0x102, 0x1e);
    Call3((void (*)())ObjectMotion_SetSpeedParameters, 0x19, 0x18000, 0xc000);
    Call3((void (*)())ObjectMotion_SetSpeedParameters, 0x1b, 0x18000, 0xc000);
    Call3((void (*)())ObjectMotion_OffsetPositionAndReset, 0x19, -0x20, -0x20);
    Call3((void (*)())ObjectMotion_CommitPositionAndActivate, 0x1b, -0x20, -0x20);
    Call3((void (*)())ObjectMotion_OffsetPositionAndReset, 0x19, 0, -0x18);
    Call3((void (*)())ObjectMotion_CommitPositionAndActivate, 0x1b, 0, -0x18);
    Call1((void (*)())ObjectMotion_CommitCurrentPositionAndActivate, 0x19);
    Call3((void (*)())ObjectMotion_ArmCallback, 0x19, 0xd000, 0);
    Call3((void (*)())ObjectMotion_ArmCallback, 0x1b, 0xb000, 0);
    Call1((void (*)())Battle_WaitMode0, 0x14);
    Func_02004b6c_(0x19, 0x100, 0x1e);
    Call3((void (*)())Func_02004b4c, 0x19, 0, 0xa);
    Call3((void (*)())ObjectMotion_SetPositionAndReset, 4, 0x88, 0x208);
    Call4((void (*)())Func_02004bec, 0x1a, 0x10, 0, 0x5000);
    Call3((void (*)())ObjectMotion_ArmCallback, 4, 0x4000, 0);
    Call1((void (*)())ObjectMotion_CommitCurrentPositionAndActivate, 0x1a);
    Call1((void (*)())Battle_WaitMode0, 0x1e);
    Call2((void (*)())Motion_SetModeAndWaitAnimation, 0x1a, 3);
    Call1((void (*)())Battle_WaitMode0, 0xa);
    Call3((void (*)())Func_02004b4c, 0x201a, 0, 0xa);
    Call2((void (*)())Motion_SetModeAndWaitAnimation, 0x19, 3);
    Call1((void (*)())Battle_WaitMode0, 0xa);
    Call3((void (*)())Func_02004b4c, 0x19, 0, 0xa);
    Call2((void (*)())Motion_SetModeAndWaitAnimation, 0x1a, 4);
    Call3((void (*)())Func_02004b4c, 0x201a, 0, 0xa);
    Func_02004b6c_(0x19, 0x109, 0x1e);
    Call3((void (*)())Func_02004b4c, 0x19, 0, 0xa);
    Call3((void (*)())ObjectMotion_ArmCallback, 0x1a, 0x8000, 0);
    Call1((void (*)())Battle_WaitMode0, 0x14);
    Call3((void (*)())Func_02004b4c, 0x201a, 0, 0xa);
    Call2((void (*)())Motion_SetVarCbAndRefresh, 0x1b, 2);
    Call1((void (*)())Battle_WaitMode0, 0x14);
    Call3((void (*)())ObjectMotion_ArmCallback, 0x1a, 0x5000, 0);
    Call3((void (*)())ObjectMotion_ArmCallback, 0x1b, 0x8000, 0);
    Call1((void (*)())Battle_WaitMode0, 0x14);
    Call3((void (*)())Func_02004b4c, 0x1b, 0, 0xa);
    Func_02004b6c_(0x19, 0x100, 0x1e);
    Call3((void (*)())ObjectMotion_ArmCallback, 0x19, 0, 0);
    Call1((void (*)())Battle_WaitMode0, 0x14);
    Call3((void (*)())Func_02004b4c, 0x19, 0, 0xa);
    Call3((void (*)())ObjectMotion_ArmCallback, 0x1b, 0x3000, 0);
    Call3((void (*)())ObjectMotion_ArmCallback, 0x19, 0x3000, 0);
    Call1((void (*)())Battle_WaitMode0, 0x14);
    Call3((void (*)())ObjectMotion_Launch, 0x19, 6, 0xf);
    Call3((void (*)())ObjectMotion_Launch, 0x19, 6, 0x17);
    Call3((void (*)())Func_02004b4c, 0x19, 0, 0xa);
    Call2((void (*)())Motion_SetVarCbAndRefresh, 0x1c, 2);
    Call1((void (*)())Battle_WaitMode0, 0xa);
    Call3((void (*)())Func_02004b4c, 0x1c, 0, 0xa);
    Call2((void (*)())Motion_SetModeAndWaitAnimation, 0x19, 3);
    Call1((void (*)())Battle_WaitMode0, 0x14);
    Call2((void (*)())Motion_SetVarCbAndRefresh, 0x1d, 2);
    Call1((void (*)())Battle_WaitMode0, 0xa);
    Call3((void (*)())Func_02004b4c, 0x1d, 0, 0xa);
    Call2((void (*)())Motion_SetModeAndWaitAnimation, 0x19, 3);
    Call1((void (*)())Battle_WaitMode0, 0xa);
    Call3((void (*)())Func_02004b4c, 0x19, 0, 0x14);
    Call3((void (*)())ObjectMotion_OffsetPositionAndReset, 0x1c, 0, 0x40);
    Call3((void (*)())ObjectMotion_OffsetPositionAndReset, 0x1d, 0, 0x40);
    Call1((void (*)())Battle_WaitMode0, 0x3c);
    Call3((void (*)())Func_02004b04, 0x1c, 0, 0);
    Call3((void (*)())Func_02004b04, 0x1d, 0, 0);
    Call3((void (*)())ObjectMotion_ArmCallback, 0x1b, 0xb000, 0);
    Call3((void (*)())ObjectMotion_ArmCallback, 0x19, 0xd000, 0);
    Call1((void (*)())Battle_WaitMode0, 0x14);
    Call3((void (*)())Func_02004b4c, 0x19, 0, 0xa);
    Call2((void (*)())Motion_SetModeAndWaitAnimation, 0x19, 3);
    Call1((void (*)())Battle_WaitMode0, 0xa);
    UiText_OpenMessageAtObject_(0x19, 0);
    if (Value2((s32 (*)())Inventory_PromptAndSetObjectMode, 4, 0) != 0) {
    Call1((void (*)())Battle_WaitMode0, 0x28);
    Call3((void (*)())ObjectMotion_ArmCallback, 0x1a, 0x8000, 0);
    Call1((void (*)())Battle_WaitMode0, 0x14);
    Call2((void (*)())Motion_SetModeAndWaitAnimation, 0x1a, 4);
    Call3((void (*)())Func_02004b4c, 0x201a, 0, 0xa);
        ((struct EventWork *)Ram_HeapSlots->event_work)->message_id += 1;
    } else {
    Call1((void (*)())Battle_WaitMode0, 0x1e);
    Call3((void (*)())ObjectMotion_ArmCallback, 0x1a, 0x8000, 0);
    Call1((void (*)())Battle_WaitMode0, 0x14);
    Call2((void (*)())Motion_SetModeAndWaitAnimation, 0x1a, 3);
    Call1((void (*)())Battle_WaitMode0, 0xa);
        ((struct EventWork *)Ram_HeapSlots->event_work)->message_id += 1;
        Call3((void (*)())Func_02004b4c, 0x201a, 0, 0xa);
    }
    Call3((void (*)())ObjectMotion_ArmCallback, 4, 0, 0);
    Call1((void (*)())Battle_WaitMode0, 0x14);
    Call2((void (*)())Motion_SetModeAndWaitAnimation, 4, 3);
    Call1((void (*)())Battle_WaitMode0, 0x1e);
    Call2((void (*)())Motion_SetModeAndWaitAnimation, 0x19, 3);
    Call1((void (*)())Battle_WaitMode0, 0x1e);
    Call3((void (*)())ObjectMotion_ArmCallback, 4, 0x4000, 0);
    Call3((void (*)())ObjectMotion_ArmCallback, 0x1a, 0x3000, 0);
    Call1((void (*)())Battle_WaitMode0, 0x14);
    Call3((void (*)())ObjectMotion_ArmCallback, 0x19, 0x3000, 0);
    Call3((void (*)())ObjectMotion_ArmCallback, 0x1b, 0x3000, 0);
    Call1((void (*)())Battle_WaitMode0, 0x14);
    Call3((void (*)())ObjectMotion_SetSpeedParameters, 4, 0xcccc, 0x6666);
    Call3((void (*)())ObjectMotion_SetSpeedParameters, 0x1a, 0xcccc, 0x6666);
    Call3((void (*)())ObjectMotion_SetSpeedParameters, 0x19, 0xcccc, 0x6666);
    Call3((void (*)())ObjectMotion_SetSpeedParameters, 0x1b, 0xcccc, 0x6666);
    Object_GetById(4)->motion_flags = 0;
    Object_GetById(0x1a)->motion_flags = 0;
    Object_GetById(0x19)->motion_flags = 0;
    Object_GetById(0x1b)->motion_flags = 0;
    Call3((void (*)())ObjectMotion_OffsetPositionAndReset, 4, 0, 0x78);
    Call3((void (*)())ObjectMotion_OffsetPositionAndReset, 0x1a, 0, 0x78);
    Call3((void (*)())ObjectMotion_OffsetPositionAndReset, 0x1b, 0, 0x50);
    Call3((void (*)())ObjectMotion_CommitPositionAndActivate, 0x19, 0, 0x50);
    {
        struct EventWork *event = Ram_HeapSlots->event_work;

        event->delay = 60;
        event->screen_effect = 0x100;
    }
    ((void (*)())Event_FinishScreenTransition)();
    ((void (*)())Event_WaitTransitionFrames)();
    Call2((void (*)())Func_02004ba4, (s32)&SceneId_Ov667, 81);
    ((void (*)())Func_02004a9c)();
}
