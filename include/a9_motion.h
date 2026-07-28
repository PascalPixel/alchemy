#ifndef ALCHEMY_A9_MOTION_H
#define ALCHEMY_A9_MOTION_H

#include "types.h"

struct Object080a9bd8 {
    u8 padding_00[6];
    s16 x;
    s16 y;
};

void Func_080a9bd8(
    struct Object080a9bd8 *, s32, s32, s32, s32);

#endif
