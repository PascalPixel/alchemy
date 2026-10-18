#include "types.h"
#include "macros.h"

void Camera_WorldToScreen(s32 *position);
void Func_080eb01c(void *sprite);

void Func_080d1df0(s32 *sprite, s32 x, s32 y, s32 z) {
  s32 position[3];

  SET_VECTOR(position, x, y, z);
  Camera_WorldToScreen(position);
  sprite[3] = position[0];
  sprite[4] = position[2];
  Func_080eb01c(sprite);
}
