#include "local.h"

void Func_020000f0(s32 unused, s32 id) {
  struct Actor *object = Actor_Get(id);
  s32 x = object->x >> 20;

  if (x == 38) {
    Event_SetFlag(0x9a8);
    object->priorityFlags = 0;
    Func_02000098(id);
    MapGrid_CopyCellAttributes(40, 19, 1, 1, x, 19);
  }
  Func_02001060_(99, 6, 4, 3, 99, 18);
}
