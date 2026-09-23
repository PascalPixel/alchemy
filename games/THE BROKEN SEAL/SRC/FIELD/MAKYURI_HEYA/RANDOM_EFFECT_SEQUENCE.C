#include "MAKYURI_HEYA.H"

#define Scene_Begin Func_0200dab4
#define Scene_Wait Func_0200daac
#define Map_CopyCellsTo Func_0200da04
#define Random_Next Func_0200d994
#define Audio_PlayCue Func_0200dc64
#define Effect_Spawn Func_0200813c
#define Camera_SetSpeed Func_0200da3c
#define SceneFlag_IsSet Func_0200da84

void Scene_Begin();
void Scene_Wait();
void Func_0200ae08();
void Func_0200d98c();
void Map_CopyCellsTo();
void Audio_PlayCue();
u32 Random_Next(void);
void Effect_Spawn(s32, s32, s32, s32, s32, s32, s32, void *);
void Camera_SetSpeed();
void Func_0200da44();
void Func_0200dc5c();
s32 SceneFlag_IsSet(s32);
void Func_0200dadc();
void Func_0200db0c();
void Func_0200db3c();
void Func_0200ae6c();
void Func_0200dabc();
void Func_0200db94();
void Func_0200db24();
void Func_0200db34();
void Func_0200db4c();
void Func_0200db1c();
void Func_0200dba4();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ s32 Value0(s32 (*f)())
{
    return f();
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

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

static __inline__ void Call8(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, void *a7)
{
    f(a0, a1, a2, a3, a4, a5, a6, a7);
}

/* Raises three randomized particle fields, then stages the actors according
 * to whether the lighthouse event flag has already been set. */
void FieldScene_RunRandomEffectActorSequence(void)
{
    struct ConfiguredEffectOptions *effect;
    s32 row_offset;
    u32 value;
    s32 zero;
    s32 phase;
    s32 particle;
    s32 x;
    s32 velocity_x;
    struct ConfiguredEffectOptions options;

    Scene_Begin();
    Scene_Wait(20);
    Func_0200ae08();
    Call1(Func_0200d98c, 0x200adcd);
    Call6(Map_CopyCellsTo, 45, 77, 45, 73, 9, 4);
    Scene_Wait(30);
    effect = &options;
    effect->mode_bits = 1;
    effect->mode = 5;
    effect->kind = 0x11e;
    effect->callback_arg = 0x200de70;
    zero = 0;
    phase = zero;
    do {
        u32 x, z;

        if ((1 & phase) != 0) {
            Audio_PlayCue(246);
        }
        value = Random_Next();
        x = value * 48;
        x >>= 16;
        x <<= 16;
        x += 0x3000000;
        value = Random_Next();
        z = value * 56;
        z >>= 16;
        z <<= 16;
        z += 0x880000;
        Effect_Spawn(x, 0, z, 0, 0, 0, 0x330001, effect);
        Call1((void (*)())Scene_Wait, 2);
        phase = (phase + 1);
    } while ((u32)phase <= 15);
    Scene_Wait(40);
    zero = 0;
    phase = zero;
    do {
        u32 x, z;
        s32 speed;

        if ((1 & phase) != 0) {
            Audio_PlayCue(246);
        }
        value = Random_Next();
        x = value * 48;
        x >>= 16;
        x <<= 16;
        x += 0x3000000;
        value = Random_Next();
        z = value * 56;
        z >>= 16;
        z <<= 16;
        z += 0x980000;
        value = Value0(Random_Next);
        speed = -((value * 10 >> 16) * 0x3333) - 0x3333;
        Effect_Spawn(x, 0, z, 0, 0, speed, 0x330001, effect);
        Scene_Wait(2);
        phase = (phase + 1);
    } while ((u32)phase <= 15);
    Scene_Wait(60);
    Audio_PlayCue(141);
    Call3(Camera_SetSpeed, 0x50000, 0x50000, 0x10000);
    Scene_Wait(60);
    effect->mode = 7;
    effect->accum18 = 0xb333;
    effect->accum1c = 0xb333;
    effect->target30 = 0x13333;
    effect->target34 = 0x13333;
    zero = 0;
    phase = zero;
    do {
        s32 speed;

        Call6(Map_CopyCellsTo, 59, (12 - phase), 48, (12 - phase), 3, 1);
        particle = 0;
        row_offset = (phase << 4);
        do {
            value = Value0(Random_Next);
            x = ((((u32)(((value << 1) + value) << 4) >> 16) << 16) + 0x3000000);
            velocity_x = (0x1999 * ((u32)(Random_Next() << 3) >> 16)) - 0x6664;
            speed = 0x1999 * ((u32)(Random_Next() << 3) >> 16);
            Effect_Spawn(x, 0, ((s32)(-((u32)particle >> 1) - row_offset) << 16) + 0xc00000, velocity_x, 0, speed, 0xd0001, effect);
            particle = (particle + 1);
            Scene_Wait(2);
        } while ((u32)particle <= 31);
        phase = (phase + 1);
    } while ((u32)phase <= 3);
    Call1(Audio_PlayCue, 0x121);
    Call3(Camera_SetSpeed, -1, -1, 0xe666);
    Func_0200da44();
    Func_0200dc5c();
    Scene_Wait(30);
    if (Value1(SceneFlag_IsSet, 0x881) != 0) {
        Call3(Func_0200dadc, 0, 0xcccc, 0x6666);
        Call3(Func_0200db0c, 0, 0x338, 232);
        Call3(Func_0200db0c, 0, 0x318, 232);
        Call3(Func_0200db0c, 0, 0x318, 208);
        Call3(Func_0200dadc, 0, 0x20000, 0x10000);
        Func_0200db3c(0, 4, 0);
        Call3(Engine_ActorWalkTo, 0, 0x318, 200);
        Scene_Wait(10);
        Engine_ActorSetAnimation(0, 18);
        Func_0200ae6c(0);
        Scene_Wait(60);
        Call1(Func_0200d98c, 0x200adcd);
        Call2(Engine_ColorBufferApplySource, 0x10000, 0);
        Call2(Engine_ColorBufferApplyTarget, 0x10005, 0);
        Engine_ColorBufferInterpolate(120);
        Scene_Wait(120);
        Call2(Engine_ColorBufferApplyTarget, 0x7fff, 0);
        Engine_ColorBufferInterpolate(60);
        Scene_Wait(60);
        Engine_EventRequestExit(9);
        Func_0200dabc();
    } else {
        Call3(Func_0200dadc, 0, 0xcccc, 0x6666);
        Call3(Func_0200dadc, 1, 0xcccc, 0x6666);
        Call3(Func_0200dadc, 2, 0xcccc, 0x6666);
        Call3(Func_0200dadc, 3, 0xcccc, 0x6666);
        Call3(Func_0200db0c, 0, 0x338, 240);
        Call3(Func_0200db94, 0, 0xa000, 20);
        Call3(Func_0200db24, 3, 0x3380000, 0xf00000);
        Call3(Func_0200db0c, 3, 0x318, 232);
        Call3(Func_0200db94, 3, 0xc000, 0);
        Scene_Wait(60);
        Call3(Func_0200db94, 3, 0x2000, 20);
        Func_0200db34(3, 3);
        Scene_Wait(40);
        Call3(Func_0200db94, 3, 0xc000, 20);
        Call3(Func_0200db0c, 3, 0x318, 200);
        Func_0200ae6c(3);
        Scene_Wait(20);
        Func_0200db4c(0, 2);
        Scene_Wait(30);
        Call3(Func_0200db0c, 0, 0x318, 232);
        Call3(Func_0200db94, 0, 0xc000, 0);
        Call3(Func_0200db24, 1, 0x3180000, 0xe80000);
        Call3(Func_0200db24, 2, 0x3180000, 0xe80000);
        Call3(Engine_ActorWalkTo, 1, 0x330, 224);
        Call3(Func_0200db0c, 2, 0x300, 224);
        Func_0200db1c(1);
        Call3(Func_0200db94, 1, 0xa000, 0);
        Call3(Func_0200db94, 2, 0xe000, 20);
        Call3(Func_0200dba4, 0, 0x102, 0);
        Call3(Func_0200dba4, 1, 0x102, 0);
        Call3(Func_0200dba4, 2, 0x102, 80);
        Call3(Func_0200db94, 1, 0x6000, 0);
        Call3(Func_0200db94, 2, 0x2000, 20);
        Func_0200db4c(0, 1);
        Scene_Wait(60);
        Call3(Func_0200dadc, 0, 0x8000, 0x4000);
        Call3(Func_0200db0c, 0, 0x318, 224);
        Call3(Func_0200db94, 1, 0x8000, 0);
        Func_0200db94(2, 0, 0);
        Call3(Func_0200db0c, 0, 0x318, 208);
        Call3(Func_0200db94, 1, 0xa000, 0);
        Call3(Func_0200db94, 2, 0xe000, 20);
        Func_0200db34(0, 3);
        Scene_Wait(20);
        Call3(Func_0200dadc, 0, 0x20000, 0x10000);
        Func_0200db3c(0, 4, 0);
        Call3(Engine_ActorWalkTo, 0, 0x318, 200);
        Scene_Wait(10);
        Engine_ActorSetAnimation(0, 18);
        Call3(Func_0200dba4, 1, 0x100, 0);
        Call3(Func_0200dba4, 2, 0x100, 0);
        Engine_ActorStartRepeatedMotion(1, 2);
        Engine_ActorStartRepeatedMotion(2, 2);
        Func_0200ae6c(0);
        Scene_Wait(60);
        Call3(Func_0200dba4, 1, 0x102, 0);
        Call3(Func_0200dba4, 2, 0x102, 80);
        Engine_ActorFaceEachOther(1, 2, 20);
        Engine_ActorSetAnimation(1, 3);
        Func_0200db34(2, 3);
        Scene_Wait(40);
        Call3(Func_0200db0c, 1, 0x318, 216);
        Call3(Func_0200db94, 2, 0xe000, 0);
        Call3(Func_0200db0c, 1, 0x318, 200);
        Scene_Wait(30);
        Func_0200ae6c(1);
        Call3(Func_0200db0c, 2, 0x318, 216);
        Call3(Func_0200db0c, 2, 0x318, 200);
        Scene_Wait(30);
        Func_0200ae6c(2);
        Call2(Engine_ColorBufferApplySource, 0x10000, 0);
        Call1(Func_0200d98c, 0x200adcd);
        Call2(Engine_ColorBufferApplyTarget, 0x10005, 0);
        Engine_ColorBufferInterpolate(120);
        Scene_Wait(120);
        Call2(Engine_ColorBufferApplyTarget, 0x7fff, 0);
        Engine_ColorBufferInterpolate(60);
        Scene_Wait(60);
        Func_0200dabc();
        Engine_EventRequestExit(8);
    }
}
