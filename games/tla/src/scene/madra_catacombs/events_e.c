#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "party_state.h"

struct EventWork154 {
    u8 unk000[0x154];
    s16 returnEntrance;
};

struct EventWork1ac {
    u8 unk000[0x1ac];
    s32 flag;
};

struct AnimationRecord18 {
    u8 unk00[0x12];
    u16 frame;
};

extern u8 MsgFieldEyesOfTruth;
extern const u8 Data_020013c0[];
extern const u8 Data_020013c4[];
extern const u8 Data_020013ca[];
extern const u8 Data_0200121c[];

struct ObjectRuntime *Object_GetById(s32 id);
s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
void WaitFrames(s32 frames);
void UiText_ShowPositionedMessageAndWait(s32 message, s32 mode);
void Battle_WaitMode0(s32 frames);
void Motion_CamBounds(s32 a, s32 b, s32 c, s32 d);
void ObjectMotion_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void ObjectMotion_EnableActionAndSetCallback(s32 id, const void *script);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object, s32 flags);
void Object_SetPartAttribute(struct ObjectRuntime *object, s32 value);
void Event_StartScreenTransition(void);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_0200110c(void);
void Func_020011ac(s32 skip);
void Func_02001114(void);
void Func_02001214(s32 cue);
void Func_020010bc(struct ObjectRuntime *object, s32 x, s32 y, s32 z);
void Func_020010c4(struct ObjectRuntime *object);
void Func_020010cc(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_020010dc(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02001134(s32 id, s32 x, s32 z);
void Func_0200113c(s32 id, s32 value);
void Func_0200115c(s32 id);
void Func_0200117c(s32 entrance);
s32 Func_02001184(s32 owner);
void Func_020011b4(void);
void Func_020011bc(s32 a, s32 b, s32 c, s32 d);
void Func_020011c4(const void *data);
void Func_020011dc(const void *data);
s32 Func_020011fc(void);
void Func_02001204(void);
void Func_02000038(const void *data);
void Func_0200014c(const void *data);
void Func_02000270(struct ObjectRuntime *object);
void Func_020008d0(struct ObjectRuntime *object);
void Func_02000958(void);
void Func_02000a28(void);
void Func_02000c94(void);
void Func_02000cf4(void);

static __inline__ s32 Flag_Test(s32 flag)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    return GameFlag_Test(flag);
}

static __inline__ void Flag_Set(s32 flag)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    GameFlag_SetBit(flag);
}

static __inline__ void Flag_Clear(s32 flag)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    GameFlag_ClearBit(flag);
}

static __inline__ void Map_CopyCells(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_020010cc(a, b, c, d, e, f);
}

static __inline__ void Map_CopyAttributes(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_020010dc(a, b, c, d, e, f);
}

static __inline__ void Actor_Place(s32 id, s32 x, s32 z)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02001134(id, x, z);
}

static __inline__ void Wait(s32 frames)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    WaitFrames(frames);
}

static __inline__ void Cue(s32 cue)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02001214(cue);
}

static __inline__ void Camera_SetBounds(s32 a, s32 b, s32 c, s32 d)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Motion_CamBounds(a, b, c, d);
}

void ObjectMotion_SetActionVariant(u32 id, s32 variant);
void Object_SetPositionAndResetMotion(struct ObjectRuntime *object, s32 x, s32 y, s32 z);

void Func_02000dcc(void)
{
    s32 a;
    s32 b;
    s32 x;
    s32 y;
    s32 z;
    struct ObjectRuntime *object;

    Flag_Clear(0x203);
    ObjectMotion_SetActionVariant(64, 3);
    Func_020011b4();
    b = 0x102;
    a = 1;
    /* FAKEMATCH: the native sets the first argument before it finishes the second (differ 2 without the barrier). */
    __asm__("" : "+r"(a), "+r"(b));
    Func_020011bc(a, b + 255, 9, 10);
    Func_0200115c(11);
    if (Flag_Test(0x8c8) != 0) {
        Map_CopyAttributes(41, 13, 2, 1, 39, 13);
        object = Object_GetById(11);
        x = 162;
        z = 216;
        x <<= 18;
        y = 0;
        /* FAKEMATCH: second barrier of the pair above: y is cleared before z is shifted. */
        __asm__("" : "+r"(x), "+r"(y), "+r"(z));
        Object_SetPositionAndResetMotion(object, x, y, z << 16);
    }
    if (Flag_Test(0x8ca) != 0)
        Actor_Place(12, 712 << 16, 424 << 16);
    Func_020011dc(Data_020013c0);
    if (Flag_Test(0x9ef) != 0)
        Actor_Place(8, 568 << 16, 280 << 16);
    Object_GetById(8)->priorityFlags &= 0xfe;
    Func_02000038(Data_020013ca);
    Func_020011c4(Data_020013c4);
    if (Flag_Test(0x8cb) != 0) {
        Map_CopyCells(75, 66, 97, 39, 2, 2);
        Map_CopyAttributes(33, 37, 2, 2, 33, 39);
        Wait(1);
    }
    if (Flag_Test(0x8ab) != 0) {
        Map_CopyCells(82, 74, 116, 16, 5, 3);
        Map_CopyAttributes(52, 11, 5, 1, 52, 18);
        if (gPartyState.entrance == 3)
            Func_02000c94();
        else
            Wait(10);
    }
}

void Func_02000f2c(void)
{
    Func_020011b4();
    Func_020011bc(0, 0x200, 8, 9);
    if (Flag_Test(0x8ab) != 0) {
        Actor_Place(10, 0, 0);
        Map_CopyCells(68, 70, 113, 11, 5, 3);
        Map_CopyCells(50, 74, 50, 75, 2, 1);
        Map_CopyAttributes(49, 9, 5, 1, 49, 13);
        if (gPartyState.entrance == 8)
            Func_02000cf4();
        else
            Wait(1);
    }
}

void Func_02000fbc(void)
{
    if (Flag_Test(0x8cc) == 0)
        Actor_Place(65, 0, 0);
}

void Func_02000fd8(void)
{
    struct ObjectRuntime *object;

    if (Flag_Test(0x8cc) != 0) {
        Actor_Place(64, 0, 0);
    } else {
        object = Object_GetById(64);
        object->collisionFlags = 8;
        object->motionFlags = 2;
        object->y = 0x200000;
        object->terrainHeight = 0x200000;
        object->update = Func_02000a28;
        Object_GetById(8)->scaleY = 0xcccc;
    }
}
