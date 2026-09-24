#include "TYPES.H"
#include "DMA.H"

extern u8 *Runtime_AllocateBlock(s32, u32);
extern void UiWork_InitFreeList(void);
extern void Scheduler_AddOrUpdateCallback(void *, s32);
extern void UiWork_InitCountersAndScheduleRefresh(s32);
extern void UiWork_UploadDirtyBlocks(void);

void UiWork_Initialize(s32 kind)
{
    u8 *work;
    volatile u32 fill;
    s32 i;
    s32 value;
    u16 *half;
    s32 UiWork_CopyTileTail(u32 src, u32 dst)
    {
        src &= 0x3ff;
        dst &= 0x3ff;
        Dma_Set((void *)(0x06000010 + src * 32),
                (void *)(0x06000000 + dst * 32),
                0x80000008, (volatile u32 *)0x040000d4);
        return ((s32 (*)(void *, s32))0x03000164)
            ((void *)(0x0600000c + dst * 32), 20);
    }

    work = Runtime_AllocateBlock(15, 0x12fc);
    fill = 0;
    Dma_Set(&fill, work, 0x850004bf, (volatile u32 *)0x040000d4);
    work[0xea3] = 1;
    /* FAKEMATCH: the 99 goes through an s32 local and a u16 pointer so it is a movs, not a halfword pool constant */
    half = (u16 *)(work + 0x12b6);
    value = 99;
    *half = value;
    work[0xea5] = 1;
    work[0xea7] = 15;
    fill = 0xf000f000;
    Dma_Set(&fill, work, 0x85000140, (volatile u32 *)0x040000d4);
    UiWork_InitFreeList();
    Scheduler_AddOrUpdateCallback(UiWork_UploadDirtyBlocks, 0x480);
    UiWork_InitCountersAndScheduleRefresh(kind);
    UiWork_CopyTileTail(0xf013, 128);
    UiWork_CopyTileTail(0xf014, 129);
    UiWork_CopyTileTail(0xf015, 130);
    {
        u8 mode = 4;

        for (i = 2; i >= 0; i--)
            work[i + 0xda0] = mode;
    }
}
