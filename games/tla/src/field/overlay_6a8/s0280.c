#include "types.h"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "call.h"
s32 Func_020045f4();

void Func_02000280(void)
{
    Call3(Func_020045f4, 0x14, 0, 0x10);
}

