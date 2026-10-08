#include "types.h"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "call.h"
extern u8 Data_02002306[];

void Func_02000e20(s32 a0)
{
    FIELD_AT(Data_02002306, u16, 0) = a0;
}

