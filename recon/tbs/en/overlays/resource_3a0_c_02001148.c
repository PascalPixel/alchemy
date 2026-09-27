/* P2: all 486 function/pool bytes exact; reviewed 488-byte span includes
 * the separate trailing alignment halfword (2026-09-27). No instruction or
 * pool differences remain. Removing the explicit pre-loop zero and its OR
 * leaves synthetic QI zero 88 (loop insn 663) as the sole dead r8 producer.
 * x remains SI 34: priority 1 then motion 0 inside the loop, later actor 19
 * motion and actor 21 y stores use its r7 zero. The natural for loop now
 * matches the entire owner. Approved production placement must prove the
 * final native alignment; raw score is 486/488, one missing halfword.
 * Prior P1 is preserved at 30cb1ea97. No gap registry or compiler changes.
 *
 * NONMATCHING P1 phased priority/motion value: 496/488 bytes, 237 differing
 * halfwords / 49 aligned edits (2026-09-27). RUNPA_MURA/EFFECT.C confirms
 * the complete priority/motion/collision/palette initialization sequence.
 * Reusing x for priority 1 then motion 0, with the natural for loop, leaves
 * both SI pseudo-35 definitions through loop optimization (193=1, 226=0).
 * It no longer hoists the loop-local motion zero. The entire loop body,
 * transition arithmetic and post-loop r7 consumers now match.
 * New remaining fact: loop.c hoists synthetic QI zero 89 (235 -> 672),
 * which is ORed into the typed priority_flags store at insn 321. It survives
 * alongside explicit pre-loop zero 34, producing dead r8 AND sl zeros and
 * an extra saved register. The old CROSS_DOORWAY zero workaround may now
 * duplicate the newly retained producer; one removal follow-up is justified.
 * Pool is eight bytes late; complete-owner admission still fails. No credit.
 *
 * Previous admitted transfer T2: 482/488 bytes, 226 halfwords / 59 edits.
 * Admitted T2 restored; failed outward O1 is preserved at 9b5ca9554.
 * Outward O1: 482/488 bytes, 224 halfwords / 63 aligned edits.
 * 2026-09-27: tested IO_WRITE_QUEUE.C's one-pass do/while(0) boundary
 * around x=0 and its motion_flags store, retaining T2's labeled outer loop
 * and CROSS_DOORWAY.C's pre-loop-zero OR. All three zero lifetime facts
 * survive, but complete normalized diff regresses from 59 to 63 edits.
 * Admission alone is insufficient: the block separates motion-store
 * scheduling from collision setup and does not repair the earlier constant
 * or sprite-priority computations. Frame 8 and topology remain equal.
 * New diagnostic: loop_optimize returns before register/alias setup when
 * LOOP_BEG is absent. This block supplies it, but loop.c reports the single
 * loop at 206..242 as phony in both passes. Initialization of the pass alone
 * is therefore not the missing outward boundary. -da assembly equals normal.
 * Reject this model; preserve T2 as canonical. No causally supported second
 * wrapper placement was found. Do not sweep one-pass blocks or zero spellings.
 * No new DONE bytes.
 *
 * Transfer T2 482/488 bytes, 226 halfwords / 59 aligned edits.
 * Retained inversion witness (2026-09-27): the corrected zero consumers
 * plus a labeled counter loop admit all three zero-lifetime constraints:
 * an unread r8 zero before the loop, mov r7,0 inside the motion-flags store
 * sequence, and r7 reused for actor 19 motion_flags and actor 21 y afterward.
 * The preheader temporary is r3, not ROM r2, and its ordering still differs.
 * Complete normalized diff is topology equal, frame 8. All 30 service calls
 * remain, but transition constants, sprite-priority computation/r0 restore,
 * preheader scheduling and pool placement differ; no whole-owner match.
 * loop dump has no recognized loop and cannot hoist x. Diagnostic -da
 * assembly equals ordinary compilation. T1 at 1a6dced6e has only six edits
 * but violates the loop-local-r7 admission; preserve it as the close witness,
 * not as a reason to resume spelling sweeps. T2 is the admitted model.
 * Exact transferred source: MAKYURI_IRIGUCHI/CROSS_DOORWAY.C's tagged
 * pre-loop-zero OR, applied to priority_flags at +0x23 while loop x feeds
 * motion_flags at +0x55. Its sibling source was not edited.
 * Missing outward evidence: a source boundary retaining the desired loop
 * optimizations without moving x's zero, not another equivalent zero spelling.
 * One transfer and one causal follow-up exhausted. No new DONE bytes.
 *
 * Transfer T1 486/488 bytes, 22 halfwords / 6 aligned edits.
 * 2026-09-27: exact MAKYURI_IRIGUCHI/CROSS_DOORWAY.C supplies a pre-loop
 * zero ORed into byte flags. Here the byte accesses are priority +0x23
 * and motion +0x55. Feeding the motion store from x and ORing zero into
 * priority fixes the dead r8 preheader value and both post-loop r7 stores.
 * All pool offsets/values now match. Complete normalized diff is topology
 * equal; remaining edits are r7's hoisted zero, preheader order/back-edge,
 * and the missing trailing alignment halfword. No partial-owner adoption.
 * T1 fails the frozen local admission: r7 must be initialized in the loop
 * at reference 020011fc, then reused at 020012a8 and 020012e0. loop.c still
 * moves x's insn 212 (global savings 2, lifetime 147) to preheader 658.
 * Diagnostic -da assembly equals normal compilation. The prior goto trial
 * predated this corrected zero-consumer graph and kept the wrong r7/r8 roles.
 * One causal control-flow follow-up is permitted; no zero/type permutations.
 * No new DONE bytes.
 *
 * Previous canonical: 490 of 488 bytes, 131 halfwords / 39 aligned edits
 * (2026-09-26). Complete owner 02001148..02001330 includes the zero at
 * 020012f4, eleven pool words 020012f8..02001320, return at 0200132c and pad.
 * Three bounded structural trials: goto loop gives 488 bytes / 86 edits,
 * suppressing zero hoisting but also changing unrelated constant selection.
 * Separate loop/post-loop actor locals give 488 bytes / 47 edits, restoring
 * the r5 +35 pointer before the palette call and the later actor in r0.
 * A narrow actor-21 zero gives this retained 490-byte / 39-edit draft, with
 * the reference's r5 zero load and pool before the epilogue. Baseline was
 * 492 bytes / 94 differing halfwords / 51 aligned edits.
 * Remaining: loop.c moves x's insn 211 to preheader insn 647 (global savings
 * two), leaving loop zero r7 hoisted and post-loop x in r8. Reference keeps
 * r7's initialization inside the loop, then uses r7 for both post-loop stores;
 * its preheader r8 zero is dead. Pool is four bytes late. Next work must
 * explain the two zeros' source lifetime, not re-sweep actor/pool spellings. */
#include "TYPES.H"
#include "FIELD_EVENT.H"

void Main_0808a5e0(s32 music);

extern s16 Data_02000240_t[][1];
extern u8 Value_02008325;
extern u8 Value_02008501;

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

s32 Func_02001148(void)
{
    struct FieldActor *actor;
    u32 n;
    s32 x;

    gEventWork->start_transition = 0x100;
    Main_0808a5e0(169);
    if (Data_02000240_t[225][0] > 9) {
        Call1((void (*)())Engine_GameFlagClear, 0x12f);
    }
    if (Value1((s32 (*)())Engine_GameFlagIsSet, 0x895)) {
        Call3((void (*)())Engine_ActorFaceDirection, 13, 0x8000, 0);
        Call3((void (*)())Engine_ActorSetPosition, 14, 0x920000, 0x1380000);
        Engine_ActorFaceDirection(14, 0, 0);
        if (Value1((s32 (*)())Engine_GameFlagIsSet, 0x89a)) {
            Engine_ActorSetPosition(17, 0, 0);
        }
    }
    if (Engine_GameFlagIsSet(0x8b0)) {
        Engine_ActorSetPosition(17, 0, 0);
    }
    for (n = 0; n <= 2; n++) {
        struct FieldActor *actor;
        actor = Engine_ActorGet(n + 23);
        x = 1;
        actor->sprite->priority = x;
        x = 0;
        actor->motion_flags = x;
        actor->collision_flags = 8;
        Engine_ActorSetSpriteFlags(actor, 0);
        Engine_ObjectSetPalette(actor, 15);
        actor->priority_flags = (actor->priority_flags & 254) | 2;
    }
    if (Value1((s32 (*)())Engine_GameFlagIsSet, 0x202)) {
        Call3((void (*)())Engine_ActorSetPosition, 14, 0x920000, 0x1380000);
        Engine_ActorFaceDirection(14, 0, 0);
    }
    if (Value1((s32 (*)())Engine_GameFlagIsSet, 0x201)) {
        Engine_ActorSetAnimation(20, 5);
        {
            s32 px = Engine_ActorGet(20)->x.fixed;

            Call6((void (*)())Engine_MapCopyCellAttributes, 3, 17, 1, 1, px >> 20, Engine_ActorGet(20)->z.fixed >> 20);
        }
        Call2((void (*)())Engine_TaskAddCallback, (s32)&Value_02008325, 0xc80);
    }
    Engine_ActorSetChildValue(18, 2);
    Engine_ActorGet(18)->update = (void (*)(union FieldObject *))&Value_02008501;
    actor = Engine_ActorGet(19);
    actor->motion_flags = x;
    actor->y.fixed = 0x100000;
    actor->target_y = 0x100000;
    actor->scale_x = 0x8ccc;
    actor->scale_y = 0x6666;
    actor->sprite->rotation = 0x8000;
    Engine_ActorSetSpriteFlags(Engine_ActorGet(21), 0);
    {
        /* FAKEMATCH: narrow local retains the short-range zero pool load. */
        u8 shown = 0;

        Engine_ActorGet(21)->motion_flags = shown;
    }
    Engine_ActorGet(21)->y.fixed = x;
    Engine_ActorGet(21)->target_y = -0x80000000;
    return 0;
}
