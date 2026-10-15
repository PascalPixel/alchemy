#include "scene.h"

/* The byte at 0x5a of an actor, as the field its bit 0 is set through. */
struct ActorSolid {
    u8 unk00[0x5a];
    u8 solid : 8;
};

void Motion_SetModeAndWaitAnimation(s32 actor, s32 mode);
void ObjectMotion_CommitPositionAndActivate(s32 actor, s32 dx, s32 dz);
void Func_0200361c(s32 a, s32 b);
void Func_020034d4(s32 a, s32 b, s32 c);

static __inline__ void ObjectMotion_CommitPositionAndActivate_(s32 actor, s32 dx, s32 dz)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    ObjectMotion_CommitPositionAndActivate(actor, dx, dz);
}

void Func_020005e8(s32 actor)
{
    ObjectMotion_SetPositionAndReset(gPartyState.currentOwner, 648, 200);
    Func_02003534(gPartyState.currentOwner, 0);
    Battle_WaitMode0(5);
    Motion_SetModeAndWaitAnimation(gPartyState.currentOwner, 6);
    Battle_WaitMode0(7);
    Func_0200361c(10, actor);
    Func_020034d4(10, 648 << 16, 196 << 16);
    Object_GetById(gPartyState.currentOwner)->unk5a[0] &= ~1;
    ObjectMotion_CommitPositionAndActivate(gPartyState.currentOwner, -16, -8);
    Battle_WaitMode0(1);
    /* FAKEMATCH: the bit is set through the 8-bit field: read as a plain byte the store aliases the owner's load that follows, which the scheduler then puts before the zero (differ 2). */
    ((struct ActorSolid *)Object_GetById(gPartyState.currentOwner))->solid |= 1;
    Func_02003534(gPartyState.currentOwner, 0);
    Battle_WaitMode0(10);
}

void Func_0200067c(s32 value)
{
    /* FAKEMATCH: declared in this order the bit takes r6 and the mask r7; as literals they take the other's register (differ 12). */
    s32 bit = 1;
    s32 mask = 0xfe;

    ObjectMotion_SetSpeedParameters(8, 0xcccc, 0x6666);
    Object_GetById(8)->unk5a[0] &= mask;
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
        Object_GetById(8)->unk5a[0] &= mask;
        ObjectMotion_CommitPositionAndActivate_(8, 0, -6);
        Battle_WaitMode0(1);
        ((struct ActorSolid *)Object_GetById(8))->solid |= bit;
        Battle_WaitMode0(10);
    }
}
