#include "types.h"
#include "ram_buffer.h"

struct Ov681Layer
{
    u8  unknown_00[8];
    s32 x;
    s32 z;
    u8  unknown_10[0x28];
};

struct Ov681MapWork
{
    u8                unknown_000[0x108];
    struct Ov681Layer layers[2];
};

extern u8 Data_020023c4[];
extern s8 Data_020053fc[][2];

void Func_02004e0c(s32 x, s32 z, s32 width, s32 height, s32 cellX, s32 cellZ);

// From the third state on, for each of the two map layers: Func_02004e0c
// with the state's origin plus the layer's cell, a 15 by 10 size and the
// layer's cell plus one.
void Func_020048a0(void)
{
    s32                  state = *(s32 *)(Data_020023c4 + 0x288);
    s8                  *origin = Data_020053fc[state];
    struct Ov681MapWork *map = (struct Ov681MapWork *)Ram_HeapSlots->map_work;

    if (state > 1)
    {
        s32                originX = origin[0];
        s32                originZ = origin[1];
        struct Ov681Layer *layer = map->layers;
        s32                i;

        for (i = 1; i >= 0; i--, layer++)
        {
            /* FAKEMATCH: the layer's cell is pinned to r2 and r3: the native holds x in r2 and z in r3, where plainly they trade places (differ 16; read in either order, as locals or inline, 18 to 24). */
            register s32 x asm("r2") = layer->x >> 20;
            register s32 z asm("r3") = layer->z >> 20;

            Func_02004e0c(originX + x, originZ + z, 15, 10, x + 1, z + 1);
        }
    }
}
