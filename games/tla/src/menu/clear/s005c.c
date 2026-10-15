#include "types.h"
#include "bg_scroll.h"
#include "dma.h"
#include "io_reg.h"
#include "io_write_queue.h"
#include "party_state.h"
#include "object_runtime.h"
#include "ram_buffer.h"
#include "resource.h"

struct ClearWork
{
    u8  unk00[20];
    u16 mode;
};

extern char ResourceId_LogoOnBlack;
extern u16  Data_020024f4[];

void                  Blend_SetDarkenTarget16(s32 frames);
void                  Func_020021f8(void *source, void *destination);
struct ObjectRuntime *Object_GetById(s32 id);
void                  Scheduler_RemoveCallbackFar(void (*callback)(void));

static __inline__ void Decode_(void *source, void *destination)
{
    /* FAKEMATCH: forwarding through the inline keeps the buffer address a constant of each call, as ☀️'s engine call layer does (called directly, the address is kept in r5: differ 22). */
    Func_020021f8(source, destination);
}

// The logo background, as ☀️'s Clear_LoadBackground and overlay 649's
// Func_02000490, then the party leader's motion flags cleared.
void Func_0200005c(void)
{
    u8  *res;
    s32  id;
    u16 *map;
    /* FAKEMATCH: the column counter is pinned to r4: the native keeps the row counter in r0 and the column counter in r4, where this C (like ☀️'s native) gives the column counter r0. */
    register u32     x asm("r4");
    u32              y;
    u16              tile;
    struct BgScroll *scroll;
    s32              blank;

    id = (s32)&ResourceId_LogoOnBlack;
    Blend_SetDarkenTarget16(0);
    {
        /* FAKEMATCH: the control value is pinned to r3 as a halfword, and a one-pass block keeps it ahead of the register's address: the native loads 0x681 before it builds BG2CNT's address (plainly, one reordered row). */
        register u16 control asm("r3");

        control = 0x681;
        do
        {
        } while (0);
        REG_BG2CNT = control;
    }
    gBgScroll[2].y = 0;
    blank = 0x1ff;
    res = Resource_GetTableEntry(id);
    Dma_Set(res, (void *)0x05000000, 0x84000070, REG_DMA3);
    res += 0x1c0;
    Decode_(res, Ram_MapCellBuffer);
    Dma_Set((void *)Ram_MapCellBuffer, (void *)0x06006800, 0x84002580, REG_DMA3);
    /* FAKEMATCH: a one-pass block ends the scheduling block, so the tilemap address loads before the first tile number is built (one reordered row without it). */
    do
    {
    } while (0);
    map = (u16 *)0x06003000;
    tile = 0x1a0;
    // Written with goto: as for loops, the pinned column counter keeps its exit test
    // ahead of the loop, and the scroll loop's zero leaves it.
    y = 0;
row:
    x = 0;
column:
    *map++ = tile++;
    x++;
    if (x <= 29)
        goto column;
    *map++ = blank;
    *map++ = blank;
    y++;
    if (y <= 19)
        goto row;
    scroll = gBgScroll;
    y = 0;
scroll_pair:
    scroll->y = 0;
    scroll->x = 0;
    scroll++;
    y++;
    if (y <= 3)
        goto scroll_pair;
    Dma_Set(gBgScroll, (void *)0x04000010, 0x84000004, REG_DMA3);
    ((struct ClearWork *)Ram_HeapSlots->mapWork)->mode = 0x1400;
    {
        struct ObjectRuntime *leader;
        /* FAKEMATCH: the flag's zero is pinned to r5 as a halfword: the native loads it from the pool after the call, where plainly it reuses the scroll loop's zero kept in r5 (differ 20 with a byte pin). */
        register u16 zero asm("r5");

        leader = Object_GetById(gPartyState.currentOwner);
        zero = 0;
        leader->motionFlags = zero;
    }
}

// As ☀️'s Clear_UpdateBlend: queues an alpha blend of BG0 and the sprites over
// the rest whose first weight rises by one every second frame (the second
// weight 16 less), and removes the callback once the first weight passes 15.
void Func_0200015c(void)
{
    s32 frame;
    u32 level;

    frame = Data_020024f4[0] + 1;
    Data_020024f4[0] = frame;
    level = (u16)frame >> 1;
    IO_WRITE_QUEUE_PUSH(0x4000050, 0x2e51, 0x20000);
    IO_WRITE_QUEUE_PUSH(0x4000052, ((16 - (u16)level) << 8) | (u16)level, 0x20000);
    if (level > 15)
        Scheduler_RemoveCallbackFar(Func_0200015c);
}
