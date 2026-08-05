#include "types.h"

/*
 * Resource 38b collision-map repaint at 0x02000cb4 (92 bytes, 2 distinct
 * callees over 3 call sites).
 *
 * Complete owner: prologue `push {r5, r6, r7, lr} / mov r7, r8 / push {r7} /
 * sub sp, #8` at 0x02000cb4, matching epilogue `add sp, #8 / pop {r3} /
 * mov r8, r3 / pop {r5, r6, r7} / pop {r0} / bx r0` at 0x02000cfe.  By the
 * epilogue rule the popped r0 is the return address, so the owner is `void`.
 * 0x02000d0a is alignment and 0x02000d0c-0x02000d0f is the single literal
 * pool; neither is code.
 *
 * Link base 0x02008000 (see resource_38b_c_02000240.c for the witnesses), so
 * the pool word 0x02009d28 is in-image data at file offset 0x1d28, not a RAM
 * global.  It is read as ten consecutive (x, z) byte pairs.
 *
 * `asrs r3, r3, #20` on the actor record's +0x08 and +0x10 words is the tile
 * grid idiom from HANDOVER section 0: >> 16 takes the 16.16 fixed-point
 * coordinate to integer pixels, and a further >> 4 converts to the 16-pixel
 * tile grid.  It is a single arithmetic shift, and it is signed.
 *
 * Call symbols are per-site, not per-import: the raw region (assets/code/
 * resource_38b_overlay.s around line 1332) spells the three calls as
 * `bl sub_02001d90`, `bl sub_02001d5c`, `bl sub_02001d74` -- three distinct
 * local veneers, even though the latter two both eventually reach the same
 * main-image renderer.  tools/overlay_call_targets.ts's main-image
 * resolution (0x0808a080 / 0x080091c0) is correct for what the veneers
 * *jump to*, but calling those main-image addresses directly from C bypasses
 * the veneers and produces a different (wrong) pc-relative displacement at
 * each site.  Use the exact local sub_ symbols instead, per the craft rule.
 */

/* Ten (x, z) tile pairs, in the overlay's own writable image at file offset
 * 0x1d28.  Overlay data is EWRAM and is not const. */
extern u8 Data_02009d28[];

/* Slot accessor veneer at sub_02001d90: Func_02001d90(slot) returns the
 * actor record, or NULL.  Typed as a byte pointer so the +0x08 / +0x10 field
 * reads are explicit. */
extern u8 *Func_02001d90();

/* The established six-argument renderer ABI: four register arguments plus two
 * stack words, here the tile x and tile z of the cell being repainted.  Two
 * distinct per-site veneers (sub_02001d5c, sub_02001d74) both eventually
 * reach the same main-image renderer at 0x080091c0. */
extern void Func_02001d5c(s32, s32, s32, s32, s32, s32);
extern void Func_02001d74(s32, s32, s32, s32, s32, s32);

void Func_02000cb4(void)
{
    u8 *actor;
    s32 tile_x;
    s32 tile_z;
    u32 i;

    /* Slot 8 is the scene's own actor; this owner does not null-check the
     * result, unlike 0x02000328 which does.  Noted, not "fixed". */
    actor = Func_02001d90(8);
    tile_x = *(s32 *)(actor + 0x08) >> 20;
    tile_z = *(s32 *)(actor + 0x10) >> 20;

    /* Ten fixed cells from the table, then the actor's own cell.  The loop
     * counter is bumped before the call and tested against 19 after, so it
     * runs for i = 2, 4, ... 20 — ten iterations.  The machine walks the
     * table with the byte index itself (not a separate 0..9 counter scaled
     * by 2), so the C loop steps the byte offset directly. */
    for (i = 0; i < 20; i += 2) {
        s32 x = (s32)Data_02009d28[i];
        s32 z = (s32)Data_02009d28[i + 1];
        Func_02001d5c(1, 0, 1, 1, x, z);
    }

    /* Same repaint with r0 = 0 rather than 1 — a different layer or kind
     * selector; the exact meaning of that first argument is not established
     * here. */
    Func_02001d74(0, 0, 1, 1, tile_x, tile_z);
}
