#include "types.h"

/*
 * resource_399 scene entry at 0x020006c0, 228 bytes.
 *
 * Complete owner: `push {r5, r6, r7, lr}` at 0x020006c0 and the interworking
 * return `pop {r5, r6, r7} / pop {r0} / bx r0` at 0x02000788.  The popped
 * register is r0, so it holds the return address — the owner is `void`.
 * Bytes 0x0200078e-0x020007a3 are alignment plus the five-word literal pool.
 *
 * Call targets resolved with `bun tools/overlay_call_targets.ts resource_399
 * 06c0 --annotate` (an overlay `bl` stores `target - 2`).  15 sites, 13
 * distinct imports; per-target multiset, which the C below matches exactly:
 *   Func_0808a018 x1, Func_0808a080 x2, Func_080f9010 x2, Func_08009178 x1,
 *   Func_0808a090 x2, Func_0808a100 x1, Func_0808a1e0 x1, Func_0808a0e0 x1,
 *   Func_0808a0d8 x1, Func_0808a010 x1, Func_0808a248 x1, Func_0808a020 x1.
 *
 * What it does:
 *  1. Clears the byte at +85 of every live scene record for selectors 8..65
 *     (`cmp r5,#65 ; bls` — the bound is inclusive and the comparison is
 *     unsigned).  Func_0808a080 returns 0 for an unoccupied selector, which is
 *     the documented "the occupying slot or 0" contract, so the null test is
 *     part of the loop rather than an error path.
 *  2. Reads the u16 at workspace + 364, subtracts 50 and sign-extends the
 *     result to s16 — the scene variant, 1..6 as the tables below are indexed.
 *  3. Announces the variant with Func_080f9010, 188 for variant 6 and 158
 *     otherwise.
 *  4. Places the camera with Func_08009178(target, x, z), where the arguments
 *     come from two in-image tables indexed by `variant - 1`: a word table at
 *     file offset 0x3190 and an s16 pair table at 0x31a8.  Both pool words are
 *     even and in the 0x0200bxxx band, so under the proven 0x02008000 link
 *     base they are in-image data, not callbacks.  The word table holds
 *     POINTERS: the sibling owner at 0x02000384 calls the same import with the
 *     literal in-image addresses 0x0200b1c0 and 0x0200b1d6, i.e. objects just
 *     past the end of the halfword table at 0x31a8, which is what the words at
 *     0x3190 must be selecting between.
 *  5. Func_0808a090(0, 0x8000, 0x4000) — the 16.16 pair setter on channel 0.
 *  6. Stores 32 into the workspace word at +448.  NOTE the displacement/value
 *     trap: `movs r2,#224 ; lsls #1` builds 448, which is added to the
 *     workspace pointer, and `subs r2,#192` then reuses the SAME register as
 *     the stored value 32.  Reading it as workspace + 256 is the natural
 *     mistake.
 *  7. Variant 6 gets a scripted pose (the 0x3333/0x1999 16.16 pair, then
 *     Func_0808a100(0, 2), Func_0808a1e0(0, 3), Func_0808a0e0(0, 0, -8));
 *     every other variant clears +85 on channel 0's own record and calls
 *     Func_0808a0d8(0, 3, -16) instead.
 *  8. Waits 16 frames and hands the variant to Func_0808a248.
 *
 * Uncertainty: the extent of the two tables is not established from this owner
 * alone; nothing here bounds the variant, so both are declared open.  Six word
 * entries fit exactly between 0x3190 and 0x31a8, and six halfword pairs
 * between 0x31a8 and the first pointed-to object at 0x31c0, which is
 * consistent with the `variant == 6` test being the top of the range — but it
 * is consistency, not proof.
 */

extern u8 **Data_03001ebc;      /* pointer cell holding the scene workspace */
extern u8 *Data_0200b190[];     /* in-image, file offset 0x3190: camera target pointer per variant */
extern s16 Data_0200b1a8[];     /* in-image, file offset 0x31a8: (x, z) pair per variant */

/* Imports, named by the main-image address in the trailing word of the overlay
 * veneer each call site reaches.  Old-style declarations are mandatory. */
void Func_0808a010();           /* frame wait */
void Func_0808a018();           /* scripted-section open */
void Func_0808a020();           /* scripted-section close */
void Func_0808a090();           /* 16.16 pair setter */
void Func_0808a0d8();
void Func_0808a0e0();           /* (selector, dx, dz) nudge */
void Func_0808a100();
void Func_0808a1e0();
void Func_0808a248();
void Func_08009178();           /* camera placement */
void Func_080f9010();
u8 *Func_0808a080();            /* scene record for a selector, or 0 */

void Func_020006c0(void)
{
    u8 *workspace;
    u8 *record;
    s32 selector;
    s16 variant;

    workspace = *Data_03001ebc;

    Func_0808a018();

    for (selector = 8; selector <= 65; selector++) {
        record = Func_0808a080(selector);
        if (record != 0) {
            record[85] = 0;
        }
    }

    variant = (s16)(*(u16 *)(workspace + 364) - 50);

    if (variant == 6) {
        Func_080f9010(188);
    } else {
        Func_080f9010(158);
    }

                  Data_0200b1a8[(variant - 1) * 2],
    Func_08009178(Data_0200b190[variant - 1],
                  Data_0200b1a8[(variant - 1) * 2 + 1]);

    Func_0808a090(0, 0x8000, 0x4000);

    *(s32 *)(*Data_03001ebc + 448) = 32;

    if (variant == 6) {
        Func_0808a090(0, 0x3333, 0x1999);
        Func_0808a100(0, 2);
        Func_0808a1e0(0, 3);
        Func_0808a0e0(0, 0, -8);
    } else {
        record = Func_0808a080(0);
        record[85] = 0;
        Func_0808a0d8(0, 3, -16);
    }

    Func_0808a010(16);
    Func_0808a248(variant);
    Func_0808a020();
}
