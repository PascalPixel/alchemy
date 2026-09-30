#include "TYPES.H"
#include "GLOBAL_CELLS.H"
#include "DMA.H"
#include "RUNTIME_MEM.H"

extern u8 Data_03001400[];

#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

void Runtime_PushSlotEntry(s32 *slot_entry, s32 slot)
{
    s32 *previous_head;
    s32 slot_offset;
    s32 clamped_slot;

    clamped_slot = slot;
    if (clamped_slot > 0xFF) {
        clamped_slot = 0xFF;
    }
    if (clamped_slot < 0) {
        clamped_slot = 0;
    }
    slot_offset = clamped_slot * 4;
    previous_head = FIELD_AT_OFFSET(slot_offset, s32 **, ((u32)&Data_03001400));
    FIELD_AT_OFFSET(slot_offset, s32 **, ((u32)&Data_03001400)) = slot_entry;
    *slot_entry = previous_head;
}

extern const u8 Render_BuildOamList[];

typedef void (*LoadedRoutine)(void *argument);

/* Linker-resolved absolute size of the routine copied into the heap. */
extern u8 LoadedRuntime_Size[];

void Runtime_CopyAndCallRoutine(void *argument)
{
    u32 size;
    LoadedRoutine routine;

    /*
     * FAKEMATCH: a loop that runs once around the size load. It is a
     * scheduling barrier: without it the size literal is loaded before the
     * argument is copied to r8.
     */
    do {
        size = (u32)LoadedRuntime_Size;
    } while (0);
    routine = (LoadedRoutine)Runtime_BumpAllocate(size);
    Dma_Set((const void *)Render_BuildOamList, (void *)routine,
            (size >> 2) | 0x84000000, (volatile u32 *)0x040000d4);
    routine(argument);
    Sys_Free((void *)routine);
}
