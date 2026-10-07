#include "TYPES.H"
#include "RAM_BUFFER.H"
#include "OBJECT_RUNTIME.H"
#include "PARTY_STATE.H"

s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
s32 GameFlag_GetByte(s32 flag);
void GameFlag_SetByte(s32 flag, s32 value);
void WaitFrames(s32 frames);
void Battle_WaitMode0(s32 frames);
struct ObjectRuntime *Object_GetById(s32 id);
void Object_SetModeById(u32 id, s32 mode);
void ObjectMotion_SetActionVariant(u32 id, s32 variant);
void ObjectMotion_ArmCallback(s32 id, s32 angle, s32 flags);
void ObjectMotion_SetAngleToward(s32 id, s32 target, s32 flags);
void ObjectMotion_OffsetPositionAndResetMotion(s32 id, s32 dx, s32 dz);
void ObjectMotion_CommitCurrentPositionAndActivate(s32 id);
void ObjectMotion_CommitPositionAndActivate(s32 id, s32 x, s32 z);
void Motion_CamBounds(s32 a, s32 b, s32 c, s32 d);
void Func_02002764(s32 id, s32 a);
void Func_02002778(s32 a, s32 b);
void Func_02002840(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02002898(void);
void Func_020028a0(void);
void Func_020028d0(s32 id, s32 x, s32 z);
void Func_020028e0(s32 id, s32 a, s32 b);
void Func_02002908(s32 id, s32 a, s32 b);
void Func_02002910(s32 id, s32 a);
void Func_02002928(void);
void Func_02002930(s32 a);
void Func_02002938(s32 a, s32 b);
void Func_02002940(s32 a, s32 b);
void Func_02002958(s32 a, s32 b, s32 c);
void Func_02002968(s32 a);
void Func_02002990(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g);
void Func_020029a8(s32 a, s32 b, s32 c);
void Func_020029b0(s32 a);
s32 Random16Far(void);
struct ObjectRuntime *Func_02002808(s32 type, s32 x, s32 y, s32 z);
void Func_02002800(struct ObjectRuntime *object, const void *data);
void Func_02002838(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void ObjectMotion_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object, s32 flags);
void Animation_SetStateFlags(struct ObjectRuntime *object, s32 flags);
void Scheduler_RemoveCallbackFar(void (*callback)(void));

extern s32 Data_02003458;
extern s32 Data_0200345c;
extern s32 Data_02003460;
extern const u8 Data_020029c8[];
void Func_02002948(void);
void Func_02002084(s32 a);
void Func_020029a0(s32 message);
void Func_020027f8(struct ObjectRuntime *object, s32 a);
void Func_02002880(struct ObjectRuntime *object, s32 a);
s32 Func_02002730(void);
void Func_0200271c(s32 a, s32 b);
void Func_0200273c(s32 a);
void Func_02002978(s32 a);
void Func_02000810(s32 id);
void Func_020008d8(s32 id);
extern u32 gFrameCount;
extern const u8 Data_02002a48[];
extern const u8 Data_02002a54[];
extern u8 MsgFieldIThoughtISawSomething;
void Func_02000c24(void);
void Func_02000ca0(void);
void Func_02000cec(void);

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

static __inline__ void GameFlag_ClearBit_(s32 flag)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    GameFlag_ClearBit(flag);
}

static __inline__ s32 GameFlag_GetByte_(s32 flag)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    return GameFlag_GetByte(flag);
}

static __inline__ void GameFlag_SetByte_(s32 flag, s32 value)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    GameFlag_SetByte(flag, value);
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

static __inline__ struct ObjectRuntime * Object_GetById_(s32 id)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    return Object_GetById(id);
}

static __inline__ void Object_SetModeById_(u32 id, s32 mode)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Object_SetModeById(id, mode);
}

static __inline__ void ObjectMotion_SetActionVariant_(u32 id, s32 variant)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    ObjectMotion_SetActionVariant(id, variant);
}

static __inline__ void ObjectMotion_ArmCallback_(s32 id, s32 angle, s32 flags)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    ObjectMotion_ArmCallback(id, angle, flags);
}

static __inline__ void ObjectMotion_SetAngleToward_(s32 id, s32 target, s32 flags)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    ObjectMotion_SetAngleToward(id, target, flags);
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

static __inline__ void ObjectMotion_CommitPositionAndActivate_(s32 id, s32 x, s32 z)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    ObjectMotion_CommitPositionAndActivate(id, x, z);
}

static __inline__ void Motion_CamBounds_(s32 a, s32 b, s32 c, s32 d)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Motion_CamBounds(a, b, c, d);
}

static __inline__ void Func_02002764_(s32 id, s32 a)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02002764(id, a);
}

static __inline__ void Func_02002778_(s32 a, s32 b)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02002778(a, b);
}

static __inline__ void Func_02002840_(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02002840(a, b, c, d, e, f);
}

static __inline__ void Func_02002898_(void)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02002898();
}

static __inline__ void Func_020028a0_(void)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_020028a0();
}

static __inline__ void Func_020028d0_(s32 id, s32 x, s32 z)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_020028d0(id, x, z);
}

static __inline__ void Func_020028e0_(s32 id, s32 a, s32 b)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_020028e0(id, a, b);
}

static __inline__ void Func_02002908_(s32 id, s32 a, s32 b)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02002908(id, a, b);
}

static __inline__ void Func_02002910_(s32 id, s32 a)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02002910(id, a);
}

static __inline__ void Func_02002928_(void)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02002928();
}

static __inline__ void Func_02002930_(s32 a)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02002930(a);
}

static __inline__ void Func_02002938_(s32 a, s32 b)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02002938(a, b);
}

static __inline__ void Func_02002940_(s32 a, s32 b)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02002940(a, b);
}

static __inline__ void Func_02002958_(s32 a, s32 b, s32 c)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02002958(a, b, c);
}

static __inline__ void Func_02002968_(s32 a)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02002968(a);
}

static __inline__ void Func_02002990_(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02002990(a, b, c, d, e, f, g);
}

static __inline__ void Func_020029a8_(s32 a, s32 b, s32 c)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_020029a8(a, b, c);
}

static __inline__ void Func_020029b0_(s32 a)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_020029b0(a);
}

static __inline__ s32 Random16Far_(void)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    return Random16Far();
}

static __inline__ struct ObjectRuntime * Func_02002808_(s32 type, s32 x, s32 y, s32 z)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    return Func_02002808(type, x, y, z);
}

static __inline__ void Func_02002800_(struct ObjectRuntime *object, const void *data)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02002800(object, data);
}

static __inline__ void Func_02002838_(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02002838(a, b, c, d, e, f);
}

static __inline__ void ObjectMotion_SetSpeedParameters_(s32 id, s32 speed, s32 acceleration)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    ObjectMotion_SetSpeedParameters(id, speed, acceleration);
}

static __inline__ void ObjectDispatch_SetSingleChildFlags_(struct ObjectRuntime *object, s32 flags)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    ObjectDispatch_SetSingleChildFlags(object, flags);
}

static __inline__ void Animation_SetStateFlags_(struct ObjectRuntime *object, s32 flags)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Animation_SetStateFlags(object, flags);
}

static __inline__ void Func_02002948_(void)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02002948();
}

static __inline__ void Func_02002084_(s32 a)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02002084(a);
}

static __inline__ void Func_020029a0_(s32 message)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_020029a0(message);
}

static __inline__ void Func_020027f8_(struct ObjectRuntime *object, s32 a)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_020027f8(object, a);
}

static __inline__ void Func_02002880_(struct ObjectRuntime *object, s32 a)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02002880(object, a);
}

static __inline__ s32 Func_02002730_(void)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    return Func_02002730();
}

static __inline__ void Func_0200271c_(s32 a, s32 b)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_0200271c(a, b);
}

static __inline__ void Func_0200273c_(s32 a)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_0200273c(a);
}

static __inline__ void Func_02002978_(s32 a)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02002978(a);
}

static __inline__ void Func_02000810_(s32 id)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02000810(id);
}

static __inline__ void Func_020008d8_(s32 id)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_020008d8(id);
}

static __inline__ void Func_02000c24_(void)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02000c24();
}

static __inline__ void Func_02000ca0_(void)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02000ca0();
}

static __inline__ void Func_02000cec_(void)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02000cec();
}

void Func_02000710(s32 id)
{
    struct ObjectRuntime *object = Object_GetById(id);
    s32 n = 60;

    object->motion_flags = 3;
    while (n != 0) {
        WaitFrames(1);
        if (object->velocity_y == 0)
            break;
        n--;
    }
    WaitFrames(10);
    object->motion_flags = 0;
}

void Func_02000744(s32 a, s32 id)
{
    struct ObjectRuntime *object = Object_GetById(id);

    if (id == 9 && object->x >> 20 == 16) {
        Func_02000710(9);
        Func_02002840(15, 26, 1, 1, 16, 26);
        GameFlag_SetBit(0xa6f);
    }
}

void Func_02000784(s32 a, s32 id)
{
    ObjectMotion_SetActionVariant(id, 3);
    Object_GetById(id)->priority_flags |= 2;
    Object_SetModeById(id, 1);
    GameFlag_SetBit(0x220);
    GameFlag_SetBit(0x222);
}

void Func_020007bc(s32 a, s32 id)
{
    Func_02002840_(28, 8, 1, 1, 26, 8);
    ObjectMotion_SetActionVariant(id, 2);
    Object_GetById(id)->priority_flags &= 0xfd;
    Func_02002778(0, id);
    GameFlag_SetBit(0x221);
    GameFlag_ClearBit(0x222);
}

void Func_02000810(s32 id)
{
    struct ObjectRuntime *leader;
    struct ObjectRuntime *object;
    s32 zero = 0;

    Object_SetModeById(id, 8);
    ObjectMotion_SetActionVariant(id, 3);
    Object_GetById(id)->priority_flags |= 2;
    Object_GetById(id)->update = (void (*)(void))zero;
    Func_02002778(0, 0);
    GameFlag_ClearBit(0x220);
    GameFlag_ClearBit(0x221);
    GameFlag_ClearBit(0x222);
    leader = Object_GetById(gPartyState.current_owner);
    object = Object_GetById(id);
    if (object->x >> 20 == leader->x >> 20 && object->z >> 20 == leader->z >> 20) {
        Func_02002910_(gPartyState.current_owner, 0x102);
        ObjectMotion_CommitPositionAndActivate_(gPartyState.current_owner, 0, 16);
        Battle_WaitMode0_(2);
        ObjectMotion_ArmCallback_(gPartyState.current_owner, 0xc000, 0);
        Battle_WaitMode0_(10);
    }
    Func_02002840_(22, 8, 1, 1, 26, 8);
}

void Func_020008d8(s32 id)
{
    Object_SetModeById(id, 7);
    ObjectMotion_SetActionVariant(id, 3);
    Object_GetById(id)->priority_flags |= 2;
    Object_GetById(id)->update = 0;
    Func_02002778(0, 0);
    GameFlag_ClearBit(0x220);
    GameFlag_ClearBit(0x222);
}

void Func_02000920(s32 a, s32 id)
{
    ObjectMotion_SetActionVariant(id, 3);
    Object_GetById(id)->priority_flags |= 2;
    Object_SetModeById(id, 1);
    if (a == 5 || GameFlag_GetByte(0x230) > 3) {
        GameFlag_SetBit(0x220);
        GameFlag_SetBit(0x222);
    }
}

void Func_0200096c(s32 a, s32 id)
{
    Func_02002840_(23, 27, 1, 1, 25, 27);
    ObjectMotion_SetActionVariant(id, 2);
    Object_GetById(id)->priority_flags &= 0xfd;
    Func_02002778(0, id);
    GameFlag_SetBit(0x221);
    GameFlag_ClearBit(0x222);
}

void Func_020009c0(s32 id)
{
    struct ObjectRuntime *leader;
    struct ObjectRuntime *object;

    Object_SetModeById(id, 8);
    ObjectMotion_SetActionVariant(id, 3);
    Object_GetById(id)->priority_flags |= 2;
    Object_GetById(id)->update = 0;
    Func_02002778(0, 0);
    GameFlag_ClearBit(0x220);
    GameFlag_ClearBit(0x221);
    GameFlag_ClearBit(0x222);
    GameFlag_SetByte(0x230, 0);
    leader = Object_GetById(gPartyState.current_owner);
    object = Object_GetById(id);
    if (object->x >> 20 == leader->x >> 20 && object->z >> 20 == leader->z >> 20) {
        Func_02002910_(gPartyState.current_owner, 0x102);
        ObjectMotion_CommitPositionAndActivate_(gPartyState.current_owner, 0, 16);
        Battle_WaitMode0_(2);
        ObjectMotion_ArmCallback_(gPartyState.current_owner, 0xc000, 0);
        Battle_WaitMode0_(10);
    }
    Func_02002840_(1, 1, 1, 1, 25, 27);
}

void Func_02000a94(s32 id)
{
    s32 zero = 0;

    Object_SetModeById(id, 7);
    ObjectMotion_SetActionVariant(id, 3);
    Object_GetById(id)->priority_flags |= 2;
    Object_GetById(id)->update = (void (*)(void))zero;
    Func_02002778(0, 0);
    GameFlag_ClearBit(0x220);
    GameFlag_SetByte(0x230, 0);
}

void Func_02000adc(s32 a, s32 id)
{
    Object_SetModeById(id, 2);
    GameFlag_SetBit(0x331);
    GameFlag_ClearBit(0x332);
}

void Func_02000afc(s32 a, s32 id)
{
    struct ObjectRuntime *object = Object_GetById(id);

    if (object->x >> 20 == 7) {
        Object_SetModeById(id, 3);
        object->collision_flags = 0;
        /* FAKEMATCH: the native stores the collision flags before addressing the priority byte. */
        __asm__ volatile("");
        object->priority_flags |= 2;
        GameFlag_SetBit(0x332);
        GameFlag_SetBit(0x333);
        Func_02002840(7, 17, 1, 1, 7, 15);
    }
}

void Func_02000b58(s32 a, s32 id)
{
    Object_SetModeById(id, 2);
    GameFlag_SetBit(0x334);
    GameFlag_ClearBit(0x335);
    Func_02002840_(27, 14, 1, 1, 27, 13);
}

void Func_02000b90(s32 a, s32 id)
{
    struct ObjectRuntime *object = Object_GetById(id);

    if (object->z >> 20 == 16) {
        Object_SetModeById(id, 3);
        object->collision_flags = 0;
        /* FAKEMATCH: the native stores the collision flags before addressing the priority byte. */
        __asm__ volatile("");
        object->priority_flags |= 2;
        GameFlag_SetBit(0x335);
        GameFlag_SetBit(0x336);
        ObjectMotion_SetSpeedParameters_(id, 0x8000, 0x4000);
        ObjectMotion_OffsetPositionAndResetMotion(id, -32, 0);
        ObjectMotion_CommitCurrentPositionAndActivate(id);
        Object_SetModeById(id, 3);
        Func_02002840(25, 18, 1, 1, 25, 16);
    }
}

void Func_02000c14(struct ObjectRuntime *object)
{
    *(s16 *)((u8 *)object->animation + 18) -= 0x800;
}

void Func_02000c24(void)
{
    s32 a;
    s32 x;
    s32 z;
    struct ObjectRuntime *object;

    Data_02003458--;
    a = Random16Far() * 32;
    x = Random16Far() * 16 + a + 0x12c0000;
    z = Random16Far() * 16 + a + 0x1040000;
    object = Func_02002808(0x11d, x, 0, z);
    Func_02002800(object, Data_020029c8);
    ObjectDispatch_SetSingleChildFlags(object, 0);
    Animation_SetStateFlags(object, 1);
    if (Data_02003458 == 0)
        Scheduler_RemoveCallbackFar(Func_02000c24);
}
