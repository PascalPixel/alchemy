#include "types.h"

struct State_080108c4 {
    u8 filler0[0x14];
    u16 flags;
};

extern struct State_080108c4 * volatile Data_03001e70;

void Map_SetWorkFlagBits9To11(u32 v)
{
    struct State_080108c4 *state = Data_03001e70;
    u32 mask = v & 0xe00;
    u32 flags = state->flags;

    flags = (flags & 0xf1ff) | mask;
    state->flags = flags;
}
