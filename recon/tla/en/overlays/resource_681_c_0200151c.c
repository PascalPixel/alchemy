#include "types.h"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "call.h"

void Func_0200151c(s32 a0)
{
    FIELD_AT(a0, s32, 8) = (FIELD_AT(a0, s32, 8) + FIELD_AT(a0, s32, 68));
    FIELD_AT(a0, s32, 12) = (FIELD_AT(a0, s32, 12) + FIELD_AT(a0, s32, 76));
    FIELD_AT(a0, s32, 16) = (FIELD_AT(a0, s32, 16) + FIELD_AT(a0, s32, 72));
    FIELD_AT((a0 + 0x62), u8, 0) = (FIELD_AT((a0 + 0x62), u8, 0) + 0xff);
}

