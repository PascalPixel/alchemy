/* Draft, not-yet-c. H1 direct aggregate fields: 228/224 bytes,
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
    obj->unknown_5b = 1;
    Engine_ObjectSetAnimation(obj, 1);
    result = 1;
    obj->rise_counter = result;
    goto done;
miss:
    obj->unknown_5b = force;
    Engine_ObjectSetAnimation(obj, 2);
    obj->rise_counter = force;
done:
    return result;
}
