/* 2026-09-27 pillars closing audit: own-ROM service veneer 080091c0 jumps
 * to 08010704, the reconstructed FIELD/COMMON/MAP/COPY_CELL_ATTRIBUTE_RECT.C.
 * Its complete source and exact STAGED_ACTOR.C callers prove the existing
 * six-s32 void signature. There is no new return-type or actor-byte relation
 * to reopen H5's 3-halfword call-order residual. Preserve its admitted zero
 * publication, frame8 and sole pool; no new variant or alignment credit.
 * H5 direct-call canonical restored; H6 counterexample at 79c045566.
 * H6 rejected: exact Shian Call6 emits the identical H5 candidate bytes:
 * 152/152, 3 halfwords / 3 aligned edits. Zero publication, frame8 and pool
 * remain exact. Inlining erases the function-pointer boundary before CSE;
 * argument setup is still r0,r1,r2,r3, not ROM r1,r3,r2,r0. Whole normalized
 * diff read, candidate.bin equals H5, -da/-fsched-verbose equals ordinary.
 * Preserve this counterexample, then retain the simpler H5 direct call.
 * One authorized model complete; no function or alignment credit.
 * H6 Call6 transfer: exact SHIAN_MURA/SETUP.C uses an old-style void
 * function-pointer inline boundary, unlike the rejected direct MapCopy
 * wrapper. Predict ROM argument order r1,r3,r2,r0, freezing H5's complete
 * zero-publication sequence, frame8, branches and pool. One trial only.
 * CANONICAL H5: 152/152 bytes, 3 halfwords / 3 aligned edits. Scoped
 * receiver removes H4's extra copy and entry regression; complete initial
 * zero/store sequence, r6 lifetime, frame8, branches and pool now exact.
 * Only MapCopy setup differs: r0,r1,r2,r3 versus ROM r1,r3,r2,r0.
 * Full normalized diff read; -da/-fsched-verbose output equals ordinary.
 * H3 witness at 0edc7d4c5, rejected H4 at 72986aeed. Three models complete;
 * no new function or alignment credit. Freeze this admitted zero sequence.
 * H5: isolate the second actor receiver in its actual publication phase.
 * H4's multi-definition USER32 caused the extra copy and changed the entry;
 * predict a single-use receiver collapses to r0 while zero/store order and
 * H3's r6 lifetime remain. No map-call or width/constant spelling change.
 * H4 rejected: 156/152 bytes, 60 halfwords / 21 aligned edits. Zero/store
 * order fixes, but receiver reuse gives USER32 two definitions (5 uses /
 * 6 insns), changes entry coordinate loads and adds r0->r2 publication copy.
 * Frame8 and r6 survive; pool moves 0x94->0x98. Full diff and -da checked.
 * H4: sched2 shows initial byte store86 and zero89 both priority1, tied
 * by original order. Separate the actual receiver lookup from publication
 * and initialize still between them; predict zero before the store while
 * preserving H3's consumer-loop lifetime, frame8 and complete pool.
 * H3 admitted: 152/152 bytes, 5 halfwords / 5 aligned edits. Zero38 now
 * spans 40 insns / 2 calls instead of 2 insns / 0 calls; r6 is initialized
 * before the column tests and consumed after both calls. Frame8 and pool
 * 0x94 exact. Remaining: zero/store order and MapCopy argument scheduling.
 * Full normalized diff read; -da/-fsched-verbose output equals ordinary.
 * H3 inversion: zero pseudo38 survives CSE through regmove, but
 * local-alloc update_equiv_regs sinks its sole use at depth0 to the final
 * byte store. That move is forbidden at nonzero loop depth. Enclose the
 * final byte publication in one pass; admission is r6 zero before column
 * tests, retained through MapCopy and ActorGet, with frame8 and pool fixed.
 * Canonical direct-call/u8 model restored after H1 and H2; complete
 * baseline candidate is byte-identical, including its single literal pool.
 * H2 rejected (2026-09-27): STAGED_STEP.C's Map_CopyCellAttributes inline
 * boundary emits the identical 152-byte baseline: 37 differing halfwords /
 * 16 aligned edits. The whole normalized diff preserves both residuals:
 * width-before-height setup and late zero rematerialization instead of r6.
 * The shared helper has no distinct argument-expansion boundary here.
 * H1 rejected (2026-09-27): literal Half zero from MAKYURI_CHOJO/LAMP.C
 * emits 160/152 bytes, 55 differing halfwords / 27 aligned edits. CSE keeps
 * an HI zero pseudo, but it is loaded only at the final byte store; no r6
 * lifetime is admitted. An extra zero pool and moved division-mask pool
 * add two branch/pool islands. Unlike LAMP there is no earlier HI zero
 * producer to share. Map size-argument order is unchanged. Whole normalized
 * diff read; -da assembly equals normal output. No new DONE bytes.
 * NONMATCHING: 152 of 152 bytes, 16 halfword edits (2026-09-24). Hand-written from the
 * resolved jump-table disassembly as a single-overlay unit binding Engine_* at
 * their import veneers. Remaining: 37 halfwords: reference keeps the motion-flag zero in r6 from before the column tests, and loads height before width for Engine_MapCopyCellAttributes.
 * 2026-09-26: widening still from u8 to s32 did not change either residual;
 * retain the original width and the verified whole-owner bindings. */
#include "TYPES.H"
#include "FIELD_EVENT.H"

void SceneState_ApplyFourRectsAndSetActor8Byte85(void);
void SceneState_ApplyRectAndSetSlotEightByte35(void);
void ActorPresentation_SetSceneCell58AndMarkActorEight(void);
void SceneState_ApplyRectAndSetActor8Byte35(void);

void Local_02001ac8(void)
{
    struct FieldActor *block = Engine_ActorGet(8);
    s32 x = block->x.fixed;
    s32 y = block->y.fixed;
    s32 column = x / 0x100000;
    u8 still;

    if (y == 0)
        Engine_ActorGet(8)->priority_flags = 2;
    SceneState_ApplyFourRectsAndSetActor8Byte85();
    {
        struct FieldActor *actor = Engine_ActorGet(8);

        still = 0;
        actor->motion_flags = 3;
    }
    if (column == 40) {
        SceneState_ApplyRectAndSetSlotEightByte35();
    } else if (column == 42) {
        ActorPresentation_SetSceneCell58AndMarkActorEight();
    } else if (column == 41) {
        SceneState_ApplyRectAndSetActor8Byte35();
    } else {
        if (column == 39)
            goto copy;
        if (column == 38)
            goto copy;
        if (column != 37)
            return;
    copy:
        Engine_MapCopyCellAttributes(61, 36, 1, 1, column, 42);
        /* FAKEMATCH: keep the zero's sole consumer inside its phase. */
        do {
            Engine_ActorGet(8)->motion_flags = still;
        } while (0);
        Engine_ActorGet(8)->y.fixed = 0x200000;
    }
}
