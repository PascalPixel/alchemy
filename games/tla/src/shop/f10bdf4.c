#include "item_icon.h"

void Func_0810bdf4(void) {
  struct IconWork *work = Ram_HeapSlots->menuRuntime;
  s32 i;

  for (i = ICON_COUNT - 1; i >= 0; i--)
    work->resources[i] = ICON_NONE;
  // The zero in a local of its own sets the registers of the loop before it;
  // as a plain 0, 28 of 29 instructions differ in tla-en.
  {
    s32 value = 0;
    work->active = value;
  }
  Task_Add(Func_0810bdb0, TASK_PASS_VBLANK + 10);
}
