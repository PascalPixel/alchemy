#include "types.h"
#include "ram_buffer.h"
#include "resource.h"
#include "mem.h"

struct EasternAlhafraSparkWork {
  u8 unk0000[0x1604];
  s16 resource;
};

void Task_RemoveFar(void (*callback)(void));
void Func_02003ec4(void);

void Func_020040c0(void) {
  struct EasternAlhafraSparkWork *work = Ram_HeapSlots->menuRuntime;

  Task_RemoveFar(Func_02003ec4);
  Resource_ResetEntry(work->resource);
  HeapSlot_Release(220);
}
