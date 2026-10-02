#include "GLOBAL_CELLS.H"
#include "TYPES.H"

struct State_080108c4 {
    u8 filler0[0x14];
    u16 flags;
};


void Map_SetWorkFlagBits9To11(u32 v)
{
    struct State_080108c4 *state = ((void *volatile *)gMapWork)[0];
    u32 mask = v & 0xe00;
    u32 flags = state->flags;

    flags = (flags & 0xf1ff) | mask;
    state->flags = flags;
}
