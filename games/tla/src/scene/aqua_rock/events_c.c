#include "local.h"

void Func_02000d64(struct Actor *object, s32 limit) {
  s32 frames = 60;

  while (frames != 0) {
    wait(1);
    frames--;
    if (object->y <= limit)
      break;
  }
}
