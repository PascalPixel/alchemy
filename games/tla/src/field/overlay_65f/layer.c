#include "ov65f.h"

struct MapLayer {
    u8 unknown_00[0x10];
    s32 unknown_10;
    s32 unknown_14;
    u8 unknown_18[0x10];
    u16 unknown_28;
    u16 unknown_2a;
    u8 unknown_2c[0x0c];
};

struct MapWork {
    u8 unknown_000[0x108];
    struct MapLayer layers[3];
};

void Func_02000280(void)
{
    struct MapWork *map = (struct MapWork *)Ram_HeapSlots->map_work;
    struct MapLayer *to = &map->layers[1];
    struct MapLayer *from = &map->layers[0];

    to->unknown_10 = from->unknown_10;
    to->unknown_14 = from->unknown_14;
    to->unknown_28 = from->unknown_28;
    to->unknown_2a = from->unknown_2a;
}
