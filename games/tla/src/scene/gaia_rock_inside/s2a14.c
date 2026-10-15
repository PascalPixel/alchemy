#include "types.h"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "call.h"
s32 Func_02004af0();

void Func_02002a14(void)
{
    Func_02004af0(0xb, 0x303);
}

