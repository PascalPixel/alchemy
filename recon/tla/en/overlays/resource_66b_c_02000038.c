#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"

struct TileSprite {
    u8 unknown_00[18];
    u16 tile;
    u8 unknown_14[6];
    u8 frame;
};

s32 Func_02000038(struct ObjectRuntime *object)
{
    u16 *tile = (u16 *)((u8 *)Ram_HeapSlots->map_work + 286);
    struct TileSprite *sprite = object->animation;

    sprite->tile = *tile;
    sprite->frame = 0;
    return 1;
}
