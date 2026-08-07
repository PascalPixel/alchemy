/*
 * BYTE-EXACT 2026-08-07.  The last two halfwords closed with the new
 * -fthumb-call-literal-arg1-first-chained: the reference transposes a
 * two-literal argument sheet exactly where the call it feeds is followed by
 * another argument setter, which is why 0x020011dc writes `movs r1, #0' first
 * while the identical pair at 0x0200128a, whose call is followed by a branch,
 * keeps register order.
 */
#include "types.h"

/*
 * Resource 3ad, conversation driver at 0x020011b8 (244 bytes, 25 call sites).
 *
 * Complete owner: `push {r5, lr}` at 0x020011b8 and `pop {r5} / pop {r0} /
 * bx r0` at 0x0200129e.  The popped branch register is r0, so it holds the
 * return address and the owner is `void`.  Two pool words sit at 0x020012a4 and
 * 0x020012a8 (dialogue ids 0x2547 and 0x254b), past the return and never
 * reached as code.
 *
 * This owner is a state machine over the overlay's own single-question steps.
 * Each of those steps returns a boolean in the low byte, which is why every
 * result is tested as `lsls r0,#24 / cmp r0,#0` - only bits 0-7 are examined.
 * The byte-exact siblings `assets/code/resource_3ad_c_020012ac.c` and
 * `..._0200141c.c` confirm the shape: each returns `Func_...(0, 0) == 0`.
 *
 * The control flow is genuinely a graph, not a tree - 0x0200122e is entered
 * from three places and 0x0200120c is a loop head re-entered from 0x0200128e -
 * so the C below uses labels and `goto` rather than duplicating call sites.
 * Duplicating them would inject phantom calls into the per-target multiset.
 *
 * Call targets resolved with `tools/overlay_call_targets.ts`.  Per-target
 * multiset over the 25 sites: Func_0808a148 x5, Func_0808a170 x4,
 * Func_0808a180 x2, Func_0808a178 x2, Func_0808a070 x1, and one site each for
 * the eleven in-overlay steps 0x020012ac, 0x020012c4, 0x020012f0, 0x0200131c,
 * 0x02001320, 0x0200134c, 0x02001368, 0x02001394, 0x0200141c, 0x02001448,
 * 0x02001760.  That is 25 call expressions below, matching the row's calls=25.
 *
 * 0x0200131c is reported `unknown` by the resolver because it has no `push`
 * prologue: it is a real four-byte leaf, `movs r0,#1 / bx lr`, sitting between
 * 0x020012f0's literal pool and 0x02001320's prologue.  Its result is discarded
 * here, so it is called for its side effect only (which is none) - kept because
 * the call site is real.
 *
 * RESIDUE (2 of 122 halfwords, 2026-08-07).  Same class as
 * resource_399_c_020002b8.c: at 0x24 the reference writes `movs r1,#0' before
 * `movs r0,#1' for Func_02002d8e(1, 0), while 0xce is the identical literal
 * pair for Func_02002e38(1, 0) and keeps r0 first.  Identical operands with
 * opposite orders means no operand-shape flag can discriminate them, and
 * -fthumb-early-literal-pool ICEs on this owner (create_fix_barrier,
 * config/arm/arm.c:5411).  Neither the singles nor the compiler cohort, nor
 * hoisting either literal into a local, nor prototyping the import moves it.
 */

/* Old-style declarations: overlay import arities vary per call site. */
   /* scene request */
   /* show a dialogue line by id */
   /* dialogue-line variant with a mode word */
   /* wait for the slot's action to finish */
    /* dialogue prompt; result selects the branch */

/* Steps in this overlay.  All but 0x0200131c/0x02001448/0x02001760 return a
 * boolean in the low byte. */



    /* four-byte leaf, returns 1; result discarded here */








extern void Func_02002d64();
extern void Func_02002d7c();
extern void Func_02002d68();
extern void Func_02002d7e();
extern void Func_02002d8e();
extern void Func_02002d80();
extern void Func_02002d8a();
extern void Func_02002d94();
extern void Func_02002d9e();
extern s8 Func_020024ba();
extern s8 Func_02002538();
extern s8 Func_0200263e();
extern void Func_0200257c();
extern s32 Func_02002d6c();
extern s8 Func_020025d4();
extern s8 Func_02002514();
extern s8 Func_0200254a();
extern s8 Func_020025ce();
extern void Func_02002e18();
extern void Func_02002e32();
extern void Func_02002e28();
extern void Func_02002e38();
extern void Func_020029f2();
extern s32 Func_020025b4();
extern void Func_020026e4();
void Func_020011b8(void)
{
    s32 line;
    s32 retryFromStep134c;

    line = 0x2547;
    Func_02002d64(line);
    Func_02002d7c(12, 0);
    line += 1;

    Func_02002d68(1, 0, 0);
    Func_02002d7e(line);
    Func_02002d8e(1, 0);

    Func_02002d80(2, 0, 0);
    Func_02002d8a(3, 0, 0);
    Func_02002d94(13, 0, 0);
    Func_02002d9e(12, 0, 0);

    retryFromStep134c = 0;

top:                                                    /* 0x0200120c */
    if (Func_020024ba() == 0) {
        goto step12c4;
    }

step1320:                                               /* 0x02001216 */
    if (Func_02002538() == 0) {
        goto finishNegative;                            /* 0x02001290 */
    }

    /* 0x02001220 */
    retryFromStep134c = 0;
    if (Func_0200263e() == 0) {
setRetry:
        retryFromStep134c = 1;
        goto step134c;
    }

    /*
     * 0x0200123e.  The retry is a while loop whose test is reached directly
     * when the step above already succeeded; the other two predecessors jump
     * straight into the body, skipping the first test.
     */
    while (Func_020025d4() == 0 && retryFromStep134c != 0) {
step134c:                                               /* 0x0200122e */
        Func_0200257c();
        if (Func_02002d6c(0, 0) == 0) {
            goto finishNegative;
        }
    }
    goto finishAffirmative;

step12c4:                                               /* 0x0200124e */
    if (Func_02002514() != 0) {
        if (Func_0200254a() != 0) {
            goto finishAffirmative;
        }
        goto setRetry;                                  /* 0x0200122c */
    }

    /* 0x02001264 */
    if (Func_020025ce() != 0) {
        goto step1320;
    }

    /* 0x0200126e: repeat the prompt with the second line pair. */
    line = 0x254b;
    Func_02002e18(line);
    line += 1;
    Func_02002e32(2, 0);
    Func_02002e28(line);
    Func_02002e38(1, 0);
    goto top;

finishNegative:                                         /* 0x02001290 */
    Func_020029f2();
    return;

finishAffirmative:                                      /* 0x02001296 */
    Func_020025b4();
    Func_020026e4();
}
