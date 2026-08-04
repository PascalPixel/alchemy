#include "types.h"

/*
 * Resource 37f, owner at 0x020000ec (68 bytes of code + a seven-word literal
 * pool at 0x02000134-0x02000153).  `push {lr}` at 0x020000ec, interworking
 * return `pop {r1} / bx r1` at 0x0200012e: r1 is popped, so r0 survives and IS
 * the result — the owner returns the selected in-image data pointer.  It makes
 * no calls at all (`calls=0` in the inventory, confirmed by
 * `tools/overlay_call_targets.ts` reporting no sites in this span).
 *
 * Same selector shape as its neighbour 0x0200007c: `Data_02000240[224]` is the
 * scenario id and `[225]` the sub-step, and every returned pool word is an even
 * 0x0200axxx address, i.e. in-image data at file offsets 0x22d8, 0x22e4,
 * 0x232c, 0x241c, 0x2524 under this overlay's 0x02008000 link base.
 *
 * The `< 11` and `> 16` arms branch to one shared body returning
 * Data_0200a32c; it is written once.
 */

extern s16 Data_02000240[];
extern u8 Data_0200a2d8[];
extern u8 Data_0200a2e4[];
extern u8 Data_0200a32c[];
extern u8 Data_0200a41c[];
extern u8 Data_0200a524[];

u8 *Func_020000ec(void)
{
    s16 scenario = Data_02000240[224];
    s16 step;

    if (scenario == 0x13) {
        return Data_0200a2e4;
    }

    if (scenario == 0x10) {
        step = Data_02000240[225];
        if (step >= 11 && step <= 13) {
            return Data_0200a41c;
        }
        if (step >= 14 && step <= 16) {
            return Data_0200a524;
        }
        return Data_0200a32c;
    }

    return Data_0200a2d8;
}
