#include "types.h"

/*
 * Resource 3b1 owner at 0x0200054c (292 bytes).
 *
 * Complete owner: `push {lr}` at 0x0200054c and `pop {r1} ; bx r1` at
 * 0x0200062e -- r1, so r0 survives and IS the result.  0x0200054c + 292 =
 * 0x02000670, where the next owner's `push {lr}` begins.  The span
 * decomposes as dispatcher (0x0200054c-0x02000567), 23-entry jump table
 * (0x02000568-0x020005c3, data), six case bodies including the default
 * (0x020005c4-0x02000631), a two-byte alignment halfword at 0x02000632 and
 * the literal pool (0x02000634-0x0200066f).
 *
 * Fourth of the nine dispatch sites the tree described nowhere; adopted with
 * `--span 292`.
 *
 * Selector `Data_02000240[225]` again, but `subs #1 ; cmp #22 ; bhi` -- the
 * covered range is 1..23, not the 10..50 of the resource_378 pair, and the
 * table is its own: 1,2 -> 0x020005c4; 4,23 -> 0x020005f0; 5 -> 0x02000602;
 * 15,17,19 -> 0x02000624; 21 -> 0x02000628; everything else default.  Note
 * 23 shares an arm with 4 while 22 does not, and the 15/17/19 arm skips 16,
 * 18 and 20 -- read entry by entry.
 *
 * Case-arm ORDER off the ROM (§5b5); per-site RAW callee names (§5b3a).
 *
 * 2208 is `movs r0, #138 ; lsls r0, #4` in the reference, so it is written as
 * the plain literal; 0x928 and 0x93e cannot be synthesised that way and are
 * pool loads in the reference, which plain literals also produce.
 */

extern s16 Data_02000240[];

extern u8 Data_0200f420[];
extern u8 Data_0200f444[];
extern u8 Data_0200f570[];
extern u8 Data_0200f6fc[];
extern u8 Data_0200f81c[];
extern u8 Data_0200f930[];
extern u8 Data_0200f984[];
extern u8 Data_0200f9c0[];
extern u8 Data_0200fb58[];
extern u8 Data_0200fd44[];
extern u8 Data_0200fedc[];

s32 Func_02006a6a();
s32 Func_02006a78();
s32 Func_02006a82();
s32 Func_02006a94();
s32 Func_02006aa8();
s32 Func_02006ab6();

u8 *Func_0200054c(void)
{
    s16 *table = Data_02000240;
    s32 scene = table[225];

    switch (scene) {
    case 1:
    case 2:
        if (Func_02006a6a(2208) != 0) {
            return Data_0200f6fc;
        }
        if (Func_02006a78(0x928) != 0 && Func_02006a82(0x93e) == 0) {
            return Data_0200f570;
        }
        return Data_0200f444;
    case 4:
    case 23:
        if (Func_02006a94(0x93e) != 0) {
            return Data_0200fedc;
        }
        return Data_0200f9c0;
    case 5:
        if (Func_02006aa8(2208) != 0) {
            return Data_0200f930;
        }
        if (Func_02006ab6(0x93e) != 0) {
            return Data_0200f984;
        }
        return Data_0200f81c;
    case 15:
    case 17:
    case 19:
        return Data_0200fb58;
    case 21:
        return Data_0200fd44;
    default:
        break;
    }

    return Data_0200f420;
}
