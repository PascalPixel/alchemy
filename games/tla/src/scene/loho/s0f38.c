#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "party_state.h"

struct EventWork154 {
    u8 unk000[0x154];
    s16 value;
};

void Battle_WaitMode0(s32 frames);
struct ObjectRuntime *Object_GetById(s32 id);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object, s32 flags);
void ObjectMotion_OffsetPositionAndResetMotion(s32 id, s32 dx, s32 dz);
void ObjectMotion_CommitCurrentPositionAndActivate(s32 id);
void ObjectMotion_ArmCallback(s32 id, s32 angle, s32 flags);
void Func_020012a8(s32 srcX, s32 srcY, s32 width, s32 height, s32 destX, s32 destY);
void Func_020012e8(void);
void Func_02001348(s32 id, s32 x, s32 z);
void Func_020013d0(s32 value);
void Func_02001448(s32 cue);

static __inline__ void Func_020012a8_(s32 srcX, s32 srcY, s32 width, s32 height, s32 destX, s32 destY)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_020012a8(srcX, srcY, width, height, destX, destY);
}

static __inline__ void Func_02001348_(s32 id, s32 x, s32 z)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02001348(id, x, z);
}

void Func_02000f38(void)
{
    s32 value = ((struct EventWork154 *)Ram_HeapSlots->eventWork)->value;

    if (value == 2)
        Func_02001348_(8, 0x900000, 0xe00000);
    else
        Func_02001348_(8, 0xe00000, 0xe00000);
    ObjectMotion_ArmCallback(4, 0xc000, 0);
    Battle_WaitMode0(10);
    Object_GetById(4)->actionFlags &= 0xfe;
    Func_020012a8_(96, 14, 72, 14, 2, 1);
    ObjectMotion_OffsetPositionAndResetMotion(4, 0, 4);
    ObjectMotion_CommitCurrentPositionAndActivate(4);
    ObjectDispatch_SetSingleChildFlags(Object_GetById(4), 0);
    Object_SetModeById(4, 13);
    ObjectMotion_OffsetPositionAndResetMotion(4, 0, 16);
    ObjectMotion_CommitCurrentPositionAndActivate(4);
    Object_SetModeById(4, 10);
    Battle_WaitMode0(14);
    Func_02001448(123);
    Func_020013d0(value);
    Func_020012e8();
}
