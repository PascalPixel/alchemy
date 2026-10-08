#include "types.h"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "call.h"
s32 Object_GetById();
s32 Func_02000a78();
s32 GameFlag_ClearBit();
s32 Func_020009b0();
s32 Func_02000a50();
s32 Func_02000a18();
s32 Func_02000a28();
s32 Func_020009b8();
extern u8 gPartyState[];
extern u8 MsgScene677No27[];

void Func_020003bc(s32 a0)
{
    s32 t0;

    t0 = Value1(Object_GetById, FIELD_AT(gPartyState, s32, 532));
    if ((((FIELD_AT(t0, u16, 6) + 0x2000) & -16384) << 16) == -1073741824) {
        Call2(Func_02000a78, 7, a0);
        Call1(GameFlag_ClearBit, 0x301);
    } else {
        Func_020009b0();
        Call1(Func_02000a50, 0);
        Call1(Func_02000a18, (s32)MsgScene677No27);
        Call2(Func_02000a28, a0, 0);
        Func_020009b8();
    }
}

