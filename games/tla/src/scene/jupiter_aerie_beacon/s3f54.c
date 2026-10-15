#include "types.h"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "call.h"
extern u8 Data_020069f8[];

void Func_02003f54(s32 a0)
{
    FIELD_AT(Data_020069f8, s32, 0) = a0;
}

