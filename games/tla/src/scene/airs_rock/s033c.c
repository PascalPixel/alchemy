#include "types.h"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "call.h"
s32 ObjectDispatch_SetSingleChildFlags();

s32 Func_0200033c(s32 a0)
{
    Call2(ObjectDispatch_SetSingleChildFlags, a0, 0);
    return 0;
}


#include "ram_buffer.h"

struct AirsRockMapLayer {
    u8 unk00[0x30];
    u8 *cells;
    u8 unk34[4];
};

struct AirsRockMapWork {
    u8 unk000[0x108];
    struct AirsRockMapLayer layers[3];
};

extern u8 gMapCellBuffer[];

s32 Func_02000348(u32 layer, s32 x, s32 y)
{
    struct AirsRockMapWork *map = (struct AirsRockMapWork *)Ram_HeapSlots->mapWork;
    u8 *cell;

    if (map == NULL)
        return 0;
    if (layer <= 2)
        cell = map->layers[layer].cells;
    else
        cell = gMapCellBuffer;
    cell += (x + (y << 7)) << 2;
    return (cell[2] << 8) | cell[3];
}

s32 Func_02000384(u32 layer, s32 x, s32 y, s32 value)
{
    struct AirsRockMapWork *map = (struct AirsRockMapWork *)Ram_HeapSlots->mapWork;
    u8 *cell;
    s32 old;

    if (map == NULL)
        return 0;
    if (layer <= 2)
        cell = map->layers[layer].cells;
    else
        cell = gMapCellBuffer;
    cell += (x + (y << 7)) << 2;
    old = (cell[2] << 8) | cell[3];
    cell[2] = value >> 8;
    cell[3] = value;
    return old;
}
