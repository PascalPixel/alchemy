#include "TYPES.H"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "CALL.H"
s32 Func_020019a4();

void Func_02001a9c(void)
{
    Func_020019a4(0x30, 0x27, 0x39, 0x1a, 1, 2);
}

