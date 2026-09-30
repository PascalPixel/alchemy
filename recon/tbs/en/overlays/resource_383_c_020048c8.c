/* NONMATCHING kuupuappu rejection H1 (2026-09-27): explicit negative
 * cone exits share the distance fallback, preserving the unsigned input
 * producer instead of staging a folded delta. Prediction failed: GCC
 * restores the same blt-to-keep plus jump-to-fallback, not reference bge
 * to fallback plus jump-to-keep. Complete candidate remains 612/612,
 * 74 halfwords / 70 aligned edits, byte-identical to the canonical (cmp).
 * Full normalized diff and identical normal/diagnostic text checked.
 * Frame4, r8 phase ownership, both signed facing producers and all pools
 * remain admitted, but this control-flow spelling supplies no new fact.
 * Retain the failed experiment, close this guard axis, no exact credit.
 * Draft, not-yet-c. Cone H1 restored: 612/612 bytes,
 * 74 differing halfwords / 70 aligned edits, 114 wrong instructions.
 * Both bounded phase trials preserved: H1 31a7f1a26, H2 8c3e32e88.
 * Retain the unsigned INPUT boundary, complete pools, frame4/r8/no-spill
 * and both signed loads. Whole owner remains non-exact; no new DONE.
 * Cone H2 rejected: 612/612, 261 halfwords/132 edits/193 wrong instructions.
 * Staging slot then subtraction in the same u32 rnd recovers the load /
 * unsigned conversion / subtraction producer order, but exposes both signed
 * bounds on one user value. Initial RTL already replaces the two guards
 * with unsigned (delta + 4095) <= 8190. Combine leaves 0x00000fff and
 * 0x1ffe0000 pool words instead of the reference -0x1000; first pool moves
 * to 0x254. Frame4/r8/both signed loads survive, but this violates the frozen
 * cone/pool admission. Normal and diagnostic text agree; whole diff read.
 * Preserve rejection, then restore H1 (31a7f1a26). No further delta/type
 * variants: independently staged unsigned INPUT is the useful boundary;
 * staging the whole delta reopens the already-known range-folding failure.
 * Admitted unsigned cone H1: 612/612 bytes,
 * 74 differing halfwords / 70 aligned edits, 114 wrong instructions.
 * Cone H1 (2026-09-27): the nested short subtraction had no zero extension
 * even in initial RTL. CSE removed HI copies and fed signed pseudo 40
 * directly to subtraction 115; combine retained that ancestry. Staging the
 * unsigned cone input in existing u32 rnd after distance admission creates
 * a real SI producer: shifts 113/114 survive CSE and combine into user 42.
 * The missing pair restores size 612 and pool words/offsets 0x258/25c/260.
 * r8 facing ownership, frame 4, no spill and both signed-facing loads remain.
 * Normal/diagnostic text agree; entire normalized difference read. The pair
 * currently precedes the slot read and reuses r0, whereas ROM loads slot
 * first and converts angle into r3. Other residuals are the cone guard,
 * low-register roles and scheduling, not a pool-size or pointer-spill wall.
 * Previous shared-pointer phase: 608/612, 261 halfwords/102 edits/166 insns.
 * H2 causal follow-up: give fallback facing the same scalar-to-slot
 * publication as entry, reusing angle after its last cone use. This restores
 * the reference ldrsh at 02004986 without losing r8, frame 4 or no-spill.
 * Pointer pseudo 32 still has nine uses/53 insns/three calls, allocated r8;
 * scalar pseudo 40 has six uses/17 insns, three sets and no crossed calls.
 * Whole diff read; normal/diagnostic text identical. H1 is 8e52ee57f.
 * Residual: missing second angle zero-extension, opposite final cone guard,
 * low-register/scheduling differences, and pool starts four bytes early.
 * Two causal models complete. Freeze r8/frame4/both signed-facing producers;
 * do not restart the rejected independent-pointer or aggregate variants.
 * H1 (2026-09-27): history has separate leader/direction, aggregate, and
 * output-helper trials, but no single source pointer reused across phases.
 * ROM replaces the dead leader in r8 with sp+2 at 02004922. One tagged pos
 * local now holds the leader, then the facing slot through cone/fallback
 * and Snap. Allocator pseudo 32 is one user pointer, set twice, nine uses
 * over 53 insns and three calls; the generated code retains it in r8.
 * Admission achieved: frame 4, no facing-address caller-save across Atan2,
 * initial signed leader-facing producer and pre-call halfword publication.
 * Normal and allocator-diagnostic text are identical. Complete owner/pool
 * was 608/612, 250 halfwords/111 aligned edits/172 wrong instructions.
 * H1 remained non-exact: fallback facing became ldrh rather than ldrsh; angle
 * zero-extension, cone branch, low reload registers and scheduling differ.
 * This new lifetime witness supersedes the stopped separate-pointer axis,
 * not its negative evidence. No new DONE bytes. Preserve this invariant.
 * Previous canonical scalar body: 608/612 bytes, 256 halfwords / 117 edits.
 * Completion steering-output model (22125318a): 604/612 bytes,
 * 269 differing halfwords, 127 aligned edits. A coherent inlined selection
 * operation publishes leader facing, computes the cone/fallback, then gives
 * the same scalar slot to the existing snap consumer. Its formal output
 * still becomes r4 and spills across Atan2: 8-byte frame, not the required
 * 4 bytes. Pointer stores also lose both reference signed facing producers
 * (ldrh replaces ldrsh). Full normalized diff and allocator ancestry read;
 * normal/diagnostic text agree. This is not an admitted invariant witness.
 * The address changes from temporary pseudo 89 (5 uses/32 insns) to user
 * parameter 44 (5 uses/39 insns), still one crossed call and allocated r4.
 * Thus a formal scalar-output parameter is not the missing register cause.
 * The failed output-interface model is committed; scalar body restored.
 * No new lifetime fact supports another pointer/aggregate/helper variation.
 * Baseline score 2026-09-26: 608 of 612 bytes, 256 differing
 * halfwords, 117 aligned edits. Typed actor/stop calls restore all pointers
 * and destination arguments; loaded -0x1000 and fixed-point random angles
 * replace the old container constants and incomplete decompiler expressions.
 * Remaining: facing pointer spills through r4 (8-byte frame instead of 4),
 * first steering-branch shape and reload registers. Separate block-local
 * facings instead allocate a 12-byte frame; that ownership trial is ruled out.
 * H1 (2026-09-27): one scene record owns its byte position and three stops;
 * one halfword direction result is reused by every snap. NEAREST_STOP.C and
 * PROMPT.C establish the 16-byte record and in/out direction contract.
 * Admission: 4-byte frame, first direction address retained across Atan2
 * without a spill; whole 612-byte owner and pool must be exact to adopt.
 * Baseline 608/612, 256 halfwords, 117 aligned edits.
 * H1 result: 604/612, 269 halfwords, 127 aligned edits. The 8-byte frame
 * and r4 direction-address spill survive; member storage also changes the
 * initial signed facing loads to ldrh. Thus the record layout is supported,
 * but a direction aggregate does not recover the missing lifetime boundary.
 * Exact siblings were read only; no adoption credit.
 * H2: restore scalar facing and expose the snap's in/out pointer across
 * steering, with a common fallback for failed leader-cone guards. H1's
 * allocator gives its generated address pseudo 166 r4: five uses over 32
 * instructions and one call. Test an explicit direction lifetime instead;
 * preserve the 4-byte/no-spill admission check and complete-owner gate.
 * H2 result: 600/612, 273 halfwords, 126 aligned edits, still 8-byte frame
 * with the direction address spilled across Atan2. Assigning the angle
 * difference before the guard folds the two signed bounds into one unsigned
 * interval, adding 0x0fff0000/0x1ffe0000 pools absent from ROM. Not admitted.
 * Stop this record/direction axis; no new DONE bytes. Canonical body restored
 * to the scalar/shared-facing baseline; retain the callee-proven record type
 * and these rejected hypotheses, not either failed pointer representation.
 * Return-width audit (2026-09-27): own main veneer 08000100 targets
 * 080044d1, the exact LIB/GEOMETRY.C u16 ArcTan2 implementation. Exact
 * COMMON/KOROSSEO/PATH_RIVAL.C also declares this service u16. Correcting
 * this draft's s32 declaration to u16 is byte-identical to its baseline:
 * 608/612 bytes, 256 differing halfwords, 117 aligned edits, unequal topology.
 * The complete normalized diff retains the same signed angle conversion,
 * 8-byte frame, r4 facing-address spill and absent second zero-extension.
 * This fixes the interface but does not admit the 4-byte/no-spill invariant.
 * Stop after this one supported trial; do not vary angle/pointer spellings.
 *
 * Producer/publication inversion audit: initial RTL already creates facing
 * address pseudo 89 (insn 152) and the halfword store (155) before Atan2
 * (71); this is not a late scheduler hoist. Local allocation records five
 * uses over 32 insns, one crossed call, LO_REGS cost 4 versus HI_REGS 24.
 * Global allocation gives it r4, then inserts caller-save insn 673 at sp+0;
 * the resulting facing slot is sp+6 and the frame grows from 4 to 8 bytes.
 * The approved route treats r4 as call-used; no route change was attempted.
 * ROM independently requires the early signed leader-facing load at
 * 0200491c, halfword publication at 02004928 before Atan2 at 0200492c,
 * unsigned slot reload at 0200495a, and signed actor fallback at 02004986.
 * Its r8 changes ownership from dead leader to facing address at 02004922.
 * Thus keeping only a scalar until Snap would remove reference accesses,
 * not recover a demonstrated producer boundary. No such trial was run.
 * Diagnostic output equals normal output and the previous 608/256/117
 * candidate byte-for-byte. Stop with this causal fact; any future inversion
 * must preserve these accesses and establish the 4-byte/no-spill admission
 * without repeating the rejected aggregate or explicit-pointer models.
 */
#include "TYPES.H"
#include "FIELD_EVENT.H"

struct Stop {
    u8 id;
    u8 pos;
    u8 unknown_02[2];
};

struct StopRecord {
    u8 x;
    u8 z;
    u8 unknown_02[2];
    struct Stop stops[3];
};

struct StopWork {
    u8 unknown_000[0x182];
    s16 raised_trigger;
    u8 unknown_184[0x18];
    s16 value_19c;
};

extern struct StopWork *gStopWork;
extern u16 gBlockedFrames;

struct StopRecord *SceneData_FindEntryAtPosition(s32 *pos);
struct StopRecord *KuupuappuHeya_SnapToNearestStop(struct StopRecord *set, s16 *facing);
s32 SceneActor_CheckTileFreeOfKinds(struct StopRecord *pos);
void SceneActor_ApplyScaledBytePairPosition(struct FieldActor *actor, struct StopRecord *pos);
u16 Math_Atan2(s32 z, s32 x);

void KuupuappuHeya_UpdateActorStops(void)
{
    void *pos;
    struct FieldActor *actor;
    struct StopWork *work;
    struct StopRecord *entry;
    struct StopRecord *dest;
    s32 dx;
    s32 dz;
    s32 blocked;
    s16 angle;
    s16 facing;
    u32 rnd;

    pos = Engine_ActorLookup(0);
    work = gStopWork;
    blocked = 0;
    actor = Engine_ActorLookup(2);
    entry = SceneData_FindEntryAtPosition(&actor->x.fixed);
    if (entry != NULL && actor->target_x == ACTOR_NO_TARGET) {
        dx = actor->x.fixed - ((struct FieldActor *)pos)->x.fixed;
        dz = actor->z.fixed - ((struct FieldActor *)pos)->z.fixed;
        angle = ((struct FieldActor *)pos)->facing;
        /* FAKEMATCH: one pointer changes from the dead leader to the snap
         * output, retaining its ownership through the steering phase. */
        pos = &facing;
        *(s16 *)pos = angle;
        angle = Math_Atan2(dz, dx);
        dx >>= 16;
        dz >>= 16;
        if (work->value_19c > 0 && dx * dx + dz * dz <= 400) {
            /* FAKEMATCH: stage the unsigned cone input before the short delta. */
            rnd = (u16)angle;
            if ((s16)(*(s16 *)pos - rnd) <= -0x1000)
                goto reject_cone;
            if ((s16)(*(s16 *)pos - rnd) >= 0x1000)
                goto reject_cone;
            goto keep_facing;
        }
reject_cone:
        if (dx * dx + dz * dz > 64) {
            /* FAKEMATCH: retain the signed scalar producer before publication. */
            angle = actor->facing;
            *(s16 *)pos = angle;
        }
keep_facing:
        dest = KuupuappuHeya_SnapToNearestStop(entry, (s16 *)pos);
        if (SceneActor_CheckTileFreeOfKinds(dest) == 0) {
            SceneActor_ApplyScaledBytePairPosition(actor, dest);
            Object_SetMode(actor, 2);
        } else {
            Object_SetMode(actor, 1);
        }
    }

    actor = Engine_ActorLookup(24);
    entry = SceneData_FindEntryAtPosition(&actor->x.fixed);
    if (entry != NULL && actor->target_x == ACTOR_NO_TARGET) {
        rnd = (u32)Engine_RandomNext() * 2 >> 16;
        rnd = (rnd * 0x60000000 - 0x30000000) >> 16;
        facing = actor->facing + rnd;
        dest = KuupuappuHeya_SnapToNearestStop(entry, &facing);
        if (SceneActor_CheckTileFreeOfKinds(dest) != 0) {
            facing = actor->facing + 0x8000;
            dest = KuupuappuHeya_SnapToNearestStop(entry, &facing);
            if (SceneActor_CheckTileFreeOfKinds(dest) == 0) {
                Engine_ActorSetAttachedEffect(24, 2);
                goto move_first;
            }
            Object_SetMode(actor, 4);
            blocked = 1;
        } else {
move_first:
            SceneActor_ApplyScaledBytePairPosition(actor, dest);
            Object_SetMode(actor, 2);
        }
    }

    actor = Engine_ActorLookup(25);
    entry = SceneData_FindEntryAtPosition(&actor->x.fixed);
    if (entry != NULL && actor->target_x == ACTOR_NO_TARGET) {
        rnd = (u32)Engine_RandomNext() * 3 >> 16;
        rnd = (rnd * 0x30000000 - 0x30000000) >> 16;
        facing = actor->facing + rnd;
        dest = KuupuappuHeya_SnapToNearestStop(entry, &facing);
        if (SceneActor_CheckTileFreeOfKinds(dest) != 0) {
            facing = actor->facing + 0x8000;
            dest = KuupuappuHeya_SnapToNearestStop(entry, &facing);
            if (SceneActor_CheckTileFreeOfKinds(dest) == 0) {
                Engine_ActorSetAttachedEffect(25, 2);
                goto move_second;
            }
            Object_SetMode(actor, 4);
            blocked += 2;
        } else {
move_second:
            SceneActor_ApplyScaledBytePairPosition(actor, dest);
            Object_SetMode(actor, 2);
        }
    }
    if (blocked != 0) {
        if (++gBlockedFrames > 29)
            work->raised_trigger = blocked + 200;
    } else {
        gBlockedFrames = blocked;
    }
}
