#include "types.h"
#include "four_object_motion.h"

#define FIXED_ONE 0x10000u
#define VERTICAL_ORIGIN 0x01e20000u

struct ObjectFlags_080ad35c {
    s8 flags;
};

s32 Func_08009008(
    u32 object,
    u32 *request,
    u32 *motion,
    u32 limit
);

void Func_080ad35c(void)
{
    struct FourObjectMotionState *state = Data_03001f2c;
    s32 index;
    u32 motion[2];
    u32 request[4];

    index = 0;
    while (1) {
        u32 object = (u32)state->objects[index];

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
