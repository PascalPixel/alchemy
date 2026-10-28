#include "work9c.h"
#include "mem.h"
#include "resource.h"
#include "task.h"
#include "dma.h"

// XXX: what the work block in heap slot 39 holds, and what this family does;
// its callers and the block's readers would settle it.

extern const u8 gData_080f0c04[];

void Func_080d8a00(void);
void Func_080d8c10(void);

void Func_080d897c(void) {
  struct Work9c *work = Runtime_AllocateBlock(156, sizeof(struct Work9c));
  void *buffer = MemAlloc(0x100);

  Dma_Fill32(0, work, sizeof(struct Work9c));
  work->vramEntry = Resource_FindFreeEntry();
  Resource_DecodeType01(gData_080f0c04, buffer);
  VramCache_Load(work->vramEntry, 0x100, buffer);
  Task_Add(Func_080d8a00, TASK_PASS_VBLANK);
  Task_Add(Func_080d8c10, TASK_PASS_VBLANK + 15);
  MemFree(buffer);
}
