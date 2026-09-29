/* The title screen's background: the Golden Sun logo's palette, tiles and a
 * 30 x 20 map counting up from tile 0x1a0, then cleared scroll registers.
 * The cell buffer is the fixed RAM buffer, reloaded for each use. */
#include "TYPES.H"
#include "DMA.H"
#include "RAM_BUFFER.H"
#include "FIELD_EVENT.H"
#include "RESOURCE_IDS.H"

extern struct MapRenderWork *gMapWork;
extern u16 gBgScroll[];

u8 *Resource_GetTableEntry(s32 resource);
void Blend_SetDarkenTarget16(s32 target);
void Resource_DecodeType01(const u8 *source, void *destination);

/* FAKEMATCH: an inline call wrapper keeps the decode's source in r4 and
   reloads the cell buffer's address after it, as the game does. */
static __inline__ void DecodeBackground(const u8 *res)
{
    Resource_DecodeType01(res, (void *)Ram_MapCellBuffer);
}

struct TitleWork {
    u8 unknown_00[20];
    u16 mode;
};

struct ScrollPair {
    u16 x;
    u16 y;
};


#define DMA3 ((volatile u32 *)0x040000d4)

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

    id = (s32)&ResourceId_GoldenSunLogo;
    Blend_SetDarkenTarget16(0);
    *(volatile u16 *)0x0400000c = 0x681;
    gBgScroll[5] = 0;
    blank = 0x1ff;
    res = Resource_GetTableEntry(id);
    Dma_Set(res, (void *)0x05000000, 0x84000070, DMA3);
    res += 0x1c0;
    DecodeBackground(res);
    Dma_Set((void *)Ram_MapCellBuffer, (void *)0x06006800, 0x84002580, DMA3);
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
    scroll = (struct ScrollPair *)gBgScroll;
    for (y = 0; y <= 3; y++) {
        scroll->y = 0;
        scroll->x = 0;
        scroll++;
    }
    Dma_Set(gBgScroll, (void *)0x04000010, 0x84000004, DMA3);
    (*(struct TitleWork **)&gMapWork)->mode = 0x1400;
}
