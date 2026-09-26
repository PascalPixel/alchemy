/* Draft, not-yet-c. Score 2026-09-26: 228 of 224 bytes, 69 differing
 * halfwords, 40 aligned edits. The loaded pool value is -0x1000; the old
 * draft mistook its BL-shaped container encoding for a constant. Eager
 * adjacent-sector calculations recover the complete angle-test block.
 * Remaining: force/pos/target allocation (reference r6/r8/r7), shared tail
 * store and four excess bytes. A u8 force parameter adds narrowing shifts
 * absent from the reference (232 bytes, 107 differing halfwords). */
#include "TYPES.H"

void Engine_ObjectSetAnimation();
s32 Runtime_ComputeFixedPointDistance();
s32 Main_08000100();



/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

s32 HaidiaMura_TestFacing(u8 *obj, u8 *target, s32 range, s32 force)
{
    s32 result;
    u8 *state;
    u8 *flag;
    u8 *pos;
    u8 *tpos;
    u32 angle;
    u32 left;
    u32 right;
    u32 dir;

    result = 0;
    state = obj + 91;
    if (*state == 1) {
        flag = obj + 98;
        if (*flag == 0) {
            Engine_ObjectSetAnimation(obj, 1);
            return 1;
        }
    } else {
        flag = obj + 98;
    }
    pos = obj + 8;
    tpos = target + 8;
    if (Runtime_ComputeFixedPointDistance(tpos, pos) >= range && force == 0)
        goto miss;
    angle = (u16)Main_08000100(*(s32 *)(target + 16) - *(s32 *)(obj + 16), *(s32 *)tpos - *(s32 *)pos);
    left = (angle - 0x1000) & 0xf000;
    right = (angle + 0x1000) & 0xf000;
    angle &= 0xf000;
    dir = *(u16 *)(obj + 6) & 0xf000;
    if (angle != dir && right != dir && left != dir && force == 0)
        goto miss;
    *state = 1;
    Engine_ObjectSetAnimation(obj, 1);
    result = 1;
    *flag = result;
    goto done;
miss:
    *state = force;
    Engine_ObjectSetAnimation(obj, 2);
    *flag = force;
done:
    return result;
}
