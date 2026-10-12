/* Near miss of Func_02000324 (120 instructions, 2 differ): the native loads the table address
 * between the two halves of found * 3 (lsls, ldr, adds); the draft adds first. Tried: a local
 * table pointer (81 differ). */
#include "../../../../games/tla/src/field/overlay_656/ov656.h"

struct Ov656Warp {
    u16 exit;
    s32 value;
    u16 x;
    u16 z;
};

extern struct Ov656Warp Data_02003904[];
extern struct Ov656Warp Data_0200397c[];
void Func_02002c28(s32 value, s32 x, s32 z);
void ObjectMotion_OffsetPositionAndResetMotion(s32 actor, s32 a, s32 b);
void ObjectMotion_CenterOnColumn(s32 actor, s32 a, s32 b);

static inline void Ov656_ObjOffset(s32 actor, s32 a, s32 b)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    ObjectMotion_OffsetPositionAndResetMotion(actor, a, b);
}

static inline void Ov656_ObjCenter(s32 actor, s32 a, s32 b)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    ObjectMotion_CenterOnColumn(actor, a, b);
}

void Func_02000324(void)
{
    struct Ov656EventWork *work = Ram_HeapSlots->event_work;
    struct Ov656Object *object;
    u8 *entry;
    s32 offset;
    u32 found;
    u32 i;

    Func_02002c68();
    Func_02002da8(0);
    found = 0;
    i = 8;
next:
    object = Object_GetById(i);
    if (object != NULL)
        object->motion_flags = 0;
    i++;
    if (i < 64)
        goto next;
    entry = (u8 *)Data_0200397c;
    i = 0;
    offset = 0;
next2:
    if (work->exit == *(u16 *)(entry + offset))
        found = i;
    i++;
    offset += 12;
    if (i < 4)
        goto next2;
    if (found < 2 && GameFlag_Test(0x93f) && GameFlag_Test(0x9e4) == 0)
        found += 4;
    Func_02002de0(158);
    ObjectMotion_SetActionVariant(gPartyState.current_owner, 1);
    Ov656_MapCopy(0, 0, Data_0200397c[found].x, Data_0200397c[found].z, 1, 2);
    Func_02002c28(Data_0200397c[found].value, Data_0200397c[found].x, Data_0200397c[found].z + 60);
    Object_SetModeById(gPartyState.current_owner, 2);
    ObjectMotion_SetSpeedParameters(gPartyState.current_owner, 0x8000, 0x4000);
    ObjectMotion_CenterOnColumn(gPartyState.current_owner, 2, -4);
    Battle_WaitMode0(4);
    Func_02002d70(work->exit);
}
