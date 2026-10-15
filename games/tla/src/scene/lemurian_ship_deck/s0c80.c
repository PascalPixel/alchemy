#include "types.h"
#include "party_state.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "object_runtime.h"
#include "scene_ids.h"
#include "call.h"
#include "flag_call.h"
#include "text/msg_ids.h"

void Battle_WaitMode0(s32 frames);
struct ObjectRuntime *Object_GetById(s32 id);
void ObjectMotion_ArmCallback(s32 id, s32 angle, s32 frames);
void ObjectMotion_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void ObjectMotion_SetPositionAndReset(s32 id, s32 x, s32 z);
void ObjectMotion_ResetAndSetPosition(s32 id, s32 x, s32 z);
void ObjectMotion_CommitCurrentPositionAndActivate(s32 id);
void Motion_SetModeAndWaitAnimation(s32 id, s32 mode);
void Motion_CamBounds(s32 x, s32 y, s32 z, s32 flags);
void UiText_OpenMessageAtObject(s32 speaker, s32 flags);
s32 Inventory_PromptAndSetObjectMode(s32 id, s32 flags);
void Func_02001f5c(void);
void Func_02001f64(void);
void Func_02001fc4(s32 id, s32 x, s32 z);
void Func_0200201c(const void *message);
void Func_02002034(s32 id, s32 flags);
void Func_02002044(s32 id, s32 angle);
s32 Func_0200204c(s32 id, s32 emote, s32 frames);
void Func_0200205c(s32 speed, s32 acceleration);
void Func_0200206c(void);
void Func_020020ac(s32 skip);
void Func_020020b4(s32 id, s32 dx, s32 dz, s32 angle);
void Func_02001f2c(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);

static __inline__ void Func_02001f2c_(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f)
{
    /* FAKEMATCH: as in ☀️'s scene layer, forwarding through the inline sets the arguments up in their own order and keeps each call's constants; a direct call builds the costly constant first or shares it (differ 2 or more). */
    Func_02001f2c(a, b, c, d, e, f);
}

s32 Func_02000c80(void)
{
    if (gPartyState.scene == (s32)&SceneId_Ov657Room1) {
        Func_02001f2c_(2, 2, 10, 24, 18, 10);
        Func_02001f2c_(0, 62, 26, 62, 3, 8);
        Func_02001f2c_(50, 65, 25, 61, 6, 1);
    }
    return 0;
}

// Unless flag 0x8ff is set: the crew sights the ship from the deck and the
// leader is asked about it. Either way flag 0x980 is set.
void Func_02000ce0(void)
{
    s32 answer;
    s32 owner;

    if (Flag_IsSet(0x8ff) != 0) {
        Flag_Set(0x980);
        return;
    }
    Func_02001f5c();
    Call1((void (*)())Func_020020ac, 0);
    Call3((void (*)())ObjectMotion_SetSpeedParameters, 4, 0xcccc, 0x6666);
    Call3((void (*)())ObjectMotion_SetPositionAndReset, 4, 264, 136);
    Func_0200201c(MsgLookItS);
    Call4((void (*)())Func_020020b4, 5, 16, -16, 0x4000);
    Call4((void (*)())Func_020020b4, 9, 32, -16, 0x4000);
    Call4((void (*)())Func_020020b4, 6, -16, -16, 0x4000);
    Call1((void (*)())Battle_WaitMode0, 80);
    Value3(Func_0200204c, 9, 0x102, 20);
    Call3((void (*)())ObjectMotion_SetSpeedParameters, 9, 0x19999, 0xcccc);
    Call3((void (*)())ObjectMotion_SetPositionAndReset, 9, 300, 152);
    Call1((void (*)())Battle_WaitMode0, 10);
    Call3((void (*)())ObjectMotion_ArmCallback, 9, 0x5000, 20);
    Call3((void (*)())ObjectMotion_ArmCallback, 9, 0x3000, 20);
    Call3((void (*)())ObjectMotion_ArmCallback, 9, 0x5000, 0);
    Call2((void (*)())Func_02002034, 9, 0);
    Call2((void (*)())Func_0200205c, 0x4cccc, 0x9999);
    Call4((void (*)())Motion_CamBounds, 0xc80000, -1, 0x1580000, 1);
    Func_0200206c();
    Call1((void (*)())Battle_WaitMode0, 20);
    Call2((void (*)())Func_0200205c, 0x13333, 0x2666);
    Call4((void (*)())Motion_CamBounds, 0x2580000, -1, 0x1580000, 1);
    Func_0200206c();
    Call1((void (*)())Battle_WaitMode0, 40);
    Call2((void (*)())Func_0200205c, 0x66666, 0xcccc);
    Call4((void (*)())Motion_CamBounds, 0x1080000, -1, 0x800000, 1);
    Func_0200206c();
    Call2((void (*)())Motion_SetModeAndWaitAnimation, 5, 4);
    Call2((void (*)())Func_02002034, 0x2005, 0);
    Call2((void (*)())Func_02002044, 6, 0);
    Call2((void (*)())Object_SetModeById, 6, 4);
    Call2((void (*)())Func_02002034, 6, 0);
    Call2((void (*)())Func_02002044, 9, 0xb000);
    Value3(Func_0200204c, 9, 0x101, 0);
    Value2((s32 (*)())UiText_OpenMessageAtObject, 9, 0);
    Call3((void (*)())ObjectMotion_ArmCallback, 6, 0x2000, 0);
    Call3((void (*)())ObjectMotion_ArmCallback, gPartyState.currentOwner, 0x2000, 0);
    answer = Value2(Inventory_PromptAndSetObjectMode, gPartyState.currentOwner, 0);
    owner = gPartyState.currentOwner;
    if (answer == 0) {
        ObjectMotion_WaitForAnimationChange(owner);
        Call2((void (*)())Object_SetModeById, 9, 3);
        Call2((void (*)())Func_02002034, 9, 0);
        ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId++;
    } else {
        ObjectMotion_WaitForAnimationChange(owner);
        Func_0200201c(MsgLookItS + 5); // the fifth message after it
        Call2((void (*)())Func_02002034, 9, 0);
    }
    Call2((void (*)())Object_SetModeById, 5, 4);
    Call2((void (*)())Func_02002034, 0x2005, 0);
    Call2((void (*)())Func_02002044, 6, 0);
    Call2((void (*)())Object_SetModeById, 6, 4);
    Call2((void (*)())Func_02002034, 6, 0);
    Call2((void (*)())Object_SetModeById, 9, 4);
    Call2((void (*)())Func_02002034, 9, 0);
    Call3((void (*)())ObjectMotion_SetSpeedParameters, 9, 0xcccc, 0x6666);
    Object_GetById(9)->actionFlags &= ~1;
    Call3((void (*)())ObjectMotion_SetPositionAndReset, 9, 288, 144);
    Call1((void (*)())Battle_WaitMode0, 1);
    Object_GetById(9)->actionFlags |= 1;
    Call2((void (*)())Func_02002034, 0x2009, 0);
    Call2((void (*)())Object_SetModeById, 6, 3);
    Call2((void (*)())Object_SetModeById, 5, 3);
    Call2((void (*)())Object_SetModeById, 4, 3);
    Call2((void (*)())Motion_SetModeAndWaitAnimation, 9, 3);
    Call3((void (*)())ObjectMotion_SetSpeedParameters, 9, 0x13333, 0x9999);
    Call3((void (*)())ObjectMotion_SetSpeedParameters, 6, 0x13333, 0x9999);
    Call3((void (*)())ObjectMotion_SetSpeedParameters, 5, 0x13333, 0x9999);
    {
        struct ObjectRuntime *target;

        Call2((void (*)())Object_SetModeById, 9, 2);
        target = Object_GetById(4);
        if (target != NULL)
            ObjectMotion_ResetAndSetPosition(9, ((s16 *)&target->x)[1], ((s16 *)&target->z)[1]);
        Call2((void (*)())Object_SetModeById, 5, 2);
        target = Object_GetById(4);
        if (target != NULL)
            ObjectMotion_ResetAndSetPosition(5, ((s16 *)&target->x)[1], ((s16 *)&target->z)[1]);
        Call2((void (*)())Object_SetModeById, 6, 2);
        target = Object_GetById(4);
        if (target != NULL)
            ObjectMotion_ResetAndSetPosition(6, ((s16 *)&target->x)[1], ((s16 *)&target->z)[1]);
    }
    Call1((void (*)())ObjectMotion_CommitCurrentPositionAndActivate, 6);
    Call3((void (*)())Func_02001fc4, 6, 0, 0);
    Call3((void (*)())Func_02001fc4, 5, 0, 0);
    Call3((void (*)())Func_02001fc4, 9, 0, 0);
    Func_02001f64();
    Flag_Set(0x980);
}
