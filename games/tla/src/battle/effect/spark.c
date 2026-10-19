#include "object_runtime.h"

void ObjectDispatch_InitializeFar(struct ObjectRuntime *object,
                                  const s32 *script);
struct ObjectRuntime *Object_Spawn(s32 kind, s32 x, s32 y, s32 z);
void Motion_SetTargetPositionFromMagnitudeAngle(struct ObjectRuntime *object,
                                                s32 magnitude, s32 angle);
u32 Random16(void);
extern const s32 gData_080f0e78[];

/// Spawns a particle (kind 0x2a1) at x, y, z that hops off in a random
/// direction 8 to 32 pixels away at 1 to 2 pixels a frame.
struct ObjectRuntime *BattleFx_SpawnScatterParticle(s32 x, s32 y, s32 z) {
  struct ObjectRuntime *object = Object_Spawn(0x2a1, x, y, z);
  s32 magnitude;

  if (object != NULL) {
    ObjectDispatch_InitializeFar(object, gData_080f0e78);
    object->speedLimit = Random16() + 0x10000;
    object->acceleration = 0x10000;
    object->motionFlags = 2;
    object->gravity = 0xa3d;
    magnitude = Random16();
    magnitude -= Random16();
    object->velocityY = magnitude;
    Motion_SetTargetPositionFromMagnitudeAngle(
        object, Random16() * 24 + 0x80000, Random16());
  }
  return object;
}
