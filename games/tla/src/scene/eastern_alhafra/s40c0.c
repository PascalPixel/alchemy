#include "types.h"
#include "ram_buffer.h"
#include "resource.h"
#include "heap_release.h"
#include "event_inline.h"

struct EasternAlhafraSparkWork {
  u8 unk0000[0x1604];
  s16 resource;
};

void Func_02003ec4(void);

void Func_020040c0(void) {
  struct EasternAlhafraSparkWork *work = Ram_HeapSlots->menuRuntime;

  FieldTask_Remove(Func_02003ec4);
  Resource_ResetEntry(work->resource);
  HeapSlot_Release(220);
}
