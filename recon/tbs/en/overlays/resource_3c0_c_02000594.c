#include "TYPES.H"
#include "IO_WRITE_QUEUE.H"

extern volatile u16 Data_04000208;
extern u8 Data_02000240[];
extern s16 Data_02000240_t[][1];

/* Queue a register write for the next frame with interrupts masked, as
 * SYSTEM/IO_WRITE_QUEUE.C does; expanded in place, so the value is only
 * computed once there is room. */
#define QUEUE_IO_WRITE(address, value, delay)                                \
    do {                                                                     \
        volatile u16 *ime;                                                   \
        struct IoWriteQueue *q;                                              \
        u32 saved;                                                           \
        s32 count;                                                           \
                                                                             \
        q = &gIoWriteQueue;                                                  \
        ime = &Data_04000208;                                                \
        saved = *ime;                                                        \
        *ime = (u16)(u32)ime;                                                \
        count = q->count;                                                    \
        if (count <= 31) {                                                   \
            u32 *destination = (u32 *)((u8 *)q + count * 12 + 4);            \
            q->count = count + 1;                                            \
            *destination++ = (value);                                        \
            *destination++ = (address);                                      \
            *destination = (delay);                                          \
        }                                                                    \
        *ime = saved;                                                        \
    } while (0)

s32 Engine_ActorGet();
s32 Engine_GameFlagIsSet();
void Engine_EventBegin();
void Engine_AudioPlayCue();
void Engine_TaskWait();
s32 Engine_GameFlagSet();
void Engine_ActorSetSpeed();
void Engine_ActorSetPosition();
void Engine_ActorFaceActor();
u8 * Engine_ActorWalkTo();
void Engine_ActorWalkToAndWait();
void Engine_EventWait();
void Engine_ActorSetAnimation();
void Engine_ActorFaceDirection();
void Engine_EventSetMessage();
void Engine_ActorShowEmote();
void Engine_EventShowMessage();
void Engine_ActorRunRepeatedMotion();
void Engine_ActorSetAnimationAndWait();
void Engine_ActorSetDestination();
void Engine_ActorWaitForMove();
void Engine_EventEnd();



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

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* NONMATCHING: 616 of 616 bytes, 90 halfword edits (2026-09-24). The
 * BLDALPHA fade queues its write through QUEUE_IO_WRITE expanded in place;
 * the IME pointer is not hoisted out of the loop as in the reference and the
 * actor, flag and a0 registers are permuted. */
void Func_02000594(s32 a0)
{
    s32 i;
    s32 p10;
    s32 p10b;
    s32 p11;
    s32 p9;
    s32 p9b;
    s32 rec;
    s32 rec2;
    s32 rec7;
    u8 *record;
    s32 v2;
    s32 base6_4000208;
    s32 v0;
    s32 v6;

    p10 = a0;
    rec7 = Value1(Engine_ActorGet, *(s32 *)&Data_02000240_t[250][0]);
    rec = Value1(Engine_ActorGet, p10);
    rec2 = Value1(Engine_GameFlagIsSet, 0x340);
    p9 = *(s16 *)(rec7 + 10);
    p11 = *(s16 *)(rec7 + 18);
    Engine_EventBegin();
    Engine_AudioPlayCue(244);
    for (i = 0; i <= 15; i++) {
        *(s32 *)(rec + 24) = (i << 11) + 0x800;
        *(s32 *)(rec + 28) = (i << 12) + 0x1000;
        if (rec2 == 0) {
            QUEUE_IO_WRITE(0x4000052, ((16 - (i + 1)) << 8) | (i + 1), 0x20000);
        }
        Engine_TaskWait(1);
    }
    Engine_GameFlagSet((0x1fe + p10));
    Call1(Engine_GameFlagSet, 0x340);
    if (Value1(Engine_GameFlagIsSet, 0x9a0) == 0) {
    } else {
        if (Value1(Engine_GameFlagIsSet, 0x9b6) != 0) {
        } else {
            Call1(Engine_GameFlagSet, 0x9b6);
            Call3(Engine_ActorSetSpeed, 13, 0x10000, 0x8000);
            Call3(Engine_ActorSetSpeed, 0, 0x18000, 0xc000);
            record = Value1(Engine_ActorGet, 0);
            if ((s32)record != 0) {
                Engine_ActorSetPosition(13, *(s32 *)((s32)record + 8), *(s32 *)((s32)record + 16));
            }
            Call3(Engine_ActorFaceActor, 13, 0x4000, 0);
            *(u8 *)(Engine_ActorGet(13) + 90) &= 254;
            Call3((void (*)())Engine_ActorWalkTo, 13, p9, (p11 - 16));
            *(u8 *)(Engine_ActorGet(0) + 90) &= 254;
            Engine_ActorWalkToAndWait(0, (p9 + 8), (p11 - 40));
            Engine_EventWait(1);
            {
                u8 *record = Engine_ActorGet(0);
                u8 value = *(volatile u8 *)&record[90];
            
                record[90] = (u8)(value | 1);
            }
            Engine_ActorSetAnimation(13, 1);
            Call3(Engine_ActorFaceDirection, 0, 0x4000, 0);
            Call1(Engine_EventSetMessage, 0x262e);
            Call3(Engine_ActorShowEmote, 13, 0x106, 60);
            Engine_EventShowMessage(13, 0);
            Call3(Engine_ActorShowEmote, 13, 0x102, 60);
            Engine_EventShowMessage(13, 0);
            Call3(Engine_ActorFaceDirection, 13, 0x2000, 0);
            Call3(Engine_ActorShowEmote, 13, 0x101, 60);
            Engine_EventShowMessage(13, 0);
            Engine_EventWait(10);
            Call3(Engine_ActorFaceDirection, 13, 0xc000, 30);
            Engine_ActorRunRepeatedMotion(13, 2);
            Engine_EventShowMessage(13, 0);
            Engine_ActorSetAnimationAndWait(13, 3);
            Engine_EventShowMessage(13, 0);
            Call3(Engine_ActorSetSpeed, 13, 0x10000, 0x8000);
            Engine_ActorSetAnimation(13, 2);
            record = Value1(Engine_ActorGet, 0);
            if ((s32)record != 0) {
                Engine_ActorSetDestination(13, *(s16 *)((s32)record + 10), *(s16 *)((s32)record + 18));
            }
            Engine_ActorWaitForMove(13);
            Engine_ActorSetPosition(13, 0, 0);
        }
    }
    Engine_EventEnd();
}
