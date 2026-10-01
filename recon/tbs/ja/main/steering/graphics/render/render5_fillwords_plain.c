/* NONMATCHING: 2026-10-01 brief Wave2 FillWords plain-source attempt.
 * Removing this one source device changes UiWork_InitializeWithResourceCounters, UiWork_Initialize.
 * First remaining difference: UiWork_InitializeWithResourceCounters: ldr	r3, .L0+20 => ldr	r2, .L0+20 (52/52 assembly lines).
 * Measured with the existing TBS agscc option set, JA edition; no option changes.
 * This reduced draft preserves the affected function and its declarations.
 * Production retains the measured device with its FAKEMATCH reason.
 */
#include "TYPES.H"
#include "DMA.H"
#include "TBS_EDITION.H"
#include "IWRAM_CALL.H"

/* The UI work block's counter limit. A member store keeps the halfword
   constant an immediate; a cast store sends it to the literal pool. */
struct UiWorkCounter {
    u8 unk_0000[RENDER_COUNTER_OFS];
    u16 limit;
};

extern u8 *Runtime_AllocateBlock(s32, u32);
extern void UiWork_InitFreeList(void);
extern s32 Scheduler_AddOrUpdateCallback(void *, s32);
extern void UiWork_SetTwoEntriesTo999(void);
extern void UiWork_InitCountersWithResourceAndScheduleRefresh(void);
extern void UiWork_UploadDirtyBlocks(void);

extern void UiWork_InitCountersAndScheduleRefresh(s32);

#if defined(TBS_EDITION_JA)
typedef s32 (*WordFillFn)(void *dst, s32 size, u32 value);

/* FAKEMATCH: passing the routine through an inline wrapper loads its fixed
   address before the fill value, the order of the Japanese literal pool; a
   plain call loads the value first. */

#endif

/* Allocates and clears the UI work block, fills its tile map with blank
   entries and schedules the block upload, as UiWork_Initialize does, but
   seeds the counters from the resource table. */
void UiWork_InitializeWithResourceCounters(void)
{
    u8 *work;
#if !defined(TBS_EDITION_JA)
    volatile u32 fill;
#endif

    work = Runtime_AllocateBlock(15, RENDER_WORK_SIZE);
#if defined(TBS_EDITION_JA)
    /* The Japanese build clears and fills the block with the IWRAM word
       routines rather than DMA. */
    Iwram_ClearWords(work, RENDER_WORK_SIZE);
#else
    fill = 0;
    Dma_Set((const void *)&fill, work, 0x850004bf, (volatile u32 *)0x040000d4);
#endif
    work[RENDER_DIRTY_OFS] = 1;
    ((struct UiWorkCounter *)work)->limit = 99;
    work[RENDER_LEVEL_OFS] = 15;
#if defined(TBS_EDITION_JA)
    Iwram_FillWords(work, 0x500, 0xf000f000);
#else
    fill = 0xf000f000;
    Dma_Set((const void *)&fill, work, 0x85000140, (volatile u32 *)0x040000d4);
#endif
    UiWork_InitFreeList();
    UiWork_SetTwoEntriesTo999();
    Scheduler_AddOrUpdateCallback(UiWork_UploadDirtyBlocks, 0x480);
    UiWork_InitCountersWithResourceAndScheduleRefresh();
}

void UiWork_Initialize(s32 kind)
;