#include "types.h"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "call.h"
extern u8 Data_020062a4[];

void Func_02003edc(s32 a0)
{
    FIELD_AT(Data_020062a4, u16, 0) = a0;
}

