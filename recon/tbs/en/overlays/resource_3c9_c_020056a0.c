/* NONMATCHING: 848 of 848 bytes, 145 halfword edits (2026-09-24). Hand-written from the
 * resolved jump-table disassembly as a single-overlay unit binding Engine_* at
 * their import veneers. Remaining: register allocation only: the reference keeps the beam in r6, the spark in r7, the pool zero (sprite flags) in r8 and the sprite in sl; this draft gives the beam r7 and the spark r8. Offsets, pool and switch layout match (848 bytes). The -dl dump shows sprite and fade local-allocated in the spark block (r6, r5); in the reference they are global (r8, sl) after frame r5, beam r6 and spark r7, so they must span more than one basic block. Rebinding the unit now needs gFrameCount, Data_00000000, Data_0200e2d0 and Data_0200e1cc as data symbols. */
#include "TYPES.H"
#include "FIELD_EVENT.H"

void Actor_ParkRecord(struct FieldActor *actor);
void Effect_AdvanceGatedRiseCounter(struct FieldActor *actor);
void Effect_UpdateCounterDrivenOrbit(void);

extern u8 Data_00000000[];
extern const u8 Data_0200e2d0[];
extern const s32 Data_0200e1cc[];

struct Spark {
    u8 unknown_00[0x64];
    u16 angle;
    u16 phase;
};

#define SUMMIT_FRAME (*(u32 *)0x0200e764)

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

void Func_020056a0(void)
{
    struct FieldActor *beam;
    struct FieldActor *spark;
    struct FieldSprite *sprite;
    u32 step;
    s32 fade;
    volatile u32 *frame;

    beam = Engine_ActorGet(23);
    frame = (volatile u32 *)&SUMMIT_FRAME;
    spark = 0;
    switch (step = *frame) {
    case 0:
        Engine_AudioPlayCue(220);
        Call3(Engine_WorkSetValuesIfNonNegative, 0x60000, 0x60000, 0x10000);
        Engine_ColorBufferApplyTarget(0x2063ff, 1);
        Engine_ColorBufferInterpolate(8);
        break;
    case 8:
        Engine_ColorBufferApplyTarget(0x10000, 1);
        Engine_ColorBufferInterpolate(8);
        break;
    case 16:
        Call3(Engine_WorkSetValuesIfNonNegative, 0x10000, 0x10000, 0x10000);
        break;
    case 24:
        beam->x.fixed = 0x1300000;
        beam->y.fixed = -0x1680000;
        beam->z.fixed = 0xa40000;
        beam->scale_x = 0x10000;
        beam->scale_y = 0x10000;
        Actor_ParkRecord(beam);
        Engine_ActorEnableActionCallback(23, Data_0200e2d0);
        break;
    case 25:
        (*frame)--;
        if (beam->y.fixed > 0) {
            Engine_ColorBufferApplyTarget(0x203210, 0);
            Engine_ColorBufferInterpolate(16);
            (*frame)++;
            Engine_ActorGet(0)->rise_counter = 1;
            Engine_ActorGet(1)->rise_counter = 1;
            Engine_ActorGet(2)->rise_counter = 1;
            Engine_ActorGet(3)->rise_counter = 1;
            Engine_ActorGet(21)->rise_counter = 1;
            Engine_ActorGet(6)->rise_counter = 1;
            break;
        }
        goto rise;
    case 26:
        (*frame)--;
        if (beam->y.fixed > 0x280000) {
            Engine_ColorBufferApplyTarget(0x10000, 0);
            Engine_ColorBufferInterpolate(40);
            (*frame)++;
            break;
        }
    rise:
        if ((gFrameCount & 7) == 0) {
            Engine_AudioPlayCue(246);
        }
        beam->y.fixed += 0x24000;
        spark = (struct FieldActor *)1;
        break;
    case 27:
    case 28:
    case 29:
    case 30:
    case 31:
    case 32:
    case 33:
    case 34:
        spark = (struct FieldActor *)1;
        break;
    case 36:
        Engine_AudioPlayCue(187);
        Engine_ColorBufferApplyTarget(0x7fff, 0);
        Engine_ColorBufferInterpolate(12);
        break;
    case 48:
        Engine_ActorStop(23);
        Engine_GameFlagSet(0x237);
        break;
    }
    if (spark != 0) {
        spark = Engine_ObjectCreate(0x11c, beam->x.fixed, beam->y.fixed - ((((u32)Engine_RandomNext() * 80) >> 16) << 16) - 0x80000, beam->z.fixed);
        if (spark != 0) {
            sprite = spark->sprite;
            Engine_ObjectSetScript(spark, Data_0200e1cc);
            Engine_ObjectSetPalette(spark, 1);
            spark->motion_flags = 0;
            ((struct Spark *)spark)->angle = (u32)Engine_RandomNext() & 0xffff000;
            ((struct Spark *)spark)->phase = 0;
            fade = (s32)Data_00000000;
            spark->rise_counter = (u32)Engine_RandomNext() >> 13;
            spark->update = (void (*)(union FieldObject *))Effect_UpdateCounterDrivenOrbit;
            spark->speed = Engine_MathSin(((u32)Engine_RandomNext() * 0xffff) >> 20) * 24;
            spark->speed = ((union FieldCoordinate *)&beam->speed)->part.pixel;
            sprite->flags = fade;
            sprite->priority = 1;
        }
    }
    SUMMIT_FRAME++;
    Effect_AdvanceGatedRiseCounter(Engine_ActorGet(0));
    Effect_AdvanceGatedRiseCounter(Engine_ActorGet(1));
    Effect_AdvanceGatedRiseCounter(Engine_ActorGet(2));
    Effect_AdvanceGatedRiseCounter(Engine_ActorGet(3));
    Effect_AdvanceGatedRiseCounter(Engine_ActorGet(21));
    Effect_AdvanceGatedRiseCounter(Engine_ActorGet(6));
}
