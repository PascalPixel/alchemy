#include "a8_state.h"
#include "types.h"

#define M2C_FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

typedef struct {
    u8 padding[15];
    u8 field_0f;
} Object0f;

void *Func_080150d8(s32, s32, s32, s32, s32, s32);
void Func_080a8604(s32, s32, s32);
s32 Func_080a9cf8(s32 arg0);

void Func_080a8088(s32 arg0) {
    s32 created;
    s32 handle;
    void *object;
    struct State080a8088 *state;

    state = *(struct State080a8088 **)0x03001F2C;
    created = 0;
    handle = state->handle;
    if (handle == 0) {
        created = Func_080a10d0(&state->handle, 0, 5, 0x1E, 0xF, 2);
        handle = state->handle;
    }
    if (created != 0) {
        object = Func_080150d8(arg0, 0, 0, handle, 0, 0);
        state->object = object;
        if ((((Object0f *)object)->field_0f = 0xF0, state->mode) == 3) {
            Func_080a33d4(state, handle);
        }
        Func_080a9cf8(handle);
        Func_080a8604(handle, arg0, 0x100);
        return;
    }
    Func_080a8604(handle, arg0, 0);
}
