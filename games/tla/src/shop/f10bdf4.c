#include "item_icon.h"

void Func_0810bdf4(void) {
  struct IconWork *work = Ram_HeapSlots->menuRuntime;
  s32 i;

  for (i = ICON_COUNT - 1; i >= 0; i--)
    work->resources[i] = ICON_NONE;
  /* FAKEMATCH(temporary): `0` in a local of its own; as the plain expression,
   * 28 of 29 instructions differ in tla-en. */
  {
    s32 value = 0;
    work->active = value;
  }
  Task_Add(Func_0810bdb0, TASK_PASS_VBLANK + 10);
}
