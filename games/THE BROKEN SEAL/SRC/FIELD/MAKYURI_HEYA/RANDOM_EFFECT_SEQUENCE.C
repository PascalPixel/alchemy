#include "MAKYURI.H"
#include "MAKYURI_HEYA.H"

void MakyuriHeya_FadePaletteToBlack();
void SceneEffect_RotatePaletteEntries97To103(void);
extern const u8 MakyuriHeya_SparkBurstScript[];
void BattleFx_PlayQueuedSound();
void MakyuriHeya_SinkActorWithSparks();

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

    Event_Begin();
    Event_Wait(20);
    MakyuriHeya_FadePaletteToBlack();
    Call1(Engine_TaskRemoveCallback, (s32)SceneEffect_RotatePaletteEntries97To103);
    Call6(Engine_MapCopyCellsTo, 45, 77, 45, 73, 9, 4);
    Event_Wait(30);
    effect = &options;
    effect->mode_bits = 1;
    effect->mode = 5;
    effect->kind = 0x11e;
    effect->callback_arg = (s32)MakyuriHeya_SparkBurstScript;
    zero = 0;
    phase = zero;
    do {
        u32 x, z;

        if ((1 & phase) != 0) {
            Audio_PlayCue(246);
        }
        value = Engine_RandomNext();
        x = value * 48;
        x >>= 16;
        x <<= 16;
        x += 0x3000000;
        value = Engine_RandomNext();
        z = value * 56;
        z >>= 16;
        z <<= 16;
        z += 0x880000;
        Effect_Spawn(x, 0, z, 0, 0, 0, 0x330001, effect);
        Call1((void (*)())Battle_WaitMode0, 2);
        phase = (phase + 1);
    } while ((u32)phase <= 15);
    Event_Wait(40);
    zero = 0;
    phase = zero;
    do {
        u32 x, z;
        s32 speed;

        if ((1 & phase) != 0) {
            Audio_PlayCue(246);
        }
        value = Engine_RandomNext();
        x = value * 48;
        x >>= 16;
        x <<= 16;
        x += 0x3000000;
        value = Engine_RandomNext();
        z = value * 56;
        z >>= 16;
        z <<= 16;
        z += 0x980000;
        value = Value0(Engine_RandomNext);
        speed = -((value * 10 >> 16) * 0x3333) - 0x3333;
        Effect_Spawn(x, 0, z, 0, 0, speed, 0x330001, effect);
        Event_Wait(2);
        phase = (phase + 1);
    } while ((u32)phase <= 15);
    Event_Wait(60);
    Audio_PlayCue(141);
    Call3(Engine_WorkSetValuesIfNonNegative, 0x50000, 0x50000, 0x10000);
    Event_Wait(60);
    effect->mode = 7;
    effect->accum18 = 0xb333;
    effect->accum1c = 0xb333;
    effect->target30 = 0x13333;
    effect->target34 = 0x13333;
    zero = 0;
    phase = zero;
    do {
        s32 speed;

        Call6(Engine_MapCopyCellsTo, 59, (12 - phase), 48, (12 - phase), 3, 1);
        particle = 0;
        row_offset = (phase << 4);
        do {
            value = Value0(Engine_RandomNext);
            x = ((((u32)(((value << 1) + value) << 4) >> 16) << 16) + 0x3000000);
            velocity_x = (0x1999 * ((u32)(Engine_RandomNext() << 3) >> 16)) - 0x6664;
            speed = 0x1999 * ((u32)(Engine_RandomNext() << 3) >> 16);
            Effect_Spawn(x, 0, ((s32)(-((u32)particle >> 1) - row_offset) << 16) + 0xc00000, velocity_x, 0, speed, 0xd0001, effect);
            particle = (particle + 1);
            Event_Wait(2);
        } while ((u32)particle <= 31);
        phase = (phase + 1);
    } while ((u32)phase <= 3);
    Call1(Audio_PlayCue, 0x121);
    Call3(Engine_WorkSetValuesIfNonNegative, -1, -1, 0xe666);
    MapRender_WaitForValues();
    BattleFx_PlayQueuedSound();
    Event_Wait(30);
    if (GameFlag_IsSet(0x881) != 0) {
        Actor_SetSpeed(0, 0xcccc, 0x6666);
        Actor_WalkToAndWait(0, 0x338, 232);
        Actor_WalkToAndWait(0, 0x318, 232);
        Actor_WalkToAndWait(0, 0x318, 208);
        Actor_SetSpeed(0, 0x20000, 0x10000);
        Actor_Jump(0, 4, 0);
        Actor_WalkTo(0, 0x318, 200);
        Event_Wait(10);
        Actor_SetAnimation(0, 18);
        MakyuriHeya_SinkActorWithSparks(0);
        Event_Wait(60);
        Call1(Engine_TaskRemoveCallback, (s32)SceneEffect_RotatePaletteEntries97To103);
        ColorBuffer_ApplySource(0x10000, 0);
        ColorBuffer_ApplyTarget(0x10005, 0);
        ColorBuffer_Interpolate(120);
        Event_Wait(120);
        ColorBuffer_ApplyTarget(0x7fff, 0);
        ColorBuffer_Interpolate(60);
        Event_Wait(60);
        Event_RequestExit(9);
        Event_End();
    } else {
        Actor_SetSpeed(0, 0xcccc, 0x6666);
        Actor_SetSpeed(1, 0xcccc, 0x6666);
        Actor_SetSpeed(2, 0xcccc, 0x6666);
        Actor_SetSpeed(3, 0xcccc, 0x6666);
        Actor_WalkToAndWait(0, 0x338, 240);
        Actor_FaceDirection(0, 0xa000, 20);
        Actor_SetPosition(3, 0x3380000, 0xf00000);
        Actor_WalkToAndWait(3, 0x318, 232);
        Actor_FaceDirection(3, 0xc000, 0);
        Event_Wait(60);
        Actor_FaceDirection(3, 0x2000, 20);
        Actor_SetAnimationAndWait(3, 3);
        Event_Wait(40);
        Actor_FaceDirection(3, 0xc000, 20);
        Actor_WalkToAndWait(3, 0x318, 200);
        MakyuriHeya_SinkActorWithSparks(3);
        Event_Wait(20);
        Actor_RunRepeatedMotion(0, 2);
        Event_Wait(30);
        Actor_WalkToAndWait(0, 0x318, 232);
        Actor_FaceDirection(0, 0xc000, 0);
        Actor_SetPosition(1, 0x3180000, 0xe80000);
        Actor_SetPosition(2, 0x3180000, 0xe80000);
        Actor_WalkTo(1, 0x330, 224);
        Actor_WalkToAndWait(2, 0x300, 224);
        Actor_WaitForMove(1);
        Actor_FaceDirection(1, 0xa000, 0);
        Actor_FaceDirection(2, 0xe000, 20);
        Actor_ShowEmote(0, 0x102, 0);
        Actor_ShowEmote(1, 0x102, 0);
        Actor_ShowEmote(2, 0x102, 80);
        Actor_FaceDirection(1, 0x6000, 0);
        Actor_FaceDirection(2, 0x2000, 20);
        Actor_RunRepeatedMotion(0, 1);
        Event_Wait(60);
        Actor_SetSpeed(0, 0x8000, 0x4000);
        Actor_WalkToAndWait(0, 0x318, 224);
        Actor_FaceDirection(1, 0x8000, 0);
        Actor_FaceDirection(2, 0, 0);
        Actor_WalkToAndWait(0, 0x318, 208);
        Actor_FaceDirection(1, 0xa000, 0);
        Actor_FaceDirection(2, 0xe000, 20);
        Actor_SetAnimationAndWait(0, 3);
        Event_Wait(20);
        Actor_SetSpeed(0, 0x20000, 0x10000);
        Actor_Jump(0, 4, 0);
        Actor_WalkTo(0, 0x318, 200);
        Event_Wait(10);
        Actor_SetAnimation(0, 18);
        Actor_ShowEmote(1, 0x100, 0);
        Actor_ShowEmote(2, 0x100, 0);
        Actor_StartRepeatedMotion(1, 2);
        Actor_StartRepeatedMotion(2, 2);
        MakyuriHeya_SinkActorWithSparks(0);
        Event_Wait(60);
        Actor_ShowEmote(1, 0x102, 0);
        Actor_ShowEmote(2, 0x102, 80);
        Actor_FaceEachOther(1, 2, 20);
        Actor_SetAnimation(1, 3);
        Actor_SetAnimationAndWait(2, 3);
        Event_Wait(40);
        Actor_WalkToAndWait(1, 0x318, 216);
        Actor_FaceDirection(2, 0xe000, 0);
        Actor_WalkToAndWait(1, 0x318, 200);
        Event_Wait(30);
        MakyuriHeya_SinkActorWithSparks(1);
        Actor_WalkToAndWait(2, 0x318, 216);
        Actor_WalkToAndWait(2, 0x318, 200);
        Event_Wait(30);
        MakyuriHeya_SinkActorWithSparks(2);
        ColorBuffer_ApplySource(0x10000, 0);
        Call1(Engine_TaskRemoveCallback, (s32)SceneEffect_RotatePaletteEntries97To103);
        ColorBuffer_ApplyTarget(0x10005, 0);
        ColorBuffer_Interpolate(120);
        Event_Wait(120);
        ColorBuffer_ApplyTarget(0x7fff, 0);
        ColorBuffer_Interpolate(60);
        Event_Wait(60);
        Event_End();
        Event_RequestExit(8);
    }
}
