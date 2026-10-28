#include "jupiter_lighthouse_room_7.h"
#include "scene_tables.h"

void Func_020009f8(s32 id, s32 x, s32 z, s32 dx, s32 dz) {
  struct Actor *object = Actor_GetById(id);

  Func_02002ba4(id, (x << 20) + PIXELS(8), (z << 20) + PIXELS(8));
  object->motionFlags = 3;
  object->terrainHeight = PIXELS(-16);
  object->y = PIXELS(-16);
  object->velocityY = PIXELS(8);
  Call3((void (*)())Actor_SetSpeedParameters, id, 0x19999, 0xcccc);
  Actor_OffsetPositionAndResetMotion(id, dx, dz);
  Func_02002c5c(222);
}
