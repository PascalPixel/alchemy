#include "TYPES.H"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "CALL.H"
s32 Func_020019a4();

void Func_02001b0c(void)
{
    Func_020019a4(0x33, 0x27, 0x50, 0x10, 1, 2);
}

