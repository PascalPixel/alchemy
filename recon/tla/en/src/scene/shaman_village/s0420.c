#include "../../../../../../games/tla/src/scene/shaman_village/shaman_village.h"

void Func_02000420(void) {
  s32 *view = (s32 *)((u8 *)Ram_HeapSlots->mapWork + 376);

  view[2] -= 0x20000;

  view[3] += 0x20000;
  if (view[3] > 0x4100000) {
    view[2] = 0;
    view[3] = 0x4000000;
  }
}
