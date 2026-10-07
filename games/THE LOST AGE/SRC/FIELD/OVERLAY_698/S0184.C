#include "TYPES.H"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "CALL.H"
s32 Func_02001aec();

void Func_02000184(void)
{
    Call1(Func_02001aec, 4);
}

