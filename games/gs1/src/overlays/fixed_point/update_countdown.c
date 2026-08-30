#include "types.h"

struct FixedPointCountdown {
    u8 pad_00[0x18];
    s32 fixed_point_18;
    s32 fixed_point_1c;
    u8 pad_20[0x44];
    s16 countdown;
};

extern u32 Func_020046fa(u32 value, u32 divisor);
extern u32 Func_02004714(void);

#define UnsignedRemainder Func_020046fa
#define NextRandomValue Func_02004714
#define UpdateFixedPointCountdown Func_02000064

s32 UpdateFixedPointCountdown(struct FixedPointCountdown *state) {
    switch (state->countdown) {
    case 6:
        state->fixed_point_18 += (s32) 0xFFFFC000;
        state->fixed_point_1c += 0x2000;
        break;
    case 4:
        state->fixed_point_18 += 0x2000;
        state->fixed_point_1c += (s32) 0xF870F001;
        break;
    case 2:
        state->fixed_point_18 += 0x1000;
        state->fixed_point_1c += (s32) 0xFFFFF800;
        break;
    case 0:
        state->fixed_point_18 = 0x10000;
        state->fixed_point_1c = 0x10000;
        state->countdown = (s16) (UnsignedRemainder(NextRandomValue(), 90) + 60);
        break;
    }
    state->countdown--;
    return 1;
}
