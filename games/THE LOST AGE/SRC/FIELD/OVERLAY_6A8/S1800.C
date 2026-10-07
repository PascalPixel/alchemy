#include "TYPES.H"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "CALL.H"
s32 Func_0200442c();

void Func_02001800(void)
{
    Call6(Func_0200442c, 0x19, 3, 1, 1, 0x19, 9);
}

