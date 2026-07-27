#include "types.h"

#define FIXED_ONE 0x10000u
#define VERTICAL_ORIGIN 0x01e20000u

struct ObjectFlags_080ad35c {
    s8 flags;
};

struct State_080ad35c {
    u8 padding_000[0x144];
    s16 vertical_origins[4];
    u8 padding_14c[0xd8];
    u32 objects[4];
    s16 positions_x[4];
    s16 positions_y[4];
};

extern struct State_080ad35c *Data_03001f2c;

s32 Func_08009008(
    u32 object,
    u32 *request,
    u32 *motion,
    u32 limit
);

void Func_080ad35c(void)
{
    struct State_080ad35c *state = Data_03001f2c;
    s32 index;
    u32 motion[2];
    u32 request[4];

    index = 0;
    while (1) {
        u32 object = state->objects[index];

        if (object != 0) {
            u32 vertical;

            vertical =
                VERTICAL_ORIGIN -
                ((u32)(s32)state->vertical_origins[index] << 16);
            ((struct ObjectFlags_080ad35c *)(object + 9))->flags &= -13;
            motion[0] = FIXED_ONE;
            motion[1] = FIXED_ONE;
            request[0] =
                (u32)(s32)state->positions_x[index] << 16;
            request[1] = vertical;
            request[2] =
                ((u32)(s32)state->positions_y[index] << 16) + vertical;
            request[3] = 0;
            Func_08009008(object, request, motion, 0x4000);
        }
        index += 1;
        if (index > 3) {
            break;
        }
    }
}
