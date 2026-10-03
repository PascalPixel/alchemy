#include "RUNTIME_MEM.H"
#include "EDITION.H"
#include "TYPES.H"
#include "WINDOW.H"
#include "CALLBACK_SCHEDULER.H"
#include "DMA.H"
#include "TBS_EDITION.H"
#include "IWRAM_CALL.H"

extern void UiWork_InitFreeList(void);
extern void UiWork_SetTwoEntriesTo999(void);
extern void UiWork_InitCountersWithResourceAndScheduleRefresh(void);
extern void UiWork_UploadDirtyBlocks(void);

extern void UiWork_InitCountersAndScheduleRefresh(s32);

#if !EDITION_INTERNATIONAL
typedef s32 (*WordFillFn)(void *dst, s32 size, u32 value);

static __inline__ s32 FillWords(WordFillFn fill, void *dst, s32 size, u32 value)
{
    /* FAKEMATCH: in Japanese, a direct call loads the fill value before the routine and reverses pool order. */
    return fill(dst, size, value);
}
#endif

/* Allocates and clears the UI work block, fills its tile map with blank
   entries and schedules the block upload, as UiWork_Initialize does, but
   seeds the counters from the resource table. */
void UiWork_InitializeWithResourceCounters(void)
{
    struct UiRenderWork *work;
#if EDITION_INTERNATIONAL
    volatile u32 fill;
#endif

    work = (struct UiRenderWork *)Runtime_AllocateBlock(15, RENDER_WORK_SIZE);
#if EDITION_INTERNATIONAL
    fill = 0;
    Dma_Set((const void *)&fill, work, 0x850004bf, (volatile u32 *)0x040000d4);
#else
    /* The Japanese build clears and fills the block with the IWRAM word
       routines rather than DMA. */
    Iwram_ClearWords(work, RENDER_WORK_SIZE);
#endif
    work->dirty = 1;
    work->glyph_resource = 99;
    work->level = 15;
#if EDITION_INTERNATIONAL
    fill = 0xf000f000;
    Dma_Set((const void *)&fill, work, 0x85000140, (volatile u32 *)0x040000d4);
#else
    FillWords(Iwram_FillWords, work, 0x500, 0xf000f000);
#endif
    UiWork_InitFreeList();
    UiWork_SetTwoEntriesTo999();
    Scheduler_AddOrUpdateCallback((s32)(UiWork_UploadDirtyBlocks), 0x480);
    UiWork_InitCountersWithResourceAndScheduleRefresh();
}

void UiWork_Initialize(s32 kind)
{
    struct UiRenderWork *work;
#if EDITION_INTERNATIONAL
    volatile u32 fill;
#endif
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
        return Iwram_ClearWords((void *)(0x0600000c + dst * 32), 20);
    }

    work = (struct UiRenderWork *)Runtime_AllocateBlock(15, RENDER_WORK_SIZE);
#if EDITION_INTERNATIONAL
    fill = 0;
    Dma_Set(&fill, work, (0x85000000 | RENDER_WORK_SIZE / 4), (volatile u32 *)0x040000d4);
#else
    /* The Japanese build clears and fills the block with the IWRAM word
       routines rather than DMA. */
    Iwram_ClearWords(work, RENDER_WORK_SIZE);
#endif
    work->dirty = 1;
    /* FAKEMATCH: the 99 goes through an s32 local and a u16 pointer so it is a movs, not a halfword pool constant */
    half = &work->glyph_resource;
    value = 99;
    *half = value;
    work->menu_state = 1;
    work->level = 15;
#if EDITION_INTERNATIONAL
    fill = 0xf000f000;
    Dma_Set(&fill, work, 0x85000140, (volatile u32 *)0x040000d4);
#else
    FillWords(Iwram_FillWords, work, 0x500, 0xf000f000);
#endif
    UiWork_InitFreeList();
    Scheduler_AddOrUpdateCallback((s32)(UiWork_UploadDirtyBlocks), 0x480);
    UiWork_InitCountersAndScheduleRefresh(kind);
    UiWork_CopyTileTail(0xf013, 128);
    UiWork_CopyTileTail(0xf014, 129);
    UiWork_CopyTileTail(0xf015, 130);
    {
        u8 mode = 4;

        for (i = 2; i >= 0; i--)
            work->tile_attributes[i] = mode;
    }
}
