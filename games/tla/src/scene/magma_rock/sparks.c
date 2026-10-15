#include "types.h"
#include "ram_buffer.h"
#include "resource.h"
#include "mem.h"

struct MagmaRockSparkWork {
  u8 unk0000[0x1604];
  s16 resource;
};

void Scheduler_RemoveCallbackFar(void (*callback)(void));
void Func_0200379c(void);

void Func_02003998(void) {
  struct MagmaRockSparkWork *work = Ram_HeapSlots->menuRuntime;

  Scheduler_RemoveCallbackFar(Func_0200379c);
  Resource_ResetEntry(work->resource);
  Runtime_ReleaseHeapBlock(220);
}
