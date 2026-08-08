/*
 * resource_3b7 owner at 0x02000104, 80 bytes (0x02000104-0x02000153).
 *
 * A one-shot scene entry.  Code runs 0x02000104-0x0200014f; the last word,
 * 0x02000150, is a literal pool holding 0x02000240 and is reached only by the
 * `ldr r3, [pc, #72]` at 0x02000106, past the `bx r0` return.  It is data.
 *
 * That pool word is the base of `Data_02000240`, the same overlay workspace the
 * byte-exact siblings `assets/code/resource_3b7_c_02000044.c` and
 * `assets/code/resource_3b7_c_020001a8.c` already name, so the layout is taken
 * from tracked material rather than rederived.  The value is used raw (it is not
 * in the 0x0200_8xxx..0200_bxxx in-image band, so no link-base shift applies).
 * The prologue computes `250 << 1` = 500 and loads the word at
 * `Data_02000240 + 500`, i.e. the s16 array's element 250 read as a 32-bit
 * value; the exact siblings index the same base as `s16`, so the byte offset is
 * spelled explicitly here rather than as an s16 index.
 *
 * Call targets were resolved with `cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml --` (an overlay
 * `bl` stores `target_offset - 2`, so the printed targets are wrong).  In
 * program order:
 *   0x02000114 -> veneer 0x020018e4 -> Func_080770c0
 *   0x02000120 -> veneer 0x020018ec -> Func_080770c8
 *   0x02000124 -> prologue 0x02000880 -> Func_02000880
 *   0x02000128 -> veneer 0x0200191c -> Func_0808a018
 *   0x02000132 -> veneer 0x02001944 -> Func_0808a0d0
 *   0x0200013e -> veneer 0x0200197c -> Func_0808a1b8
 *   0x02000142 -> prologue 0x02000ac8 -> Func_02000ac8
 *   0x02000146 -> veneer 0x02001924 -> Func_0808a020
 * Completeness: 8 sites over 8 distinct targets, matching the tool's
 * `sites=8 distinct_targets=8` and the inventory row's calls=8.
 *
 * Func_080770c0 / Func_080770c8 are the tested-and-set event flag pair (the
 * same 0x200-style flag idiom as semantic/overlays/resource_3b8_c_02000108.c);
 * Func_0808a018 / Func_0808a020 are the established cutscene-script bracket,
 * Func_0808a0d0 places a slot and Func_0808a1b8 moves it (see
 * semantic/overlays/resource_373_c_02002f14.c).
 *
 * The epilogue is `pop {r5} / pop {r0} / bx r0`, so r0 holds the popped return
 * address and the owner returns nothing: void.  r0 is overwritten before any
 * read, so the owner takes no argument.
 *
 * Uncertainties: the imports' real interfaces are unknown, hence the old-style
 * declarations; Func_080770c0 is declared s32 because its result is tested.
 * Whether the flag test's set arm is meant to fall through into the common tail
 * is not in doubt (the `bne` skips exactly the two-call set block), but the
 * meaning of flag 512 is not established here.
 */

#include "types.h"

extern s16 Data_02000240[];

s32 Func_020019fa();
void Func_02001a0e();
void Func_020009a6();
void Func_02001a46();
void Func_02001a78();
void Func_02001abc();
void Func_02000c0c();
void Func_02001a6c();

                     

                     

void Func_02000104(void)
{
    /* Loaded before the first call; r5 is preserved across all of them. */
    s32 slot = *(s32 *)((unsigned char *)Data_02000240 + 500);

    if (Func_020019fa(512) == 0) {
        Func_02001a0e(512);
        Func_020009a6();
    }

    Func_02001a46();
    Func_02001a78(slot, 0x78, 0x98);
    Func_02001abc(slot, 0x4000, 0);     /* 128 << 7 */
    Func_02000c0c();
    Func_02001a6c();
}
