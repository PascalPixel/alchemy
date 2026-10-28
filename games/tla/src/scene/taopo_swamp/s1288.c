#include "local.h"

void Func_02001288(void) {
  if ((gFrameCount & 7) == 0 && (u32)Random16Far() < 0x4000) {
    s32 x = (Random16Far() << 5) + 0x2080000;
    s32 z = 0xb00000 - (Random16Far() << 3);
    s32 y = 0xa00000;
    struct Actor *object = Func_02002808(0x12d, x, y, z);
    s32 scale = 0x547a;

    if (object != NULL) {
      object->scaleX = scale;
      object->scaleY = scale;
      object->gravity = 0x7333;
      Func_02002800(object, gData_02002a54);
      Actor_SetSingleChildFlags(object, 0);
      Func_02002880(object, 3);
    }
  }
}
