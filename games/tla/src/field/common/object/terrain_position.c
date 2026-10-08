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

#define Object_SetPositionAndResetMotionFar ((s32 (*)(struct ObjectRuntime*,s32,s32,s32))Object_SetPositionAndResetMotionFar)


void ObjectMotion_SetHorizontalPositionWithTerrain(u32 object_id, s32 x, s32 z)
{
    struct ObjectRuntime *object = ObjectTable_Get(object_id);
    s32 terrain_height;

    if (object != NULL) {
        Object_ResetMotion(object);
        Object_SetPositionAndResetMotionFar(object, x, object->y, z);
        if (object->motion_flags & 1) {
            terrain_height = Map_GetTerrainHeightFar(object->terrain_id, object->x, object->z);
            object->y = object->y - object->terrain_height + terrain_height;
            object->terrain_height = terrain_height;
        }
    }
}

void ObjectMotion_SetHorizontalPositionAndAngleWithTerrain(u32 object_id, s32 x, s32 z, s16 angle)
{
    struct ObjectRuntime *object = ObjectTable_Get(object_id);
    s32 terrain_height;

    if (object != NULL) {
        Object_ResetMotion(object);
        Object_SetPositionAndResetMotionFar(object, x, object->y, z);
        object->angle = angle;
        if (object->motion_flags & 1) {
            terrain_height = Map_GetTerrainHeightFar(object->terrain_id, object->x, object->z);
            object->y = object->y - object->terrain_height + terrain_height;
            object->terrain_height = terrain_height;
        }
    }
}

void ObjectMotion_SetPositionWithTerrain(u32 object_id, s32 x, s32 y, s32 z)
{
    struct ObjectRuntime *object = ObjectTable_Get(object_id);
    s32 terrain_height;

    if (object != NULL) {
        Object_ResetMotion(object);
        Object_SetPositionAndResetMotionFar(object, x, y, z);
        if (object->motion_flags & 1) {
            terrain_height = Map_GetTerrainHeightFar(object->terrain_id, object->x, object->z);
            object->y = object->y - object->terrain_height + terrain_height;
            object->terrain_height = terrain_height;
        }
    }
}

void ObjectMotion_MoveToObject(u32 object_id, u32 other_id)
{
    struct ObjectRuntime *object = ObjectTable_Get(object_id);
    struct ObjectRuntime *other = ObjectTable_Get(other_id);

    if (object != NULL && other != NULL) {
        Object_SetPositionAndResetMotionFar(object, other->x, other->y, other->z);
        object->terrain_height = other->terrain_height;
    }
}
