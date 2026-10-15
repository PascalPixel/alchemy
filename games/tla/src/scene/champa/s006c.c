#include "types.h"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "call.h"

void Func_0200006c(s32 a0)
{
    FIELD_AT(a0, s32, 24) = (FIELD_AT(a0, s32, 24) + -1536);
    FIELD_AT(a0, s32, 28) = (FIELD_AT(a0, s32, 28) + -1536);
}

