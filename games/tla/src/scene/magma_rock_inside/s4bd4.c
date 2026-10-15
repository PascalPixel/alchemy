#include "local.h"

struct MagmaRockInsideSparkWork {
  u8 unk0000[0x1604];
  s16 resource;
};

void Resource_ResetEntry(s32 entry);
void Runtime_ReleaseHeapBlock(s32 slot);
void Func_020049d8(void);

void Func_02004bd4(void) {
  struct MagmaRockInsideSparkWork *work = Ram_HeapSlots->menuRuntime;

  Scheduler_RemoveCallbackFar(Func_020049d8);
  Resource_ResetEntry(work->resource);
  Runtime_ReleaseHeapBlock(220);
}
