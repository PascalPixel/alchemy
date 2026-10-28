#include "types.h"
#include "mem.h"
#include "dma.h"
#include "task.h"
#include "io_reg.h"
#include "resource.h"
#include "ram_buffer.h"
#include "system.h"

#define AFFINE_EFFECT_SLOT 232
void Func_0804cda8(void);

void *AffineEffect_InitializeWork(void) {
  void *work = Runtime_AllocateBlock(AFFINE_EFFECT_SLOT, 152);

  Dma_Fill32(0, work, 152);
  Task_Add((TaskFunc)Func_0804cda8, TASK_PASS_VBLANK - 10);
  return work;
}

#define FIELD_AT_OFFSET(base, type, offset) (*(type *)((u8 *)(base) + (offset)))
void UiWork_Finalize(struct Work *work, s32 release);

void Menu_EndResourceSelection(void) {
  struct Work *child;
  s32 i;
  u16 *entry;
  void *work;

  work = Ram_HeapSlots->menuSelectWork;
  Task_Remove(Func_0804cda8);
  child = FIELD_AT_OFFSET(work, struct Work *, 0x78);
  if (child != 0) {
    UiWork_Finalize(child, 2);
  }
  i = 0;
  while (i < (s32)FIELD_AT_OFFSET(work, s16, 0x8E)) {
    entry = (u16 *)((u8 *)work + 0x12) + i * 10;
    Resource_ResetEntry(*entry);
    i += 1;
  }
  Runtime_ReleaseHeapBlock(232);
  WaitFrames(1U);
}
