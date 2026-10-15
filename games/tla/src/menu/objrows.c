#include "types.h"
#include "ram_buffer.h"

void Scheduler_RemoveCallback(s32);
void Func_08043b70(void);
void ResourceObject_ReleaseFar(void *);

void Menu_ClearFirstObjectRowAndScheduleUpdate(void) {
  u8 *base = (u8 *)Ram_HeapSlots->menuRuntime;
  s32 offset;
  s32 zero;
  s32 count;

  Scheduler_RemoveCallback((s32)Func_08043b70);
  zero = 0;
  offset = 248;
  count = 3;
  do {
    void *entry = *(void **)(offset + (unsigned int)base);

    if (entry != 0) {
      ResourceObject_ReleaseFar(entry);
      *(s32 *)(offset + (unsigned int)base) = zero;
    }
    count--;
    offset += 4;
  } while (count >= 0);
}
