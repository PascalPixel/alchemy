/* ☀️'s DisplayTransition_FillTilemapAndSolidTile over ⚓️'s heap slot 31: differ 2, the frame's sub sp comes before the work and fill loads instead of after them. Holding the fill in a register local, declaring it first or copying the colour did not move it. It would head ⚓️'s PALETTE5. */
#include "TYPES.H"
#include "RUNTIME_MEM.H"
#include "DISPTRAN.H"
#include "DMA.H"
#include "SYSTEM.H"
#include "CALLBACK_SCHEDULER.H"
#include "IWRAM_CALL.H"
#include "RAM_BUFFER.H"
void DisplayTransition_FillTilemapAndSolidTile(s32 color)
{
    struct DisplayTransitionState *work = Ram_HeapSlots->display_transition;
    volatile u32 fill = 0xf000f000;
    Dma_Set(&fill, (void *)0x06002000, 0x85000140, (volatile u32 *)0x040000d4);
    if (color != -1) {
        u32 pattern = 0;
        s32 cnt;
        u32 *tile;
        for (cnt = 7; cnt >= 0; --cnt) pattern = (pattern << 4) | color;
        tile = work->solid_tile;
        for (cnt = 7; cnt >= 0; --cnt) *tile++ = pattern;
        Dma_Set(work->solid_tile, (void *)0x06000000, 0x84000008, (volatile u32 *)0x040000d4);
    }
}
