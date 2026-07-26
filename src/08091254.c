#include "types.h"

struct State_08091254 {
    u8 padding[0x2a01];
    u8 values[2];
};

extern struct State_08091254 *Data_03001ed0;
void Func_0809088c(s16 *arg0, s16 *arg1, s16 *arg2, s32 arg3);

void Func_08091254(s32 value)
{
    struct State_08091254 *state = Data_03001ed0;

    if (state != 0) {
        state->values[0] = value;
        state->values[1] = 0;
        Func_0809088c((u8 *)state + 0x380,
                      (u8 *)state + 0xe00,
                      (u8 *)state + 0x1880,
                      value);
    }
}
