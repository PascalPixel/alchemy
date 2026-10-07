#include "TYPES.H"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "CALL.H"
s32 Func_020019a4();

void Func_02001af0(void)
{
    Func_020019a4(0x33, 0x27, 0x3d, 0xd, 1, 2);
}

