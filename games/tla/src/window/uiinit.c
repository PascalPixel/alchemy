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
#include "heap_release.h"
#include "window.h"
#include "task.h"
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

void WindowSystem_InitField(void) {
  struct WindowWork *work;

  work = (struct WindowWork *)HeapSlot_AllocEwram(
      60, sizeof(struct WindowWork));
  Iwram_ClearWords(work, sizeof(struct WindowWork));
  work->dirty = 1;
  work->glyphResource = 99;
  work->level = 15;
  FillWords(Iwram_FillWords, work->tilemap, sizeof(work->tilemap), 0xf000f000);
  WindowSprite_InitializeFreeList();
  Portrait_ClearSlots();
  Task_Add((s32)WindowSystem_UploadTilemap, TASK_PASS_FRAME);
  WindowSystem_StartFieldUpdate();
}

void WindowSystem_InitBattle(s32 kind) {
  struct WindowWork *work;
  s32 i;
  s32 WindowSystem_CopyTileLowerRows(u32 src, u32 dst) {
    src &= 0x3ff;
    dst &= 0x3ff;
    Dma_Set((void *)(0x06000010 + src * 32), (void *)(0x06000000 + dst * 32),
            DMA_COPY_HALFWORDS(8), REG_DMA3);
    return Iwram_ClearWords((void *)(0x0600000c + dst * 32), 20);
  }

  work = (struct WindowWork *)HeapSlot_AllocEwram(
      60, sizeof(struct WindowWork));
  Iwram_ClearWords(work, sizeof(struct WindowWork));
  work->dirty = 1;
  work->glyphResource = 99;
  work->menuState = 1;
  work->level = 15;
  Iwram_FillWords(work->tilemap, sizeof(work->tilemap), 0xf000f000);
  WindowSprite_InitializeFreeList();
  Task_Add((s32)WindowSystem_UploadTilemap, TASK_PASS_FRAME);
  WindowSystem_StartBattleUpdate(kind);
  WindowSystem_CopyTileLowerRows(0xf013, 128);
  WindowSystem_CopyTileLowerRows(0xf014, 129);
  WindowSystem_CopyTileLowerRows(0xf015, 130);
  {
    u8 mode = 4;

    for (i = 2; i >= 0; i--)
      work->tileAttributes[i] = mode;
  }
}
