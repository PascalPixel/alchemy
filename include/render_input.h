#ifndef ALCHEMY_RENDER_INPUT_H
#define ALCHEMY_RENDER_INPUT_H

#include "types.h"

struct RenderInput {
    u8 unknown_00[12];
    u16 x;
    u16 y;
};

void *Func_0801eadc(
    s32 no,
    s32 flags,
    struct RenderInput *input,
    s32 offset_x,
    s32 offset_y);

#endif
