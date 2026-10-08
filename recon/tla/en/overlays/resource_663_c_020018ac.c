#include "types.h"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "call.h"
extern u8 IwramFillWords[];

void Func_020018ac(s32 a0, s32 a1)
{
    ((s32 (*)())((s32)IwramFillWords + 116))(((((FIELD_AT(a0, s32, 0) - FIELD_AT(a1, s32, 0)) >> 16) * ((FIELD_AT(a0, s32, 0) - FIELD_AT(a1, s32, 0)) >> 16)) + (((FIELD_AT(a0, s32, 8) - FIELD_AT(a1, s32, 8)) >> 16) * ((FIELD_AT(a0, s32, 8) - FIELD_AT(a1, s32, 8)) >> 16))));
}

