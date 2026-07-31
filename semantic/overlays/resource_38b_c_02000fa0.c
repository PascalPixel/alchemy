typedef unsigned char u8;
typedef signed int s32;

/*
 * Resource 38b collision-rectangle repaint at 0x02000fa0 (120 bytes, 5 call
 * sites over 2 distinct callees).
 *
 * Complete owner: `push {r5, lr} / sub sp, #8` at 0x02000fa0 and the matching
 * `add sp, #8 / pop {r5} / pop {r0} / bx r0` at 0x02001010.  The popped r0 is
 * the return address, so the owner is `void`.  The owner ends at 0x02001016,
 * immediately before the import veneer table at 0x02001018; it carries no
 * literal pool.
 *
 * Called from 0x02000d10.  This is the "repaint two collision rectangles"
 * member of the transition family: it repaints the two fixed cells (14, 6)
 * and (14, 9), choosing each cell's first argument — the passability or layer
 * selector — according to whether the scene actor is currently standing on
 * that row.
 *
 * `asrs r3, r3, #20` on the record's +0x10 word is the tile-grid idiom:
 * >> 16 to integer pixels, >> 4 to the 16-pixel grid, as one signed shift.
 *
 * The four Func_080091c0 sites are genuinely four separate `bl`s, one per
 * arm; they are written as four calls deliberately.  Two pairs are
 * behaviourally near-identical, but per HANDOVER section 0 identical
 * behaviour is not a licence to merge arms — the per-target multiset counts
 * call sites, not distinct behaviour.
 *
 * Both callees are import veneers resolved with
 * tools/overlay_call_targets.ts.
 */

/* Slot accessor; returns the record or NULL. */
extern u8 *Func_0808a080();

/* Six-argument renderer ABI: four register arguments plus tile x and tile z
 * on the stack. */
extern void Func_080091c0(s32, s32, s32, s32, s32, s32);

void Func_02000fa0(void)
{
    u8 *actor;
    s32 tile_z;

    actor = Func_0808a080(8);
    if (actor == 0) {
        return;
    }

    tile_z = *(s32 *)(actor + 0x10) >> 20;

    /* Cell (14, 6).  In the taken arm the stacked z is the register holding
     * tile_z, which the test has just proved equals 6, so both arms pass the
     * same coordinates and differ only in the first argument. */
    if (tile_z == 6) {
        Func_080091c0(2, 0, 1, 1, 14, 6);
    } else {
        Func_080091c0(0, 0, 1, 1, 14, 6);
    }

    /* The coordinate is reloaded from the record rather than reused, so a
     * side effect of the first repaint would be observed here. */
    tile_z = *(s32 *)(actor + 0x10) >> 20;

    /* Cell (14, 9), same shape. */
    if (tile_z == 9) {
        Func_080091c0(2, 0, 1, 1, 14, 9);
    } else {
        Func_080091c0(1, 0, 1, 1, 14, 9);
    }
}
