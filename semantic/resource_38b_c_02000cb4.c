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
 * Call targets resolved with tools/overlay_call_targets.ts; both callees are
 * import veneers, so the names are real main-image addresses.
 */

/* Ten (x, z) tile pairs, in the overlay's own writable image at file offset
 * 0x1d28.  Overlay data is EWRAM and is not const. */
extern u8 Data_02009d28[];

/* Slot accessor: Func_0808a080(slot) returns the actor record, or NULL.
 * Typed as a byte pointer so the +0x08 / +0x10 field reads are explicit. */
extern u8 *Func_0808a080();

/* The established six-argument renderer ABI: four register arguments plus two
 * stack words, here the tile x and tile z of the cell being repainted. */
extern void Func_080091c0(s32, s32, s32, s32, s32, s32);

void Func_02000cb4(void)
{
    u8 *actor;
    s32 tile_x;
    s32 tile_z;
    s32 i;

    /* Slot 8 is the scene's own actor; this owner does not null-check the
     * result, unlike 0x02000328 which does.  Noted, not "fixed". */
    actor = Func_0808a080(8);
    tile_x = *(s32 *)(actor + 0x08) >> 20;
    tile_z = *(s32 *)(actor + 0x10) >> 20;

    /* Ten fixed cells from the table, then the actor's own cell.  The loop
     * counter is bumped before the call and tested against 19 after, so it
     * runs for i = 2, 4, ... 20 — ten iterations. */
    for (i = 0; i < 10; i++) {
        Func_080091c0(1, 0, 1, 1,
                      (s32)Data_02009d28[i * 2],
                      (s32)Data_02009d28[i * 2 + 1]);
    }

    /* Same repaint with r0 = 0 rather than 1 — a different layer or kind
     * selector; the exact meaning of that first argument is not established
     * here. */
    Func_080091c0(0, 0, 1, 1, tile_x, tile_z);
}
