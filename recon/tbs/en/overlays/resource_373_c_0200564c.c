/* Draft, not-yet-c. Completion audit H3 plain-byte publication:
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
    if (Runtime_ComputeFixedPointDistance(tpos, pos) >= range && force == 0)
        goto miss;
    angle = (u16)Main_08000100(target->z.fixed - obj->z.fixed, *tpos - *pos);
    left = (angle - 0x1000) & 0xf000;
    right = (angle + 0x1000) & 0xf000;
    angle &= 0xf000;
    dir = obj->facing & 0xf000;
    if (angle != dir && right != dir && left != dir && force == 0)
        goto miss;
    /* FAKEMATCH: plain-byte publication, as in WORLD_MAP/LINKED_EFFECTS.C,
     * avoids the aggregate store's synthetic QImode zero/mask producer. */
    *(u8 *)&obj->unknown_5b = 1;
    Engine_ObjectSetAnimation(obj, 1);
    result = 1;
    /* FAKEMATCH: keep the success flag store inside its own control scope. */
    do {
        *(u8 *)&obj->rise_counter = result;
    } while (0);
    goto done;
miss:
    *(u8 *)&obj->unknown_5b = force;
    Engine_ObjectSetAnimation(obj, 2);
    *(u8 *)&obj->rise_counter = force;
done:
    return result;
}
