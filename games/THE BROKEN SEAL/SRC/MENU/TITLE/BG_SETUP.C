#include "TYPES.H"
#include "DMA.H"
#include "FIELD_EVENT.H"

extern u8 Value_0000001a[];
extern u16 Data_03001ad0[];

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
    Engine_ResourceDecodeType01(res, (void *)0x02010000);
}

/* Load the title background: palette, tiles and a 30 x 20 map counting up from
 * tile 0x1a0, then clear the scroll registers. */
void Title_LoadBackground(void)
{
    u8 *res;
    s32 id;
    u16 *map;
    u32 x;
    u32 y;
    s32 tile;
    struct ScrollPair *scroll;
    s32 blank;

    id = (s32)Value_0000001a;
    Engine_BlendSetDarkenTarget16(0);
    *(volatile u16 *)0x0400000c = 0x681;
    Data_03001ad0[5] = 0;
    blank = 0x1ff;
    res = Engine_ResourceGetTableEntry(id);
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

            /* FAKEMATCH: keep the signed tile wrap in the high half. */
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
    for (y = 0; y <= 3; y++) {
        scroll->y = 0;
        scroll->x = 0;
        scroll++;
    }
    Dma_Set(Data_03001ad0, (void *)0x04000010, 0x84000004, DMA3);
    (*(struct TitleWork **)0x03001e70)->mode = 0x1400;
}
