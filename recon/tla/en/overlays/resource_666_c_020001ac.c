#include "types.h"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "call.h"
s32 Object_GetById();
s32 Func_02000bc4();
s32 Func_02000b24();
s32 Func_02000b3c();
extern u8 gPartyState[];
extern u8 MsgScene666No14[];

void Func_020001ac(s32 a0)
{
    s32 t0;

    t0 = Value1(Object_GetById, FIELD_AT(gPartyState, s32, 532));
    if ((((FIELD_AT(t0, u16, 6) + 0x2000) & -16384) << 16) == -1073741824) {
        Call2(Func_02000bc4, 9, a0);
    } else {
        Call1(Func_02000b24, (s32)MsgScene666No14);
        Call2(Func_02000b3c, a0, 0);
    }
}

