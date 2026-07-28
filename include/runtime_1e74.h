#ifndef ALCHEMY_RUNTIME_1E74_H
#define ALCHEMY_RUNTIME_1E74_H

#include "types.h"

struct Runtime1e74 {
    u8 unknown_000[0x655];
    s8 flag_655;
    u8 unknown_656[0x1a2];
    s32 value_7f8;
    s32 value_7fc;
    volatile s32 phase;
    s32 value_804;
    s32 value_808;
    u8 unknown_80c[0x14];
    s32 parameter;
    s32 value_824;
};

typedef char Runtime1e74_SizeGuard[
    sizeof(struct Runtime1e74) == 0x828 ? 1 : -1];

static inline struct Runtime1e74 *Runtime1e74_Get(void)
{
    return *(struct Runtime1e74 **)0x03001E74;
}

#endif
