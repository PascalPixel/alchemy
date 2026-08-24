#include "types.h"

extern s32 *Func_0200146a(s32 actor);

#define GetActorPosition Func_0200146a
#define IsActor9AtTile15x54 Func_02000334

s32 IsActor9AtTile15x54(void)
{
    s32 *actor = GetActorPosition(9);
    s32 z = actor[4];
    s32 x;
    s32 z_tile;
    s32 x_tile;

    if (z < 0) {
        z += 0x000FFFFF;
    }
    x = actor[2];
    z_tile = z >> 20;
    if (x < 0) {
        x += 0x000FFFFF;
    }
    x_tile = x >> 20;
    if (x_tile == 15 && z_tile == 54) {
        return 1;
    }
    return 0;
}
