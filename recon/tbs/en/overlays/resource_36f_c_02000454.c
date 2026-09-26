/* NONMATCHING: 228 bytes, candidate 228, 13 differing halfwords, 13 halfword
 * edits (2026-09-26). The signed tile wrap, complete pools and scroll-clear
 * loop now agree. Remaining: blank uses r5 instead of r6, forcing the tile
 * increment's reload into r6 instead of r5; the resource copy also schedules
 * before the blank load. The global dump allocates blank before reload.
 * Single-overlay unit binding Engine_* at their import veneers. Remaining:
 * the tile increment: the reference forms (tile << 16) + 0x10000 with the
 * constant in r5 inside the row loop, which also pushes the blank halfword
 * into r6; here tile + 1 is added before the shift, and -O shows the same.
 * Goto loops (as in Title_RevealSpriteRow) stop the hoisting; the 0x2010000
 * buffer is spelled once as a symbol so it is not shared. */
#include "TYPES.H"
#include "DMA.H"

extern u8 Value_0000001a[];
extern u8 Data_02010000[];
extern u16 Data_03001ad0[];

void Main_080002a8(s32 mode);
u8 *Main_08000290(s32 resource);
void Main_080001a8(const u8 *source, void *destination);

struct TitleWork {
    u8 unknown_00[20];
    u16 mode;
};

struct ScrollPair {
    u16 x;
    u16 y;
};

#define DMA3 ((volatile u32 *)0x040000d4)

static __inline__ void DecodeBackground(const u8 *res)
{
    Main_080001a8(res, (void *)0x02010000);
}

/* Load the title background: palette, tiles and a 30 x 20 map counting up from
 * tile 0x1a0, then clear the scroll registers. */
void Func_02000454(void)
{
    u8 *res;
    s32 id;
    u16 *map;
    u32 x;
    u32 y;
    s32 tile;
    struct ScrollPair *scroll;
    u32 i;
    s32 blank;

    id = (s32)Value_0000001a;
    Main_080002a8(0);
    *(volatile u16 *)0x0400000c = 0x681;
    Data_03001ad0[5] = 0;
    res = Main_08000290(id);
    blank = 0x1ff;
    Dma_Set(res, (void *)0x05000000, 0x84000070, DMA3);
    res += 0x1c0;
    DecodeBackground(res);
    Dma_Set((void *)0x02010000, (void *)0x06006800, 0x84002580, DMA3);
    map = (u16 *)0x06003000;
    tile = 0x1a0;
    y = 0;
col:
    {
        x = 0;
    row:
        {
            s32 old = tile;
            tile = ((old << 16) + 0x10000) >> 16;
            *map++ = old;
        }
        if (++x <= 29)
            goto row;
        *map++ = blank;
        *map++ = blank;
    }
    if (++y <= 19)
        goto col;
    scroll = (struct ScrollPair *)Data_03001ad0;
    for (i = 0; i <= 3; i++) {
        scroll->y = 0;
        scroll->x = 0;
        scroll++;
    }
    Dma_Set(Data_03001ad0, (void *)0x04000010, 0x84000004, DMA3);
    (*(struct TitleWork **)0x03001e70)->mode = 0x1400;
}
