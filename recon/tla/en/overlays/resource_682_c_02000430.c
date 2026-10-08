#include "types.h"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "call.h"
s32 Object_GetById();
s32 Func_02002adc();
s32 Func_02002af4();
s32 Func_02002b04();
s32 Object_SetActionCallback();
extern u8 MsgScene682No07[];

void Func_02000430(s32 a0)
{
    s32 t0;

    t0 = Object_GetById();
    FIELD_AT(t0, s32, 28) = 0x10000;
    FIELD_AT(t0, s32, 24) = 0x10000;
    Call1(Func_02002adc, (s32)MsgScene682No07);
    Call2(Func_02002af4, a0, 0);
    Call2(Func_02002b04, a0, 0);
    Call2(Object_SetActionCallback, t0, 8);
}

