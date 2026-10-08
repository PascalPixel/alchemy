#include "types.h"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "call.h"
s32 Func_02000098();
extern u8 Data_0200277c[];

void Func_0200010c(s32 a0)
{
    Call3(Func_02000098, ((a0 << 3) + (s32)Data_0200277c), a0, 0);
}

