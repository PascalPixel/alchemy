/* NONMATCHING: current 728 / 732 bytes, 234 differing halfwords, 50 edits.
 * Whole owner [02004c68,02004f44), including ten pool words.
 * 2026-09-26 bounded H1: a lowering-phase inline routine returns speed;
 * the caller owns the clamp store. This DOES rebuild 0x04000000 at the
 * clamp (movs/lsls/str), unlike the old shared compare/store expression.
 * But return-path block placement moves the lowering wait/deceleration
 * after the actor sequence, and splits speed ownership across r6/r8.
 * Candidate 724 / reference 732 bytes, 354 differing halfwords, 123 edits.
 * Full normalized diff read; rejected for topology and lifetime changes.
 * Preserve this counterexample: the clamp can rematerialize in ordinary C,
 * but a phase-return boundary is not the reference's whole-owner structure.
 * H1 is preserved at 2403f5f54.
 * H2, suggested by the independently matched message decoder: restore
 * whole-function speed ownership and give the clamp a u16 pixel-height
 * constituent (0x400 << 16), separate from the SImode comparison bound.
 * Complete output is byte-identical to the old 728-byte baseline. The
 * constituent folds before it can separate the compare/store expressions;
 * no extra pool, runtime load or frame slot survives. Full normalized diff
 * read and binary equality checked. Stop after these two hypotheses.
 *
 * Previous baseline: 732 bytes, candidate 728, 234 differing halfwords, 50 halfword
 * edits (2026-09-26). The explicit lowering loop now has the reference's
 * topology. CSE reuses its 0x4000000 comparison for the following store,
 * removing four bytes and shifting the remaining body and pool. The actor
 * reset also schedules its animation argument before the zero store.
 * Hand-written from the resolved disassembly as a single-overlay unit binding
 * Engine_* at their import veneers. Remaining: the lowering loop. As a for
 * (;;) with a break the sizes agree but loop.c rotates the loop (enters at
 * the wait, 43 halfwords); as a goto loop the layout is the reference's but
 * CSE reuses the compare's 0x4000000 for the store after the loop, which the
 * reference rebuilds (4 bytes short). One scheduling pair after the walk
 * (zero before the unknown_44 store).
 * Sol Mercury H2 (2026-09-27): MAP_SCROLL.H and the exact scroll updater
 * identify the record at map work +0x164 as layers[2], not a local layer 7.
 * The +0x0c field is offset_y; +0x1c is speed_y. Reusing the shared record
 * and slot declaration emits byte-identical output to the 728-byte baseline
 * (234 differing halfwords / 50 aligned edits); complete normalized diff
 * read and binary equality checked. Keep this semantic correction.
 * The clamp still shares its comparison pseudo through CSE2 and reload.
 * New exact function bytes and alignment: both 0.
 * Sol Mercury H3: an inline clamp setter fails to rematerialize the bound.
 * The complete diff shows the lowering wait/deceleration block moved below
 * the actor sequence and the exit test inverted; speed remains in sl.
 * CSE still shares r2 at the clamp. Preserve this negative candidate in its
 * own commit, then restore H2 before testing the independent actor-store
 * residual. Do not retry the inline clamp boundary. Credit remains 0.
 * Sol Mercury H4: a one-iteration reset scope prevents the animation
 * argument from preceding the store, but moves the zero before ActorGet
 * and changes the preceding walk argument order. Complete diff read:
 * 728/732 bytes, 233 differing halfwords / 54 aligned edits. This local
 * scheduler counterexample does not meet the predicted emitted sequence;
 * preserve it, then restore H2. No new exact function or alignment bytes.
 * The shared record correction is retained; clamp/reset boundary axes end
 * here until new compiler or ordinary-source evidence supplies a new fact.
 * Final checkpoint restores H2's canonical body. Its whole emitted extent
 * is byte-identical to the prior baseline; only the shared map-record
 * semantics and the recorded bounded facts remain. Both owners are C not
 * yet written, not assembly. No adoption and no credit reclassification. */
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "MAP_SCROLL.H"

void Main_080091a0(void);

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

void Func_02004c68(void)
{
    struct MapLayerScroll *layer;
    s32 speed;
    u16 lower_height;

    layer = &Data_03001e70->layers[2];
    speed = 0x9c28;
    layer->offset_y = 0x4890000;
    layer->speed_y = 0;
    Engine_ActorGet(0)->motion_flags = 0;
    Engine_ActorGet(0)->z.fixed += -0x890000;
    Engine_ActorGet(0)->target_z = Engine_ActorGet(0)->z.fixed;
    Engine_ActorGet(13)->motion_flags = 0;
    Engine_ActorSetPosition(13, 0x2a80000, 0x1b80000);
    Engine_ActorGet(13)->z.fixed += -0x890000;
    Engine_ActorGet(13)->target_z = Engine_ActorGet(13)->z.fixed;
    Engine_MapRedraw();
    Engine_TaskWait(1);
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(60);
    Main_080091a0();
    Engine_AudioPlayCue(223);
    lower_height = 0x400;
lower:
    {
        layer->offset_y -= speed;
        Engine_ActorGet(0)->z.fixed += speed;
        Engine_ActorGet(0)->target_z = Engine_ActorGet(0)->z.fixed;
        Engine_ActorGet(13)->z.fixed += speed;
        Engine_ActorGet(13)->target_z = Engine_ActorGet(13)->z.fixed;
        if (layer->offset_y <= 0x4000000)
            goto lowered;
        if ((*(u32 *)0x03001e40 & 15) == 0 && speed > 0xccb)
            speed += -0x560;
        Engine_TaskWait(1);
        goto lower;
    }
lowered:
    /* FAKEMATCH: retain the clamp's pixel-height constituent in HImode. */
    do { layer->offset_y = lower_height << 16; } while (0);
    Engine_MapRedraw();
    Engine_TaskWait(2);
    Engine_ActorGet(0)->motion_flags = 3;
    Engine_ActorGet(13)->z.fixed = 0x1b80000;
    Engine_ActorGet(13)->target_z = Engine_ActorGet(13)->z.fixed;
    Engine_EventWait(30);
    Call3(Engine_ActorWalkToAndWait, 0, 0x2c0, 0x248);
    *(s32 *)Engine_ActorGet(0)->unknown_44 = 0;
    Engine_ActorSetAnimation(0, 6);
    Engine_EventWait(6);
    Engine_ActorSetAnimation(0, 7);
    Engine_ActorGet(0)->speed = 0x30000;
    Engine_ActorGet(0)->acceleration = 0x20000;
    Engine_AudioPlayCue(152);
    Engine_ActorGet(0)->velocity_y = 0x40000;
    Engine_ActorSetSpriteFlags(Engine_ActorGet(0), 0);
    Call3(Engine_ActorSetDestination, 0, 0x2e0, 0x248);
    Engine_ActorWaitForMove(0);
    Engine_ActorSetSpriteFlags(Engine_ActorGet(0), 1);
    *(s32 *)Engine_ActorGet(0)->unknown_44 = 0x4000;
    Engine_ActorSetAnimation(0, 6);
    Engine_EventWait(6);
    Engine_ActorFaceDirection(0, 0x8000, 30);
    Engine_AudioPlayCue(223);
    goto raise;
raise_wait:
    if ((*(u32 *)0x03001e40 & 7) == 0 && speed <= 0xcccc) {
        speed += 0x1999;
    }
    Engine_TaskWait(1);
raise:
    {
        layer->offset_y += speed;
        Engine_ActorGet(13)->z.fixed -= speed;
        Engine_ActorGet(13)->target_z = Engine_ActorGet(13)->z.fixed;
        if (layer->offset_y <= 0x488ffff) {
            goto raise_wait;
        }
    }
    layer->offset_y = 0x4000000;
    Call6(Engine_MapCopyCellsTo, 45, 91, 40, 91, 5, 4);
    Call6(Engine_MapCopyCellAttributes, 104, 34, 5, 4, 40, 34);
    Engine_MapRedraw();
    Engine_TaskWait(2);
    Engine_ActorSetPosition(13, 0, 0);
    Engine_EventWait(30);
    Engine_ActorGet(0)->priority_flags |= 1;
}
