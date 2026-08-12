#include "types.h"
#include "four_object_motion.h"

void Func_08015418(void *);
s32 Func_080022ec(s32, s32);
s32 Func_08009008(u32, u32 *, u32 *, u32);

#define FIXED_ONE 0x10000u
#define VERTICAL_ORIGIN 0x01e20000u

struct ObjectFlags_080ad40c {
    s8 flags;
};

void Func_080ad40c(void)
{
    struct FourObjectMotionState *state = Data_03001f2c;
    s32 index;
    u32 motion[2];
    u32 request[4];

    Func_08015418((void *)0x06002500);
    index = 0;
    while (1) {
        u32 object = (u32)state->objects[index];

        if (object != 0) {
            u32 vertical;
            s32 phase = state->phases[index];

            vertical =
                VERTICAL_ORIGIN -
                ((u32)(s32)state->vertical_origins[index] << 16);
            ((struct ObjectFlags_080ad40c *)(object + 9))->flags &= -13;
            if (phase < 0) {
                motion[0] = -phase;
                motion[1] = -phase;
            } else {
                phase += Func_080022ec(FIXED_ONE - phase, 3);
                motion[0] = phase;
                motion[1] = phase;
                state->phases[index] = motion[0];
            }
            request[0] =
                (u32)(s32)state->positions_x[index] << 16;
            request[1] = vertical;
            request[2] =
                ((u32)(s32)state->positions_y[index] << 16) + vertical;
            request[3] = 0;
            Func_08009008(
                object,
                request,
                motion,
                state->positions_y[index] < 0 ? 0x8000 : 0x4000
            );
        }
        index += 1;
        if (index > 3) {
            break;
        }
    }
}
