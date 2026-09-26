/* DRAFT: 1036 of 1144 bytes, 569 differing halfwords. Title-screen object
 * layout: three sprite groups placed along a perspective line from the table
 * at 0x080F39AB, alternating two layouts on odd and even frames, the rest of
 * the 120 entries parked, then DMA to OAM.
 * Remaining: the reference addresses every group entry through a register
 * offset (movs r1, #24; str r3, [r6, r1], with r4 as a scratch), which looks
 * like reload rematerialising offset pseudos that equal constants; this C
 * folds them into immediate offsets and is 108 bytes short. The reference
 * keeps base in sl, the distance in r9 and the entry count in r8. */
#include "TYPES.H"
#include "DMA.H"

struct TitleObj {
    u32 attr01;
    u32 attr2;
};

struct TitleWork {
    s32 unk00[2];
    s32 timer;
    s32 frame;
    s32 unk10;
    s32 rise;
    struct TitleObj obj[13];
};

extern u16 gBgScroll[];
extern u8 Data_080f39ab[];

s32 Math_Div(s32 value, s32 divisor);

#define WRAP256(v) \
    while ((v) > 255) (v) -= 256; \
    while ((v) < 0) (v) += 256

#define PLACE(i, half) \
    x = Data_080f39ab[i]; \
    y = Math_Div(dist * (x - 104), 80) + base - (half); \
    x -= (half); \
    WRAP256(y)

#define QUAD16(t) \
    work->obj[n].attr01 = ((x + 4) << 16) | y | 0x40002400; \
    work->obj[n + 1].attr01 = ((x + 20) << 16) | y | 0x50002400; \
    work->obj[n + 2].attr01 = ((x + 4) << 16) | (u8)(y + 16) | 0x60002400; \
    work->obj[n + 3].attr01 = ((x + 20) << 16) | (u8)(y + 16) | 0x70002400; \
    work->obj[n].attr2 = (t); \
    work->obj[n + 1].attr2 = (t); \
    work->obj[n + 2].attr2 = (t); \
    work->obj[n + 3].attr2 = (t)

#define QUAD32(t) \
    work->obj[n].attr01 = ((x + 4) << 16) | y | 0x80002400; \
    work->obj[n + 1].attr01 = ((x + 36) << 16) | y | 0x90002400; \
    work->obj[n + 2].attr01 = ((x + 4) << 16) | (u8)(y + 32) | 0xa0002400; \
    work->obj[n + 3].attr01 = ((x + 36) << 16) | (u8)(y + 32) | 0xb0002400; \
    work->obj[n].attr2 = (t); \
    work->obj[n + 1].attr2 = (t); \
    work->obj[n + 2].attr2 = (t); \
    work->obj[n + 3].attr2 = (t)

void Func_080f2028(void)
{
    struct TitleWork *work = *(struct TitleWork **)0x03001efc;
    u32 n;
    s32 base;
    s32 top;
    s32 dist;
    s32 x;
    s32 y;

    n = 0;
    if (*(u8 *)0x03001d20 == 0) {
        if ((++work->frame & 3) == 0)
            work->rise++;
    }
    base = 48 - gBgScroll[3];
    top = 144 - work->rise;
    if (work->timer < 0x118) {
        if ((work->frame & 1) == 0) {
            dist = top - base;
            PLACE(0, 16);
            QUAD16(232);
            PLACE(2, 16);
            work->obj[n + 4].attr01 = ((x + 4) << 16) | y | 0x80002400;
            work->obj[n + 4].attr2 = 128;
            n = 5;
            PLACE(4, 32);
            QUAD32(192);
        } else {
            dist = top - base;
            PLACE(1, 16);
            QUAD16(232);
            PLACE(3, 16);
            n = 4;
            QUAD16(224);
            n = 8;
            PLACE(5, 32);
            QUAD32(160);
        }
        n += 4;
    }
    for (; n <= 119; n++)
        work->obj[n].attr01 = 0x400020a0;
    *(u16 *)0x04000050 = 0x3f50;
    *(u16 *)0x04000052 = 0x0e0e;
    Dma_Set(work->obj, (void *)0x07000000, ((n * 8) >> 2) | 0x84000000, (volatile u32 *)0x040000d4);
    y = 32 - gBgScroll[3];
    WRAP256(y);
    work->obj[12].attr01 = y | 0xc05c2000;
    work->obj[12].attr2 = 0x800;
    Dma_Set(&work->obj[12], (void *)(0x07000000 + n * 8), 0x84000002, (volatile u32 *)0x040000d4);
    Dma_Set(work->obj, (void *)0x07000000, 0x84000008, (volatile u32 *)0x040000d4);
}
