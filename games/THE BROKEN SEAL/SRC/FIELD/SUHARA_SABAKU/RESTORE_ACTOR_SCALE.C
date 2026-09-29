/* FAKEMATCH: one-pass IME wrappers and a signed queue limit preserve the
 * queued write order, as in VISIT_BLEND.C. */
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "IO_WRITE_QUEUE.H"
#include "IO_REG.H"



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

static __inline__ void SetFlagBits(u8 *flags, u8 bits)
{
    *flags |= bits;
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
    /* FAKEMATCH: x precedes z to break their equal allocation priority. */
    s32 p9;
    s32 p11;
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
    ime = &REG_IME;
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
            SetFlagBits(&Engine_ActorGet(0)->unknown_5a, 1);
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
