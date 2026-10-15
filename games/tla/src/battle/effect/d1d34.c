#include "types.h"
#include "dma.h"

s32 Scheduler_SuspendTasks();
void Func_080d1ae4();

void Func_080d1d34(void) { Scheduler_SuspendTasks(Func_080d1ae4); }

s32 Scheduler_ResumeTasks();

void Func_080d1d44(void) { Scheduler_ResumeTasks(Func_080d1ae4); }

void *Runtime_AllocateHeapBlock();
void Runtime_ReleaseHeapBlock();
void VramCache_Load();

void Func_080d1d54(s32 solid) {
  void *buffer = Runtime_AllocateHeapBlock(56, 0x400);

  if (solid == 0) {
    Dma_Fill32(0, buffer, 0x200);
  } else {
    Dma_Fill32(0x01010101, buffer, 0x200);
  }
  VramCache_Load(94, 0x200, buffer);
  Runtime_ReleaseHeapBlock(56);
}
