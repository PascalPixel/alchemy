#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "party_state.h"
#include "iwram_call.h"

struct WarpTarget {
    s32 scene;
    u16 x;
    u16 z;
};

s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void WaitFrames(s32 frames);
void Battle_WaitMode0(s32 frames);
s32 ArcTan2(s32, s32);
struct ObjectRuntime *Object_GetById(s32 id);
void Animation_SetIndexAndInitObjects(struct ObjectRuntime *object, s32 index);
void ObjectMotion_ArmCallback(s32 id, s32 angle, s32 flags);
void ObjectMotion_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void ObjectMotion_CenterOnColumn(s32 id, s32 column, s32 offset);
void ObjectMotion_OffsetPositionAndResetMotion(s32 id, s32 dx, s32 dz);
void ObjectMotion_CommitCurrentPositionAndActivate(s32 id);
void Object_LinkPair(s32 id, s32 other, s32 mode);
void Object_SetModeById(u32 id, s32 mode);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_02001b7c(struct ObjectRuntime *object, s32 mode);
void Func_02001b84(s32 scene, s32 x, s32 z);
void Func_02001ba4(struct ObjectRuntime *object, s32 value);
void Func_02001bcc(void);
void Func_02001bd4(void);
void Func_02001c2c(s32 id, s32 x, s32 z);
void Func_02001c6c(s32 message);
void Func_02001c7c(s32 id, s32 a, s32 b);
void Func_02001c84(s32 a, s32 b);
void Func_02001cac(s32 id, s32 a);
void Func_02001cd4(s32 a);
void Func_02001cf4(s32 a, s32 b, s32 c);
void Func_02001d04(s32 a);
void Func_02001d44(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g);
s32 Func_02001d54(struct ObjectRuntime *object);
s32 Func_02001d5c(s32 x, s32 z, s32 layer);
void Func_02001d64(s32 x, s32 z, s32 layer, s32 value);
void Func_02001d7c(s32 a);

extern const struct WarpTarget Data_020021bc;
extern const struct WarpTarget Data_020021c4;
extern const struct WarpTarget Data_020021cc;
extern const struct WarpTarget Data_020021d4;
extern const struct WarpTarget Data_020021dc;
extern const struct WarpTarget Data_020021e4;
extern const struct WarpTarget Data_020021ec;

extern u8 MsgTheDoorIs;
extern u8 MsgItHasBeen;
extern u8 MsgEveryoneSpoilsMe;
extern u8 MsgWeAreThe;
extern u8 MsgAreThereOther;

static __inline__ s32 GameFlag_Test_(s32 flag)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    return GameFlag_Test(flag);
}

static __inline__ void GameFlag_SetBit_(s32 flag)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    GameFlag_SetBit(flag);
}

static __inline__ void WaitFrames_(s32 frames)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    WaitFrames(frames);
}

static __inline__ void Battle_WaitMode0_(s32 frames)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Battle_WaitMode0(frames);
}

static __inline__ s32 ArcTan2_(s32 a0, s32 a1)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    return ArcTan2(a0, a1);
}

static __inline__ struct ObjectRuntime * Object_GetById_(s32 id)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    return Object_GetById(id);
}

static __inline__ void Animation_SetIndexAndInitObjects_(struct ObjectRuntime *object, s32 index)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Animation_SetIndexAndInitObjects(object, index);
}

static __inline__ void ObjectMotion_ArmCallback_(s32 id, s32 angle, s32 flags)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    ObjectMotion_ArmCallback(id, angle, flags);
}

static __inline__ void ObjectMotion_SetSpeedParameters_(s32 id, s32 speed, s32 acceleration)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    ObjectMotion_SetSpeedParameters(id, speed, acceleration);
}

static __inline__ void ObjectMotion_CenterOnColumn_(s32 id, s32 column, s32 offset)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    ObjectMotion_CenterOnColumn(id, column, offset);
}

static __inline__ void ObjectMotion_OffsetPositionAndResetMotion_(s32 id, s32 dx, s32 dz)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    ObjectMotion_OffsetPositionAndResetMotion(id, dx, dz);
}

static __inline__ void ObjectMotion_CommitCurrentPositionAndActivate_(s32 id)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    ObjectMotion_CommitCurrentPositionAndActivate(id);
}

static __inline__ void Object_LinkPair_(s32 id, s32 other, s32 mode)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Object_LinkPair(id, other, mode);
}

static __inline__ void Object_SetModeById_(u32 id, s32 mode)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Object_SetModeById(id, mode);
}

static __inline__ void Event_FinishScreenTransition_(void)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Event_FinishScreenTransition();
}

static __inline__ void Event_WaitTransitionFrames_(void)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Event_WaitTransitionFrames();
}

static __inline__ void Func_02001b7c_(struct ObjectRuntime *object, s32 mode)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02001b7c(object, mode);
}

static __inline__ void Func_02001b84_(s32 scene, s32 x, s32 z)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02001b84(scene, x, z);
}

static __inline__ void Func_02001ba4_(struct ObjectRuntime *object, s32 value)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02001ba4(object, value);
}

static __inline__ void Func_02001bcc_(void)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02001bcc();
}

static __inline__ void Func_02001bd4_(void)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02001bd4();
}

static __inline__ void Func_02001c2c_(s32 id, s32 x, s32 z)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02001c2c(id, x, z);
}

static __inline__ void Func_02001c6c_(s32 message)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02001c6c(message);
}

static __inline__ void Func_02001c7c_(s32 id, s32 a, s32 b)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02001c7c(id, a, b);
}

static __inline__ void Func_02001c84_(s32 a, s32 b)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02001c84(a, b);
}

static __inline__ void Func_02001cac_(s32 id, s32 a)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02001cac(id, a);
}

static __inline__ void Func_02001cd4_(s32 a)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02001cd4(a);
}

static __inline__ void Func_02001cf4_(s32 a, s32 b, s32 c)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02001cf4(a, b, c);
}

static __inline__ void Func_02001d04_(s32 a)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02001d04(a);
}

static __inline__ void Func_02001d44_(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02001d44(a, b, c, d, e, f, g);
}

static __inline__ s32 Func_02001d54_(struct ObjectRuntime *object)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    return Func_02001d54(object);
}

static __inline__ s32 Func_02001d5c_(s32 x, s32 z, s32 layer)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    return Func_02001d5c(x, z, layer);
}

static __inline__ void Func_02001d64_(s32 x, s32 z, s32 layer, s32 value)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02001d64(x, z, layer, value);
}

static __inline__ void Func_02001d7c_(s32 a)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02001d7c(a);
}

void Func_02000428(void)
{
    Func_02001d44_(37, 72, 2, 2, 39, 71, 2);
}

void Func_02000448(void)
{
    Func_02001d44_(59, 64, 4, 3, 49, 71, 2);
}

void Func_02000468(void)
{
    Func_02001d44_(52, 69, 1, 1, 53, 69, 2);
}

void Func_02000488(void)
{
    Func_02001d44_(53, 64, 1, 1, 52, 64, 2);
}

void Func_020004a8(void)
{
    struct ObjectRuntime *leader = Object_GetById(gPartyState.currentOwner);

    Func_02001d44_(59, 64, 4, 3, 49, 71, 2);
    if (GameFlag_Test(0xfbb) == 0) {
        Func_02001c2c_(69, 0x3280000, 0x4980000);
        if (leader->x >> 20 == 50 && leader->z >> 20 == 73) {
            Func_02001bcc();
            Func_02001d04(0);
            Func_02001cac(gPartyState.currentOwner, 0x102);
            ObjectMotion_OffsetPositionAndResetMotion_(gPartyState.currentOwner, 16, 0);
            leader->velocityY = 0x60000;
            ObjectMotion_CommitCurrentPositionAndActivate(gPartyState.currentOwner);
            Func_02001bd4();
        }
    }
}
