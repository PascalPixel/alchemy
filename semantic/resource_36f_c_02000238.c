#include "types.h"

/*
 * resource_36f owner at 0x02000238, 176 bytes: the overlay's per-frame sprite
 * task — rebuild the column of 18 sprite requests and submit the visible ones.
 *
 * Role known in advance from the call graph, no disassembly needed: the owner
 * at 0x020002e8 installs it with `Func_080000d0(0x02008239, 3200)`, and under
 * this overlay's proven 0x02008000 link base 0x02008239 is file offset 0x0238
 * plus the Thumb bit — this function's own address.  That is HANDOVER's
 * "callback pool word = function start + 1" witness, and it is also a third
 * independent confirmation of the link base for this overlay (the offset-0
 * export table gives six more; see resource_36f_c_020001c0.c).
 *
 * Complete owner.  Prologue `push {r5, r6, r7, lr} / mov r7,r8 / push {r7}` at
 * 0x02000238 with a 4-byte frame (`sub sp,#4`); single epilogue at 0x020002c8
 * ending `pop {r0} / bx r0`, so the popped word is the return address and the
 * owner is **void**.  Code runs 0x02000238..0x020002d3; the literal pool is
 * 0x020002d4..0x020002e7 and 0x020002e8 is the next prologue.  176 bytes,
 * matching the inventory row.  r4 is used as scratch without being saved, which
 * is what the reference does.
 *
 * Call targets resolved with `bun tools/overlay_call_targets.ts resource_36f
 * 0238`: exactly ONE site, 0x020002b2 -> veneer 0x0570 -> Func_080001e8, the
 * 12-byte sprite-request submitter whose role is established by the tracked
 * `semantic/overlays/resource_3ba_c_020033a0.c`.  Note the arity: that source
 * calls it `(entry, 255, 12)`, but this site never sets r2 (the register holds
 * a leftover from the visibility test), so it is a two-argument site.  Overlay
 * imports varying in arity between call sites is the documented norm, which is
 * why the declarations below are old-style.
 *
 * Pool words, all four: 0x02008650 and 0x020086a0 and 0x0200868c are in-image
 * (file offsets 0x0650 / 0x06a0 / 0x068c); 0x03001b10 and 0x03001e40 are below
 * the link band and are RAM globals — HANDOVER's two-sided band test.
 *
 * 0x02008650 is the cached palette slot that Func_020001c0 fills in.  It keys a
 * 4-byte-per-entry RAM table at 0x03001b10 whose halfword at +2 is the slot's
 * character base; `>> 5` turns that into a tile number, and the column steps two
 * tiles per row.
 *
 * The visibility test is the interesting part and is transcribed literally:
 * `remaining = (s16)counter / 2 - row` (an `asrs #16` / `lsrs #31` / `adds` /
 * `asrs #1` pair, i.e. a C division truncating toward zero, not an arithmetic
 * shift), clamped at 0, then forced to 0 for the last three rows while bit 0 of
 * the RAM flag word at 0x03001e40 is set.  Only a non-zero `remaining` submits.
 *
 * NOTE — the write cursor and the submit cursor are DIFFERENT pointers and they
 * are not merged.  `r6` advances 12 bytes every row; `r4` advances only on a
 * row that is actually submitted.  So a skipped row leaves the submit cursor
 * pointing at the stale entry, and the next submitted row re-sends that older
 * slot rather than the one just written.  That is what the reference does; it
 * reads as an original-source defect but nothing here distinguishes it from
 * intent, and "tidying" it would change behaviour.
 */

/* Old-style declarations: overlay imports vary in arity between call sites. */
void Func_080001e8();           /* queue a 12-byte sprite request */

/* In-image: the cached palette slot, and the frame counter this task bumps. */
extern s16 Data_02008650[];
extern u16 Data_0200868c[];
/* In-image: the 18-entry, 12-byte-per-entry sprite request scratch. */
extern u32 Data_020086a0[];

void Func_02000238(void)
{
    u32 *write = Data_020086a0;
    u32 *submit = Data_020086a0;
    s32 tile = *(u16 *)((u8 *)0x03001b10 + (Data_02008650[0] << 2) + 2) >> 5;
    u32 row;

    for (row = 0; row <= 17; row++) {
        s32 remaining;

        write[0] = 0;
        write[1] = (u32)((88 + (row << 3)) << 16) | 136 | 0x8400;   /* 132 << 8 */
        write[2] = 0xf000 | (u32)tile;                              /* 240 << 8 */
        write += 3;

        remaining = (s32)(s16)Data_0200868c[0] / 2 - row;
        if (remaining < 0) {
            remaining = 0;
        }
        if (remaining <= 2 && (*(u32 *)0x03001e40 & 1) != 0) {
            remaining = 0;
        }

        if (remaining != 0) {
            Func_080001e8(submit, 255);
            submit += 3;
        }

        tile += 2;
    }

    Data_0200868c[0]++;
}
