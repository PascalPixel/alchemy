#include "types.h"
#include "system.h"
#include "objdisp.h"
#include "actor.h"
#include "fixed_math.h"

extern const s32 gData_080f0e78[];

struct Actor *Actor_Spawn(s32 kind, s32 x, s32 y, s32 z);
void Motion_SetTargetPositionFromMagnitudeAngle(struct Actor *object,
                                                s32 magnitude, s32 angle);
void Actor_Destroy(struct Actor *object);

/// The object spins as it rises for 31 frames, shrinks, and bursts into 8
/// particles (object 673).
// As ☀️'s UpdateRisingParticleBurst.
void UpdateRisingParticleBurst(void *object) {
  struct Actor *src = object;
  struct Actor *child;
  s32 i;
  s32 dist;
  u32 spread;

  Sound_PlayFar(154);
  for (i = 30; i >= 0; i--) {
    src->y += 1 << 16;
    src->angle += 0x2000; // 1/8 turn
    src->scaleX += -Q16_ONE / 32;
    src->scaleY += -Q16_ONE / 32;
    wait(1);
  }
  for (i = 7; i >= 0; i--) {
    child = Actor_Spawn(673, src->x, src->y, src->z);
    if (child != 0) {
      s32 speed;
      // One 1.0 for both uses, set before the block's calls; a literal at
      // each use differs.
      s32 unit = 0x10000;

      Actor_StartScriptFar((struct ActorView *)child,
                                   gData_080f0e78);
      speed = Random16();
      child->acceleration = unit;
      child->speedLimit = speed + unit;
      child->motionFlags = 2;
      child->gravity = 0xa3d;
      spread = Random16();
      child->velocityY = spread - Random16();
      dist = Random16() * 24;
      dist += 8 << 16;
      Motion_SetTargetPositionFromMagnitudeAngle(child, dist, Random16());
    }
  }
  Sound_PlayFar(131);
  Actor_Destroy(src);
}
