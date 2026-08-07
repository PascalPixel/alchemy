#include "types.h"

/*
 * Resource 37f, owner at 0x02001ac8 (76 bytes of code + a two-word literal
 * pool at 0x02001b1c-0x02001b23).  `push {r5, lr}` at 0x02001ac8, interworking
 * return `pop {r5} / pop {r0} / bx r0` at 0x02001b14 — r0 is the popped return
 * address, so the owner is `void`.
 *
 * All four call sites were resolved with `tools/lib/overlay_call_targets.ts`; all
 * four are import veneers.
 *
 * Link base: this overlay is linked at 0x02008000 (witness — the byte-exact
 * `assets/code/resource_37f_c_02000030.c` returns `Data_02009cd4`,
 * `Data_02009d04` and `Data_02009d64`, which land at file offsets 0x1cd4 /
 * 0x1d04 / 0x1d64, immediately past the import veneer band that ends at
 * 0x02001ccc).  So the pool word 0x0200a69c is in-image data at offset
 * 0x269c, and it is written here, which is consistent with the overlay image
 * being writable EWRAM rather than ROM.  It is an idle-timer countdown.
 *
 * Uncertainty: Func_080000f8 is treated as a 0..0xffff generator; the
 * expression `(u32)(value * 120) >> 16` is the standard scale-to-range idiom
 * and is spelled here exactly as the shifts compute it
 * (`((v << 4) - v) << 3`, then a logical right shift by 16).
 */

extern s32 Data_0200a69c;

void Func_080091f0();
s32 Func_080000f8();
void Func_080f9010();

void Func_02001ac8(void)
{
    s32 timer;
    u32 roll;

    timer = Data_0200a69c;
    if (timer != 0) {
        timer--;
        Data_0200a69c = timer;
        if (timer == 40) {
            Func_080091f0(-1, -1, 0xe666);
        }
        return;
    }

    roll = (u32)Func_080000f8();
    if (((roll * 120) >> 16) != 0) {
        return;
    }

    Func_080f9010(0x8a);
    Func_080091f0(0x10000, 0x20000, 0x10000);
    Data_0200a69c = 80;
}
