/* Draft, not exact (2026-10-01, slice-11): score 14545, 327 instructions
   off; 1036 of 1144 bytes. Title intro frame callback: lays the three sprite
   groups of the scrolling pictures along a perspective line, parks the rest
   of the object table and copies it to OAM.
   Remaining: the reference addresses every entry as work + index with the
   index in a register. For the third group it computes n * 8 after the wrap
   loops and adds 24, 32, +8, 48, 28, +8, +8, +8; for the first two groups
   (n is 0) the same registers are loaded with the folded constants (movs
   r1, #24; str r3, [r6, r1]). That is what reload leaves when the shifted
   index is a pseudo equal to 0 that was set in an earlier block than the
   stores and got no register. Here cse2 folds it inside the block and
   combine makes immediate offsets, 108 bytes short. The shifted index has
   to be computed before the wrap loops of each group; no natural spelling
   found yet (an inline wrap inside the first store folded the same way).
   n = 5 and n = 8 fold the third group too; the reference does not, so
   they are probably n += 5 and n += 8. */
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
    u32 objects[120][2];
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
    x = Data_080f39ab[column]; \
    y = dist * (x - 104) / 80 + base - (half); \
    x -= (half); \
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

void Func_080f2028(void)
{
    struct IntroWork *work;
    u32 n;
    s32 base;
    s32 top;
    s32 dist;
    s32 x;
    s32 y;

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
            dist = top - base;
            PLACE(0, 16);
            QUAD(n, 16, 0x40000000, 232);
            PLACE(2, 16);
            ENTRY(n + 4, 0) = ((x + 4) << 16) | y | 0x80002400;
            ENTRY(n + 4, 1) = 128;
            n = 5;
            PLACE(4, 32);
            QUAD(n, 32, 0x80000000, 192);
        } else {
            dist = top - base;
            PLACE(1, 16);
            QUAD(n, 16, 0x40000000, 232);
            PLACE(3, 16);
            QUAD(n + 4, 16, 0x40000000, 224);
            n = 8;
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
    y = 32 - gBgScroll[1].y;
    WRAP256(y);
    ENTRY(12, 0) = y | 0xc05c2000;
    ENTRY(12, 1) = 0x800;
    Dma_Set(&ENTRY(12, 0), (void *)(0x07000000 + n * 8), 0x84000002, (volatile u32 *)0x040000d4);
    Dma_Set(&ENTRY(0, 0), (void *)0x07000000, 0x84000008, (volatile u32 *)0x040000d4);
}
