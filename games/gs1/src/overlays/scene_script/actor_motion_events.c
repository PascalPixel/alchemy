#include "types.h"

#define SceneEffect_UpdateMotion Func_02000104

union MotionWork {
  struct {
    u32 unk_00[2];
    s32 x, y, z;
    u32 unk_14;
    s32 accum_x, accum_y;
    u32 unk_20[4];
    s32 rate_x, rate_y;
    u32 unk_38[3];
    s32 velocity_x, velocity_y, velocity_z;
    u16 *record;
    u8 unk_54[16];
    u16 angle_step;
  } fields;
  u8 bytes[102];
};

void SceneEffect_UpdateMotion(union MotionWork *work)
{
    u16 *record;
    work->fields.x += work->fields.velocity_x;
    work->fields.y += work->fields.velocity_y;
    work->fields.z += work->fields.velocity_z;
    work->fields.accum_x += work->fields.rate_x;
    work->fields.accum_y += work->fields.rate_y;
    record = work->fields.record;
    record[15] += work->fields.angle_step;
}
#define EffectDescriptorTable Data_0200929c
#define GetPartyEffect Func_0200125e
#define SpawnEffect Func_02001204
#define SetEffectVariant Func_0200120e
#define SetEffectDescriptor Func_02001228
#define SetEffectMode Func_0200137a
#define ScaleEffectDeltaFromAccumulated Func_020012d4
#define ScaleEffectDeltaFromOrigin Func_020012ec
#define ScaleEffectVerticalDelta Func_020012fa
#define SetEffectCallbackMode Func_02001328
#define SetEffectCallbackArgument Func_02001338
#include "configured_effect_spawn.h"

void SpawnConfiguredEffect(s32 x, s32 y, s32 z, s32 vx, s32 vy, s32 vz,
                           u32 flags, const struct ConfiguredEffectOptions *options)
{
#include "configured_effect_spawn_body.inc"
}

#define SceneDialogue_RunActorFifteenDialogue Func_02000864
#include "types.h"

struct SceneActor {
    u8 unk_00[6];
    u16 facing;
    u8 unk_08[92];
    u16 state_flags;
};

#define SceneDialogue_RunActorFourteenFlagDialogue Func_0200058c
struct SceneActor *Func_02001696(s32);
void Func_02001696_a(void);
void Func_0200171e(s32);
void Func_020016f6(s32, s32);
void Func_02001720(s32, s32, s32);
s32 Func_02001690(s32);
void Func_0200177a(s32, s32, s32);
void Func_02001774(s32, s32, s32);
void Func_0200177e(s32, s32, s32);
void Func_020016ce(s32);
void Func_02001774_a(s32);
void Func_02001796(s32, s32, s32);
void Func_02001680(s32);
void Func_02001714(void);
void Func_020016f6_a(s32);
extern u8 SceneMessage_ActorFourteenBase;

static __inline__ s32 Scene_QueryFlag(s32 (*func)(s32), s32 flag)
{
    return func(flag);
}

static __inline__ void Scene_SetFlag(void (*func)(s32), s32 flag)
{
    func(flag);
}

static __inline__ void Scene_Call3(void (*func)(s32, s32, s32), s32 a, s32 b, s32 c)
{
    func(a, b, c);
}

void SceneDialogue_RunActorFourteenFlagDialogue(void)
{
    struct SceneActor *actor = Func_02001696(14);
    s16 facing = (s16)actor->facing;
    s32 text;

    actor->state_flags |= 2;
    Func_02001696_a();
    text = (s32)&SceneMessage_ActorFourteenBase;
    Func_0200171e(text);
    Func_020016f6(14, 0);
    Func_02001720(14, 0, 2);
    if (Scene_QueryFlag(Func_02001690, 0x300) == 0) {
        Scene_Call3(Func_0200177a, 14, 256, 60);
        Func_02001774(14, 0, 10);
        Func_0200177e(14, 0, 10);
        Scene_SetFlag(Func_020016ce, 0x300);
    }
    Func_02001774_a(text + 2);
    Func_02001796(14, 0, 10);
    actor->facing = facing;
    Func_02001680(1);
    Func_02001714();
    {
        s32 shown = 1;
        actor->state_flags = shown;
    }
    Func_020016f6_a(0x307);
}

struct SceneActor *Func_0200196a(s32);
void Func_02001960(void);
void Func_020019e6(s32);
void Func_02000d38(s32);
void Func_02001978(void);
struct SceneActor *Func_0200198e(s32);

void SceneDialogue_RunActorFifteenDialogue(void)
{
    {
        struct SceneActor *actor = Func_0200196a(15);
        actor->state_flags |= 2;
    }
    Func_02001960();
    Func_020019e6(0x1cc1);
    Func_02000d38(15);
    Func_02001978();
    {
        s32 clear = 0;
        struct SceneActor *actor = Func_0200198e(15);
        actor->state_flags = clear;
    }
}

#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02000e04();
s32 Func_02001970();
void Func_020019a2();
void Func_020019bc();
s32 Func_020019c0();
s32 Func_020019f4();
void Func_020019fe();
void Func_02001a06();
s32 Func_02001a1e();
s32 Func_02001a1e_a();
void Func_02001a1e_b();
void Func_02001a3a();
s32 Func_02001a44();
s32 Func_02001a44_a();
void Func_02001a4a();
s32 Func_02001a56();
s32 Func_02001a64();
void Func_02001a90();
void Func_02001aa6();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call0(void (*f)())
{
    f();
}

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunScene385SequenceA(void)
{
    u32 i;
    s32 rec7;
    s32 record;
    s32 v5;

    rec7 = Value1(Func_02001970, 0x308);
    if (rec7 == 0) {
        Func_020019a2();
        *(u8 *)(Func_020019c0(16) + 91) = 1;
        Func_02001a06(16, 1);
        Call2((void (*)())Func_02001a1e, 16, 1);
        Func_020019bc(20);
        Call1(Func_02001a4a, 0x1cb5);
        Call3((void (*)())Func_02001a44, 16, 0, 2);
        Value2(Func_02001a64, 16, 0);
        if (Value2(Func_020019f4, 0, 0) != 0) {
            bump_step(1);
        }
        Func_02001a90(16, 0);
        *(u8 *)(Func_02001a1e_a(16) + 91) = rec7;
        Func_02001a3a(16, 2);
        Call0(Func_02001a1e_b);
        Call1(Func_020019fe, 0x308);
    } else {
        Call1(Func_02001aa6, 0x1cc2);
        *(u8 *)(Func_02001a44_a(16) + 91) = 1;
        Func_02000e04(16);
        v5 = 0;
        *(u8 *)(Func_02001a56(16) + 91) = v5;
    }
}
