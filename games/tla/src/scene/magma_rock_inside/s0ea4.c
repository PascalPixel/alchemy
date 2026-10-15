#include "local.h"

void Func_02000ea4(struct ObjectRuntime *object) {
  s32 dx, dy, dz;

  object->x += dx = object->bounce;
  object->y += dy = object->gravity;
  object->z += dz = object->unk4c;
  dx -= dx >> 3;
  dy -= dy >> 3;
  dz -= dz >> 3;
  object->bounce = dx;
  object->gravity = dy;
  object->unk4c = dz;
}
