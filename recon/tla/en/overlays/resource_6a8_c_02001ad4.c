#include "TYPES.H"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "CALL.H"
s32 Func_020019a4();

void Func_02001ad4(void)
{
    Func_020019a4(0x32, 0x27, 0x38, 0x13, 1, 2);
}

