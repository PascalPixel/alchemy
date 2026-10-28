// Near miss, differ 6 of 178 (2026-10-26). The arc's bound is a local the
// loop compares with (the native keeps it in r11), while the divisions use
// the literal 20 (42 -> 6). Left: order only, the scale constants and the arc's
// position copies, where after a store the native issues another instruction
// before the next load.
// An event command. Creates object sprite at (x, 0, z), which grows
// from 0.4 to full size while spinning, turns it and the controlled owner to
// face each other, plays the owner's variant 3 and Func_080d489c(, 258), then
// bounces the object twice (cue 152) and, with cue 146, carries it in a
// 40-pixel arc onto the owner over 20 frames.
#include "types.h"
#include "fixed_math.h"
#include "actor.h"

s32 Party_GetLeaderActor(void);
struct Actor *Actor_GetById(u32 actor);
struct Actor *Func_080200c0(s32 sprite, s32 x, s32 y, s32 z);
void Actor_SetSingleChildFlagsFar(struct Actor *object,
                                           s32 flags);
void wait(s32 frames);
void FacingObject_TurnPairToFaceEachOther(struct Actor *first,
                                          struct Actor *second);
void Event_Wait(s32 frames);
void Actor_StartRepeatedMotion(s32 actor, s32 variant);
void Func_080d489c(s32 actor, s32 value);

void Func_080e28d4(s32 sprite, s32 x, s32 z) {
  s32 ownerId = Party_GetLeaderActor();
  struct Actor *owner = Actor_GetById(ownerId);
  struct Actor *object = Func_080200c0(sprite, x << 16, 0, z << 16);
  s32 from[3];
  s32 to[3];
  s32 i;
  s32 steps;

  if (object == NULL)
    return;
  Actor_SetSingleChildFlagsFar(object, 0);
  object->gravity = 0x5999;
  object->velocityY = 0x50000;
  object->scaleY = object->scaleX = 0x6666;
  do {
    wait(1);
    object->scaleY = object->scaleX = object->scaleX + 0x3d7;
    object->angle += 0x1000;
  } while (object->scaleX <= 0xffff);
  object->scaleY = object->scaleX = 0x10000;
  Actor_SetMode(owner, 1);
  FacingObject_TurnPairToFaceEachOther(object, owner);
  owner->action = owner->angle;
  Event_Wait(10);
  Actor_StartRepeatedMotion(ownerId, 3);
  Func_080d489c(ownerId, 258);
  Event_Wait(32);
  object->gravity = 0x10000;
  object->velocityY = 0x60000;
  Sound_PlayFar(152);
  Event_Wait(15);
  object->velocityY = 0x60000;
  Sound_PlayFar(152);
  Event_Wait(23);
  Sound_PlayFar(146);

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
    wait(1);
  }
}
