#include "item_icon.h"

void Func_0810be3c(void) {
  struct IconWork *work = Ram_HeapSlots->menuRuntime;
  s32 i;

  Task_Remove(Func_0810bdb0);
  for (i = 0; i < ICON_COUNT; i++) {
    if (work->resources[i] != ICON_NONE)
      Resource_ResetEntry(work->resources[i]);
  }
}

void Func_0810be70(s32 index, s32 x, s32 y) {
  struct IconWork *work = Ram_HeapSlots->menuRuntime;

  work->sprites[index].attr.x = x;
  work->sprites[index].attr.y = y;
}

void Func_0810bea8(s32 active) {
  struct IconWork *work = Ram_HeapSlots->menuRuntime;

  work->active = active;
}
