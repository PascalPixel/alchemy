#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "actor.h"

#define Actor_SetPositionAndResetMotionFar                                    \
  ((s32 (*)(struct Actor *, s32, s32,                                  \
            s32))Actor_SetPositionAndResetMotionFar)

void Actor_SetHorizontalPositionWithTerrain(u32 objectId, s32 x, s32 z) {
  struct Actor *object = ObjectTable_Get(objectId);
  s32 terrainHeight;

  if (object != NULL) {
    Actor_ResetMotion(object);
    Actor_SetPositionAndResetMotionFar(object, x, object->y, z);
    if (object->motionFlags & 1) {
      terrainHeight =
          Map_GetTerrainHeightFar(object->terrainId, object->x, object->z);
      object->y = object->y - object->terrainHeight + terrainHeight;
      object->terrainHeight = terrainHeight;
    }
  }
}

void Actor_SetHorizontalPositionAndAngleWithTerrain(u32 objectId, s32 x,
                                                           s32 z, s16 angle) {
  struct Actor *object = ObjectTable_Get(objectId);
  s32 terrainHeight;

  if (object != NULL) {
    Actor_ResetMotion(object);
    Actor_SetPositionAndResetMotionFar(object, x, object->y, z);
    object->angle = angle;
    if (object->motionFlags & 1) {
      terrainHeight =
          Map_GetTerrainHeightFar(object->terrainId, object->x, object->z);
      object->y = object->y - object->terrainHeight + terrainHeight;
      object->terrainHeight = terrainHeight;
    }
  }
}

void Actor_SetPositionWithTerrain(u32 objectId, s32 x, s32 y, s32 z) {
  struct Actor *object = ObjectTable_Get(objectId);
  s32 terrainHeight;

  if (object != NULL) {
    Actor_ResetMotion(object);
    Actor_SetPositionAndResetMotionFar(object, x, y, z);
    if (object->motionFlags & 1) {
      terrainHeight =
          Map_GetTerrainHeightFar(object->terrainId, object->x, object->z);
      object->y = object->y - object->terrainHeight + terrainHeight;
      object->terrainHeight = terrainHeight;
    }
  }
}

void Actor_MoveToObject(u32 objectId, u32 otherId) {
  struct Actor *object = ObjectTable_Get(objectId);
  struct Actor *other = ObjectTable_Get(otherId);

  if (object != NULL && other != NULL) {
    Actor_SetPositionAndResetMotionFar(object, other->x, other->y, other->z);
    object->terrainHeight = other->terrainHeight;
  }
}
