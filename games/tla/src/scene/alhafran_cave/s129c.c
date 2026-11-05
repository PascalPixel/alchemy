#include "local.h"

void Func_0200129c(const u16 *entry, s32 mode, s32 frame) {
  struct AlhafranCaveEffectWork *work = Ram_HeapSlots->effectWork;
  while (entry[0] != 0xffff) {
    u32 value = *entry;
    s32 id = work->objectId;

    if (id == value) {
      struct Actor *object = Actor_GetById(id);
      s32 kind;
      s32 flag;

      entry++;
      kind = entry[0];
      flag = entry[1];

      if (frame <= 7)
        object->scaleY -= 0x1f00;
      if (mode == 1) {
        GameFlag_SetBit(flag);
        Func_02001320(id, kind, flag, object->terrainId);
        object->scaleY = 0x10000;
      }
      return;
    }
    entry += 3;
  }
}
