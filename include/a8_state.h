#ifndef ALCHEMY_A8_STATE_H
#define ALCHEMY_A8_STATE_H

#include "types.h"

struct State080a8088 {
    u8 padding_000[0x24];
    s32 handle;
    u8 padding_028[0x20];
    void *entries[32];
    u8 padding_0c8[0xb4];
    void *object;
    u8 padding_180[0xa0];
    u16 mode;
};

void Func_080a33d4(struct State080a8088 *, s32);

#endif
