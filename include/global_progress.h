#ifndef ALCHEMY_GLOBAL_PROGRESS_H
#define ALCHEMY_GLOBAL_PROGRESS_H

#include "types.h"
#include "layout_guard.h"

struct GlobalProgressPartialView {
    u8 padding_000[0x118];
    s32 value_118;
    s8 value_11c;
};

LAYOUT_OFFSET_GUARD(
    GlobalProgress_Value118Offset,
    struct GlobalProgressPartialView,
    value_118,
    0x118);
LAYOUT_OFFSET_GUARD(
    GlobalProgress_Value11cOffset,
    struct GlobalProgressPartialView,
    value_11c,
    0x11c);

extern u8 Data_02000240[];

static inline struct GlobalProgressPartialView *GlobalProgress_Get(void)
{
    return (struct GlobalProgressPartialView *)Data_02000240;
}

#endif
