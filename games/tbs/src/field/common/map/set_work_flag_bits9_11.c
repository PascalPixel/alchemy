#include "global_cells.h"
#include "types.h"
#include "map.h"

void Map_SetWorkFlagBits9To11(u32 v)
{
    struct MapState *state = gMapWork[0];
    u32 mask = v & 0xe00;
    u32 flags = state->flags;

    flags = (flags & 0xf1ff) | mask;
    state->flags = flags;
}
