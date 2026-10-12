/* Near miss, differ 2 of 61: the native loads the workspace (ldr r3) before it starts the summary offset (movs r2, #192); this form starts the offset first. An empty asm barrier after the load puts the pair right but holds the prologue's sub sp before it, which the native schedules after the offset (differ 2 again). */
#include "types.h"
#include "ram_buffer.h"
#include "save_state.h"
#include "dma.h"
#include "flash.h"

s32 SaveState_ReadSummaries(void)
{
    struct SaveWorkspace *work = Ram_HeapSlots->serial_work;
    struct SaveSummary *summary = work->summary;
    s32 count = 0;
    u32 i;

    for (i = 0; i <= 2; i++) {
        volatile u32 zero;
        u32 slot;

        zero = 0;
        Dma_Set(&zero, summary, 0x85000010, (volatile u32 *)0x040000d4);
        slot = SaveState_FindLatestSlot(i);
        if (slot <= 14) {
            ReadFlash(slot, 0, (u8 *)summary, 64);
            ReadFlash(slot + 1, 16, (u8 *)summary + 56, 4);
            count++;
        }
        summary++;
    }
    return count;
}
