#include "types.h"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "call.h"
s32 Object_GetById();
s32 Func_02000bcc();
s32 Func_02000b24();
s32 Func_02000b3c();
extern u8 gPartyState[];
extern u8 MsgScene666No49[];

void Func_0200024c(s32 a0)
{
    s32 t0;

    t0 = Value1(Object_GetById, FIELD_AT(gPartyState, s32, 532));
    if ((((FIELD_AT(t0, u16, 6) + 0x2000) & -16384) << 16) == -1073741824) {
        Call1(Func_02000bcc, a0);
    } else {
        Call1(Func_02000b24, (s32)MsgScene666No49);
        Call2(Func_02000b3c, a0, 0);
    }
}

