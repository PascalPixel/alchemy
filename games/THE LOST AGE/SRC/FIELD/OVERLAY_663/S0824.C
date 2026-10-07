#include "TYPES.H"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "CALL.H"
s32 Func_02003c88();

void Func_02000824(void)
{
    Call1(Func_02003c88, 0);
}

