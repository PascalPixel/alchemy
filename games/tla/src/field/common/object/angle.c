#include "event_work.h"
#include "object_lookup.h"
#include "types.h"
#include "field_sprite.h"
#include "object_runtime.h"

struct ObjectRuntime *Object_GetById(s32 id);
void WaitFrames(s32 count);
void ObjectDispatch_SetSingleChildFlagsFar(struct ObjectRuntime *object,
                                           u32 flags);
void Audio_PlayCue(s32 cue);
void ObjectMotion_ArmCallback(s32 id, s32 value, s32 mode);
void Func_080d3378(struct ObjectRuntime *source, s32 attribute);
void Event_NoOp(void);

/// An object rises out of the ground: it plays `mode` for 10 frames, is placed
/// on its cell's centre line raised 12 pixels, then climbs 2 pixels a frame for
/// 14 frames to cue 217, dropping a particle of kind `particle` every other
/// frame (none for -1), and is then let fall back, waiting up to 180 frames for
/// it to land unless `particle` has flag 0x1000.
// No ☀️ twin found. The landing wait matches only as a guarded do-while: as
// while (y > ground) { WaitFrames(1); if (++i >= 180) break; } agscc keeps
// the test at the top and jumps back to it (differ 18), where the native has
// the test copied below the body as loop rotation would place it.
void BattleFx_RunRisingObjectSequence(s32 id, s32 mode, s32 particle) {
  struct ObjectRuntime *object = Object_GetById(id);
  s32 z = object->z;
  s32 x = (object->x & 0xfff00000) + 0x80000;
  s32 frame;
  u32 i;

  if (object == NULL)
    return;
  frame = ((struct FieldSprite *)object->animation)->nextFrame;
  Object_SetMode(object, mode);
  WaitFrames(10);
  Object_SetMode(object, 1);
  ObjectDispatch_SetSingleChildFlagsFar(object, 0);
  object->motionFlags |= 2;
  object->velocityY = 0x40000;
  Object_SetPosition(object, x, object->y, z + 0xc0000);
  WaitFrames(6);
  Audio_PlayCue(217);
  ObjectMotion_ArmCallback(id, 0x5000, 0);
  object->motionFlags = 0;
  for (i = 0; i < 14; i++) {
    object->y -= 0x20000;
    object->targetY = object->y;
    WaitFrames(1);
    if (particle != -1 && (i & 1))
      Func_080d3378(object, particle);
  }
  object->motionFlags = 3;
  object->velocityY = 0x30000;
  Object_SetPosition(object, x, object->y, z + 0x100000);
  Object_CommitPosition(object);
  if (particle & 0x1000) {
    WaitFrames(4);
    return;
  }
  i = 0;
  if (object->y > object->terrainHeight) {
    do {
      WaitFrames(1);
      if (++i >= 180)
        break;
    } while (object->y > object->terrainHeight);
  }
  ObjectDispatch_SetSingleChildFlagsFar(object, frame);
  WaitFrames(2);
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

void ObjectMotion_SetAngleToward(s32 firstId, s32 secondId, s32 wait) {
  struct ObjectPairPosition *first;
  struct ObjectPairPosition *second;

  first = ObjectTable_Get(firstId);
  second = ObjectTable_Get(secondId);
  if (first != 0 && second != 0) {
    first->angle = ArcTan2(second->z - first->z, second->x - first->x);
    EventRuntime_Wait(wait);
  }
}
