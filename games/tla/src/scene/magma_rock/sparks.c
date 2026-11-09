#include "types.h"
#include "ram_buffer.h"
#include "resource.h"
#include "heap_release.h"
#include "event_inline.h"

struct MagmaRockSparkWork {
  u8 unk0000[0x1604];
  s16 resource;
};

void Func_0200379c(void);

void Func_02003998(void) {
  struct MagmaRockSparkWork *work = Ram_HeapSlots->menuRuntime;

  FieldTask_Remove(Func_0200379c);
  Resource_ResetEntry(work->resource);
  HeapSlot_Release(220);
}
