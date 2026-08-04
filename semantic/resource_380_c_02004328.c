#include "types.h"

/*
 * resource_380 owner at 0x02004328, 148 bytes: a drain-until-room
 * pump.  Saves the s16 counter at Data_03001ebc+472 (the same +472
 * counter idiom resource_3b9's giant touches), primes two channels,
 * then loops: while fewer than 4 of 30 slots are free, request more
 * (0x111c), pop an event pair with Func_080b0058, and hand any
 * non--1 result to Func_080772b0.  On exit it flushes four times with
 * id 224 and restores the saved counter.
 *
 * Complete owner: `push {r5,r6,r7,lr}` at 0x02004328 through
 * `pop {r0} / bx r0` at 0x020043ae, then the three-word literal pool
 * 0x020043b0-0x020043bb (0x03001ebc, 0x111b, 0x111c); next owner's
 * prologue at 0x020043bc.
 *
 * CORRECTION (name sweep): this file's callee names came from a naive
 * pc-relative decode and were wrong; they are resolved here through the
 * overlay's import-veneer table under the +2 rule
 * (tools/overlay_call_targets.ts) to their main-ROM identities.  The
 * earlier cross-file claim -- that this row's one-argument callee was
 * called with two arguments by the 0x020043bc sibling, forcing an
 * old-style declaration there -- is STRUCK: the two rows were never
 * calling the same function, only the same phantom name.  Both calls
 * here are Func_08077248, taking one argument.
 *
 * Uncertainty: callee roles beyond call shape remain open;
 * Func_080f9010 receives only r0=0x53 here.
 */

extern void Func_08015040(s32 arg0, s32 arg1);
extern void Func_08077030(s32 arg0);
extern s32 Func_08077248(s32 arg0);
extern void Func_080772b0(s32 arg0, s32 arg1);
extern void Func_0808a398(s32 arg0, s32 arg1);
extern s32 Func_080b0058(s32 *out_first, s32 *out_second);
extern void Func_080f9010(s32 arg0);

void Func_02004328(void)
{
    u8 *record = *(u8 **)0x03001ebc;
    s16 saved = *(s16 *)(record + 472);
    s32 first;
    s32 second;

    Func_080f9010(0x53);
    Func_0808a398(224, 3);
    Func_08015040(0x111b, 1);
    for (;;) {
        s32 free_slots = 30 - Func_08077248(0) - Func_08077248(1);

        if (free_slots > 3)
            break;
        Func_08015040(0x111c, 1);
        if (Func_080b0058(&second, &first) != -1)
            Func_080772b0(second, first);
    }
    Func_08077030(224);
    Func_08077030(224);
    Func_08077030(224);
    Func_08077030(224);
    *(s16 *)(record + 472) = saved;
}
