#include "types.h"

struct State {
    u8 filler0[848];
    u32 previous;
    u32 current;
    u16 kind;
    u16 flags;
};

s32 Func_080048f4(s32 arg0, s32 arg1);
s32 Func_080072f0(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void Func_080041d8(const void *arg0, s32 arg1);

void Func_080936a0(s32 arg0, s32 arg1)
{
    struct State *state = *(struct State **)0x03001e70;
    s32 handle;
    s32 result;

    handle = Func_080048f4(27, 0xccc);
    if (*(s16 *)(handle + 414) != 3)
        return;
    result = Func_080072f0(arg0, 0x10000, 0, 0x0300013c);
    state->previous = state->current;
    state->current = result;
    state->kind = arg1;
    state->flags = 0;
    Func_080041d8((const void *)0x080935d5, 0xc94);
}
