/* Func_0200115c: differ 2 of 613. One pair is left: after the fourth Func_0200343c the native shifts the scale (lsls r5) before it adds the object to the flags offset (adds r2, r2, r7); here the add has five dependents and the shift three, because the barriers below hide the later stores of r5.
   What this draft measured: every call with a constant goes through a scene inline (direct calls share their costly constants in r5 to r10: differ 298); the two palette rows are read with one walking pointer; the locals are declared current, target, position. The asm barriers are devices, each needed for one pair: before the object fields, after each launch (which is also called as returning a value, so r0 loads last), around the target address in the palette copy, and after the party pointer. A do-while block in their place moves the hoisted zero out of r10. */
#include "../../../../../games/tla/src/scene/champa_cliff_caves/scene.h"
#include "scene_ids.h"
#include "flag_call.h"

#define PALETTE ((u16 *)0x05000000)
#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))
#define WORD_AT(base, offset) (*(s32 *)((u8 *)(base) + (offset)))

struct EventCounters {
    u8 unk000[0x1c4];
    u16 count;
};

struct Palette5 {
    u16 colour[5];
};

struct Position {
    s32 x;
    s32 y;
    s32 z;
};

void ObjectDispatch_SetSingleChildFlags(struct FieldActor *object, s32 flags);
void Motion_SetVarCbAndRefresh(s32 actor, s32 variant);
void Motion_SetModeAndWaitAnimation(s32 actor, s32 mode);
void Motion_CamBounds(s32 a, s32 b, s32 c, s32 d);
void ObjectMotion_CommitPositionAndActivate(s32 actor, s32 dx, s32 dz);
void ObjectMotion_ResetAndSetPositionInMode2(s32 actor, s32 x, s32 z);
void Party_SetFields1f2And1f4(s32 scene, s32 entrance);
void Party_SetFields1eeAnd1f0(s32 scene, s32 entrance);
void Func_020034d4(s32 actor, s32 x, s32 z);
s32 Func_02003544(s32 actor, s32 value, s32 frames);
void Func_0200351c(s32 actor, s32 value, s32 frames);
void Func_02003564(void);
void Func_020035bc(void);
void Func_0200343c(s32 x, s32 z, s32 a, s32 b);
void Func_02003434(s32 a, s32 b, s32 c);
void Func_0200357c(s32 actor, s32 value);
void Func_02000734(struct Palette5 *from, struct Palette5 *to, s32 frames);
void Func_02002f58(struct Position *position);
void Func_02001138(void);

static __inline__ void Func_0200351c_(s32 actor, s32 value, s32 frames)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_0200351c(actor, value, frames);
}

static __inline__ void Func_02003544_(s32 actor, s32 value, s32 frames)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02003544(actor, value, frames);
}

static __inline__ void Func_020034d4_(s32 actor, s32 x, s32 z)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_020034d4(actor, x, z);
}

static __inline__ void Motion_CamBounds_(s32 a, s32 b, s32 c, s32 d)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Motion_CamBounds(a, b, c, d);
}

static __inline__ void Func_0200343c_(s32 x, s32 z, s32 a, s32 b)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_0200343c(x, z, a, b);
}

static __inline__ void Func_02003434_(s32 a, s32 b, s32 c)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02003434(a, b, c);
}

static __inline__ void ObjectMotion_ResetAndSetPositionInMode2_(s32 actor, s32 x, s32 z)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    ObjectMotion_ResetAndSetPositionInMode2(actor, x, z);
}

static __inline__ void ObjectMotion_SetPositionAndReset_(s32 actor, s32 x, s32 z)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    ObjectMotion_SetPositionAndReset(actor, x, z);
}

static __inline__ void ObjectMotion_SetSpeedParameters_(s32 id, s32 speed, s32 acceleration)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    ObjectMotion_SetSpeedParameters(id, speed, acceleration);
}

static __inline__ void UiText_OpenMessageAtObject_(s32 speaker, s32 flags)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    ((s32 (*)(s32, s32))UiText_OpenMessageAtObject)(speaker, flags);
}

void Func_0200115c(void)
{
    struct Palette5 current;
    struct Palette5 target;
    struct Position position;
    const u16 *colour;
    u16 *to;
    struct PartyState *party;
    struct FieldActor *object = Object_GetById(13);

    if (Flag_IsSet(0x97e))
        return;
    Func_0200345c();
    Func_020035b4(0);
    ObjectMotion_SetPositionAndReset(gPartyState.currentOwner, 584, 248);
    if (Flag_IsSet(0x974)) {
        Func_02003534_(gPartyState.currentOwner, 0);
        Func_0200350c((s32)MsgBackForMore);
        Func_0200351c_(11, 0, 5);
        Func_0200362c(36);
        Motion_CamBounds_(664 << 16, -1, 0x1000000, 1);
        Func_02003564();
        Battle_WaitMode0(5);
        Motion_SetModeAndWaitAnimation(8, 4);
        Func_0200351c_(0x2008, 0, 5);
        Func_02003544_(8, 265, 30);
        Func_0200351c_(0x2008, 0, 5);
    } else {
        Flag_Set(0x974);
        Func_0200350c((s32)MsgHoldItRight);
        Func_0200351c_(11, 0, 5);
        Func_0200362c(36);
        ObjectMotion_ArmCallback(gPartyState.currentOwner, 0, 0);
        Motion_CamBounds_(664 << 16, -1, 0x1000000, 1);
        Func_02003564();
        Battle_WaitMode0(5);
        ObjectMotion_Launch(11, 6, 15);
        ObjectMotion_Launch(11, 6, 23);
        Func_0200351c_(11, 0, 5);
        Func_02003544_(11, 263, 30);
        Func_0200351c_(11, 0, 5);
        Func_02003544_(8, 257, 30);
        Func_02003534_(8, 0x3000);
        Battle_WaitMode0(10);
        Func_0200351c_(8, 0, 5);
        Func_02003544_(11, 256, 30);
        Func_02003534_(11, 0xb000);
        Battle_WaitMode0(10);
        Func_0200351c_(11, 0, 5);
        Func_02003544_(11, 263, 30);
        Func_0200351c_(11, 0, 5);
        Motion_SetModeAndWaitAnimation(8, 3);
        Func_0200351c_(8, 0, 5);
        Func_02003534_(8, 0x8000);
        Battle_WaitMode0(5);
        Motion_SetModeAndWaitAnimation(8, 4);
        Func_0200351c_(8, 0, 5);
        Func_02003534_(11, 0x8000);
        Battle_WaitMode0(30);
        Func_02003534_(11, 0xb000);
        Battle_WaitMode0(10);
        Func_02003544_(11, 258, 30);
        Func_0200351c_(11, 0, 5);
        Func_02003534_(11, 0x8000);
        Battle_WaitMode0(20);
        Motion_SetVarCbAndRefresh(11, 2);
        Battle_WaitMode0(5);
        Func_0200351c_(11, 0, 5);
        Func_02003544_(8, 257, 30);
        Func_0200351c_(8, 0, 5);
        Func_02003534_(11, 0xb000);
        Battle_WaitMode0(10);
        Func_02003544_(11, 265, 45);
        Func_02003534_(11, 0x300a);
        Battle_WaitMode0(10);
        Func_0200351c_(11, 0, 5);
        Func_02003544_(8, 258, 30);
        Func_02003534_(8, 0x3000);
        Battle_WaitMode0(10);
        Func_0200351c_(8, 0, 5);
        Func_02003534_(11, 0xb000);
        Battle_WaitMode0(10);
        Motion_SetModeAndWaitAnimation(8, 4);
        Battle_WaitMode0(5);
        Func_02003534_(8, 0x8000);
        Battle_WaitMode0(5);
        Func_0200351c_(8, 0, 5);
        Motion_SetModeAndWaitAnimation(11, 3);
        Battle_WaitMode0(10);
        Motion_SetModeAndWaitAnimation(11, 3);
        Battle_WaitMode0(5);
        Func_0200351c_(11, 0, 5);
        Func_02003534_(11, 0x8000);
        Battle_WaitMode0(5);
    }
    Func_0200350c((s32)MsgOhAllRightListen);
    Func_02003534_(8, 0xb000);
    Battle_WaitMode0(20);
    Object_GetById(8)->facing = 0x5000;
    Object_SetModeById(8, 5);
    Battle_WaitMode0(30);
    Object_SetModeById(8, 6);
    Func_020034d4_(12, 724 << 16, 252 << 16);
    WORD_AT(Object_GetById(12), 24) = 0x13333;
    WORD_AT(Object_GetById(12), 28) = 0x13333;
    ObjectDispatch_SetSingleChildFlags(Object_GetById(12), 0);
    UiText_OpenMessageAtObject_(0x2008, 0);
    if (Inventory_PromptAndSetObjectMode(4, 0) == 0) {
        Func_020035bc();
        Battle_WaitMode0(15);
        Func_020034d4_(12, 0, 0);
        Object_SetModeById(8, 1);
        Object_GetById(8)->facing = 0x8000;
        Func_0200351c_(0x2008, 0, 5);
        ((struct EventCounters *)Ram_HeapSlots->eventWork)->count++;
        ObjectMotion_CommitPositionAndActivate(4, -16, 0);
        Battle_WaitMode0(5);
    } else {
        Battle_WaitMode0(25);
        ((struct EventCounters *)Ram_HeapSlots->eventWork)->count++;
        Func_0200351c_(0x2008, 0, 5);
        Func_02003544_(8, 263, 30);
        Func_0200351c_(0x2008, 0, 5);
        Func_0200362c(78);
        Func_0200343c_(656 << 16, 208 << 16, 0, -4);
        Func_0200343c_(656 << 16, 224 << 16, 0, -4);
        Func_0200343c_(656 << 16, 240 << 16, 0, -4);
        Func_0200343c_(656 << 16, 256 << 16, 0, -4);
        /* FAKEMATCH: barrier, see the header. */
        __asm__("");
        WORD_AT(object, 72) = 0x4000;
        object->motionFlags = 2;
        object->terrainHeight = 0xfc190000;
        ((s32 (*)(s32, s32, s32))ObjectMotion_Launch)(12, 10, 0);
        /* FAKEMATCH: barrier, see the header. */
        __asm__("");
        Func_0200362c(146);
        ObjectMotion_SetActionVariant(12, 1);
        ObjectMotion_SetSpeedParameters_(12, 0x30000, 0x18000);
        ObjectMotion_ResetAndSetPositionInMode2_(12, 644, 248);
        Battle_WaitMode0(21);
        Func_020034d4_(12, 0, 0);
        Battle_WaitMode0(50);
        Func_02003434_(0x20000, 0x20000, 0x10000);
        Battle_WaitMode0(70);
        Func_0200362c(148);
        Battle_WaitMode0(25);
        colour = (const u16 *)0x050000e4;
        current.colour[0] = *colour++;
        current.colour[1] = *colour++;
        /* FAKEMATCH: barrier, see the header. */
        __asm__("");
        to = target.colour;
        /* FAKEMATCH: the target address is taken here, see the header. */
        asm volatile("" : "+l"(to));
        current.colour[2] = *colour++;
        current.colour[3] = *colour++;
        current.colour[4] = *colour++;
        colour = (const u16 *)0x050001a4;
        to[0] = *colour++;
        to[1] = *colour++;
        to[2] = *colour++;
        to[3] = *colour++;
        to[4] = *colour++;
        Func_02000734(&current, (struct Palette5 *)to, 48);
        Battle_WaitMode0(20);
        Func_0200362c(194);
        position.x = 0x02960000;
        position.y = 0;
        position.z = 0xee0000;
        Func_02002f58(&position);
        Func_0200362c(147);
        Func_020034d4_(13, 664 << 16, 248 << 16);
        WORD_AT(object, 72) = 0x10000;
        object->motionFlags = 2;
        object->terrainHeight = 0;
        object->y = 0x200000;
        WORD_AT(object, 24) = 0x4000;
        WORD_AT(object, 108) = (s32)Func_02001138;
        WORD_AT(object, 28) = 0x4000;
        ((s32 (*)(s32, s32, s32))ObjectMotion_Launch)(13, 11, 0);
        /* FAKEMATCH: barrier, see the header. */
        __asm__("");
        Func_0200362c(147);
        ObjectMotion_SetSpeedParameters_(13, 0x33333, 0x19999);
        ObjectMotion_SetPositionAndReset_(13, 584, 248);
        Party_SetFields1f2And1f4((s32)&SceneId_Ov685Room4, 98);
        Party_SetFields1eeAnd1f0((s32)&SceneId_Ov685Room4, 99);
        party = &gPartyState;
        /* FAKEMATCH: barrier, see the header. */
        __asm__("");
        party->battleRule24b = 2;
        Func_0200357c(12, 5);
    }
    Func_02003464();
}
