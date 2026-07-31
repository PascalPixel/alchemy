typedef unsigned short u16;
typedef int s32;

/*
 * resource_39c owner at 0x02002844, 108 bytes: brighten eight palette-RAM
 * color entries at 0x05000050 toward white (0x7fff), one shared 5-bit step
 * per frame, waiting between passes until every entry has reached full
 * white.
 *
 * Complete owner: `push {r5, r6, lr}` at 0x02002844 through `pop {r5, r6} /
 * pop {r0} / bx r0` at 0x20028a8-0x20028ac, followed by one alignment
 * halfword; the next owner's prologue is at 0x020028b0.  No incoming
 * arguments are read before being overwritten, so `void`.
 *
 * Not found by the structural inventory walk (unindexed): reached only by
 * `bl`, resolved with `bun tools/overlay_call_targets.ts`'s `+2` rule.
 *
 * The two pool words this owner needs (the 0x1f channel mask and the
 * 0x05000050 base pointer) are stored INLINE in the function body at
 * 0x02002850-0x02002857, skipped over by the `b.n 0x2002858` at 0x200284e,
 * rather than placed after the epilogue -- an inline-pool shape, not the
 * usual trailing-pool one.
 *
 * Each color entry is a packed GBA BGR555 halfword (5 bits per channel).
 * The outer loop reloads the base pointer and restarts the eight-entry pass
 * every time; the inner loop nudges every channel below 31 up by one step,
 * and counts (in a variable reset every outer pass) how many of the eight
 * entries are already all-31 (fully white).  The owner keeps repeating the
 * whole 8-entry pass -- one helper call per pass -- until all eight are
 * white in the same pass.
 *
 * Uncertainty: Func_0200821e is called once per pass with the constant
 * argument 2; given it is the only thing separating passes, it is almost
 * certainly a frame/vblank sync, but that is not established here.
 */

extern void Func_0200821e(s32 arg0);

void Func_02002844(void)
{
    u16 *palette;
    s32 whiteCount;
    s32 index;
    u16 color;
    s32 red, green, blue;

    do {
        palette = (u16 *)0x05000050;
        whiteCount = 0;

        for (index = 0; index <= 7; index++) {
            color = *palette;
            red = color & 0x1f;
            green = (color >> 5) & 0x1f;
            blue = (color >> 10) & 0x1f;

            if (red == 31 && green == 31 && blue == 31) {
                whiteCount++;
            } else {
                if (red <= 30) red++;
                if (green <= 30) green++;
                if (blue <= 30) blue++;
                *palette = (u16)((blue << 10) | (green << 5) | red);
            }

            palette++;
        }

        Func_0200821e(2);
    } while (whiteCount <= 7);
}
