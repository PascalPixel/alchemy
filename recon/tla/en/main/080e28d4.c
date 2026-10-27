// Near miss, differ 6 of 178 (2026-10-26). The arc's bound is a local the
// loop compares with (the native keeps it in r11), while the divisions use
// the literal 20 (42 -> 6). Left: order only, the scale constants and the arc's
// position copies, where after a store the native issues another instruction
// before the next load.
// An event sequence. Creates object sprite at (x, 0, z), which grows
// from 0.4 to full size while spinning, turns it and the controlled owner to
// face each other, plays the owner's variant 3 and Func_080d489c(, 258), then
// bounces the object twice (cue 152) and, with cue 146, carries it in a
// 40-pixel arc onto the owner over 20 frames.
#include "types.h"
#include "fixed_math.h"
#include "object_runtime.h"

s32 EventRuntime_GetControlledOwner(void);
struct ObjectRuntime *Object_GetById(u32 actor);
struct ObjectRuntime *Func_080200c0(s32 sprite, s32 x, s32 y, s32 z);
void ObjectDispatch_SetSingleChildFlagsFar(struct ObjectRuntime *object,
                                           s32 flags);
void WaitFrames(s32 frames);
void FacingObject_TurnPairToFaceEachOther(struct ObjectRuntime *first,
                                          struct ObjectRuntime *second);
void EventRuntime_Wait(s32 frames);
void ObjectMotion_SetVariantCallback(s32 actor, s32 variant);
void Func_080d489c(s32 actor, s32 value);

void Func_080e28d4(s32 sprite, s32 x, s32 z) {
  s32 ownerId = EventRuntime_GetControlledOwner();
  struct ObjectRuntime *owner = Object_GetById(ownerId);
  struct ObjectRuntime *object = Func_080200c0(sprite, x << 16, 0, z << 16);
  s32 from[3];
  s32 to[3];
  s32 i;
  s32 steps;

  if (object == NULL)
    return;
  ObjectDispatch_SetSingleChildFlagsFar(object, 0);
  object->gravity = 0x5999;
  object->velocityY = 0x50000;
  object->scaleY = object->scaleX = 0x6666;
  do {
    WaitFrames(1);
    object->scaleY = object->scaleX = object->scaleX + 0x3d7;
    object->angle += 0x1000;
  } while (object->scaleX <= 0xffff);
  object->scaleY = object->scaleX = 0x10000;
  Object_SetMode(owner, 1);
  FacingObject_TurnPairToFaceEachOther(object, owner);
  owner->action = owner->angle;
  EventRuntime_Wait(10);
  ObjectMotion_SetVariantCallback(ownerId, 3);
  Func_080d489c(ownerId, 258);
  EventRuntime_Wait(32);
  object->gravity = 0x10000;
  object->velocityY = 0x60000;
  Audio_PlayCue(152);
  EventRuntime_Wait(15);
  object->velocityY = 0x60000;
  Audio_PlayCue(152);
  EventRuntime_Wait(23);
  Audio_PlayCue(146);

  from[0] = object->x;
  from[1] = object->y;
  from[2] = object->z;
  to[0] = owner->x;
  to[1] = owner->y;
  to[2] = owner->z;
  steps = 20;
  for (i = 0; i < steps; i++) {
    object->x = from[0] + i * (to[0] - from[0]) / 20;
    object->y = from[1] + i * (to[1] - from[1]) / 20 +
                Trig_Sin((i << 15) / 20) * 40;
    object->z = from[2] + i * (to[2] - from[2]) / 20;
    WaitFrames(1);
  }
}
