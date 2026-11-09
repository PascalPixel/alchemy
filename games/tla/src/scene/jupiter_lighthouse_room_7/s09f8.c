#include "jupiter_lighthouse_room_7.h"
#include "scene_tables.h"
#include "event_inline.h"

void Func_020009f8(s32 id, s32 x, s32 z, s32 dx, s32 dz) {
  struct Actor *object = Actor_Get(id);

  Actor_SetPosition(id, (x << 20) + PIXELS(8),
                                         (z << 20) + PIXELS(8));
  object->motionFlags = 3;
  object->terrainHeight = PIXELS(-16);
  object->y = PIXELS(-16);
  object->velocityY = PIXELS(8);
  Actor_SetSpeed(id, 0x19999, 0xcccc);
  Actor_SetDestinationOffset(id, dx, dz);
  Event_PlaySound(222);
}
