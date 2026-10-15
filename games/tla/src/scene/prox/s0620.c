#include "types.h"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "call.h"
#include "object_runtime.h"
#include "party_state.h"

struct ObjectRuntime *Object_GetById();
s32 Func_02004464();
s32 Func_020045ec();
s32 Func_0200451c();
s32 Func_0200453c();
s32 Func_0200446c();
extern u8 MsgProx33[];

void Func_02000620(void)
{
    Func_02004464();
    Call1(Func_020045ec, 0);
    Call1(Func_0200451c, (s32)MsgProx33);
    Call2(Func_0200453c, 0xf, 0);
    Func_0200446c();
}

s32 Func_02000644(void)
{
    u16 turn;
    s32 facing;

    turn = Object_GetById(gPartyState.currentOwner)->angle + 0x5fff;
    facing = 1;
    if (turn >= 0x3fff)
        facing = 0;
    return facing;
}
