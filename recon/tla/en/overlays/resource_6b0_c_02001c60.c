#include "types.h"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "call.h"
s32 Func_020000f4();
s32 Func_020034b0();
s32 Func_02003448();
s32 Battle_WaitMode0();
s32 Func_02000158();
extern u8 gPartyState[];
extern u8 MsgClothing[];

void Func_02001c60(void)
{
    Func_020000f4();
    Func_020034b0();
    FIELD_AT(gPartyState, u8, 706) = 2;
    Call2(Func_02003448, (s32)MsgClothing, 0x63);
    Call1(Battle_WaitMode0, 0x28);
    Func_02000158();
}

