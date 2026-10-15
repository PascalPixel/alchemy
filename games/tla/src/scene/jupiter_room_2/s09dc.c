#include "types.h"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "call.h"
extern u8 Data_02002b38[];

void Func_020009dc(s32 a0)
{
    FIELD_AT(Data_02002b38, s32, 0) = a0;
}

