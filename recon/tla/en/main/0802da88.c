#include "types.h"

struct MapBlock {
    u8 unk0[2];
    u8 kind;
    u8 unk3;
};

extern struct MapBlock gMapBlocks[];

u8 Func_0802da88(s32 *position)
{
    s32 z = position[2];
    s32 x = position[0] / 0x200000 & 31;

    z = z / 0x200000 & 31;
    return gMapBlocks[x + z * 32].kind;
}
