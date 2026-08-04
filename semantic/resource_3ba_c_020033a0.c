#include "types.h"

/*
 * resource_3ba owner at 0x020033a0, 964 bytes (0x020033a0-0x02003763):
 * 932 bytes of code plus the eight-word literal pool at 0x02003744, which ends
 * exactly where the next owner (0x02003764) begins.  Nothing live crosses the
 * row boundary.
 *
 * Prologue `push {r5, r6, r7, lr}` plus the high-register saves
 * `mov r7, fp / mov r6, sl / mov r5, r9 / push {r5, r6, r7}` and
 * `mov r7, r8 / push {r7}`, then `sub sp, #20`.  The unwind at
 * 0x02003732..0x02003742 ends `pop {r0} ; bx r0`, so r0 holds the popped
 * return address and the owner returns nothing.  Every exit reaches it.  No
 * argument register is read before being written: this is a per-frame task,
 * and 0x02003764 is the owner that installs it with
 * `Func_080000d0(0x0200b3a1, 0xc76)` — 0x0200b3a1 being this function's
 * address plus the Thumb bit under the proven 0x02008000 link base.
 *
 * CALL ACCOUNTING.  Twenty `bl` sites, all resolved with
 * `bun tools/overlay_call_targets.ts resource_3ba --annotate`:
 * Func_080001e8 x8, Func_03000380 x4, Func_0808a400 x2, and one each of
 * Func_080770c0, Func_08000170, Func_080001a8, Func_080001c8, Func_08000178
 * and Func_080001c0.  The C below reproduces that multiset exactly: each of
 * the two counted loops contains its site once, and the two near-identical
 * endpoint blocks at 0x02003622 and 0x020036b0 are written separately because
 * they ARE separate sites — behavioural similarity is not a licence to merge.
 * The inventory's `calls=19` is the usual low lower bound.
 *
 * POOL WORDS, under the 0x02008000 base proven for this overlay by the
 * byte-exact `assets/code/resource_3ba_c_02000158.c`:
 *   0x03001f3c  IWRAM pointer CELL holding the scene record — one
 *               dereference, exactly as the byte-exact
 *               `assets/code/resource_3ba_c_0200384c.c` spells it
 *               (`u8 *state = Data_03001f3c`), which also reads this record's
 *               +224 and +240 fields;
 *   0x03001b10  IWRAM metrics table, four bytes per graphics slot;
 *   0x03001e40  IWRAM status word, low nibble gated below;
 *   0x040000d4  DMA3 source/destination/control;
 *   0x0200bef4  even -> in-image DATA at file offset 0x3ef4, the palette DMAd
 *               to OBJ palette bank 14;
 *   0x0200bf14  even -> in-image DATA at file offset 0x3f14, the same tile
 *               source that 0x02003764 uploads;
 *   0x050003c0  OBJ palette RAM;
 *   0x80000010  DMA enable, 16 halfword units.
 *
 * THE RECORD.  It is the block that 0x02003764 fills immediately after
 * allocating it, and every field this task reads is one that owner wrote:
 *   +216 s16  graphics/palette slot   (0x02003764: Func_080001d0's result)
 *   +218 u16  phase counter           (0x02003764: cleared)
 *   +220 s16  abort flag              (0x02003764: cleared)
 *   +222 s16  first endpoint selector (0x02003764 arg 1)
 *   +224 s16  second endpoint selector(0x02003764 arg 2)
 *   +230 s16  link count              (0x02003764 arg 3)
 *   +232 s32  x origin                (0x02003764 arg 4)
 *   +236 s32  z origin                (0x02003764 arg 5)
 * The sprite requests are built at the record's own base, three words each,
 * and handed to Func_080001e8 twelve bytes at a time.  Two cursors run in the
 * assembly — r8 lagging one entry behind the [sp,#12] write pointer — and they
 * are always equal at each call site, so one cursor is written here.
 *
 * SHAPE.  A per-frame ramp/chain renderer.  While the abort flag is clear it
 * ramps the phase counter up (or, once story flag 0x106 is set, back down),
 * uploading the palette and tiles exactly once on the frame the counter first
 * reaches 1.  When the counter reaches 0 it releases the graphics slot through
 * Func_080001c0 and stops.  Otherwise it emits a head sprite, `linkCount`
 * body sprites, two joint sprites, another `linkCount` mirrored body sprites,
 * a tail sprite, and — only once the IWRAM status nibble exceeds 4 — one
 * endpoint sprite per participating actor, positioned by dividing the actor's
 * offset from the record's origin by 0xe0000.
 *
 * UNCERTAINTIES.
 *  - The request words are written as raw u32 triples because their layout is
 *    not established elsewhere in the tree; the bit arithmetic is transcribed
 *    literally rather than named.
 *  - The final Func_080001e8 site at 0x0200372e sets only r0 and r1; r2 still
 *    holds the write cursor, not the 12 that every other site passes.  That is
 *    reproduced as a two-argument call, with an old-style declaration, rather
 *    than "corrected" to 12.
 *  - `(counter + 1) << 16 == 0x10000` is the assembly's test for the counter
 *    having just become 1; it is written that way here.
 *  - Func_03000380 is the relocated IWRAM divide helper reached through an
 *    ordinary veneer (no r3/r4 load, so not a `call_via`), the same shape
 *    HANDOVER records for resource_370.
 *  - The second loop's guard is `0 < linkCount` — r6 was reset to 0 before it,
 *    so it is a fresh count, not a continuation of the first loop's index.
 */

/* Import veneers, named by the main-image function each one reaches.
 * Old-style declarations: arities vary between call sites in this overlay. */
s32 Func_080770c0();            /* test a story flag */
s32 Func_08000170();            /* reserve a graphics handle */
void Func_08000178();           /* release a graphics handle */
void Func_080001a8();           /* upload image data to a handle */
void Func_080001c0();           /* release the graphics slot */
void Func_080001c8();           /* upload a palette ramp */
void Func_080001e8();           /* queue a 12-byte sprite request */
u8 *Func_0808a400();            /* actor record by selector, or 0 */
s32 Func_03000380();            /* relocated IWRAM divide */

/* In-image data. */
extern u8 Data_0200bef4[];      /* file offset 0x3ef4 */
extern u8 Data_0200bf14[];      /* file offset 0x3f14 */

/* IWRAM. */
extern u8 *Data_03001f3c;       /* pointer CELL to the scene record */
extern u16 Data_03001b10[];     /* metrics table, two halfwords per slot */
extern u32 Data_03001e40;

void Func_020033a0(void)
{
    u8 *record;
    u32 *entry;
    u32 tile;
    u32 column;
    s32 counter;
    s32 linkCount;
    s32 i;
    s32 handle;
    u8 *actor;
    s32 across;
    s32 down;

    record = Data_03001f3c;
    entry = (u32 *)record;

    /* Metrics halfword at +2 of the slot's four-byte entry, scaled down. */
    tile = Data_03001b10[*(s16 *)(record + 216) * 2 + 1] >> 5;

    linkCount = *(s16 *)(record + 230);

    if (*(s16 *)(record + 220) != 0) {
        *(u16 *)(record + 218) = 2;
    } else if (Func_080770c0(0x106) != 0) {         /* 131 << 1 */
        if (*(s16 *)(record + 218) > 0) {
            *(u16 *)(record + 218) = *(u16 *)(record + 218) - 1;
        }
    } else if (*(s16 *)(record + 218) <= 1) {
        u16 raised = *(u16 *)(record + 218) + 1;

        *(u16 *)(record + 218) = raised;

        /* The frame the counter first becomes 1: load the graphics. */
        if (((s32)raised << 16) == 0x10000) {
            *(volatile u32 *)0x040000d4 = (u32)Data_0200bef4;
            *(volatile u32 *)0x040000d8 = 0x050003c0;
            *(volatile u32 *)0x040000dc = 0x80000010;

            handle = Func_08000170(0x200);
            Func_080001a8(Data_0200bf14, handle);
            Func_080001c8(*(s16 *)(record + 216), 0x200, handle);
            Func_08000178(handle);
        }
    }

    counter = *(s16 *)(record + 218);
    if (counter == 0) {
        Func_080001c0(*(s16 *)(record + 216));
        return;
    }

    column = ((u32)(counter * 6) - 8) & 0xff;

    /* Head. */
    entry[0] = 0;
    entry[1] = (u32)((104 - (linkCount << 4)) << 16) | column | 0x8000;
    entry[2] = tile | 0xe400;
    Func_080001e8(entry, 255, 12);
    entry += 3;

    /* Body, top half. */
    for (i = 0; (u32)i < (u32)linkCount; i++) {
        entry[0] = 0;
        entry[1] = (u32)((96 - (i << 4)) << 16) | column | 0x40000000;
        entry[2] = (tile + 2) | 0xe400;
        Func_080001e8(entry, 255, 12);
        entry += 3;
    }

    /* Two joint sprites. */
    entry[0] = 0;
    entry[1] = 0x700000 | column | 0x8000;          /* 224 << 15 */
    entry[2] = (tile + 6) | 0xe400;
    Func_080001e8(entry, 255, 12);
    entry += 3;

    entry[0] = 0;
    entry[1] = 0x780000 | column | 0x8000 | 0x10000000;   /* 240 << 15 */
    entry[2] = (tile + 6) | 0xe400;
    Func_080001e8(entry, 255, 12);
    entry += 3;

    /* Body, mirrored half.  The vertical term starts at 0x800000 and steps by
     * 0x100000 per link (128 << 16 and 128 << 13). */
    {
        u32 vertical = 0x800000;

        for (i = 0; (u32)i < (u32)linkCount; i++) {
            entry[0] = 0;
            entry[1] = column | vertical | 0x40000000 | 0x10000000;
            entry[2] = (tile + 2) | 0xe400;
            Func_080001e8(entry, 255, 12);
            entry += 3;
            vertical += 0x100000;
        }
    }

    /* Tail.  Note that this one ORs into the running `column` value, which is
     * how the assembly leaves it; nothing reads it afterwards. */
    column |= (u32)(((linkCount << 4) + 128) << 16);
    column |= 0x8000;
    column |= 0x10000000;

    entry[0] = 0;
    entry[1] = column;
    entry[2] = tile | 0xe400;
    Func_080001e8(entry, 255, 12);
    entry += 3;

    if ((Data_03001e40 & 15) <= 4) {
        return;
    }

    /* Endpoint sprite for the second selector. */
    actor = Func_0808a400(*(s16 *)(record + 224));
    if (actor != 0) {
        across = Func_03000380(*(s32 *)(actor + 8) - *(s32 *)(record + 232),
                               0xe0000);            /* 224 << 12 */
        down = across + 112;
        across = Func_03000380(*(s32 *)(actor + 16) - *(s32 *)(record + 236),
                               0xe0000);
        across = across + (*(s16 *)(record + 218) * 6);

        entry[0] = 0;
        entry[1] = (((u32)(across - 4)) & 0xff) | (u32)(down << 16)
                   | 0x40000000;
        entry[2] = (tile + 12) | 0xe400;
        Func_080001e8(entry, 255, 12);
        entry += 3;
    }

    /* Endpoint sprite for the first selector. */
    actor = Func_0808a400(*(s16 *)(record + 222));
    if (actor == 0) {
        return;
    }

    across = Func_03000380(*(s32 *)(actor + 8) - *(s32 *)(record + 232),
                           0xe0000);
    down = across + 112;
    across = Func_03000380(*(s32 *)(actor + 16) - *(s32 *)(record + 236),
                           0xe0000);
    across = across + (*(s16 *)(record + 218) * 6);

    entry[0] = 0;
    entry[1] = (((u32)(across - 4)) & 0xff) | (u32)(down << 16) | 0x40000000;
    entry[2] = (tile + 8) | 0xe400;

    /* Only r0 and r1 are set at this site; see the note above. */
    Func_080001e8(entry, 255);
}
