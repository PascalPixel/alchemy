#include "TYPES.H"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "CALL.H"
s32 Func_02001d3c();

void Func_02000a98(void)
{
    Func_02001d3c();
}

