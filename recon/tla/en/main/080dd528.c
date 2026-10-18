// Near miss, differ 8 of 120 (2026-10-18): ☀️'s BattleFx_StartItemBreak with
// ⚓️'s objects 278 and 673, a direct cue and Object_SetMode. Left: order
// only. The native loads the source's x and y before it finishes the angle
// (adds r5, r3, r0; movs r3, #192 after the loads), and loads the angle
// before it subtracts the second jitter (ldrh r3, [r2, #6] before subs r5).
// A held angle behind an empty asm moves every register (differ 92); the
// angle added first, differ 33.
#include "types.h"
#include "system.h"
#include "objdisp.h"
#include "object_runtime.h"

extern const s32 gData_080f0e78[];

struct ObjectRuntime *Object_Spawn(s32 kind, s32 x, s32 y, s32 z);
void Motion_SetTargetPositionFromMagnitudeAngle(struct ObjectRuntime *object,
                                                s32 magnitude, s32 angle);
void BattleFx_UpdateItemBreakFragment(void);

#define SE_ITEM_BREAK 138

void *BattleFx_StartItemBreak(void *object) {
  struct ObjectRuntime *source = object;
  struct ObjectRuntime *parent;
  struct ObjectRuntime *child;
  s32 angle;
  s32 radius;
  s32 speed;
  s32 fragmentIndex;
  u32 spread;
  u32 jitter;

  angle = (source->angle + 0x2000) & 0xc000;
  parent = Object_Spawn(278, source->x, source->y + (16 << 16), source->z);
  if (parent == 0)
    return 0;
  parent->scaleY = 0x4000;
  parent->scaleX = 0x4000;
  parent->update = BattleFx_UpdateItemBreakFragment;
  parent->speedLimit = 2 << 16;
  parent->acceleration = 2 << 16;
  parent->motionFlags = 0;
  Object_SetMode(parent, 3);
  Motion_SetTargetPositionFromMagnitudeAngle(parent, 16 << 16, angle);

  for (fragmentIndex = 7; fragmentIndex >= 0; fragmentIndex--) {
    child = Object_Spawn(673, source->x, source->y + (16 << 16), source->z);
    if (child != 0) {
      ObjectDispatch_InitializeFar((struct DispatchObject *)child,
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
  Audio_PlayCue(SE_ITEM_BREAK);
  return parent;
}
