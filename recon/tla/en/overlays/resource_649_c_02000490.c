/* Draft, differ 20 of 94: ☀️'s Title_LoadBackground for overlay 649. Left:
 * the native keeps the row counter in r0 and the column counter in r4 (this C,
 * like ☀️'s native, the other way round, also in the scroll loop) and loads
 * 0x681 before building BG2CNT's address. Tried: x and y declared either way
 * round, goto row and column loops, five minutes of alchemy permute (18, with
 * the tile loops in a one-pass block). */
#include "types.h"
#include "bg_scroll.h"
#include "dma.h"
#include "io_reg.h"
#include "ram_buffer.h"
#include "resource.h"

struct TitleWork {
    u8 unknown_00[20];
    u16 mode;
};

extern char ResourceId_LogoOnBlack;

void Blend_SetDarkenTarget16(s32 frames);

static __inline__ void Decode_(void *source, void *destination)
{
    /* FAKEMATCH: forwarding through the inline keeps the buffer address a constant of each call, as ☀️'s engine call layer does (called directly, the address is kept in r5). */
    Resource_DecodeType01(source, destination);
}

void Func_02000490(void)
{
    u8 *res;
    s32 id;
    u16 *map;
    u32 x;
    u32 y;
    u16 tile;
    struct BgScroll *scroll;
    s32 blank;

    id = (s32)&ResourceId_LogoOnBlack;
    Blend_SetDarkenTarget16(0);
    REG_BG2CNT = 0x681;
    gBgScroll[2].y = 0;
    blank = 0x1ff;
    res = Resource_GetTableEntry(id);
    Dma_Set(res, (void *)0x05000000, 0x84000070, REG_DMA3);
    res += 0x1c0;
    Decode_(res, Ram_MapCellBuffer);
    Dma_Set((void *)Ram_MapCellBuffer, (void *)0x06006800, 0x84002580, REG_DMA3);
    map = (u16 *)0x06003000;
    tile = 0x1a0;
    for (y = 0; y < 20; y++) {
        for (x = 0; x < 30; x++)
            *map++ = tile++;
        *map++ = blank;
        *map++ = blank;
    }
    scroll = gBgScroll;
    y = 0;
scroll_loop:
    scroll->y = 0;
    scroll->x = 0;
    scroll++;
    y++;
    if (y <= 3)
        goto scroll_loop;
    Dma_Set(gBgScroll, (void *)0x04000010, 0x84000004, REG_DMA3);
    ((struct TitleWork *)Ram_HeapSlots->map_work)->mode = 0x1400;
}
