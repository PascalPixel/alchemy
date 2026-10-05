/* 2026-10-05 sixth wave, finisher 3: alchemy diff 6 (was 382). Left: in
 * each arm's third group the listing copies n out of r8 one slot earlier
 * (mov r1, r8 before adds r3, r0, #0); a scheduling difference only.
 * What moved it:
 * - The object table is volatile (the OAM shadow the DMA copies). Combine
 *   runs with init_recog_no_volatile, so it cannot fold the first two
 *   groups' constant offsets into the stores, and each stays in a register
 *   (movs r1, #24; str r3, [r6, r1]) as listed (382 to 275; the whole work
 *   struct volatile instead scores 290).
 * - PLACE subtracts half from the column byte first and gives the line
 *   x + half - 104: the column load then lands in r3 and is copied to x,
 *   as listed (275 to 121).
 * - No dist local: (top - base) is written in the line and GCSE keeps one
 *   copy of it in r9 (121 to 41).
 * - The OAM line at the end has its own local (41 to 6; reusing y 41,
 *   top 24, x 177, base 222).
 * Trials on this body: x as u8 313, s16 315, u16 534; n as s32 418, u8 391;
 * the line read twice from the table 385; y first in the word 24; the
 * shape term first or in the middle 6.
 */
/* DRAFT of Title_PlaceIntroSprites: the title intro frame callback lays the
   three sprite groups of the scrolling pictures along a perspective line,
   parks the rest of the object table and copies it to OAM. n advances after
   each group (n += 4, n++); GCSE propagates constants twice, so the first
   two groups fold and the third shifts n at run time, as listed. */
#include "TYPES.H"
#include "DMA.H"
#include "MAP_SCROLL.H"
#include "RAM_BUFFER.H"

struct IntroWork {
    s32 back_rows;
    s32 front_rows;
    s32 frame;
    s32 tick;
    s32 state;
    s32 rise;
    volatile u32 objects[120][2];
};

extern u8 gDebugPaused;
extern const u8 Data_080f39ab[];

/* Word N of object entry I: the entries follow the six counters. */
#define ENTRY(i, word) (work->objects[i][word])

#define WRAP256(v) \
    while ((v) > 255) \
        (v) -= 256; \
    while ((v) < 0) \
        (v) += 256

#define PLACE(column, half) \
    x = Data_080f39ab[column] - (half); \
    y = (top - base) * (x + (half) - 104) / 80 + base - (half); \
    WRAP256(y)

#define QUAD(i, size, shape, tile) \
    ENTRY(i, 0) = ((x + 4) << 16) | y | ((shape) + 0x00002400); \
    ENTRY((i) + 1, 0) = ((x + 4 + (size)) << 16) | y | ((shape) + 0x10002400); \
    ENTRY((i) + 2, 0) = ((x + 4) << 16) | (u8)(y + (size)) | ((shape) + 0x20002400); \
    ENTRY((i) + 3, 0) = ((x + 4 + (size)) << 16) | (u8)(y + (size)) | ((shape) + 0x30002400); \
    ENTRY(i, 1) = (tile); \
    ENTRY((i) + 1, 1) = (tile); \
    ENTRY((i) + 2, 1) = (tile); \
    ENTRY((i) + 3, 1) = (tile)

void Title_PlaceIntroSprites(void)
{
    struct IntroWork *work;
    u32 n;
    s32 base;
    s32 top;
    s32 x;
    s32 y;
    s32 line;

    work = Ram_WorkSlot[43];
    n = 0;
    if (gDebugPaused == 0) {
        if ((++work->tick & 3) == 0)
            work->rise++;
    }
    base = 48 - gBgScroll[1].y;
    top = 144 - work->rise;
    if (work->frame < 0x118) {
        if ((work->tick & 1) == 0) {
            PLACE(0, 16);
            QUAD(n, 16, 0x40000000, 232);
            n += 4;
            PLACE(2, 16);
            ENTRY(n, 0) = ((x + 4) << 16) | y | 0x80002400;
            ENTRY(n, 1) = 128;
            n++;
            PLACE(4, 32);
            QUAD(n, 32, 0x80000000, 192);
        } else {
            PLACE(1, 16);
            QUAD(n, 16, 0x40000000, 232);
            n += 4;
            PLACE(3, 16);
            QUAD(n, 16, 0x40000000, 224);
            n += 4;
            PLACE(5, 32);
            QUAD(n, 32, 0x80000000, 160);
        }
        n += 4;
    }
    for (; n <= 119; n++)
        ENTRY(n, 0) = 0x400020a0;
    *(volatile u16 *)0x04000050 = 0x3f50;
    *(volatile u16 *)0x04000052 = 0x0e0e;
    Dma_Set(&ENTRY(0, 0), (void *)0x07000000, ((n * 8) >> 2) | 0x84000000, (volatile u32 *)0x040000d4);
    line = 32 - gBgScroll[1].y;
    WRAP256(line);
    ENTRY(12, 0) = line | 0xc05c2000;
    ENTRY(12, 1) = 0x800;
    Dma_Set(&ENTRY(12, 0), (void *)(0x07000000 + n * 8), 0x84000002, (volatile u32 *)0x040000d4);
    Dma_Set(&ENTRY(0, 0), (void *)0x07000000, 0x84000008, (volatile u32 *)0x040000d4);
}
