#include "worka0.h"
#include "heap_release.h"
#include "resource.h"
#include "task.h"
#include "dma.h"

// XXX: what the work block in heap slot 40 holds, and what this family does;
// its callers and the block's readers would settle it.

extern const u8 gData_080f0c04[];

void Func_080d9b80(void);
void Func_080d9d40(void);

void Func_080d9b08(void) {
  struct WorkA0 *work = HeapSlot_AllocEwram(160, sizeof(struct WorkA0));
  void *buffer = Heap_Alloc(0x900);

  Dma_Fill32(0, work, sizeof(struct WorkA0));
  work->vramEntry = Resource_FindFreeEntry();
  Resource_DecodeType01(gData_080f0c04, buffer);
  VramCache_Load(work->vramEntry, 64, buffer);
  Task_Add(Func_080d9b80, TASK_PASS_VBLANK);
  Task_Add(Func_080d9d40, TASK_PASS_VBLANK + 15);
  Heap_Release(buffer);
}
