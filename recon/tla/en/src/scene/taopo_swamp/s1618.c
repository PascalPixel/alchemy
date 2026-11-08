#include "../../../../../../games/tla/src/scene/taopo_swamp/local.h"

void Func_02001618(void) {
  if ((gFrameCount & 63) == 0) {
    s32 x = 0x1980000;
    s32 z = 0x1b80000;
    s32 y = 0xa00000;
    struct Actor *object = Func_02002808(0x12d, x, y, z);
    s32 scale = SPLIT_CONSTANT(0x5400, 122);

    if (object != NULL) {
      object->scaleX = scale;
      object->scaleY = scale;
      object->gravity = SPLIT_CONSTANT(0x7300, 51);
      Func_02002800(object, gData_02002a60);
      Actor_SetSingleChildFlags(object, 0);
    }
  }
}
