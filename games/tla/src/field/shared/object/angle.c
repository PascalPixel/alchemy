#include "event_work.h"
#include "object_lookup.h"
#include "types.h"
#include "field_sprite.h"
#include "actor.h"

struct Actor *Actor_GetById(s32 id);
void wait(s32 count);
void Actor_SetSingleChildFlagsFar(struct Actor *object,
                                           u32 flags);
void Sound_PlayFar(s32 cue);
void Actor_ArmCallback(s32 id, s32 value, s32 mode);
void Func_080d3378(struct Actor *source, s32 attribute);
void Event_NoOp(void);

/// Makes an object jump down off a ledge in front of it: it plays `mode` for 10
/// frames, hops 12 pixels forward from its cell's centre line, then sinks 2
/// pixels a frame for 14 frames from cue 217, dropping a particle of kind
/// `particle` every other frame (none for -1), and jumps on 16 pixels to land,
/// waiting up to 180 frames for it unless `particle` has flag 0x1000. The
/// Broken Seal's twin is Actor_JumpDownLedge.
// The landing wait matches only as a guarded do-while: as
// while (y > ground) { wait(1); if (++i >= 180) break; } agscc keeps
// the test at the top and jumps back to it (differ 18), where the native has
// the test copied below the body as loop rotation would place it.
void Actor_JumpDownLedge(s32 id, s32 mode, s32 particle) {
  struct Actor *object = Actor_GetById(id);
  s32 z = object->z;
  s32 x = (object->x & 0xfff00000) + 0x80000;
  s32 frame;
  u32 i;

  if (object == NULL)
    return;
  frame = ((struct FieldSprite *)object->animation)->nextFrame;
  Actor_SetMode(object, mode);
  wait(10);
  Actor_SetMode(object, 1);
  Actor_SetSingleChildFlagsFar(object, 0);
  object->motionFlags |= 2;
  object->velocityY = 0x40000;
  Actor_SetTargetPosition(object, x, object->y, z + 0xc0000);
  wait(6);
  Sound_PlayFar(217);
  Actor_ArmCallback(id, 0x5000, 0);
  object->motionFlags = 0;
  for (i = 0; i < 14; i++) {
    object->y -= 0x20000;
    object->targetY = object->y;
    wait(1);
    if (particle != -1 && (i & 1))
      Func_080d3378(object, particle);
  }
  object->motionFlags = 3;
  object->velocityY = 0x30000;
  Actor_SetTargetPosition(object, x, object->y, z + 0x100000);
  Actor_CommitPosition(object);
  if (particle & 0x1000) {
    wait(4);
    return;
  }
  i = 0;
  if (object->y > object->terrainHeight) {
    do {
      wait(1);
      if (++i >= 180)
        break;
    } while (object->y > object->terrainHeight);
  }
  Actor_SetSingleChildFlagsFar(object, frame);
  wait(2);
  Event_NoOp();
}

struct ObjectPairPosition {
  u8 unk00[6];
  s16 angle;
  s32 x;
  s32 y;
  s32 z;
};

s32 ArcTan2(s32, s32);

void Actor_SetAngleToward(s32 firstId, s32 secondId, s32 wait) {
  struct ObjectPairPosition *first;
  struct ObjectPairPosition *second;

  first = ObjectTable_Get(firstId);
  second = ObjectTable_Get(secondId);
  if (first != 0 && second != 0) {
    first->angle = ArcTan2(second->z - first->z, second->x - first->x);
    Event_Wait(wait);
  }
}
