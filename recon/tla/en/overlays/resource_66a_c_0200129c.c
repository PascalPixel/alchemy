// Near miss, differ 51 of 61: as Func_02001230, the native rebuilds the
// 0xffff terminator at each test (its first move sits before the jump to
// the test) where the draft hoists it into ip.
#include "../../../../games/tla/src/scene/alhafran_cave/local.h"

void Func_0200129c(const u16 *entry, s32 mode, s32 frame) {
  struct AlhafranCaveEffectWork *work = Ram_HeapSlots->effectWork;

  for (; entry[0] != 0xffff; entry += 3) {
    s32 id = work->objectId;

    if (id == entry[0]) {
      struct ObjectRuntime *object = Object_GetById(id);
      u16 kind = entry[1];
      u16 flag = entry[2];

      if (frame <= 7)
        object->scaleY -= 0x1f00;
      if (mode == 1) {
        GameFlag_SetBit(flag);
        Func_02001320(id, kind, flag, object->terrainId);
        object->scaleY = 0x10000;
      }
      return;
    }
  }
}
