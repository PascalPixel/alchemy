#include "types.h"

/*
 * Resource 378 owner at 0x0200014c (304 bytes).
 *
 * Complete owner: `push {lr}` at 0x0200014c and `pop {r1} ; bx r1` at
 * 0x02000242.  The popped register is r1, so r0 survives and IS the result.
 * 0x0200014c + 304 = 0x0200027c, where the next owner's `push {lr}` begins,
 * and the span decomposes as dispatcher (0x0200014c-0x02000167), 41-entry
 * jump table (0x02000168-0x0200020b, data), seven case bodies including the
 * default (0x0200020c-0x02000245), a two-byte alignment halfword at
 * 0x02000246 and the literal pool (0x02000248-0x0200027b).
 *
 * Found by the `mov pc` census (`tools/overlay_dispatch_sites.ts`): this was
 * one of nine dispatch sites the tree described NOWHERE -- no overlay
 * inventory row, no semantic draft, no exact C -- so it is adopted with
 * `--span 304` rather than from an inventory row.
 *
 * Same family as `resource_371:037c`: the selector is `Data_02000240[225]`
 * read as a signed halfword, and `subs #10 ; cmp #40 ; bhi` makes the covered
 * range 10..50 inclusive.  Family resemblance is NOT evidence, so the table
 * was read entry by entry rather than carried over: this owner's 41 entries
 * group as 10,12 -> 0x0200020c; 11 -> 0x02000210; 20,21,50 -> 0x02000214;
 * 32 -> 0x02000218; 29 -> 0x0200021c; 35 -> 0x02000220; everything else to
 * the default at 0x02000224.  Note 50 shares an arm with 20 and 21 while its
 * neighbours 48 and 49 do not -- a shape no sibling would have predicted.
 *
 * Case-arm ORDER is taken off the ROM (HANDOVER §5b5): group the entries by
 * value and sort the distinct values ascending, which puts the 20/21/50 arm
 * third and the 29 arm after the 32 arm, not in selector order.
 *
 * The default arm asks two questions in sequence and returns a different
 * table for each answer; the two `bl` sites carry per-site RAW names
 * (HANDOVER §5b3a), read off `overlay_show resource_378 0x14c`.
 */

extern s16 Data_02000240[];

extern u8 Data_0200be34[];
extern u8 Data_0200be70[];
extern u8 Data_0200bec4[];
extern u8 Data_0200bf0c[];
extern u8 Data_0200bf78[];
extern u8 Data_0200bfd8[];
extern u8 Data_0200c038[];
extern u8 Data_0200c080[];
extern u8 Data_0200c0ec[];

s32 Func_020037f8();
s32 Func_02003806();

u8 *Func_0200014c(void)
{
    s16 *table = Data_02000240;
    s32 scene = table[225];

    switch (scene) {
    case 10:
    case 12:
        return Data_0200be70;
    case 11:
        return Data_0200bec4;
    case 20:
    case 21:
    case 50:
        return Data_0200bf0c;
    case 32:
        return Data_0200c0ec;
    case 29:
        return Data_0200c038;
    case 35:
        return Data_0200c080;
    default:
        break;
    }

    if (Func_020037f8(0x87a) != 0) {
        return Data_0200bfd8;
    }
    if (Func_02003806(0x815) != 0) {
        return Data_0200bf78;
    }
    return Data_0200be34;
}
