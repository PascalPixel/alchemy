#include "worka0.h"
#include "mem.h"
#include "resource.h"
#include "callback_scheduler.h"
#include "dma.h"

extern const u8 gData_080f0c04[];

void Func_080d9b80(void);
void Func_080d9d40(void);

void Func_080d9b08(void) {
  struct WorkA0 *work = Runtime_AllocateBlock(160, sizeof(struct WorkA0));
  void *buffer = MemAlloc(0x900);

  Dma_Fill32(0, work, sizeof(struct WorkA0));
  work->vramEntry = Resource_FindFreeEntry();
  Resource_DecodeType01(gData_080f0c04, buffer);
  VramBlock_LoadCached(work->vramEntry, 64, buffer);
  Scheduler_AddOrUpdateCallback(Func_080d9b80, 0x480);
  Scheduler_AddOrUpdateCallback(Func_080d9d40, 0x48f);
  MemFree(buffer);
}
