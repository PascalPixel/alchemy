#include "local.h"
#include "version.h"

void Func_0200061c(struct Actor *object);

void Func_02000644(s32 unused, s32 id) {
  struct Actor *object = Actor_Get(id);
  s32 x;

  if (id == 9) {
    x = object->x >> 20;
    if (x == 39) {
#if !TLA_PATCH_1
      object->motionFlags = 3;
      System_Wait(15);
      object->motionFlags = 0;
      Event_SetFlag(0x95c);
      MapGrid_CopyCellAttributes(41, 9, 1, 1, x, id);
#else
      Func_02003d7c();
      FieldEvent_ResolvePendingActions(0);
      Func_0200061c(object);
      Event_SetFlag(0x95c);
      MapGrid_CopyCellAttributes(41, 9, 1, 1, x, id);
      Func_02003d84();
#endif
    }
  }
}
