#include "types.h"

/*
 * Resource 3ad, dialogue step at 0x02001394 (136 bytes, 13 call sites).
 *
 * Complete owner: `push {r5, lr}` at 0x02001394, `pop {r5} / pop {r1} / bx r1`
 * at 0x02001412.  The popped branch register is r1, not r0, so r0 survives the
 * epilogue and IS the result - this owner returns s32.  One pool word (0x255e,
 * the first dialogue line id) sits at 0x02001418, past the return.
 *
 * Call targets resolved with `tools/lib/overlay_call_targets.ts`; every one is an
 * import veneer:
 *   0x0200139c -> 0x02001b94 -> Func_0808a148
 *   0x020013a8 -> 0x02001bcc -> Func_0808a1e8
 *   0x020013b0 -> 0x02001ba4 -> Func_0808a170
 *   0x020013b8 -> 0x02001bb4 -> Func_0808a180
 *   0x020013c4 -> 0x02001bbc -> Func_0808a1b8
 *   0x020013ca -> 0x02001b1c -> Func_0808a010
 *   0x020013d2 -> 0x02001b7c -> Func_0808a110
 *   0x020013d8 -> 0x02001ba4 -> Func_0808a170
 *   0x020013e0 -> 0x02001bb4 -> Func_0808a180
 *   0x020013ee -> 0x02001bcc -> Func_0808a1e8
 *   0x020013f4 -> 0x02001ba4 -> Func_0808a170
 *   0x020013fc -> 0x02001bac -> Func_0808a178
 *   0x02001404 -> 0x02001b34 -> Func_0808a070
 * Per-target multiset: Func_0808a170 x3, Func_0808a1e8 x2, Func_0808a180 x2,
 * and one each of Func_0808a148, Func_0808a1b8, Func_0808a010, Func_0808a110,
 * Func_0808a178, Func_0808a070 - 13 sites against 13 C call expressions,
 * matching the row's calls=13.
 *
 * The dialogue ids are consecutive from the pooled base: 0x255e, 0x255f, and
 * 0x2560 after `adds r5, #2`.  The byte-exact sibling
 * `assets/code/resource_3ad_c_0200141c.c` uses the neighbouring id 0x255c in
 * the same shape, which corroborates the reading.
 */

/* Old-style declarations: overlay import arities vary per call site. */
void Func_0808a010();   /* wait n frames */
s32 Func_0808a070();    /* dialogue prompt; result selects the branch */
void Func_0808a110();   /* scene-presentation request */
void Func_0808a148();   /* scene request */
void Func_0808a170();   /* show a dialogue line by id */
void Func_0808a178();   /* dialogue-line variant with a mode word */
void Func_0808a180();   /* wait for the slot's action to finish */
void Func_0808a1b8();   /* face the slot along a heading */
void Func_0808a1e8();   /* play animation id for n frames */

s32 Func_02001394(void)
{
    s32 line = 0x255e;

    Func_0808a148(2, 0, 0);
    /* 0x102 built as `movs r1,#129 / lsls r1,#1`. */
    Func_0808a1e8(2, 0x102, 60);
    Func_0808a170(line);
    Func_0808a180(2, 0);

    /* 0x3000 built as `movs r1,#192 / lsls r1,#6`. */
    Func_0808a1b8(12, 0x3000, 0);
    Func_0808a010(30);
    Func_0808a110(12, 4);
    Func_0808a170(line + 1);
    Func_0808a180(12, 0);

    line += 2;
    Func_0808a1e8(3, 0x102, 60);
    Func_0808a170(line);
    Func_0808a178(3, 0);

    /*
     * `negs r3,r0 / orrs r3,r0 / lsrs r3,#31 / movs r0,#1 / subs r0,r0,r3` is
     * the compiler's `== 0` idiom: bit 31 of (-x | x) is set for every non-zero
     * x, so the result is 1 exactly when the prompt returned 0.
     */
    return Func_0808a070(0, 0) == 0;
}
