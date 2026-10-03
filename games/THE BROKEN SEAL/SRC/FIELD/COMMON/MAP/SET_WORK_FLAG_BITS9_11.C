#include "GLOBAL_CELLS.H"
#include "TYPES.H"
#include "MAP.H"



void Map_SetWorkFlagBits9To11(u32 v)
{
    struct MapState *state = ((void *volatile *)gMapWork)[0];
    u32 mask = v & 0xe00;
    u32 flags = state->flags;

    flags = (flags & 0xf1ff) | mask;
    state->flags = flags;
}
