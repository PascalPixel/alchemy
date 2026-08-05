#include "types.h"

/*
 * resource_39c owner at 0x02002e08, 100 bytes: fade seven palette-RAM color
 * entries at 0x050000c2 toward black (0), one shared 5-bit step per frame,
 * waiting between passes until every entry has reached full black.  The
 * twin of the brighten-to-white owner at 0x02002844
 * (resource_39c_c_02002844.c) -- same packed-BGR555 loop shape, opposite
 * direction, a different target range and helper.
 *
 * Complete owner: `push {r5, r6, lr}` at 0x02002e08 through `pop {r5, r6} /
 * pop {r0} / bx r0` at 0x2002e66-0x2002e6a; the next owner's prologue is at
 * 0x02002e6c.  No incoming arguments are read before being overwritten, so
 * `void`.
 *
 * Not found by the structural inventory walk (unindexed): reached only by
 * `bl`, resolved with `bun tools/overlay_call_targets.ts`'s `+2` rule.
 *
 * As with the white twin, the mask and base-address pool words are stored
 * INLINE in the function body (0x02002e34/0x02002e38) and skipped by a
 * forward branch rather than placed after the epilogue.
 *
 * Differences from the white twin, all confirmed against the bytes: seven
 * entries, not eight (the inner-loop bound is `index <= 6`); the completion
 * test re-reads the just-stored packed halfword and checks it against 0,
 * not the three unpacked channels against 31 (equivalent here, since every
 * channel clamps independently at 0); the store is unconditional every
 * iteration, not skipped when already black (harmless -- storing 0 over 0
 * is a no-op); and the outer-loop exit is `blackCount != 7`, not
 * `whiteCount <= 7` -- behaviourally the same shape since blackCount only
 * ever counts 0..7 in a single pass.
 *
 * Uncertainty: Func_080000c0 is called once per pass with the constant
 * argument 5 (the white twin's helper takes 2); presumed to be the same
 * kind of frame/vblank sync, not established here.
 */

extern void Func_080000c0(s32 arg0);

void Func_02002e08(void)
{
    u16 *palette;
    s32 blackCount;
    u32 index;
    u16 color;
    s32 red, green, blue;

    do {
        palette = (u16 *)0x050000c2;
        blackCount = 0;

        for (index = 0; index <= 6; index++) {
            color = *palette;
            red = color & 0x1f;
            green = (color >> 5) & 0x1f;
            blue = (color >> 10) & 0x1f;

            if (red > 0) red--;
            if (green > 0) green--;
            if (blue > 0) blue--;

            *palette = (u16)((blue << 10) | (green << 5) | red);

            if (*palette == 0) {
                blackCount++;
            }

            palette++;
        }

        Func_080000c0(5);
    } while (blackCount != 7);
}
