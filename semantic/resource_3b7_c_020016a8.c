/*
 * resource_3b7 owner at 0x020016a8, 148 bytes (0x020016a8-0x0200173b).
 *
 * Scene initialiser: lays out four actor records from three parallel in-image
 * tables, resets a camera/state block, retags two slot records, and installs
 * the per-frame task at Func_02000e5c.
 *
 * Code runs 0x020016a8-0x0200171b.  The trailing 32 bytes,
 * 0x0200171c-0x0200173b, are a literal pool of eight words - 0x0200a070,
 * 0x0200a05a, 0x0200a0d0, 0x0200a062, 0x0200a05e, 0xffe20000, 0x00000c83,
 * 0x02008e5d.  They lie past the `bx r0` return and are read only by the
 * `ldr rN, [pc, ...]` loads, so they are data.  (The disassembler renders
 * 0x02001732 as a NEON `vmull.u32`, which is exactly the trap the pool guard
 * exists for.)
 *
 * LINK BASE - this owner supplies the decisive witness.  The pool word
 * 0x02008e5d is odd, i.e. a Thumb entry, and 0x02008e5d - 1 - 0x8000 = 0xe5c,
 * which is a real prologue in this overlay (the 2,124-byte row at
 * 0x02000e5c).  It is handed to Func_080000d0, the established task installer,
 * so it is a callback pointer.  That, together with 0x02009fc0 resolving to the
 * eight BLDALPHA halfwords Func_02000880 consumes and 0x0200a018 resolving to
 * the fifteen-word table Func_02000d70 indexes, fixes the overlay's link base
 * at 0x02008000 on three independent witnesses.
 *
 * Under that base the image occupies 0x02008000-0x0200a069 (decoded size
 * 0x206a).  So the five remaining pool addresses split cleanly:
 *   0x0200a05a, 0x0200a05e, 0x0200a062  -> file offsets 0x205a/0x205e/0x2062,
 *      inside the image, and `assets/code/resource_3b7_overlay.s` spells the
 *      bytes there literally: X tile coords 0x50, 0xa0, 0x50, 0x48; Z tile
 *      coords 0x20, 0x68, 0x44, 0x48; headings 0x0000, 0x0001, 0x0000, 0x8000
 *      - four entries each, which is exactly the loop trip count.
 *   0x0200a070, 0x0200a0d0 -> past the image end, i.e. the overlay's scratch
 *      EWRAM immediately above it.  These are written, never read here.
 *
 * Call targets were resolved with `cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml --` (an overlay
 * `bl` stores `target_offset - 2`).  In program order:
 *   0x020016f8 -> veneer 0x0200193c -> Func_0808a080
 *   0x020016fe -> veneer 0x0200188c -> Func_08009080
 *   0x02001704 -> veneer 0x0200193c -> Func_0808a080
 *   0x0200170a -> veneer 0x0200188c -> Func_08009080
 *   0x02001712 -> veneer 0x0200186c -> Func_080000d0
 * Completeness: 5 sites over 3 distinct targets, matching the tool's
 * `sites=5 distinct_targets=3` and the inventory row's calls=5.
 *
 * The epilogue is `pop {r5, r6, r7} / pop {r0} / bx r0`, so r0 holds the popped
 * return address and the owner returns nothing: void.  r0 is written before any
 * read, so it takes no argument.
 *
 * Uncertainties:
 *  - r4 is written without being saved, although the prologue pushes only
 *    {r5, r6, r7, lr}.  It holds the Z-coordinate table cursor across the loop
 *    and no call happens inside the loop, so nothing observable depends on it.
 *    This is the same unsaved-r4 idiom already recorded for resource_371 and
 *    resource_372; it is noted rather than "fixed".
 *  - r0 is not reloaded between each Func_0808a080 and the Func_08009080 that
 *    follows it, so the looked-up record is that call's first argument.  The
 *    record is not null-tested here, unlike at Func_02000dd0.
 *  - The field names below are descriptive only.  The `<< 16` promotion of a
 *    byte coordinate to 16.16 and the +0/+4/+8 word triple match the position
 *    layout established in semantic/overlays/resource_373_c_02002f14.c, but the
 *    24-byte stride and the four cleared halfwords at +14..+20 are read off the
 *    stores alone.
 *  - The second heading value is 0x0001 rather than a multiple of 0x4000; the
 *    bytes are not in doubt (0x2064 is `01 00`), only the meaning.
 */

#include "types.h"

extern u8 Data_0200a05a[];      /* in-image 0x205a: four X tile coordinates */
extern u8 Data_0200a05e[];      /* in-image 0x205e: four Z tile coordinates */
extern u16 Data_0200a062[];     /* in-image 0x2062: four headings */
extern u8 Data_0200a070[];      /* scratch EWRAM above the image */
extern u8 Data_0200a0d0[];      /* scratch EWRAM above the image: 4 x 24 bytes */

u8 *Func_02003036();
void Func_02002f8c();
u8 *Func_02003042();
void Func_02002f98();
void Func_02002f80();

                     
void Func_02000e5c();           /* the installed per-frame task */

void Func_020016a8(void)
{
    u8 *state = Data_0200a070;
    u8 *z_table = Data_0200a05e;
    u8 *x_table = Data_0200a05a;
    u16 *heading_table = Data_0200a062;
    u8 *record = Data_0200a0d0;
    s32 index = 0;

    do {
        *(s32 *)(record + 0) = (s32)*x_table << 16;
        *(s32 *)(record + 8) = (s32)*z_table << 16;
        *(s32 *)(record + 4) = 0;
        *(u16 *)(record + 12) = *heading_table;
        *(u16 *)(record + 14) = 0;
        *(u16 *)(record + 16) = 0;
        *(u16 *)(record + 18) = 0;
        *(u16 *)(record + 20) = 0;

        index++;
        x_table++;
        z_table++;
        heading_table++;
        record += 24;
    } while (index != 4);

    *(s32 *)(state + 4) = (s32)0xffe20000;      /* -30.0 in 16.16 */
    *(s32 *)(state + 8) = 0;
    *(s32 *)(state + 12) = 0x640000;            /* 200 << 15, i.e. 100.0 */
    *(s32 *)(state + 64) = 0;
    *(s32 *)(state + 68) = 0;
    *(s32 *)(state + 72) = 0;
    *(s32 *)(state + 76) = 0;

    /* r0 carries each lookup's result straight into the retag call. */
    Func_02002f8c(Func_02003036(20), 2);
    Func_02002f98(Func_02003042(21), 2);

    Func_02002f80(Func_02000e5c, 0xc83);
}
