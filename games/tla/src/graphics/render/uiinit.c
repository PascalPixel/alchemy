#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "mem.h"
#include "window.h"
#include "callback_scheduler.h"
#include "dma.h"
#include "iwram_call.h"
#include "io_reg.h"

typedef s32 (*WordFillFn)(void *dst, s32 size, u32 value);

static __inline__ s32 FillWords(WordFillFn fill, void *dst, s32 size,
                                u32 value) {
  /* FAKEMATCH: a direct call loads the fill value before the routine and
   * reverses pool order, as in ☀️'s Japanese build. */
  return fill(dst, size, value);
}

void UiWork_InitializeWithResourceCounters(void) {
  struct UiRenderWork *work;

  work = (struct UiRenderWork *)Runtime_AllocateBlock(
      60, sizeof(struct UiRenderWork));
  Iwram_ClearWords(work, sizeof(struct UiRenderWork));
  work->dirty = 1;
  work->glyphResource = 99;
  work->level = 15;
  FillWords(Iwram_FillWords, work->tilemap, sizeof(work->tilemap), 0xf000f000);
  UiWork_InitFreeList();
  UiWork_ClearPortraitSlots();
  Scheduler_AddOrUpdateCallback((s32)UiWork_UploadDirtyBlocks, 0xc80);
  UiWork_InitCountersWithResourceAndScheduleRefresh();
}

void UiWork_Initialize(s32 kind) {
  struct UiRenderWork *work;
  s32 i;
  s32 UiWork_CopyTileTail(u32 src, u32 dst) {
    src &= 0x3ff;
    dst &= 0x3ff;
    Dma_Set((void *)(0x06000010 + src * 32), (void *)(0x06000000 + dst * 32),
            0x80000008, REG_DMA3);
    return Iwram_ClearWords((void *)(0x0600000c + dst * 32), 20);
  }

  work = (struct UiRenderWork *)Runtime_AllocateBlock(
      60, sizeof(struct UiRenderWork));
  Iwram_ClearWords(work, sizeof(struct UiRenderWork));
  work->dirty = 1;
  work->glyphResource = 99;
  work->menuState = 1;
  work->level = 15;
  Iwram_FillWords(work->tilemap, sizeof(work->tilemap), 0xf000f000);
  UiWork_InitFreeList();
  Scheduler_AddOrUpdateCallback((s32)UiWork_UploadDirtyBlocks, 0xc80);
  UiWork_InitCountersAndScheduleRefresh(kind);
  UiWork_CopyTileTail(0xf013, 128);
  UiWork_CopyTileTail(0xf014, 129);
  UiWork_CopyTileTail(0xf015, 130);
  {
    u8 mode = 4;

    for (i = 2; i >= 0; i--)
      work->tileAttributes[i] = mode;
  }
}
