#include "types.h"
#include "scene.h"
#include "abi/map/shared/set_work_flag_bits9_11.h"

struct State_080108c4 {
    u8 filler0[0x14];
    u16 flags;
};

extern struct State_080108c4 *volatile gCam;

void Map_SetWorkFlagBits9To11(u32 v)
{
    struct State_080108c4 *state = gCam;
    u32 mask = v & 0xe00;
    u32 flags = state->flags;

    flags = (flags & 0xf1ff) | mask;
    state->flags = flags;
}
