#include "types.h"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "call.h"
s32 Object_GetById();
s32 Func_02004dac();
s32 Func_02004f34();
extern u8 Data_0200559c[];

s32 Func_020006f8(void)
{
    Call2(Func_02004dac, Value1(Object_GetById, 9), (s32)Data_0200559c);
    Call2(Func_02004f34, 0xa, 0x102);
    return 0;
}

