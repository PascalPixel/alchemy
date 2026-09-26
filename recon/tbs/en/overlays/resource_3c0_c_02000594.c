/* NONMATCHING: reference/candidate 616 bytes, 8 differing halfwords and
 * 8 aligned edits (2026-09-26). Complete owner 02000594..020007fc, pool
 * 020007e0..020007fc; late actor wrappers pass IDs 8..12. Own-ROM imports
 * audited against FIELD_EVENT.H and the matching visit/altar blend owners.
 * Three structural hypotheses, closed:
 * 1. Typed queue plus advance-in-place frame: 596 bytes, 295 raw / 103
 *    aligned differences; queue writes matched but lost persistent IME.
 * 2. Persistent IME and distinct next frame across TaskWait: 612 bytes,
 *    286 raw / 62 aligned differences; queue and loop instructions matched.
 * 3. Shared GameState selector and typed FieldActor coordinates: retained
 *    616/8/8. Baseline was 616/288/92. Remaining: saved x/z are fp/r9
 *    instead of r9/fp (six halfwords), and the bit-0 byte update reverses
 *    r2/r3 (two). Do not reopen with declaration or register spelling sweeps.
 * FAKEMATCH: the queue keeps the matched one-pass IME wrappers and a signed
 * variable limit, like VISIT_BLEND.C; the byte-flag read remains volatile.
 */
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "IO_WRITE_QUEUE.H"

extern volatile u16 Data_04000208;


/* FAKEMATCH: Call sites spelled through these wrappers pass their constants straight
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

void SuharaSabaku_RestoreActorScaleAndBlend(s32 a0)
{
    s32 i;
    s32 next_frame;
    volatile u16 *ime;
    struct IoWriteQueue *q;
    u32 saved;
    u16 count;
    s32 limit;
    s32 p10;
    s32 p10b;
    s32 p11;
    s32 p9;
    s32 p9b;
    struct FieldActor *rec;
    s32 rec2;
    struct FieldActor *rec7;
    struct FieldActor *record;
    s32 v2;
    s32 queue_pos;
    s32 base6_4000208;
    s32 v0;
    s32 v6;

    p10 = a0;
    rec7 = Engine_ActorGet(gGameState.selected_actor);
    rec = Engine_ActorGet(p10);
    rec2 = Value1(Engine_GameFlagIsSet, 0x340);
    p9 = rec7->x.part.pixel;
    p11 = rec7->z.part.pixel;
    ime = &Data_04000208;
    Engine_EventBegin();
    Engine_AudioPlayCue(244);
    i = 0;
    do {
        rec->scale_x = ((i << 11) + 0x800);
        rec->scale_y = ((i << 12) + 0x1000);
        if (rec2 == 0) {
            q = &gIoWriteQueue;
            next_frame = i + 1;
            do {
                do {
                    saved = *ime;
                } while (0);
                *ime = (u16)ime;
                limit = 31;
                count = q->count;
                if (count <= limit) {
                    u32 *dst = (u32 *)((u8 *)q + count * 12 + 4);

                    *(u16 *)&q->count = count + 1;
                    *dst++ = ((16 - next_frame) << 8) | next_frame;
                    *dst++ = 0x04000052;
                    *dst = 0x20000;
                }
                *ime = saved;
            } while (0);
        } else {
            next_frame = i + 1;
        }
        Engine_TaskWait(1);
        i = next_frame;
    } while (i <= 15);
    Engine_GameFlagSet((0x1fe + p10));
    Call1(Engine_GameFlagSet, 0x340);
    if (Value1(Engine_GameFlagIsSet, 0x9a0) == 0) {
    } else {
        if (Value1(Engine_GameFlagIsSet, 0x9b6) != 0) {
        } else {
            Call1(Engine_GameFlagSet, 0x9b6);
            Call3(Engine_ActorSetSpeed, 13, 0x10000, 0x8000);
            Call3(Engine_ActorSetSpeed, 0, 0x18000, 0xc000);
            record = Engine_ActorGet(0);
            if ((s32)record != 0) {
                Engine_ActorSetPosition(13, record->x.fixed, record->z.fixed);
            }
            Call3(Engine_ActorFaceActor, 13, 0x4000, 0);
            Engine_ActorGet(13)->unknown_5a &= 254;
            Engine_ActorWalkTo(13, p9, (p11 - 16));
            Engine_ActorGet(0)->unknown_5a &= 254;
            Engine_ActorWalkToAndWait(0, (p9 + 8), (p11 - 40));
            Engine_EventWait(1);
            {
                struct FieldActor *record = Engine_ActorGet(0);
                u8 value = *(volatile u8 *)&record->unknown_5a;
            
                record->unknown_5a = (u8)(value | 1);
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
            record = Engine_ActorGet(0);
            if ((s32)record != 0) {
                Engine_ActorSetDestination(13, record->x.part.pixel, record->z.part.pixel);
            }
            Engine_ActorWaitForMove(13);
            Engine_ActorSetPosition(13, 0, 0);
        }
    }
    Engine_EventEnd();
}
