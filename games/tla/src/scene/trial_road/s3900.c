#include "trial_road.h"
#include "text/msg_ids.h"

/* The byte at 0x5a of an actor, as the field its bit 0 is set through. */
struct ActorSolid {
    u8 unk00[0x5a];
    u8 solid : 8;
};

extern const u8 Data_02005c88[];
extern const u8 Data_02005be8[];
extern const u8 Data_02005d28[];
extern const u8 Data_02005dc8[];
extern const u8 Data_02005e40[];
extern const u8 Data_02005ed4[];

void Battle_WaitMode0(s32 frames);
void Motion_CamBounds(s32 a, s32 b, s32 c, s32 d);
void Motion_SetModeAndWaitAnimation(s32 actor, s32 mode);
void ObjectMotion_EnableActionAndSetCallback(s32 actor, const u8 *action);
void ObjectMotion_SetSpeedParameters(s32 actor, s32 speed, s32 acceleration);
void ObjectMotion_CommitPositionAndActivate(s32 actor, s32 dx, s32 dz);
void ObjectMotion_CommitCurrentPositionAndActivate(s32 actor);
void ObjectMotion_OffsetPositionAndResetMotion(s32 actor, s32 dx, s32 dz);
void ObjectMotion_SetPositionAndReset(s32 actor, s32 x, s32 z);
void ObjectMotion_ArmCallback(s32 actor, s32 angle, s32 frames);
void Event_StartScreenTransition(void);
void Func_02005880(void);
void Func_020059e8(s32 value);
void Func_02005970(s32 a, s32 b);
void Func_02005980(void);
void Func_02005938(s32 message);
void Func_02005940(s32 actor, s32 flags);
void Func_02005838(s32 a, s32 b, s32 c);
void Func_02005990(s32 value);

static __inline__ void Motion_CamBounds_(s32 a, s32 b, s32 c, s32 d)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Motion_CamBounds(a, b, c, d);
}

static __inline__ void Func_020058e0_(s32 id, s32 x, s32 z)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_020058e0(id, x, z);
}

static __inline__ void Func_02005970_(s32 a, s32 b)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02005970(a, b);
}

static __inline__ void ObjectMotion_SetSpeedParameters_(s32 actor, s32 speed, s32 acceleration)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    ObjectMotion_SetSpeedParameters(actor, speed, acceleration);
}

static __inline__ void ObjectMotion_ArmCallback_(s32 actor, s32 angle, s32 frames)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    ObjectMotion_ArmCallback(actor, angle, frames);
}

static __inline__ void Func_02005838_(s32 a, s32 b, s32 c)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02005838(a, b, c);
}

static __inline__ void ObjectMotion_CommitPositionAndActivate_(s32 actor, s32 dx, s32 dz)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    ObjectMotion_CommitPositionAndActivate(actor, dx, dz);
}

static __inline__ void ObjectMotion_OffsetPositionAndResetMotion_(s32 actor, s32 dx, s32 dz)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    ObjectMotion_OffsetPositionAndResetMotion(actor, dx, dz);
}

static __inline__ void ObjectMotion_SetPositionAndReset_(s32 actor, s32 x, s32 z)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    ObjectMotion_SetPositionAndReset(actor, x, z);
}

void Func_02003900(void)
{
    s32 i;

    Func_02005880();
    Func_020059e8(0);
    Func_020058e0_(gPartyState.currentOwner, 0, 0x3f80000);
    Func_020058e0_(8, 0x3f80000, 0x3f80000);
    for (i = 9; i < 80; i++) {
        struct ObjectRuntime *object = Object_GetById(i);

        if (object->x != 0)
            object->x -= 0x900000;
    }
    Motion_CamBounds_(-1, -1, -1, 0);
    ObjectMotion_EnableActionAndSetCallback(13, Data_02005c88);
    ObjectMotion_EnableActionAndSetCallback(14, Data_02005be8);
    Event_StartScreenTransition();
    Battle_WaitMode0(20);
    Func_02005970_(0x20000, 0x4000);
    Motion_CamBounds_(184 << 16, -1, 600 << 16, 1);
    Func_02005980();
    Motion_CamBounds_(120 << 16, -1, 552 << 16, 1);
    Func_02005980();
    Motion_CamBounds_(120 << 16, -1, 408 << 16, 1);
    Func_02005980();
    Motion_CamBounds_(120 << 16, -1, 120 << 16, 1);
    Func_02005938((s32)MsgTrialRoadTrapInstructions);
    Func_02005940(8, 0);
    Func_02005980();
    Motion_CamBounds_(216 << 16, -1, 120 << 16, 1);
    Func_02005980();
    Func_02005940(8, 0);
    Func_02005940(4, 0);
    ObjectMotion_EnableActionAndSetCallback(13, Data_02005d28);
    Object_RefreshSelectorById(13);
    Object_SetModeById(13, 1);
    Battle_WaitMode0(10);
    ObjectMotion_SetSpeedParameters_(13, 0x4000, 0x2000);
    OBJECT_BYTE(Object_GetById(13), 0x5a) &= ~1;
    ObjectMotion_CommitPositionAndActivate_(13, 0, 8);
    Battle_WaitMode0(1);
    /* FAKEMATCH: the bit is set through the 8-bit field, as in overlay 685: read as a plain byte the store takes the order of a store that aliases what follows. */
    ((struct ActorSolid *)Object_GetById(13))->solid |= 1;
    Battle_WaitMode0(20);
    ObjectMotion_SetSpeedParameters_(13, 0x20000, 0x10000);
    Object_SetModeById(13, 5);
    ObjectMotion_OffsetPositionAndResetMotion_(13, 0, -40);
    ObjectMotion_CommitCurrentPositionAndActivate(13);
    Func_02005a68(103);
    Func_02005838_(0x20000, 0x40000, 0x10000);
    Object_GetById(13)->velocityY = 0x40000;
    Func_02005838_(-1, -1, 0xe666);
    OBJECT_BYTE(Object_GetById(13), 0x5a) &= ~1;
    ObjectMotion_CommitPositionAndActivate_(13, 0, 16);
    Battle_WaitMode0(1);
    ((struct ActorSolid *)Object_GetById(13))->solid |= 1;
    Battle_WaitMode0(60);
    ObjectMotion_ArmCallback_(13, 0x4000, 0);
    Battle_WaitMode0(20);
    Motion_SetModeAndWaitAnimation(13, 4);
    Func_02005940(8, 0);
    ObjectMotion_ArmCallback_(13, 0xa000, 0);
    Motion_SetModeAndWaitAnimation(13, 3);
    Battle_WaitMode0(10);
    ObjectMotion_SetSpeedParameters_(13, 0x10000, 0x8000);
    ObjectMotion_CommitPositionAndActivate_(13, -16, -16);
    ObjectMotion_ArmCallback_(13, 0x8000, 0);
    Func_02005a68(124);
    Object_SetModeById(9, 6);
    Battle_WaitMode0(30);
    Func_02005940(8, 0);
    ObjectMotion_SetPositionAndReset_(13, 200, 168);
    ObjectMotion_ArmCallback_(13, 0xc000, 0);
    Battle_WaitMode0(40);
    Func_02005a68(125);
    Object_SetModeById(9, 5);
    Battle_WaitMode0(20);
    Func_02005a68(138);
    Map_Stamp(127, 3, 1, 2, 48, 3);
    Map_CopyCells(0, 127, 1, 1, 14, 8);
    Battle_WaitMode0(20);
    ObjectMotion_EnableActionAndSetCallback(13, Data_02005dc8);
    Object_RefreshSelectorById(13);
    Func_02005a68(123);
    Func_02005940(8, 0);
    ObjectMotion_EnableActionAndSetCallback(14, Data_02005e40);
    Motion_CamBounds_(328 << 16, -1, 120 << 16, 1);
    Func_02005980();
    Func_02005940(8, 0);
    Func_02005940(8, 0);
    ObjectMotion_ArmCallback_(14, 0xe000, 0);
    Motion_SetModeAndWaitAnimation(14, 3);
    Battle_WaitMode0(10);
    ObjectMotion_SetSpeedParameters_(14, 0x10000, 0x8000);
    ObjectMotion_CommitPositionAndActivate_(14, 16, -16);
    ObjectMotion_ArmCallback_(14, 0, 0);
    Func_02005a68(124);
    Object_SetModeById(11, 6);
    Object_SetModeById(12, 6);
    Battle_WaitMode0(30);
    ObjectMotion_SetPositionAndReset_(14, 350, 168);
    ObjectMotion_ArmCallback_(14, 0xc000, 0);
    Battle_WaitMode0(40);
    Func_02005a68(125);
    Object_SetModeById(11, 5);
    Battle_WaitMode0(40);
    ObjectMotion_SetPositionAndReset_(14, 364, 168);
    ObjectMotion_ArmCallback_(14, 0xc000, 0);
    Battle_WaitMode0(40);
    Func_02005a68(125);
    Object_SetModeById(12, 5);
    Battle_WaitMode0(20);
    Func_02005a68(138);
    Map_Stamp(127, 3, 1, 2, 53, 3);
    Map_CopyCells(0, 127, 1, 1, 19, 8);
    Battle_WaitMode0(30);
    ObjectMotion_ArmCallback_(14, 0xa000, 0);
    Battle_WaitMode0(10);
    Motion_SetModeAndWaitAnimation(14, 3);
    Func_02005940(8, 0);
    Motion_CamBounds_(408 << 16, -1, 120 << 16, 1);
    Func_02005980();
    Func_02005940(8, 0);
    ObjectMotion_EnableActionAndSetCallback(14, Data_02005ed4);
    Object_RefreshSelectorById(14);
    Func_02005940(8, 0);
    Func_02005990(10);
}
