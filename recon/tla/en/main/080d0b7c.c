/* Near miss, differ 2: the listing loads the work pointer and the fill word
 * and then makes the frame (ldr r5; ldr r3; sub sp, #4); this draft makes
 * the frame first. Tried: the fill word as a forced temporary (frame between
 * the two loads, differ 2), an empty asm barrier before the fill store
 * (differ 4). */
#include "types.h"
#include "mem.h"
#include "disptran.h"
#include "dma.h"
#include "system.h"
#include "callback_scheduler.h"
#include "iwram_call.h"
#include "ram_buffer.h"
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
