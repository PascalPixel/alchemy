/* NONMATCHING: 228 bytes, candidate 234, 42 differing halfwords (2026-09-24).
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

#define DMA3 ((volatile u32 *)0x040000d4)

/* Load the title background: palette, tiles and a 30 x 20 map counting up from
 * tile 0x1a0, then clear the scroll registers. */
void Local_02000454(void)
{
    u8 *res;
    s32 id;
    u16 *map;
    u32 x;
    u32 y;
    s16 tile;
    u16 *scroll;
    u32 i;
    u16 blank;

    id = (s32)Value_0000001a;
    Main_080002a8(0);
    *(volatile u16 *)0x0400000c = 0x681;
    Data_03001ad0[5] = 0;
    res = Main_08000290(id);
    blank = 0x1ff;
    Dma_Set(res, (void *)0x05000000, 0x84000070, DMA3);
    res += 0x1c0;
    Main_080001a8(res, Data_02010000);
    Dma_Set((void *)0x02010000, (void *)0x06006800, 0x84002580, DMA3);
    map = (u16 *)0x06003000;
    tile = 0x1a0;
    y = 0;
col:
    {
        x = 0;
    row:
        *map++ = tile++;
        if (++x <= 29)
            goto row;
        *map++ = blank;
        *map++ = blank;
    }
    if (++y <= 19)
        goto col;
    scroll = Data_03001ad0;
    i = 0;
clr:
    {
        scroll[1] = 0;
        scroll[0] = 0;
        scroll += 2;
    }
    if (++i <= 3)
        goto clr;
    Dma_Set(Data_03001ad0, (void *)0x04000010, 0x84000004, DMA3);
    (*(struct TitleWork **)0x03001e70)->mode = 0x1400;
}
