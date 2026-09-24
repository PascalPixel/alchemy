/* NONMATCHING: 352 bytes, candidate 348, 27 differing halfwords (2026-09-24).
 * Unit symbol Data_02000240_t 0x02000240 (data). Remaining: the reference
 * schedules the counter decrement after the angle store and dumps a literal
 * pool before the epilogue behind a branch. */
#include "TYPES.H"

u8 * Engine_ActorGet();
s32 Engine_GameFlagIsSet();
void Main_080091b8();
void Engine_GameFlagSet();
void Engine_AudioPlayCue();
void Engine_EventBegin();
void Main_0808a238();
void Engine_ActorSetAnimation();
void Engine_ActorSetSpriteFlags();
void Engine_ActorSetAttachedEffect();
void Engine_EventWait();
void Engine_CameraMoveTo();
void Engine_ActorSetSpritePriority();
void Engine_TaskWait();


extern u8 Data_0000002d[];
extern u8 Data_00002000[];
extern s32 Data_02000240_t[];

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

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

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

void ToretoHeya_Func02000334(s32 a0, s32 a1, s32 a2, s32 a3)
{
    u32 i;
    s32 p10;
    s32 p11;
    s32 p8;
    s32 p8b;
    s32 p9;
    u8 *rec8;
    s32 record;
    s32 base5_200add0;
    s32 v5;
    s32 slot8;
    s32 slot12;
    u8 *p7;

    slot8 = a1;
    p11 = a2;
    slot12 = a0;
    p9 = a3;
    p10 = ((Data_02000240_t[119] >> 20) + 64);
    p8 = (Data_02000240_t[121] >> 20);
    p7 = (u8 *)Data_02000240_t[125];
    rec8 = Engine_ActorGet((s32)p7);
    base5_200add0 = 0x200add0;
    if (p9 == *(s16 *)(*(s32 *)base5_200add0)) {
    } else {
        *(u16 *)(*(s32 *)base5_200add0) = p9;
        if (Value1(Engine_GameFlagIsSet, slot12) == 0) {
            Call6(Main_080091b8, slot8, p11, 1, 1, p10, p8);
            Engine_GameFlagSet(slot12);
        } else {
            *(s16 *)(*(s32 *)base5_200add0) = -1;
            Call6(Main_080091b8, slot8, (p11 + 1), 1, 1, p10, p8);
            Engine_AudioPlayCue(206);
            Engine_EventBegin();
            Main_0808a238((s32)Data_0000002d, p9);
            Engine_ActorSetAnimation((s32)p7, 27);
            record = Engine_ActorGet((s32)p7);
            Engine_ActorSetSpriteFlags(record, 0);
            Call2(Engine_ActorSetAttachedEffect, (s32)p7, 0x101);
            Engine_EventWait(30);
            Call4(Engine_CameraMoveTo, -1, -1, -1, 0);
            rec8[85] = 2;
            *(s32 *)((s32)rec8 + 20) = -0xa00000;
            *(s32 *)((s32)rec8 + 72) = 0x8000;
            Engine_AudioPlayCue(204);
            Engine_EventWait(3);
            rec8[34] = 2;
            v5 = 29;
            Engine_ActorSetSpritePriority((s32)p7, 3);
            do {
                *(u16 *)((s32)rec8 + 6) += (s32)Data_00002000;
                v5 = (v5 - 1);
                Engine_TaskWait(1);
            } while (v5 >= 0);
            if (p9 != 50) {
                Call1(Engine_GameFlagSet, 0x122);
            }
        }
    }
    p8b = *(s16 *)(*(s32 *)base5_200add0);
}
