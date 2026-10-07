#include "TYPES.H"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "CALL.H"
s32 Func_0200102c();

void Func_02000f4c(void)
{
    Func_0200102c();
}

