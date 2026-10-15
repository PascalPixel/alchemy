#include "types.h"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "call.h"
extern u8 gData_02002198[];

void Func_0200022c(s32 a0)
{
    FIELD_AT(gData_02002198, s32, 0) = a0;
}

