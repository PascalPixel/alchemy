#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "object_runtime.h"

#define Object_SetPositionAndResetMotionFar \
    ((s32 (*)(struct ObjectRuntime *, s32, s32, s32))Object_SetPositionAndResetMotionFar)

void ObjectMotion_SetHorizontalPositionWithTerrain(u32 objectId, s32 x, s32 z)
{
    struct ObjectRuntime *object = ObjectTable_Get(objectId);
    s32                   terrainHeight;

    if (object != NULL)
    {
        Object_ResetMotion(object);
        Object_SetPositionAndResetMotionFar(object, x, object->y, z);
        if (object->motionFlags & 1)
        {
            terrainHeight = Map_GetTerrainHeightFar(object->terrainId, object->x, object->z);
            object->y = object->y - object->terrainHeight + terrainHeight;
            object->terrainHeight = terrainHeight;
        }
    }
}

void ObjectMotion_SetHorizontalPositionAndAngleWithTerrain(u32 objectId, s32 x, s32 z, s16 angle)
{
    struct ObjectRuntime *object = ObjectTable_Get(objectId);
    s32                   terrainHeight;

    if (object != NULL)
    {
        Object_ResetMotion(object);
        Object_SetPositionAndResetMotionFar(object, x, object->y, z);
        object->angle = angle;
        if (object->motionFlags & 1)
        {
            terrainHeight = Map_GetTerrainHeightFar(object->terrainId, object->x, object->z);
            object->y = object->y - object->terrainHeight + terrainHeight;
            object->terrainHeight = terrainHeight;
        }
    }
}

void ObjectMotion_SetPositionWithTerrain(u32 objectId, s32 x, s32 y, s32 z)
{
    struct ObjectRuntime *object = ObjectTable_Get(objectId);
    s32                   terrainHeight;

    if (object != NULL)
    {
        Object_ResetMotion(object);
        Object_SetPositionAndResetMotionFar(object, x, y, z);
        if (object->motionFlags & 1)
        {
            terrainHeight = Map_GetTerrainHeightFar(object->terrainId, object->x, object->z);
            object->y = object->y - object->terrainHeight + terrainHeight;
            object->terrainHeight = terrainHeight;
        }
    }
}

void ObjectMotion_MoveToObject(u32 objectId, u32 otherId)
{
    struct ObjectRuntime *object = ObjectTable_Get(objectId);
    struct ObjectRuntime *other = ObjectTable_Get(otherId);

    if (object != NULL && other != NULL)
    {
        Object_SetPositionAndResetMotionFar(object, other->x, other->y, other->z);
        object->terrainHeight = other->terrainHeight;
    }
}
