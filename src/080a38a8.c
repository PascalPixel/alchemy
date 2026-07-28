#include "far_runtime.h"
#include "types.h"

struct State080a38a8 {
    u8 padding_00[0x20];
    s32 *handle;
};

void Func_080a9a5c(s32 arg0, s32 arg1, s32 arg2);
void Func_080a9cbc(void);

void Func_080a38a8(s32 mode) {
    struct State080a38a8 *state;

    state = *(struct State080a38a8 **)0x03001F2C;
    Func_080a9cbc();
    Func_08015270(state->handle);
    Func_080a9a5c(state->handle, mode, 0);
}
