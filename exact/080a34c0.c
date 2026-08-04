#include "types.h"

void Func_080030f8(s32);
void Func_080a1114(void *, s32);
void Func_080a195c(void);
void Func_080a345c(void);

void Func_080a34c0(void) {
    u8 *state;
    s8 *controller;

    state = *(u8 **)0x03001F2C;
    Func_080a195c();
    Func_080a345c();
    Func_080030f8(1);
    controller = *(s8 **)(state + 0x17C);
    controller[5] = 0xD;
    Func_080a1114(state + 0x10, 1);
    Func_080a1114(state + 0x20, 1);
    Func_080a1114(state + 0x10C, 1);
    Func_080a1114(state + 0x24, 1);
    Func_080a1114(state + 0x28, 1);
    Func_080a1114(state + 0x2C, 1);
    Func_080a1114(state + 0x30, 1);
    Func_080a1114(state + 0x34, 1);
    Func_080a1114(state + 0x38, 1);
    Func_080a1114(state + 0x3C, 1);
    Func_080a1114(state + 0x40, 1);
}
