// Draft, differ 4 of 95: a Trial Road room's step check until flag 648 is set:
// object 8 mirrors the leader about (296, 248) while the leader is within 80
// pixels of that point or within 48 of the object; when the leader reaches the
// point (within 4 pixels), with sounds 146 and 122, the object hops away to
// (344, 264) on its action script and the flag is set.
#include "../../../../games/tla/src/scene/trial_road/trial_road.h"

extern const u8 gData_02005b44[];

void Func_020057f8(struct ObjectRuntime *object, s32 x, s32 y, s32 z);
void ObjectMotion_EnableActionAndSetCallback(s32 id, const void *script);

void Func_02001dd4(void) {
  struct ObjectRuntime *partner = Object_GetById(8);
  struct ObjectRuntime *leader = Object_GetById(gPartyState.currentOwner);
  s32 dx;
  s32 dz;
  s32 near;
  s32 far;

  if (GameFlag_Test(648))
    return;
  dx = (leader->x - partner->x) >> 16;
  dz = (leader->z - partner->z) >> 16;
  near = dx * dx + dz * dz;
  dx = (leader->x >> 16) - 296;
  dz = (leader->z >> 16) - 248;
  far = dx * dx + dz * dz;
  if (far <= 6400 || near <= 2304)
    Func_020057f8(partner, (296 - dx) << 16, partner->y, (248 - dz) << 16);
  if (far > 15)
    return;
  Func_02005a68(146);
  Func_02005a68(122);
  partner->speedLimit = 0x20000;
  partner->acceleration = 0x20000;
  partner->motionFlags = 2;
  partner->terrainHeight = 0;
  partner->collisionFlags = 1;
  partner->velocityY = 0xa0000;
  ObjectMotion_EnableActionAndSetCallback(8, gData_02005b44);
  Func_020057f8(partner, 344 << 16, partner->y, 264 << 16);
  GameFlag_SetBit(648);
}
