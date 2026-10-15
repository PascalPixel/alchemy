#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "party_state.h"

void GameFlag_SetBit(s32 flag);
void Battle_WaitMode0(s32 frames);
struct ObjectRuntime *Object_GetById(s32 id);
void Object_LinkPair(s32 a, s32 b, s32 c);
void ObjectMotion_ArmCallback(s32 id, s32 angle, s32 flags);
void ObjectMotion_OffsetPositionAndReset(u32 id, s32 dx, s32 dz);
void ObjectMotion_CommitCurrentPositionAndActivate(s32 id);
void ObjectMotion_CommitPositionAndActivate(s32 id, s32 x, s32 z);
void ObjectMotion_SetAngleToward(s32 id, s32 target, s32 flags);
void ObjectMotion_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void ObjectMotion_SetPositionAndReset(s32 id, s32 x, s32 z);
void ObjectMotion_ResetAndSetPosition(s32 id, s32 x, s32 z);
void Motion_SetModeAndWaitAnimation(s32 id, s32 mode);
void Motion_SetVarCbAndRefresh(s32 id, s32 variant);
void Motion_CamBounds(s32 x, s32 y, s32 z, s32 d);
void Func_02002b28(void);
void Func_02002b30(void);
void Func_02002b88(s32 id, s32 a, s32 b);
void Func_02002bd0(const u8 *message);
void Func_02002be0(s32 id, s32 a, s32 b);
void Func_02002bf8(s32 id, s32 a);
struct ObjectRuntime *Func_02002c08(s32 id, s32 emote, s32 frames);
void Func_02002c28(void);
void Func_02002c58(s32 a);
void Func_02002c68(s32 id, s32 a, s32 b, s32 c);

extern const u8 MsgYouThereHold[];

static __inline__ void ArmCallback(s32 id, s32 angle, s32 flags)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does (direct: differ 72 of 475). */
    ObjectMotion_ArmCallback(id, angle, flags);
}

static __inline__ void OffsetPos(u32 id, s32 dx, s32 dz)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does (direct: differ 10 of 475). */
    ObjectMotion_OffsetPositionAndReset(id, dx, dz);
}

static __inline__ void CommitPos(s32 id, s32 x, s32 z)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does (direct: differ 31 of 475). */
    ObjectMotion_CommitPositionAndActivate(id, x, z);
}

static __inline__ void Speed(s32 id, s32 speed, s32 acceleration)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does (direct: differ 24 of 475). */
    ObjectMotion_SetSpeedParameters(id, speed, acceleration);
}

static __inline__ void SetPos(s32 id, s32 x, s32 z)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does (direct: differ 51 of 475). */
    ObjectMotion_SetPositionAndReset(id, x, z);
}

static __inline__ void CamBounds(s32 x, s32 y, s32 z, s32 d)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does (direct: differ 33 of 475). */
    Motion_CamBounds(x, y, z, d);
}

static __inline__ void ArmCallbackAndRefresh(s32 id, s32 a)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does (direct: differ 44 of 475). */
    Func_02002bf8(id, a);
}

static __inline__ struct ObjectRuntime *Emote(s32 id, s32 emote, s32 frames)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does (direct: differ 8 of 475). */
    return Func_02002c08(id, emote, frames);
}

static __inline__ void LaunchFromFocused(s32 id, s32 a, s32 b, s32 c)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does (direct: differ 36 of 475). */
    Func_02002c68(id, a, b, c);
}

// "You there, hold": actors 24 and 25 talk with 32 and 4, 25 is moved
// about with its priority bit cleared and set, and 32 is placed at the
// current owner before flag 0x910 is set.
void Func_02000ac4(void)
{
    struct ObjectRuntime *object;

    Func_02002b28();
    Func_02002c58(0);
    Func_02002bd0(MsgYouThereHold);
    Func_02002be0(24, 0, 2);
    CamBounds(238 << 17, -1, 210 << 17, 1);
    OffsetPos(25, 3, 8);
    CommitPos(24, -3, 8);
    ObjectMotion_CommitCurrentPositionAndActivate(25);
    SetPos(4, 472, 426);
    Battle_WaitMode0(10);
    ArmCallbackAndRefresh(gPartyState.current_owner, 0xc000);
    LaunchFromFocused(32, 16, 1, 0xc000);
    Func_02002c28();
    ObjectMotion_CommitCurrentPositionAndActivate(32);
    ArmCallback(4, 0, 0);
    ArmCallback(32, 0x8000, 0);
    Battle_WaitMode0(30);
    Battle_WaitMode0(10);
    ArmCallback(4, 0xc000, 0);
    ArmCallback(32, 0xc000, 0);
    Battle_WaitMode0(20);
    Battle_WaitMode0(10);
    Motion_SetVarCbAndRefresh(32, 2);
    Battle_WaitMode0(20);
    Func_02002be0(32, 0, 2);
    Battle_WaitMode0(10);
    Motion_SetModeAndWaitAnimation(24, 3);
    Battle_WaitMode0(20);
    Func_02002be0(25, 0, 2);
    Battle_WaitMode0(10);
    ObjectMotion_SetAngleToward(24, 25, 0);
    Battle_WaitMode0(10);
    ObjectMotion_SetAngleToward(25, 24, 0);
    Battle_WaitMode0(10);
    Motion_SetModeAndWaitAnimation(24, 3);
    Battle_WaitMode0(30);
    ObjectMotion_SetAngleToward(25, 24, 0);
    Battle_WaitMode0(10);
    ObjectMotion_SetAngleToward(25, 32, 0);
    Motion_SetModeAndWaitAnimation(25, 3);
    Battle_WaitMode0(30);
    ArmCallback(24, 0x5000, 0);
    Object_GetById(25)->priority_flags &= 0xfe;
    ObjectMotion_SetActionVariant(25, 3);
    SetPos(25, 457, 402);
    CommitPos(25, 16, 0);
    SetPos(25, 472, 420);
    Battle_WaitMode0(30);
    SetPos(25, 484, 420);
    Object_GetById(25)->priority_flags |= 1;
    ArmCallback(25, 0x4000, 0);
    Battle_WaitMode0(60);
    Battle_WaitMode0(10);
    ArmCallback(25, 0xc000, 0);
    Battle_WaitMode0(10);
    Motion_SetModeAndWaitAnimation(25, 4);
    Func_02002be0(25, 0, 2);
    Battle_WaitMode0(10);
    Motion_SetModeAndWaitAnimation(24, 3);
    Func_02002be0(24, 0, 2);
    ArmCallbackAndRefresh(25, 0x4000);
    Battle_WaitMode0(10);
    Motion_SetModeAndWaitAnimation(25, 3);
    Battle_WaitMode0(5);
    Func_02002be0(25, 0, 2);
    Object_GetById(25)->priority_flags &= 0xfe;
    ObjectMotion_SetActionVariant(25, 3);
    CommitPos(25, 0, -16);
    SetPos(25, 475, 400);
    CommitPos(25, -16, 0);
    Object_GetById(25)->priority_flags |= 1;
    CamBounds(238 << 17, -1, 198 << 17, 1);
    SetPos(25, 459, 416);
    ArmCallback(25, 0x2000, 0);
    Battle_WaitMode0(10);
    Emote(32, 0x101, 50);
    Func_02002be0(32, 0, 2);
    Battle_WaitMode0(10);
    Motion_SetVarCbAndRefresh(24, 2);
    Battle_WaitMode0(20);
    Func_02002be0(24, 0, 2);
    Battle_WaitMode0(10);
    Emote(32, 0x100, 0);
    Emote(4, 0x100, 50);
    Battle_WaitMode0(20);
    Motion_SetModeAndWaitAnimation(25, 4);
    ArmCallback(32, 0xb000, 0);
    Func_02002be0(25, 0, 2);
    Battle_WaitMode0(10);
    Object_LinkPair(4, 32, 0);
    Battle_WaitMode0(20);
    Battle_WaitMode0(10);
    ArmCallback(4, 0xc000, 0);
    ArmCallback(32, 0xc000, 0);
    Emote(25, 0x109, 50);
    Func_02002be0(24, 0, 2);
    Emote(25, 0x103, 20);
    ArmCallback(32, 0xb000, 0);
    Func_02002be0(25, 0, 2);
    Battle_WaitMode0(10);
    Motion_SetModeAndWaitAnimation(24, 3);
    Battle_WaitMode0(30);
    ArmCallback(32, 0xd000, 0);
    Func_02002be0(24, 0, 2);
    Speed(24, 0x4ccc, 0x2666);
    Speed(25, 0x4ccc, 0x2666);
    OffsetPos(24, 3, -8);
    OffsetPos(25, -3, -8);
    ObjectMotion_CommitCurrentPositionAndActivate(24);
    ObjectMotion_CommitCurrentPositionAndActivate(25);
    ArmCallback(25, 0x3000, 0);
    ArmCallback(24, 0x5000, 0);
    Battle_WaitMode0(20);
    Battle_WaitMode0(10);
    Object_SetModeById(32, 3);
    Motion_SetModeAndWaitAnimation(4, 3);
    Battle_WaitMode0(30);
    Object_SetModeById(32, 2);
    object = Object_GetById(gPartyState.current_owner);
    if (object != NULL)
        ObjectMotion_ResetAndSetPosition(32, ((s16 *)&object->x)[1], ((s16 *)&object->z)[1]);
    ObjectMotion_CommitCurrentPositionAndActivate(32);
    Func_02002b88(32, 0, 0);
    GameFlag_SetBit(0x910);
    Func_02002b88(26, 0, 0);
    Func_02002b30();
}
