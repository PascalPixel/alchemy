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

/* One symbol PER CALL SITE, named at the site's PC-relative-decoded address
   (see resource_382:3ac for the rule). Repeated callees therefore appear
   several times under different names: Func_08077030 is called four times
   and Func_08015040 and Func_08077248 twice each. */
extern void Func_02008e52(s32 arg0);                        /* 0x0200433c -> 080f9010 */
extern void Func_02008e4a(s32 arg0, s32 arg1);              /* 0x02004344 -> 0808a398 */
extern void Func_02008c6a(s32 arg0, s32 arg1);              /* 0x0200434c -> 08015040 */
extern s32 Func_02008cd0(s32 arg0);                         /* 0x02004352 -> 08077248 */
extern s32 Func_02008cda(s32 arg0);                         /* 0x0200435c -> 08077248 */
extern void Func_02008c88(s32 arg0, s32 arg1);              /* 0x0200436a -> 08015040 */
extern s32 Func_02008e80(s32 *out_first, s32 *out_second);  /* 0x02004372 -> 080b0058 */
extern void Func_02008d08(s32 arg0, s32 arg1);              /* 0x02004382 -> 080772b0 */
extern void Func_02008cd8(s32 arg0);                        /* 0x0200438a -> 08077030 */
extern void Func_02008cde(s32 arg0);                        /* 0x02004390 -> 08077030 */
extern void Func_02008ce4(s32 arg0);                        /* 0x02004396 -> 08077030 */
extern void Func_02008cea(s32 arg0);                        /* 0x0200439c -> 08077030 */

/* STILL-OPEN residual (36/148 differing bytes): confined entirely to the
 * bl-instruction displacement halfwords at every one of the 12 call sites
 * below (verified with tools/overlay_call_targets.ts's +2 rule -- see
 * scratch diff at semantic history). The reference's stored displacement is
 * consistently 0x4000 larger than what this candidate links to. Renaming the
 * callees three different ways -- main-ROM Func_08xxxxxx names (this file),
 * per-site overlay-local Func_0200xxxx numeric veneer addresses, and the
 * same numeric addresses declared old-style (empty-paren K&R, matching
 * exact/resource_3a7_c_02000944.c's "physical overlay veneer alias"
 * pattern) -- produced byte-IDENTICAL wrong displacements every time, so
 * this is not a call-symbol-naming problem. Every other instruction in the
 * function (all data ops, the do-while restructure, arg setup) matches the
 * reference exactly. Looks like a toolchain/link-time veneer-placement
 * quirk for this owner's freshly-referenced import slots, not something a
 * C-source edit can reach; escalate rather than re-attempt renaming. */

void Func_02004328(void)
{
    u8 *record = *(u8 **)0x03001ebc;
    s16 saved = *(s16 *)(record + 472);
    s32 first;
    s32 second;
    s32 free_slots;

    Func_02008e52(0x53);
    Func_02008e4a(224, 3);
    Func_02008c6a(0x111b, 1);
    do {
        free_slots = 30 - Func_02008cd0(0);
        free_slots -= Func_02008cda(1);

        if (free_slots <= 3) {
            Func_02008c88(0x111c, 1);
            if (Func_02008e80(&second, &first) != -1)
                Func_02008d08(second, first);
        }
    } while (free_slots <= 3);
    Func_02008cd8(224);
    Func_02008cde(224);
    Func_02008ce4(224);
    Func_02008cea(224);
    *(s16 *)(record + 472) = saved;
}
