#include "object_runtime.h"

void Func_08009140(struct ObjectRuntime *);
s32 Func_080091a8(u8, s32, s32);

void Func_08092454(u32 object_id, s32 x, s32 y, s32 z)
{
    struct ObjectRuntime *object;
    s32 terrain_height;
    s32 tile_x;
    s32 tile_z;

    object = Func_0808ba1c(object_id);
    if (object != NULL) {
        Func_08009140(object);
        object->velocity_x = 0;
        object->velocity_y = 0;
        object->velocity_z = 0;
        object->target_y = 0x80000000;
        object->target_x = 0x80000000;
        object->x = x;
        object->y = y;
        object->z = z;
        if (1 & object->flags) {
            s32 terrain_id = object->terrain_id;
            tile_x = x;
            if (tile_x < 0) {
                tile_x += 0xFFFF;
            }
            tile_x >>= 0x10;
            tile_z = z;
            if (tile_z < 0) {
                tile_z += 0xFFFF;
            }
            terrain_height =
                Func_080091a8(terrain_id, tile_x, tile_z >> 0x10) << 0x10;
            object->y = (object->y - object->terrain_height) + terrain_height;
            object->terrain_height = terrain_height;
        }
    }
}
