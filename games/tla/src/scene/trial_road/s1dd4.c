#include "trial_road.h"
#include "event_inline.h"

extern const u8 gData_02005b44[];

void Func_02001dd4(void) {
  struct Actor *partner = Actor_GetById(8);
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  s32 dx;
  s32 dz;
  s32 near;
  s32 far;
  s32 y;

  if (GameFlag_Test(648))
    return;
  dx = (leader->x - partner->x) >> 16;
  dz = (leader->z - partner->z) >> 16;
  near = dx * dx + dz * dz;
  dx = (leader->x >> 16) - 296;
  dz = (leader->z >> 16) - 248;
  far = dx * dx + dz * dz;
  if (far <= 6400 || near <= 2304)
    Actor_SetMoveTarget(partner, (296 - dx) << 16, partner->y,
                        (248 - dz) << 16);
  if (far > 15)
    return;
  Sound_Play(146);
  Sound_Play(122);
  partner->speedLimit = 0x20000;
  partner->acceleration = 0x20000;
  partner->motionFlags = 2;
  partner->terrainHeight = 0;
  partner->collisionFlags = 1;
  partner->velocityY = 0xa0000;
  Actor_SetMovementTypeById(8, gData_02005b44);
  y = partner->y;
  Actor_SetMoveTarget(partner, 344 << 16, y, 264 << 16);
  GameFlag_SetBit(648);
}
