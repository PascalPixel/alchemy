#include "types.h"
#include "system.h"
#include "objdisp.h"
#include "actor.h"

extern const s32 gData_080f0e78[];

struct Actor *Actor_Spawn(s32 kind, s32 x, s32 y, s32 z);
void Motion_SetTargetPositionFromMagnitudeAngle(struct Actor *object,
                                                s32 magnitude, s32 angle);
void BattleEffect_UpdateItemBreakFragment(void);

#define SE_ITEM_BREAK 138

/// The Broken Seal's BattleEffect_StartItemBreak with this game's objects
/// 278 and 673: spawns the breaking item above object and eight fragments
/// that scatter from it.
void *BattleEffect_StartItemBreak(void *object) {
  struct Actor *source = object;
  struct Actor *parent;
  struct Actor *child;
  s32 angle;
  s32 radius;
  s32 speed;
  s32 fragmentIndex;
  u32 spread;
  u32 jitter;

  angle = (source->angle + 0x2000) & 0xc000;
  parent = Actor_Spawn(278, source->x, source->y + (16 << 16), source->z);
  if (parent == 0)
    return 0;
  parent->scaleY = 0x4000;
  parent->scaleX = 0x4000;
  parent->update = BattleEffect_UpdateItemBreakFragment;
  parent->speedLimit = 2 << 16;
  parent->acceleration = 2 << 16;
  parent->motionFlags = 0;
  Actor_SetMode(parent, 3);
  Motion_SetTargetPositionFromMagnitudeAngle(parent, 16 << 16, angle);

  for (fragmentIndex = 7; fragmentIndex >= 0; fragmentIndex--) {
    child = Actor_Spawn(673, source->x, source->y + (16 << 16), source->z);
    if (child != 0) {
      Actor_StartScriptFar((struct DispatchObject *)child,
                                   gData_080f0e78);
      speed = Random16() + 0x10000;
      child->acceleration = 1 << 16;
      child->speedLimit = speed;
      child->motionFlags = 2;
      child->gravity = 0x51e;
      spread = Random16();
      child->velocityY = spread - Random16();
      radius = Random16() * 24 + (8 << 16);
      jitter = Random16();
      Motion_SetTargetPositionFromMagnitudeAngle(
          child, radius, ((jitter - Random16()) >> 3) + source->angle);
    }
  }
  Sound_PlayFar(SE_ITEM_BREAK);
  return parent;
}
