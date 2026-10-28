#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "actor.h"
#include "system.h"
#include "actor_script_view.h"
#include "edition.h"

struct ActorView;

void Actor_SetSingleChildFlagsFar(struct ActorView *, u32);

void ObjectEvent_SetMode5WithCue(u32 objectId) {
  Actor_SetMode(ObjectTable_Get(objectId), 5);
  Sound_PlayFar(125);
  WaitFrames(12);
}

void ObjectEvent_Spring(u32 objectId) {
  struct Actor *object;

  object = ObjectTable_Get(objectId);
  WaitFrames(18);
  Actor_SetMode(object, 7);
  Sound_PlayFar(146);
  if (object != 0) {
    object->velocityY = 0x80000;
    Actor_SetSingleChildFlagsFar((struct ActorView *)object, 1);
  }
}

void ObjectEvent_SetMode4WithCue(u32 objectId) {
  Actor_SetMode(ObjectTable_Get(objectId), 4);
  Sound_PlayFar(124);
  WaitFrames(12);
}

void ObjectEvent_SetMode2(u32 objectId) {
  Actor_SetMode(ObjectTable_Get(objectId), 2);
}

void ObjectEvent_SetMode6WithCue(u32 objectId) {
  Actor_SetMode(ObjectTable_Get(objectId), 6);
  Sound_PlayFar(124);
  WaitFrames(12);
}

struct Actor *ObjectEvent_GetObject(u32 objectId) {
  return ObjectTable_Get(objectId);
}

void ObjectEvent_Remove(u32 objectId) { Func_080caa2c(objectId); }

void ObjectEvent_PlaceOnGround(u32 objectId, s32 x, s32 z) {
  struct ActorScriptView *object;

  object = (struct ActorScriptView *)ObjectTable_Get(objectId);
  if (object == 0)
    return;
  if (x == -1)
    x = object->homeX << 16;
  if (z == -1)
    z = object->homeZ << 16;
  object->x = x;
  object->z = z;
  object->y = object->terrainHeight = Map_GetTerrainHeightFar(0, x, z);
}
