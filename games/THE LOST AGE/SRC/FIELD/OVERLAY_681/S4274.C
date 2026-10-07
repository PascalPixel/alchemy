#include "TYPES.H"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "CALL.H"
s32 Func_0200495c();

void Func_02004274(s32 a0)
{
    Call2(Func_0200495c, 1, a0);
}

