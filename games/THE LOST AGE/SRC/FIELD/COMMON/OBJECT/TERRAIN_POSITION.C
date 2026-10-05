#include "OBJECT_RUNTIME.H"

/* The rest of ☀️'s position-by-id run from FIELD/COMMON/OBJECT/OBJECT16.C
   after ObjectMotion_CommitCurrentPositionAndActivate, where ⚓️ asks the
   terrain height at an object's 16.16 position; ⚓️ adds the angle variant
   and the move to another object. */

s32 Object_SetPositionAndResetMotionFar(struct ObjectRuntime *, s32, s32, s32);
s32 Map_GetTerrainHeightFar(u8, s32, s32);

/* ☀️'s: places the object at x, z at its current height and, when it
   follows the ground, lifts it by the change in terrain height there. */
void ObjectMotion_SetHorizontalPositionWithTerrain(u32 object_id, s32 x, s32 z)
{
    struct ObjectRuntime *object = ObjectTable_Get(object_id);
    s32 terrain_height;

    if (object != NULL) {
        Object_ResetMotion(object);
        Object_SetPositionAndResetMotionFar(object, x, object->y, z);
        if (object->flags & 1) {
            terrain_height = Map_GetTerrainHeightFar(object->terrain_id, object->x, object->z);
            object->y = object->y - object->terrain_height + terrain_height;
            object->terrain_height = terrain_height;
        }
    }
}

/* The same, also turning the object to angle. */
void ObjectMotion_SetHorizontalPositionAndAngleWithTerrain(u32 object_id, s32 x, s32 z, s16 angle)
{
    struct ObjectRuntime *object = ObjectTable_Get(object_id);
    s32 terrain_height;

    if (object != NULL) {
        Object_ResetMotion(object);
        Object_SetPositionAndResetMotionFar(object, x, object->y, z);
        object->angle = angle;
        if (object->flags & 1) {
            terrain_height = Map_GetTerrainHeightFar(object->terrain_id, object->x, object->z);
            object->y = object->y - object->terrain_height + terrain_height;
            object->terrain_height = terrain_height;
        }
    }
}

/* ☀️'s: places the object at x, y, z and, when it follows the ground,
   lifts it by the terrain height there. */
void ObjectMotion_SetPositionWithTerrain(u32 object_id, s32 x, s32 y, s32 z)
{
    struct ObjectRuntime *object = ObjectTable_Get(object_id);
    s32 terrain_height;

    if (object != NULL) {
        Object_ResetMotion(object);
        Object_SetPositionAndResetMotionFar(object, x, y, z);
        if (object->flags & 1) {
            terrain_height = Map_GetTerrainHeightFar(object->terrain_id, object->x, object->z);
            object->y = object->y - object->terrain_height + terrain_height;
            object->terrain_height = terrain_height;
        }
    }
}

/* Places one object where another stands, at its terrain height. */
void ObjectMotion_MoveToObject(u32 object_id, u32 other_id)
{
    struct ObjectRuntime *object = ObjectTable_Get(object_id);
    struct ObjectRuntime *other = ObjectTable_Get(other_id);

    if (object != NULL && other != NULL) {
        Object_SetPositionAndResetMotionFar(object, other->x, other->y, other->z);
        object->terrain_height = other->terrain_height;
    }
}
