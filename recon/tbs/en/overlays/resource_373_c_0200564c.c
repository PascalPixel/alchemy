/* NONMATCHING: 224 bytes, candidate 232 (2026-09-24). Readable facing test;
 * remaining: register allocation of force/pos/target (reference: r6, r8, r7)
 * and the early-return layout. The pool constant -0x46b0ffa is the ROM value. */
#include "TYPES.H"

void Engine_ObjectSetAnimation();
s32 Runtime_ComputeFixedPointDistance();
s32 Main_08000100();



/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

s32 HaidiaMura_Func0200564c(u8 *obj, u8 *target, s32 range, s32 force)
{
    s32 result;
    u8 *state;
    u8 *flag;
    u8 *pos;
    u8 *tpos;
    u32 angle;
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
    dir = *(u16 *)(obj + 6) & 0xf000;
    if ((angle & 0xf000) != dir && ((angle + 0x1000) & 0xf000) != dir && ((angle + -0x46b0ffa) & 0xf000) != dir && force == 0)
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
