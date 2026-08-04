#include "types.h"

extern void Func_080f2ebc(s16 *arg0, s16 *arg1, s16 *arg2, s32 arg3);

typedef struct {
    u8 padding_0000[0x400];
    s16 first[0x600];
    s16 second[0x600];
    s16 output[0xa00];
    u8 padding_3000;
    s8 value;
    s8 zero;
} State;

void Func_080f3858(s32 arg0) {
    State *state = *(State **)0x03001ED0;

    if (state != 0) {
        state->value = arg0;
        state->zero = 0;
        Func_080f2ebc(state->first, state->second, state->output, arg0);
    }
}
