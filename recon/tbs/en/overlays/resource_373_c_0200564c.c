/* Draft, not-yet-c. H8 separate failure arms: 224/224, 9 HW / 9 edits.
 * 2026-09-27: transfer SHIAN_MURA/ACTOR_TRACKING.C's nested condition and
 * per-condition literal-zero publication, keeping Haidia's byte widths.
 * Unlike H4's two-predecessor miss label, local CSE knows incoming force
 * is zero in each separate failure arm. All four byte stores become the
 * QI subreg of user force 35, with no independent zero producer. Force now
 * has seven uses / 52 instructions / four calls; late tail merging recovers
 * the ROM's shared failure arm while preserving separate success stores.
 * Complete pool, frame, pos r8, state sl and flag r9 now match. Residual is
 * solely force r7 versus tpos r6 (reference r6/r7): allocation order starts
 * actor32, tpos38, force35, pos37. No exact credit; the complete role
 * invariant remains unadmitted. Entry-lifetime H7 is preserved at 5be983472.
 *
 * H7 entry coordinate lifetime: 220/224, 72 HW / 49 edits.
 * 2026-09-27: transfer the exact KUUPUAPPU_MURA/FACING_RANGE.C and
 * KUUPUAPPU_MURA_SAI/MOTION_EVENT.C entry-owned coordinate pointers, keeping
 * this owner's early tracking return, byte widths and canonical void ABI.
 * Allocation explains the change: pos/tpos pseudos 37/38 grow from 13/11
 * instructions to 20/18, with three uses and one crossed call unchanged.
 * Force 35 remains five uses over 47 insns, but now ranks before both;
 * force reaches r6 and the two final byte-store tails remain separate.
 * NOT admitted: pos/tpos become fp/sl, state/flag become r8/r7, and address
 * producers move before the fast path. The pool is four bytes early.
 * Thus entry ownership proves the force-ranking cause, not a matching
 * source model. Do not tune declaration order or move producer statements
 * around the early branch without new semantic ownership evidence.
 * Prior retained H3: 224/224 bytes, 59 HW / 39 edits.
 * H6 is preserved at 4e0c098df; its required invariant was not admitted.
 * H6 reused force publication (2026-09-27):
 * Single change from H3: force = 0 at miss, then the existing byte stores.
 * 228/224 bytes, 38 differing halfwords / 23 aligned edits. Unlike H4/H5,
 * CSE retains the second definition of incoming user pseudo 35. Its uses
 * rise from 5/47 insns/1 set to 6/48 insns/2 sets; allocation order changes
 * from tpos,pos,force to tpos,force,pos. Force moves r8 to r7, pos reaches
 * r8, tpos remains r6, and the success/failure store tails stay separate.
 * The failure assignment survives as movs r7,#0; this extra instruction
 * adds a padding halfword and moves the signed -0x1000 pool four bytes late.
 * State/flag r9/sl also remain swapped. Reference force r6 / tpos r7 and
 * complete 224-byte extent are NOT admitted. STOP after the authorized
 * single reuse trial; preserve H3 as the baseline, not this closer score.
 * Retained H3 baseline: 224/224 bytes,
 * 59 differing halfwords / 39 aligned edits. H4 (1795299e8) and H5
 * (7b73c2004) preserve the two rejected literal-zero models below.
 * H5 failure zero width follow-up (2026-09-27):
 * Explicit block-local s32 stopped = 0 retains SI pseudo 83 through CSE,
 * rather than H4's QI pseudo 85. It still does not share the incoming force
 * pseudo at the two-predecessor failure label. Local allocation reserves
 * r5 for stopped; force spills. Full 224-byte output is byte-identical to
 * H4: 70 differing halfwords / 49 aligned edits, signed pool unchanged.
 * STOP: one model plus one causal follow-up. The useful invariant is that
 * independent literal failure zero prevents late tail merging; its lifetime
 * is not the reference's incoming force r6. No new source fact supports
 * another variant. Retain H3's stronger 59 HW / 39 edits as the baseline.
 * H4 literal failure publication (2026-09-27):
 * 224/224 bytes, 70 differing halfwords / 49 aligned edits. Transfer the
 * exact SHIAN tracking sibling's literal-zero failure stores, retaining
 * this owner's byte widths. CSE shares QI zero 85 across the failure call;
 * local allocation gives it r5 and force spills. The two store tails now
 * stay separate, but reference force r6 / pos r8 / tpos r7 is not recovered.
 * Pool remains signed -0x1000 at 02005728. Baseline H3 was 59 HW / 39 edits.
 * Its tails were distinct through allocation and merged in jump2 only
 * after both failure force and success result reloaded into r3.
 * Completion audit H3 plain-byte publication:
 * 224/224 bytes, 59 differing halfwords / 39 aligned edits, byte-identical
 * to H2. Exact WORLD_MAP/LINKED_EFFECTS.C plain-byte writes remove initial
 * RTL QI zero/mask producers (old 80/90/100/110), but those had already died
 * before allocation here: force remains r8, pos r7, tpos r6, and the final
 * byte store still merges. Normal and diagnostic text agree. This excludes
 * the pillar's surviving-QI-zero mechanism; no further cast/store sweep.
 * Exact SHIAN_MURA/ACTOR_TRACKING.C is the same algorithm, but its +0x64
 * signed-halfword state and separate failure paths differ from this owner's
 * +0x62 byte and shared failure arm. It is not evidence to change our widths.
 * Continue only with a new cause for the distinct success/failure byte-store
 * ownership or position-versus-force allocation, not declaration ordering.
 * H2 success-store one-pass boundary:
 * 224/224 bytes, 59 differing halfwords / 39 aligned edits. The boundary
 * shortens the merged tail but does not split it; force/position lifetimes
 * still differ. Signed -0x1000 pool and complete extent are preserved.
 * STOP: complete typed model plus two structural follow-ups exhausted. H1 direct aggregate fields: 228/224 bytes,
 * 69 differing halfwords / 40 aligned edits; byte-identical to H0.
 * Negative result: direct members do not separate the flag-store tail. Typed actor/interface H0 (2026-09-27):
 * 228/224 bytes, 69 differing halfwords / 40 aligned edits; unchanged.
 * Typed coordinates/callees alone do not remove the merged flag-store tail.
 * Complete 224-byte extent includes signed -0x1000 pool at 02005728.
 * Exact local distance helper uses two s32 coordinate pointers; actor state
 * and flag are FIELD_EVENT.H unknown_5b and rise_counter. Budget: this model
 * plus at most two structural follow-ups; only exact bytes earn adoption.
 * Baseline: Score 2026-09-26: 228 of 224 bytes, 69 differing
 * halfwords, 40 aligned edits. The loaded pool value is -0x1000; the old
 * draft mistook its BL-shaped container encoding for a constant. Eager
 * adjacent-sector calculations recover the complete angle-test block.
 * Remaining: force/pos/target allocation (reference r6/r8/r7), shared tail
 * store and four excess bytes. A u8 force parameter adds narrowing shifts
 * absent from the reference (232 bytes, 107 differing halfwords). */
#include "FIELD_EVENT.H"

s32 Runtime_ComputeFixedPointDistance(s32 *first_position, s32 *second_position);
u16 Main_08000100(s32 z, s32 x);

s32 HaidiaMura_TestFacing(struct FieldActor *obj, struct FieldActor *target, s32 range, s32 force)
{
    s32 result;
    s32 *pos;
    s32 *tpos;
    u32 angle;
    u32 left;
    u32 right;
    u32 dir;

    result = 0;
    if (obj->unknown_5b == 1) {
        if (obj->rise_counter == 0) {
            Engine_ObjectSetAnimation(obj, 1);
            return 1;
        }
    }
    pos = &obj->x.fixed;
    tpos = &target->x.fixed;
    if (Runtime_ComputeFixedPointDistance(tpos, pos) < range || force != 0) {
        angle = (u16)Main_08000100(target->z.fixed - obj->z.fixed, *tpos - *pos);
        left = (angle - 0x1000) & 0xf000;
        right = (angle + 0x1000) & 0xf000;
        angle &= 0xf000;
        dir = obj->facing & 0xf000;
        if (angle == dir || right == dir || left == dir || force != 0) {
            /* FAKEMATCH: plain-byte publication avoids synthetic QI masks. */
            *(u8 *)&obj->unknown_5b = 1;
            Engine_ObjectSetAnimation(obj, 1);
            result = 1;
            *(u8 *)&obj->rise_counter = result;
        } else {
            *(u8 *)&obj->unknown_5b = 0;
            Engine_ObjectSetAnimation(obj, 2);
            *(u8 *)&obj->rise_counter = 0;
        }
    } else {
        *(u8 *)&obj->unknown_5b = 0;
        Engine_ObjectSetAnimation(obj, 2);
        *(u8 *)&obj->rise_counter = 0;
    }
    return result;
}
