#ifndef ALCHEMY_B5_CONTEXT_H
#define ALCHEMY_B5_CONTEXT_H

#include "types.h"
#include "layout_guard.h"

struct B5Context {
    void *object;
    u8 padding_04[8];
    s32 word_0c;
    s32 word_10;
    u8 padding_14[0x0c];
    void *excluded_20;
    void *excluded_24;
    u8 padding_28[2];
    s16 suppress_updates;
};

LAYOUT_OFFSET_GUARD(B5Context_ObjectOffset, struct B5Context, object, 0x00);
LAYOUT_OFFSET_GUARD(B5Context_Word0cOffset, struct B5Context, word_0c, 0x0c);
LAYOUT_OFFSET_GUARD(B5Context_Word10Offset, struct B5Context, word_10, 0x10);
LAYOUT_OFFSET_GUARD(
    B5Context_SuppressOffset, struct B5Context, suppress_updates, 0x2a);

struct B5Context *Func_080b5098(s32);

#endif
