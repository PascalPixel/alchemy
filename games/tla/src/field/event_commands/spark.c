#include "actor.h"
#include "call.h"
#include "fixed_math.h"

void Actor_StartScriptFar(struct Actor *object, const s32 *script);
struct Actor *Actor_Spawn(s32 kind, s32 x, s32 y, s32 z);
void Motion_SetTargetPositionFromMagnitudeAngle(struct Actor *object,
                                                s32 magnitude, s32 angle);
u32 Random16(void);
extern const s32 gData_080f0e78[];

/// Spawns a particle (kind 0x2a1) at x, y, z that hops off in a random
/// direction 8 to 32 pixels away at 1 to 2 pixels a frame.
struct Actor *BattleEffect_SpawnScatterParticle(s32 x, s32 y, s32 z) {
  /* FAKEMATCH(barrier): SPLIT_CONSTANT builds each constant as the game does, a
   * shifted byte plus an addend; as plain constants, 0 of 48 instructions
   * differ in tla-en. */
  struct Actor *object = Actor_Spawn(SPLIT_CONSTANT(0x2a1), x, y, z);
  s32 magnitude;

  if (object != NULL) {
    Actor_StartScriptFar(object, gData_080f0e78);
    object->speedLimit = Random16() + Q16_ONE;
    object->acceleration = Q16_ONE;
    object->motionFlags = 2;
    object->gravity = SPLIT_CONSTANT(0xa3d);
    magnitude = Random16();
    magnitude -= Random16();
    object->velocityY = magnitude;
    Motion_SetTargetPositionFromMagnitudeAngle(
        object, Random16() * 24 + 0x80000, Random16());
  }
  return object;
}
