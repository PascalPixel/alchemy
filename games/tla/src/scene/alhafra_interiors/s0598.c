#include "types.h"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "call.h"
s32 Func_02000bbc();
extern u8 gData_02001c60[];

s32 Func_02000598(s32 a0)
{
    Call4(Func_02000bbc, (s32)gData_02001c60, FIELD_AT(a0, s32, 8), (FIELD_AT(a0, s32, 12) + 0x80000), FIELD_AT(a0, s32, 16));
    return 0;
}

