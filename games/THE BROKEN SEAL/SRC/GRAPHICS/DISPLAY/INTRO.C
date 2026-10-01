#include "TYPES.H"
#include "DMA.H"
#include "MAP_SCROLL.H"
#include "RAM_BUFFER.H"
#include "RESOURCE_IDS.H"

/* The title intro's work block, heap slot 43. */
struct IntroWork {
    s32 back_rows;
    s32 front_rows;
    s32 frame;
    s32 tick;
    s32 state;
    s32 unknown_14;
};

u8 *Resource_GetTableEntry(s32 index);
s32 Resource_DecodeByteLz(const void *source, void *destination);

/* Title intro: load both scrolling pictures and lay out their tilemaps.
   Each map row is thirty running tiles and two blank ones; the rows below
   the first eleven start a second run. Both layers start 96 lines down,
   behind a full-screen window, and the work block's counters are cleared. */
void Title_LoadIntroBackgrounds(void)
{
    struct IntroWork *work;
    u8 *data;
    u16 *map;
    s32 x;
    s32 y;
    s32 tile;
    s32 blank;

    work = Ram_WorkSlot[43];
    *(volatile u16 *)0x04000000 = 0;
    data = Resource_GetTableEntry((s32)&ResourceId_IntroGraphicsA);
    Dma_Set(data, (void *)0x05000200, 0x84000080, (volatile u32 *)0x040000d4);
    *(u16 *)0x05000200 = 0;
    data += 0x200;
    Resource_DecodeByteLz(data, Ram_MapCellBuffer);
    Dma_Set(Ram_MapCellBuffer, (void *)0x06010000, 0x80000f00, (volatile u32 *)0x040000d4);
    data = Resource_GetTableEntry((s32)&ResourceId_IntroGraphicsC);
    Dma_Set(data, (void *)0x05000000, 0x84000080, (volatile u32 *)0x040000d4);
    *(u16 *)0x05000000 = 0;
    data += 0x200;
    Resource_DecodeByteLz(data, Ram_MapCellBuffer);
    Dma_Set(Ram_MapCellBuffer + 0x2940, (void *)0x06000000, 0x80002760, (volatile u32 *)0x040000d4);
    Dma_Set(Ram_MapCellBuffer + 0xa140, (void *)0x06004ec0, 0x80004ec0, (volatile u32 *)0x040000d4);

    /* FAKEMATCH: goto loops and a tile counted in its high half keep the
       increment's constant inside each strip, where the loop pass would
       hoist it. */
    blank = 0x1ff;
    map = (u16 *)0x0600f000;
    tile = 0x267;
    y = 0;
front_top:
    x = 29;
front_top_cell:
    {
        s32 old = tile;

        tile = ((old << 16) + 0x10000) >> 16;
        *map++ = old;
    }
    if (--x >= 0)
        goto front_top_cell;
    *map++ = blank;
    *map++ = blank;
    if (++y <= 10)
        goto front_top;
    tile = 0x13b;
    y = 11;
front_rest:
    x = 29;
front_rest_cell:
    {
        s32 old = tile;

        tile = ((old << 16) + 0x10000) >> 16;
        *map++ = old;
    }
    if (--x >= 0)
        goto front_rest_cell;
    *map++ = blank;
    *map++ = blank;
    if (++y <= 31)
        goto front_rest;
    map = (u16 *)0x0600f800;
    tile = 300;
    y = 0;
back_top:
    x = 29;
back_top_cell:
    {
        s32 old = tile;

        tile = ((old << 16) + 0x10000) >> 16;
        *map++ = old;
    }
    if (--x >= 0)
        goto back_top_cell;
    *map++ = blank;
    *map++ = blank;
    if (++y <= 10)
        goto back_top;
    tile = 0;
    y = 11;
back_rest:
    x = 29;
back_rest_cell:
    {
        s32 old = tile;

        tile = ((old << 16) + 0x10000) >> 16;
        *map++ = old;
    }
    if (--x >= 0)
        goto back_rest_cell;
    *map++ = blank;
    *map++ = blank;
    if (++y <= 31)
        goto back_rest;

    *(volatile u16 *)0x0400000a = 0x1f43;
    *(volatile u16 *)0x0400000c = 0x1e81;
    *(volatile u16 *)0x04000040 = 0xf0;
    *(volatile u16 *)0x04000044 = 0x9f;
    *(volatile u16 *)0x04000042 = 0xf0;
    *(volatile u16 *)0x04000046 = 0x9f;
    *(volatile u16 *)0x04000048 = 0x1616;
    for (y = 0; y < 4; y++) {
        gBgScroll[y].y = 0;
        gBgScroll[y].x = 0;
    }
    gBgScroll[1].y = 96;
    gBgScroll[2].y = 96;
    work->frame = 0;
    work->back_rows = 0;
    work->front_rows = 0;
    work->tick = 0;
    work->unknown_14 = 0;
    work->state = 0;
    Dma_Set(gBgScroll, (void *)0x04000010, 0x84000004, (volatile u32 *)0x040000d4);
    *(volatile u16 *)0x04000050 = 0x3fbf;
    *(volatile u16 *)0x04000052 = 0x1010;
    *(volatile u16 *)0x04000050 = 0x3f44;
}
