#ifndef ALCHEMY_M7_INTERFACES_H
#define ALCHEMY_M7_INTERFACES_H

#include "types.h"

struct Bounds080a23f4 {
    u8 padding_00[8];
    u16 left;
    u16 top;
    u16 right;
    u16 bottom;
};

struct Object080a1c {
    u8 padding_00[6];
    s16 x;
    s16 y;
};

void Func_080a1c2c(
    struct Object080a1c **, s32, s32, s32, s32);
void Func_080a1c6c(
    struct Object080a1c **, s32, s32, s32, s32);
void Func_080a23f4(
    struct Bounds080a23f4 *, s32, s32, s32, s32);

#endif
