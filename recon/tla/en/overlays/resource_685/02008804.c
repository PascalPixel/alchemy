/* Func_02000804: differ 4. The native loads r0 last for Func_02003544(8, 258, 30) and for UiText_OpenMessageAtObject(8, 0) (r2 and r1 first); here, where both calls stand near the end of their block, the arguments tie on dependents and keep the order of the call (r0 first). Inline wrappers for either call or one helper for both give the same order. Func_020005e8 and Func_0200067c below are adopted (games/tla/src/field/overlay_685/s05e8.c) and stay here only so this draft compiles alone. */
#include "../../../../../games/tla/src/field/overlay_685/scene.h"

struct ActorSolid {
    u8 unknown_00[0x5a];
    u8 solid : 8;
};

struct EventCounters {
    u8 unknown_000[0x1c4];
    u16 count;
};

void Motion_SetVarCbAndRefresh(s32 actor, s32 variant);
void Motion_SetModeAndWaitAnimation(s32 actor, s32 mode);
void ObjectMotion_SetPositionAndReset(s32 actor, s32 x, s32 z);
void ObjectMotion_CommitPositionAndActivate(s32 actor, s32 dx, s32 dz);
void Func_020005e8(s32 actor);
static __inline__ void ObjectMotion_CommitPositionAndActivate_(s32 actor, s32 dx, s32 dz)
{
    ObjectMotion_CommitPositionAndActivate(actor, dx, dz);
}

void Func_0200067c(s32 value);
void Func_02003544(s32 actor, s32 value, s32 frames);
void Func_0200351c(s32 actor, s32 value, s32 frames);
void Func_0200361c(s32 a, s32 b);
void Func_020034d4(s32 a, s32 b, s32 c);

void Func_020005e8(s32 actor)
{
    ObjectMotion_SetPositionAndReset(gPartyState.current_owner, 648, 200);
    Func_02003534(gPartyState.current_owner, 0);
    Battle_WaitMode0(5);
    Motion_SetModeAndWaitAnimation(gPartyState.current_owner, 6);
    Battle_WaitMode0(7);
    Func_0200361c(10, actor);
    Func_020034d4(10, 648 << 16, 196 << 16);
    Object_GetById(gPartyState.current_owner)->unknown_5a[0] &= ~1;
    ObjectMotion_CommitPositionAndActivate(gPartyState.current_owner, -16, -8);
    Battle_WaitMode0(1);
    ((struct ActorSolid *)Object_GetById(gPartyState.current_owner))->solid |= 1;
    Func_02003534(gPartyState.current_owner, 0);
    Battle_WaitMode0(10);
}

void Func_0200067c(s32 value)
{
    s32 bit = 1;
    s32 mask = 0xfe;

    ObjectMotion_SetSpeedParameters(8, 0xcccc, 0x6666);
    Object_GetById(8)->unknown_5a[0] &= mask;
    ObjectMotion_CommitPositionAndActivate(8, 0, 6);
    Battle_WaitMode0(1);
    ((struct ActorSolid *)Object_GetById(8))->solid |= bit;
    Battle_WaitMode0(10);
    Object_SetModeById(8, 8);
    Battle_WaitMode0(15);
    Func_020034d4(10, 0, 0);
    Battle_WaitMode0(15);
    Object_SetModeById(8, 1);
    Battle_WaitMode0(15);
    if (value == 0) {
        Object_GetById(8)->unknown_5a[0] &= mask;
        ObjectMotion_CommitPositionAndActivate_(8, 0, -6);
        Battle_WaitMode0(1);
        ((struct ActorSolid *)Object_GetById(8))->solid |= bit;
        Battle_WaitMode0(10);
    }
}

void Func_02000804(s32 actor)
{
    Func_0200345c();
    Func_020035b4(0);
    Func_0200350c((s32)MsgThisLooksLike);
    Func_020005e8(actor);
    Func_02003534_(8, 0x3000);
    Object_SetModeById(8, 7);
    Battle_WaitMode0(30);
    Func_02003544(8, 257, 50);
    Object_SetModeById(8, 1);
    Battle_WaitMode0(15);
    Func_0200351c(8, 0, 5);
    Object_SetModeById(8, 7);
    Battle_WaitMode0(30);
    Func_02003544(8, 261, 50);
    Object_SetModeById(8, 1);
    Battle_WaitMode0(15);
    Func_0200351c(8, 0, 5);
    Object_SetModeById(8, 7);
    Battle_WaitMode0(30);
    Func_02003544(8, 256, 50);
    Object_SetModeById(8, 1);
    Battle_WaitMode0(15);
    Func_0200351c(8, 0, 5);
    Motion_SetVarCbAndRefresh(8, 2);
    Battle_WaitMode0(8);
    Func_0200351c(8, 0, 5);
    Func_0200067c(0);
    Func_02003534(8, 0x8000);
    Battle_WaitMode0(10);
    Func_02003544(8, 258, 30);
    UiText_OpenMessageAtObject(8, 0);
    if (Inventory_PromptAndSetObjectMode(4, 0) == 0) {
        Battle_WaitMode0(20);
        Func_0200351c(8, 0, 5);
        ((struct EventCounters *)Ram_HeapSlots->event_work)->count++;
    } else {
        Battle_WaitMode0(35);
        ((struct EventCounters *)Ram_HeapSlots->event_work)->count++;
        Func_0200351c(8, 0, 5);
    }
    Motion_SetModeAndWaitAnimation(8, 3);
    Func_0200351c(8, 0, 5);
    Func_02003534_(8, 0x5000);
    Func_02003464();
}
