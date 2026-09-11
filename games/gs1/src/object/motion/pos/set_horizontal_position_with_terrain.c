#include "scene.h"
#include "abi/object/motion/pos/set_horizontal_position_with_terrain.h"
#include "object_runtime.h"

void Object_ResetMotion(struct ObjectRuntime *);

void Motion_SetHPosTerrain(u32 object_id, s32 x, s32 z)
{
    struct ObjectRuntime *object;
    s32 terrain_height;
    s32 tile_x;
    s32 tile_z;
    s32 terrain_id;

    object = ObjectTable_Get(object_id);
    if (object != NULL) {
        Object_ResetMotion(object);
        object->velocity_x = 0;
        object->velocity_y = 0;
        object->velocity_z = 0;
        object->target_y = 0x80000000;
        object->target_x = 0x80000000;
        object->x = x;
        object->z = z;
        if (1 & object->flags) {
            terrain_id = object->terrain_id;
            tile_x = x;
            if (tile_x < 0) {
                tile_x += 0xFFFF;
            }
            tile_x = tile_x >> 0x10;
            tile_z = z;
            if (tile_z < 0) {
                tile_z += 0xFFFF;
            }
            tile_z = tile_z >> 0x10;
            terrain_height = Obj_Place(terrain_id, tile_x, tile_z) << 0x10;
            object->y = (object->y - object->terrain_height) + terrain_height;
            object->terrain_height = terrain_height;
        }
    }
}
