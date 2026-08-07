#include "types.h"

/*
 * Resource 3ca per-frame curtain task at 0x020011c4 (232 bytes,
 * 0x020011c4-0x020012ab).
 *
 * Complete owner: `push {r5, r6, r7, lr}` at 0x020011c4, interworking return
 * `pop {r5, r6, r7} / pop {r0} / bx r0` at 0x02001290.  The popped register is
 * r0, so it holds the return address and the owner is `void`.  The tail of the
 * row is padding plus this owner's literal pool, all data:
 *
 *   0x02001296  0x0000       alignment
 *   0x02001298  0x02009c1a   in-image halfword, file offset 0x1c1a
 *   0x0200129c  0x03001b10   IWRAM descriptor table
 *   0x020012a0  0x02009c18   in-image halfword, file offset 0x1c18
 *   0x020012a4  0x02009af8   in-image record array, file offset 0x1af8
 *   0x020012a8  0x80004000   fixed bits of the packed OAM pair
 *
 * This is the task 0x020012ac installs (pool word 0x020091c5 =
 * Func_020011c4 + Thumb bit), which is also the witness for the 0x02008000
 * link base.  A second, free confirmation falls out of the layout: the record
 * array is 24 entries of 12 bytes at file offset 0x1af8, and 0x1af8 + 288 =
 * 0x1c18, exactly where the counter halfword this owner drives begins.
 *
 * There is one call site, resolved with tools/lib/overlay_call_targets.ts:
 * 0x02001286 -> veneer 0x02001364 -> Func_080001e8.  It is inside the final
 * loop, so it is one C call expression covering 24 executions; placed calls
 * per target are Func_080001e8 x1 site, matching the row's `calls=1`.
 *
 * Reading of the packed word, which is the one interpretive step here.  Each
 * record's middle word is
 *
 *     (offset & 0xff) | (column << 21) | 0x80004000
 *
 * i.e. a pair of OAM attributes packed into one u32: the low halfword is
 * attr0 with y = offset & 0xff and shape bits 14-15 = 01 (wide), the high
 * halfword is attr1 with x = column << 5 (0, 32, ... 224) and size bits
 * 14-15 = 10.  Eight columns spaced 32 pixels apart across the 240-pixel
 * screen is what fixes this reading; the alternative — treating the masked
 * value as an x coordinate — would put all eight sprites at the same place.
 * The third word is the shared attr2.
 *
 * UNCERTAINTY: the record's first word is written as zero and never read here,
 * so its meaning comes from Func_080001e8, which is not reconstructed.
 */

/* One entry of the array Func_080001e8 consumes. */
typedef struct {
    u32 unk0;    /* always written 0 by this owner */
    u32 attr01;  /* attr0 in the low halfword, attr1 in the high halfword */
    u32 attr2;
} Sprite_020011c4;

extern Sprite_020011c4 Data_02009af8[]; /* 24 entries, file offset 0x1af8 */
extern u16 Data_02009c18;               /* frame counter, set to 0x30 by 0x020012ac */
extern u16 Data_02009c1a;               /* handle stored by 0x020012ac */

/* Main-image import reached through this overlay's veneer table.  Old-style
 * declaration: import arity varies between sites elsewhere in this overlay. */
void Func_080001e8();

void Func_020011c4(void)
{
    Sprite_020011c4 *out;
    s32 handle;
    s32 counter;
    s32 half;
    u32 column;
    u32 i;
    u32 attr2;

    /*
     * The IWRAM table at 0x03001b10 is indexed by the handle 0x020012ac stored
     * away, four bytes per entry.  Its +2 halfword divided by 32 is a tile
     * index — 32 bytes is one 4bpp 8x8 tile — and becomes attr2 for every
     * record written below.
     */
    handle = (s16)Data_02009c1a;
    attr2 = (u32)(*(u16 *)((char *)0x03001b10 + handle * 4 + 2) >> 5);

    /* Advance the wipe by one frame; it stops rather than wrapping. */
    if ((s16)Data_02009c18 != 0) Data_02009c18 = (u16)(Data_02009c18 - 1);

    out = Data_02009af8;

    /*
     * Top band.  The counter halfword is re-read on every iteration of the
     * original loop, but nothing in the loop writes it, so it is loop
     * invariant and is hoisted here.  `half` is the C division counter / 2,
     * which is what the `lsrs #31 / adds / asrs #1` sequence computes.
     */
    counter = (s16)Data_02009c18;
    half = -counter / 2;
    for (column = 0; column <= 7; column++) {
        out->unk0 = 0;
        out->attr01 = ((u32)half & 0xff) | ((u32)column << 21) | 0x80004000;
        out->attr2 = attr2;
        out++;
    }

    /* Middle band, 136 scanlines below the top band's mirror image. */
    half = counter / 2 + 136;
    for (column = 0; column <= 7; column++) {
        out->unk0 = 0;
        out->attr01 = ((u32)half & 0xff) | ((u32)column << 21) | 0x80004000;
        out->attr2 = attr2;
        out++;
    }

    /*
     * Bottom band, 152 below.  This group re-reads the counter from memory
     * rather than reusing the register, but again nothing has written it.
     */
    half = (s16)Data_02009c18 / 2 + 152;
    for (column = 0; column <= 7; column++) {
        out->unk0 = 0;
        out->attr01 = ((u32)half & 0xff) | ((u32)column << 21) | 0x80004000;
        out->attr2 = attr2;
        out++;
    }

    /* Publish all 24 records. */
    for (i = 0; i <= 23; i++) Func_080001e8(&Data_02009af8[i], 255);
}
