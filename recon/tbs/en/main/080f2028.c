/* Draft, not exact: score 13465, 295 instructions off (was 14545 and 327).
   Title intro frame callback: lays the three sprite groups of the scrolling
   pictures along a perspective line, parks the rest of the object table and
   copies it to OAM.
   Found: the stores address an entry as work + (offset + constant) with the
   sum in a register, which an inline store taking a byte offset reproduces
   (a struct or array index puts the constant in the instruction instead).
   Remaining: this draft keeps the offset in r7, which pushes x to r5 and y
   to r4; the reference has x in r7, y in r5, and for the first two groups
   loads each folded constant (movs r1, #24; str r3, [r6, r1]) while the
   third shifts n in r12 after its wrap loops. So the first two groups' offset
   is one pseudo, set once to n * 8 and equal to 0, that got no register and
   that reload replaced. Set once before the first wrap loop it is folded
   before GCSE and every store becomes an immediate offset; set once per
   group it takes a register. It has to become constant only after the loop
   pass, as an invariant moved out of a loop would. */
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

static __inline__ void Poke(struct IntroWork *work, u32 offset, u32 value)
{
    *(u32 *)((u8 *)work + offset) = value;
}

#define QUAD(i, size, shape, tile) \
    Poke(work, o + ((i) - n) * 8 + 24, ((x + 4) << 16) | y | ((shape) + 0x00002400)); \
    Poke(work, o + ((i) - n) * 8 + 32, ((x + 4 + (size)) << 16) | y | ((shape) + 0x10002400)); \
    Poke(work, o + ((i) - n) * 8 + 40, ((x + 4) << 16) | (u8)(y + (size)) | ((shape) + 0x20002400)); \
    Poke(work, o + ((i) - n) * 8 + 48, ((x + 4 + (size)) << 16) | (u8)(y + (size)) | ((shape) + 0x30002400)); \
    Poke(work, o + ((i) - n) * 8 + 28, (tile)); \
    Poke(work, o + ((i) - n) * 8 + 36, (tile)); \
    Poke(work, o + ((i) - n) * 8 + 44, (tile)); \
    Poke(work, o + ((i) - n) * 8 + 52, (tile))

void Func_080f2028(void)
{
    struct IntroWork *work;
    u32 n;
    s32 base;
    s32 top;
    s32 dist;
    s32 x;
    s32 y;
    u32 o;

    work = Ram_WorkSlot[43];
    n = 0;
    if (gDebugPaused == 0) {
        if ((++work->tick & 3) == 0)
            work->rise++;
    }
    o = n * 8;
    base = 48 - gBgScroll[1].y;
    top = 144 - work->rise;
    if (work->frame < 0x118) {
        if ((work->tick & 1) == 0) {
            dist = top - base;
            PLACE(0, 16);
            QUAD(n, 16, 0x40000000, 232);
            PLACE(2, 16);
            Poke(work, o + 56, ((x + 4) << 16) | y | 0x80002400);
            Poke(work, o + 60, 128);
            n = 5;
            o = n * 8;
            PLACE(4, 32);
            QUAD(n, 32, 0x80000000, 192);
        } else {
            dist = top - base;
            PLACE(1, 16);
            QUAD(n, 16, 0x40000000, 232);
            PLACE(3, 16);
            QUAD(n + 4, 16, 0x40000000, 224);
            n = 8;
            o = n * 8;
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
