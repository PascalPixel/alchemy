/* Draft, differ 32 of 103: ☀️'s Clear_LoadBackground for overlay 64A. Left:
 * the native keeps the row counter in r0 and the column counter in r4 (this
 * C, like ☀️'s native, the other way round), clears the scroll pairs with the
 * zero rebuilt each pass (a goto loop does that, but then the motion flag's
 * pool zero moves into it: 39), and loads 0x681 before building BG2CNT's
 * address. Tried: x and y declared either way round, a separate scroll
 * counter, goto row and column loops. */
#include "types.h"
#include "bg_scroll.h"
#include "dma.h"
#include "io_reg.h"
#include "party_state.h"
#include "object_runtime.h"
#include "ram_buffer.h"
#include "resource.h"

struct ClearWork {
    u8 unknown_00[20];
    u16 mode;
};

extern char ResourceId_LogoOnBlack;
extern u8 gMapCellBuffer[];

void Blend_SetDarkenTarget16(s32 frames);
void Func_020021f8(void *source, void *destination);
struct ObjectRuntime *Object_GetById(s32 id);

static __inline__ void Decode_(void *source, void *destination)
{
    /* FAKEMATCH: forwarding through the inline keeps the buffer address a constant of each call, as ☀️'s engine call layer does (called directly, the address is kept in r5: differ 22). */
    Func_020021f8(source, destination);
}

void Func_0200005c(void)
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
    for (y = 0; y <= 3; y++) {
        scroll->y = 0;
        scroll->x = 0;
        scroll++;
    }
    Dma_Set(gBgScroll, (void *)0x04000010, 0x84000004, REG_DMA3);
    ((struct ClearWork *)Ram_HeapSlots->map_work)->mode = 0x1400;
    {
        struct ObjectRuntime *leader;

        leader = Object_GetById(gPartyState.current_owner);
        leader->motion_flags = 0;
    }
}
