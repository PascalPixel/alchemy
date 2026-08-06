#include "types.h"

/*
 * resource_399 scripted step at 0x0200088c, 232 bytes.
 *
 * Complete owner: `push {r5, lr}` at 0x0200088c and the interworking return
 * `pop {r5} / pop {r0} / bx r0` at 0x02000954.  The popped register is r0, so
 * it holds the return address — the owner is `void`.  Bytes
 * 0x0200095a-0x02000973 are alignment plus the six-word literal pool.
 *
 * Call targets resolved with `bun tools/overlay_call_targets.ts resource_399
 * 088c --annotate` (an overlay `bl` stores `target - 2`).  21 sites, 14
 * distinct imports; per-target multiset, matched by the C below:
 *   Func_0808a080 x1, Func_080770c0 x1, Func_080b0008 x1, Func_0808a018 x2,
 *   Func_0808a148 x1, Func_0808a010 x2, Func_0808a170 x2, Func_0808a180 x2,
 *   Func_0808a1b8 x1, Func_0808a020 x2, Func_0808a208 x1, Func_0808a210 x2,
 *   Func_0808a218 x2, Func_0808a0b0 x1.
 *
 * This owner is a TWIN of 0x020007a4: the two are the same 232-byte routine
 * differing only in the channel numbers (12/10 against 13/11) and three
 * message ids (0x164b/0x153e against 0x164d/0x1543).  The pool word
 * 0x0200a5ec, the flag id 0x881 and every coordinate constant are shared,
 * and `overlay_call_targets.ts` reports the same 21 sites over the same 14
 * targets for both, which is the cheapest available correctness proof for
 * the second file.
 *
 * Same shape as the owner at 0x02000974: read the heading halfword at +6 of
 * the channel-0 scene record, then in each arm of the Func_080770c0(0x881)
 * story flag apply the biased UNSIGNED range test
 * `(u32)((heading << 16) + 0x5fff0000) <= 0x3ffe0000` — the actor facing
 * within one quadrant of a reference direction.  The test is assembled once
 * per flag arm; hoisting it into a local does not change the call multiset.
 *
 * Note the asymmetry in the flag-clear arm: when the heading test FAILS there,
 * the `bhi` goes straight to the shared epilogue and the owner does nothing at
 * all, so that arm has no else branch.
 *
 * Constants are built by shifts, as this compiler always spells 16.16 whole
 * numbers: `movs #192 ; lsls #11` = 0x60000 (6.0), `movs #192 ; lsls #8` =
 * 0xc000 (0.75), `movs #213 ; lsls #17` = 0x1aa0000 (426.0),
 * `movs #246 ; lsls #17` = 0x1ec0000 (492.0), `movs #154 ; lsls #18` =
 * 0x2680000 (616.0), and `movs #128 ; lsls #7` = 0x4000, a quarter turn in the
 * 16-bit angle unit.
 *
 * The pool word 0x0200a5ec passed to Func_0808a0b0 is EVEN and in the
 * 0x0200axxx band, so under the proven 0x02008000 link base it is in-image
 * data at file offset 0x25ec — an animation script, which is the established
 * role of the second argument of Func_0808a0b0.
 *
 * Uncertainty: Func_0808a210's four arguments read as a camera target with -1
 * meaning "leave this axis alone" (the same -1 placeholder the converted
 * resource_3aa uses at its Func_0808a210 sites), but that reading is inferred
 * from the argument shape rather than established from the import itself.
 */

extern u8 Data_0200a5ec[];      /* in-image animation script, file offset 0x25ec */

/* Imports, named by the main-image address in the trailing word of the overlay
 * veneer each call site reaches.  Old-style declarations are mandatory. */
           /* frame wait */
           /* scripted-section open */
           /* scripted-section close */
           /* (selector, script) */
           /* scene request */
           /* message line */

           /* orientation over frames */

           /* camera target */


            /* scene record for a selector, or 0 */
            /* story-flag query */

extern u8 * Func_02002ada();
extern s32 Func_02002aac();
extern void Func_02002c84();
extern void Func_02002aea();
extern void Func_02002ba4();
extern void Func_02002af2();
extern void Func_02002bd0();
extern void Func_02002be0();
extern void Func_02002c04();
extern void Func_02002b20();
extern void Func_02002b2a();
extern void Func_02002c46();
extern void Func_02002c60();
extern void Func_02002c6c();
extern void Func_02002b4a();
extern void Func_02002b9a();
extern void Func_02002c30();
extern void Func_02002c40();
extern void Func_02002c92();
extern void Func_02002c9e();
extern void Func_02002b8a();
void Func_0200088c(void)
{
    s16 heading;
    s32 facing_near;

    heading = *(s16 *)(Func_02002ada(0) + 6);
    facing_near = ((u32)(((s32)heading << 16) + 0x5fff0000) <= 0x3ffe0000);

    if (Func_02002aac(0x881) != 0) {
        if (facing_near) {
            Func_02002c84(11, 13);
        } else {
            Func_02002aea();
            Func_02002ba4(13, 0, 0);
            Func_02002af2(10);
            Func_02002bd0(0x164d);
            Func_02002be0(13, 0);
            Func_02002c04(13, 0x4000, 10);
            Func_02002b20();
        }
    } else if (facing_near) {
        Func_02002b2a();
        Func_02002c46(0x60000, 0xc000);
        Func_02002c60(0x1aa0000, -1, 0x1ec0000, 1);
        Func_02002c6c();
        Func_02002b4a(20);
        Func_02002b9a(13, Data_0200a5ec);
        Func_02002c30(0x1543);
        Func_02002c40(13, 0);
        Func_02002c92(0x1aa0000, -1, 0x2680000, 1);
        Func_02002c9e();
        Func_02002b8a();
    }
}
