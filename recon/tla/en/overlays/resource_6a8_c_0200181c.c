#include "types.h"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "call.h"
s32 Func_020043e4();

void Func_0200181c(s32 a0)
{
    FIELD_AT(a0, s32, 8) = (FIELD_AT(a0, s32, 8) + (FIELD_AT((a0 + 0x64), s16, 0) << 8));
    FIELD_AT(a0, s32, 12) = (FIELD_AT(a0, s32, 12) + 0x8000);
    FIELD_AT(a0, s32, 24) = (FIELD_AT(a0, s32, 24) + 0x51e);
    FIELD_AT(a0, s32, 28) = (FIELD_AT(a0, s32, 28) + 0x51e);
    FIELD_AT((a0 + 0x64), u16, 0) = (FIELD_AT((a0 + 0x64), u16, 0) + 2);
    FIELD_AT(a0, s32, 104) = (FIELD_AT(a0, s32, 104) - 1);
    if ((FIELD_AT(a0, s32, 104) - 1) == 0) {
        Func_020043e4();
    }
}

