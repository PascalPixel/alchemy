#include "types.h"
#include "ram_buffer.h"
#include "resource.h"
#include "mem.h"

struct MagmaRockSparkWork {
  u8 unk0000[0x1604];
  s16 resource;
};

void Task_RemoveFar(void (*func)(void));
void Func_0200379c(void);

void Func_02003998(void) {
  struct MagmaRockSparkWork *work = Ram_HeapSlots->menuRuntime;

  Task_RemoveFar(Func_0200379c);
  Resource_ResetEntry(work->resource);
  HeapSlot_Release(220);
}
