#include "types.h"

extern u8 *Data_03001f2c;

struct MarkerObject {
    u8 pad00[4];
    u8 state;
    u8 active;
    u8 pad06[6];
    u16 timer;
};

struct MarkerObject *Func_080150c8(u32 resource, u32 flags, s32 x, s32 y, s32 tile);

s32 Func_080ae99c(s32 x, s32 y, s32 tile, s32 variant)
{
    struct MarkerObject *object;
    u32 resource;
    u16 *resource_pointer;

    if (variant == 0)
        resource_pointer = (u16 *)(Data_03001f2c + 0x392);
    else
        resource_pointer = (u16 *)(Data_03001f2c + 0x394);
    resource = *resource_pointer;
    object = Func_080150c8(resource, 0x40000000, x, y, tile);
    if (object == 0)
        return -1;
    object->state = 0;
    object->timer = 0;
    object->active = 1;
    return 1;
}
