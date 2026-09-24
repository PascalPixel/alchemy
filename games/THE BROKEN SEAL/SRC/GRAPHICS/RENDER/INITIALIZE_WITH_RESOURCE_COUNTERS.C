#include "TYPES.H"
#include "DMA.H"

/* The UI work block's counter limit. A member store keeps the halfword
   constant an immediate; a cast store sends it to the literal pool. */
struct UiWorkCounter {
    u8 unk_0000[0x12b6];
    u16 limit;
};

extern u8 *Runtime_AllocateBlock(s32, u32);
extern void UiWork_InitFreeList(void);
extern s32 Scheduler_AddOrUpdateCallback(void *, s32);
extern void UiWork_SetTwoEntriesTo999(void);
extern void UiWork_InitCountersWithResourceAndScheduleRefresh(void);
extern void UiWork_UploadDirtyBlocks(void);

/* Allocates and clears the UI work block, fills its tile map with blank
   entries and schedules the block upload, as UiWork_Initialize does, but
   seeds the counters from the resource table. */
void UiWork_InitializeWithResourceCounters(void)
{
    u8 *work;
    volatile u32 fill;

    work = Runtime_AllocateBlock(15, 0x12fc);
    fill = 0;
    Dma_Set((const void *)&fill, work, 0x850004bf, (volatile u32 *)0x040000d4);
    work[0xea3] = 1;
    ((struct UiWorkCounter *)work)->limit = 99;
    work[0xea7] = 15;
    fill = 0xf000f000;
    Dma_Set((const void *)&fill, work, 0x85000140, (volatile u32 *)0x040000d4);
    UiWork_InitFreeList();
    UiWork_SetTwoEntriesTo999();
    Scheduler_AddOrUpdateCallback(UiWork_UploadDirtyBlocks, 0x480);
    UiWork_InitCountersWithResourceAndScheduleRefresh();
}
