#include "types.h"
#include "ram_buffer.h"
#include "dma.h"
#include "io_reg.h"
#include "bg_scroll.h"
#include "btldisp.h"

extern const u8 Data_0812cc74[];

void Func_08125c5c(void)
{
    u32 index = *(s32 *)Ram_HeapSlots->unknown_84[9] - 1;

    if (index <= 31)
        Dma_Set(Data_0812cc74 + (index >> 2) * 32, (void *)0x06005000, 0x84000008, REG_DMA3);
}

void BattlePres_AdvanceTransitionTimer(void)
{
    s32 scroll;
    struct BattleCamera *camera;
    u32 *timer;
    u32 frame;
    u32 next;

    timer = (u32 *)Ram_HeapSlots->unknown_84[9];
    frame = *timer;
    camera = (struct BattleCamera *)Ram_HeapSlots->camera_work;
    scroll = 0x34 - frame;
    if (scroll > 0x20)
        scroll = 0x20;
    if (scroll < 0)
        scroll = 0;
    gBgScroll[0].y = scroll;
    if (frame <= 0x50U)
        camera->yaw = 45 * frame * 8 + 0xaf80;
    next = (*timer = *timer + 1);
    if (next <= 0x50U) {
        BattlePres_SetupTransitionScene(0, 0, 0, 0xb4 - next);
        return;
    }
    BattlePres_SetupTransitionScene(0, 0, 0, 0x64);
}

// Draws the transition's two rows of tiles, closing in from row 13 as the
// timer runs; the lower row takes the vertically flipped tiles.
void BattlePres_DrawTransitionRows(void)
{
    u32 i;
    s32 frame;
    s32 aligned;
    s32 tile;
    u32 row;
    u16 *tiles;

    frame = *(s32 *)Ram_HeapSlots->unknown_84[9];
    if (frame >= 0 && frame <= 79)
    {
        tile = (7 & frame) + 0xf081;
        if (frame >= 0)
            aligned = frame;
        else
            aligned = frame + 7;
        {
            /* FAKEMATCH: the native loads the tilemap's address after the 13 and before it subtracts the row from it; plainly the subtraction comes first (one reordered row, differ 2). Both values are pinned to the registers they already take and the barrier holds them until both are loaded. Measured against it: the base in a local, in the pointer first, as an array index, and pinned alone with and without the barrier. */
            register s32 start asm("r2") = 13;
            /* FAKEMATCH: the second pin of the barrier above. */
            register u32 base asm("r5") = 0x06006000;

            /* FAKEMATCH: the barrier of the two pins above. */
            __asm__("" : "+l"(start), "+l"(base));
            row = start - (aligned >> 3);
            i = 0;
            tiles = (u16 *)((row << 6) + base);
        }
        do
        {
            i++;
            *tiles = tile;
            tiles++;
        } while (i != 32);

        tile |= 0x800;
        aligned = frame;
        if (frame < 0)
            aligned += 7;
        row = (aligned >> 3) + 13;
        if (row <= 20)
        {
            i = 0;
            tiles = (u16 *)((row << 6) + 0x06006000);
            do
            {
                i++;
                *tiles = tile;
                tiles++;
            } while (i != 32);
        }
    }
}
