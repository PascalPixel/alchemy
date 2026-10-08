#include "types.h"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "call.h"
extern u8 Data_02006b72[];

void Func_02004b48(s32 a0)
{
    FIELD_AT(Data_02006b72, u16, 0) = a0;
}

