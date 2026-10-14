/* Draft, differ 16 of 45, registers only: the native holds x in r2 and z in r3;
 * this draft swaps them. Tried: x declared first (18), sums as locals (18),
 * the reads inline in the call (24), an indexed for loop (20), a forwarding
 * inline (18). alchemy permute matched only with an inline adder for each
 * argument, a device rejected here. */
#include "../../../../games/tla/include/types.h"
#include "../../../../games/tla/include/ram_buffer.h"

struct Ov681Layer {
    u8 unknown_00[8];
    s32 x;
    s32 z;
    u8 unknown_10[0x28];
};

struct Ov681MapWork {
    u8 unknown_000[0x108];
    struct Ov681Layer layers[2];
};

extern u8 Data_020023c4[];
extern s8 Data_020053fc[][2];
void Func_02004e0c(s32 x, s32 z, s32 width, s32 height, s32 cellX, s32 cellZ);

void Func_020048a0(void)
{
    s32 state = *(s32 *)(Data_020023c4 + 0x288);
    s8 *origin = Data_020053fc[state];
    struct Ov681MapWork *map = (struct Ov681MapWork *)Ram_HeapSlots->map_work;

    if (state > 1) {
        s32 originX = origin[0];
        s32 originZ = origin[1];
        struct Ov681Layer *layer = map->layers;
        s32 i;

        for (i = 1; i >= 0; i--, layer++) {
            s32 z = layer->z >> 20;
            s32 x = layer->x >> 20;

            Func_02004e0c(originX + x, originZ + z, 15, 10, x + 1, z + 1);
        }
    }
}
