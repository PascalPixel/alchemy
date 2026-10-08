#include "types.h"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "call.h"
s32 Object_GetById();
s32 Func_020029f4();
s32 Func_02000080();
extern u8 gPartyState[];
extern u8 Func_0200006c[];
extern u8 Data_020031fc[];

void Func_020000fc(s32 a0)
{
    s32 t0;

    if ((u32)(a0 - 1) <= 1) {
        t0 = Value1(Object_GetById, FIELD_AT(gPartyState, s32, 532));
        FIELD_AT(t0, s32, 108) = (s32)Func_0200006c;
    }
    if (a0 == 7) {
        Call1(Func_020029f4, 8);
        Call3(Func_02000080, (s32)Data_020031fc, 7, 1);
    } else {
        Call3(Func_02000080, ((s32)Data_020031fc + 8), a0, 0);
    }
}

