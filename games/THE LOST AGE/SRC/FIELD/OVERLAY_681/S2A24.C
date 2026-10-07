#include "TYPES.H"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "CALL.H"
s32 Func_02004af0();

void Func_02002a24(void)
{
    Func_02004af0(0xc, 0x304);
}

