#include "types.h"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "call.h"

void Func_02000410(void)
{
    FIELD_AT((FIELD_AT(0x3000000, s32, 108) + 0x16a), u16, 0) = 0x1f;
    FIELD_AT((FIELD_AT(0x30000e0, s32, 0) + 0x34), u8, 0) = 1;
}

