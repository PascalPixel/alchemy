#include "types.h"
#include "party_state.h"
#include "object_runtime.h"

extern const u8 MsgWellThereS[];

#include "flag_call.h"
struct ObjectRuntime *Object_GetById(s32 id);
void WaitFrames(s32 frames);
void Func_02001f5c(void);
void Func_02001f64(void);
void Func_020020ac(s32 a);
void Func_02001fd4(s32 id, s32 target);
void Func_02002044(s32 id, s32 angle);
void Func_0200201c(const u8 *message);
void Func_02002034(s32 id, s32 a);
void Func_02001fc4(s32 id, s32 a, s32 b);
void ObjectMotion_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void ObjectMotion_ArmCallback(s32 id, s32 angle, s32 flags);
void ObjectMotion_SetPositionAndReset(s32 id, s32 x, s32 z);
void ObjectMotion_CommitCurrentPositionAndActivate(s32 id);
void ObjectMotion_ResetAndSetPosition(s32 id, s32 x, s32 z);

static __inline__ void SetSpeed(s32 id, s32 speed, s32 acceleration)
{
    /* FAKEMATCH: as in ☀️'s scene layer, forwarding through the inline sets the arguments up in their own order and keeps each call's constants; a direct call builds the costly constant first or shares it (differ 2 or more). */
    ObjectMotion_SetSpeedParameters(id, speed, acceleration);
}

static __inline__ void SetPosition(s32 id, s32 x, s32 z)
{
    /* FAKEMATCH: as in ☀️'s scene layer, forwarding through the inline sets the arguments up in their own order and keeps each call's constants; a direct call builds the costly constant first or shares it (differ 2 or more). */
    ObjectMotion_SetPositionAndReset(id, x, z);
}

void Func_02001074(void)
{
    struct ObjectRuntime *object;

    if (Flag_IsSet(0x212))
        return;
    if (Flag_IsSet(0x988))
        return;
    Flag_Set(0x212);
    if (Flag_IsSet(0x211)) {
        Func_02001f5c();
        Func_020020ac(0);
        Func_02001fd4(9, gPartyState.currentOwner);
        WaitFrames(1);
        ObjectMotion_ArmCallback(gPartyState.currentOwner, 0, 0);
        SetSpeed(9, 0xcccc, 0x6666);
        SetPosition(9, 416, 428);
        Func_02002044(9, 0x8000);
        Func_0200201c(MsgWellThereS);
        Func_02002034(9, 0);
        Object_SetModeById(9, 2);
        object = Object_GetById(gPartyState.currentOwner);
        if (object != 0)
            ObjectMotion_ResetAndSetPosition(9, *(s16 *)((u8 *)object + 10), *(s16 *)((u8 *)object + 18));
        ObjectMotion_CommitCurrentPositionAndActivate(9);
        Func_02001fc4(9, 0, 0);
        Flag_Set(0x988);
        Func_02001f64();
    } else if (Flag_IsSet(0x210)) {
        Flag_Set(0x211);
    } else {
        Flag_Set(0x210);
    }
}
