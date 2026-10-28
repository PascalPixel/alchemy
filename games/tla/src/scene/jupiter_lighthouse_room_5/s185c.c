#include "types.h"
#include "ram_buffer.h"

struct SparkWork {
  u8 unk0000[0x1d9c];
  s16 resource;
};

void Task_RemoveFar(void (*callback)(void));
void Resource_ResetEntry(s32 entry);
void Runtime_ReleaseHeapBlock(s32 slot);
void Func_02001538(void);

void Func_0200185c(void) {
  struct SparkWork *work = Ram_HeapSlots->menuRuntime;

  Task_RemoveFar(Func_02001538);
  Resource_ResetEntry(work->resource);
  Runtime_ReleaseHeapBlock(220);
}
