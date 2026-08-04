#include "types.h"

s32 Func_080022ec(s32, s32);
s32 Func_080162d4(s32, s32, s32, s32, s32);

typedef struct State {
    u8 pad00[0x78];
    s32 handle;
    u8 pad7c[0x12];
    s16 count;
    s16 field90;
    s16 field92;
    s16 field94;
} State;

void Func_08028808(s32 arg0, s32 arg1, s32 arg2) {
    State *state;
    s16 *countPtr;
    s16 count;
    s32 i;
    s32 position;
    s32 width;
    u8 *entry;

    state = *(State **)0x03001F38;
    state->field90 = arg1 + 2;
    state->field92 = arg2;
    state->field94 = arg0;
    count = state->count;
    countPtr = &state->count;
    width = count * 3;
    width += Func_080022ec(state->field90 * 2, 3);
    width += (u32)width >> 31;
    width >>= 1;
    position = 15 - width;
    i = 0;
    if (i < count) {
        entry = (u8 *)state;
        do {
            *(s16 *)(entry + 0xC) = position * 8;
            *(s16 *)(entry + 0xE) = arg0 * 8;
            i++;
            position += 3;
            entry += 0x14;
        } while (i < *countPtr);
    }
    state->handle = Func_080162d4(position, arg0, state->field90, 3, 2);
}
