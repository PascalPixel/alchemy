#include "types.h"

/*
 * Build one numbered group out of the 20-byte roster at 0x080c3f34.
 *
 * The roster is a flat list of members separated by rows whose s8 kind is
 * -1: everything between two separators belongs to one group, and the owner
 * walks the whole list counting separators until the running group number
 * equals the caller's, building every member it passes while inside that
 * group and skipping every member outside it.  Reaching the caller's group's
 * own separator ends the walk and returns 0; running past row 332 without
 * ever reaching it returns 1.
 *
 * Building a member resets it - Func_08077150 and then Func_080771f0 with the
 * row's byte at +1 - and then clears its record: the four bytes at +0x118 and
 * the four at +0x11c, the four words at +0xf8 and the four at +0x108, and
 * thirty-two halfwords running down from +0xd4 in steps of four.  The row's
 * two halfwords at +0x0e are handed to Func_08077088 when set.
 *
 * The four s8 counts at +0x02 then drive four running slot numbers, one per
 * count, that persist across the whole group: each count issues that many
 * Func_080771a8 / Func_080771b0 pairs at the current slot and advances it.
 * Sixteen Func_08077058 calls follow, and finally the row's four halfwords at
 * +0x06 are resolved by Func_08077028 and passed to Func_08077050 when set.
 *
 * The kind byte doubles as the handle every one of those calls takes, which
 * is why the row is read before the row-332 bound is tested rather than
 * after - the load is what the bound is protecting against, and the owner
 * does it in that order.
 *
 * The epilogue at 0x080b551c ends `pop {r1} ; bx r1`, so r0 is the result.
 * One direct caller, 0x080b578e, and the address is also published as a
 * Thumb pointer at 0x080b5114.
 */

void Func_08077168(s32 slot);
void Func_08077150(s32 handle);
void Func_080771f0(s32 handle, s32 parameter);
void *Func_08077008(s32 handle);
void Func_08077088(s32 handle, s32 item);
void Func_080771a8(s32 handle, s32 group, s32 slot);
void Func_080771b0(s32 handle, s32 group, s32 slot);
void Func_08077058(s32 handle, s32 value);
s32 Func_08077028(s32 handle, s32 item);
void Func_08077050(s32 handle, s32 resolved);

/* One 20-byte roster row at 0x080c3f34. */
struct Member_080b5368 {
    s8 kind;
    u8 parameter;
    s8 counts[4];
    u16 items[4];
    u16 extras[2];
    u16 unused_12;
};

s32 Func_080b5368(s32 wanted)
{
    struct Member_080b5368 *roster = (struct Member_080b5368 *)0x080c3f34;
    s32 slots[4];
    s32 group;
    s32 counter;
    s32 kind;
    s32 overflow;
    s32 step;
    u32 index;
    u8 *record;

    Func_08077168(0);
    Func_08077168(1);
    Func_08077168(2);
    Func_08077168(3);
    Func_08077168(5);

    for (group = 3; group >= 0; group--) {
        slots[group] = 0;
    }

    counter = 0;
    overflow = 0;
    index = 0;
    for (;;) {
        kind = roster[index].kind;
        if (index > 332) {
            overflow = 1;
            break;
        }
        if (kind == -1) {
            if (counter == wanted) {
                break;
            }
            counter++;
            index++;
            continue;
        }
        if (counter != wanted) {
            index++;
            continue;
        }

        Func_08077150(kind);
        Func_080771f0(kind, roster[index].parameter);
        record = (u8 *)Func_08077008(kind);

        for (step = 0; step < 4; step++) {
            record[280 + step] = 0;
            record[284 + step] = 0;
            *(s32 *)(record + 248 + step * 4) = 0;
            *(s32 *)(record + 264 + step * 4) = 0;
        }
        for (step = 0; step < 32; step++) {
            *(u16 *)(record + 212 - step * 4) = 0;
        }

        for (step = 0; step < 2; step++) {
            if (roster[index].extras[step] != 0) {
                Func_08077088(kind, roster[index].extras[step]);
            }
        }

        for (group = 0; group < 4; group++) {
            s32 issued = 0;

            while (issued < roster[index].counts[group]) {
                Func_080771a8(kind, group, slots[group]);
                Func_080771b0(kind, group, slots[group]);
                slots[group]++;
                issued++;
            }
        }

        for (step = 0; step < 16; step++) {
            Func_08077058(kind, 0);
        }

        for (step = 0; step < 4; step++) {
            if (roster[index].items[step] != 0) {
                Func_08077050(kind,
                              Func_08077028(kind, roster[index].items[step]));
            }
        }

        index++;
    }

    return overflow;
}
