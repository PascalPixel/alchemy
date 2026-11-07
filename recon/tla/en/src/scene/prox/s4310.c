#include "types.h"
#include "ram_buffer.h"

struct SparkWork {
  u8 unk0000[0x1604];
  s16 resource;
};

void Task_RemoveFar(void (*func)(void));
void Resource_ResetEntry(s32 entry);
void HeapSlot_Release(s32 slot);
void Func_02004114(void);

void Func_02004310(void) {
  struct SparkWork *work = Ram_HeapSlots->menuRuntime;

  Task_RemoveFar(Func_02004114);
  Resource_ResetEntry(work->resource);
  HeapSlot_Release(220);
}
