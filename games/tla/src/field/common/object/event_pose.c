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
#include "object_runtime.h"
#include "system.h"
#include "script_object_runtime.h"
#include "edition.h"

struct DispatchObject;

void ObjectDispatch_SetSingleChildFlagsFar(struct DispatchObject *, u32);

void ObjectEvent_SetMode5WithCue(u32 objectId) {
  Object_SetMode(ObjectTable_Get(objectId), 5);
  Audio_PlayCue(125);
  WaitTicks(12);
}

void ObjectEvent_Spring(u32 objectId) {
  struct ObjectRuntime *object;

  object = ObjectTable_Get(objectId);
  WaitTicks(18);
  Object_SetMode(object, 7);
  Audio_PlayCue(146);
  if (object != 0) {
    object->velocityY = 0x80000;
    ObjectDispatch_SetSingleChildFlagsFar((struct DispatchObject *)object, 1);
  }
}

void ObjectEvent_SetMode4WithCue(u32 objectId) {
  Object_SetMode(ObjectTable_Get(objectId), 4);
  Audio_PlayCue(124);
  WaitTicks(12);
}

void ObjectEvent_SetMode2(u32 objectId) {
  Object_SetMode(ObjectTable_Get(objectId), 2);
}

void ObjectEvent_SetMode6WithCue(u32 objectId) {
  Object_SetMode(ObjectTable_Get(objectId), 6);
  Audio_PlayCue(124);
  WaitTicks(12);
}

struct ObjectRuntime *ObjectEvent_GetObject(u32 objectId) {
  return ObjectTable_Get(objectId);
}

void ObjectEvent_Remove(u32 objectId) { Func_080caa2c(objectId); }

void ObjectEvent_PlaceOnGround(u32 objectId, s32 x, s32 z) {
  struct ScriptObjectRuntime *object;

  object = (struct ScriptObjectRuntime *)ObjectTable_Get(objectId);
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
