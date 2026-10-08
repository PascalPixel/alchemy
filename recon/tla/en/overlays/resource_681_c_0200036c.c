#include "types.h"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "call.h"
s32 ObjectMotion_SetActionVariant();
s32 OverlayObject_PrepareObject();
s32 Func_02004eec();
s32 Func_02005014();
s32 GameFlag_SetBit();
s32 Func_02004f6c();
extern u8 gPartyState[];

void Func_0200036c(void)
{
    Call2(ObjectMotion_SetActionVariant, FIELD_AT(gPartyState, s32, 532), 3);
    OverlayObject_PrepareObject(0x2c80000, 0, 0x1d40000);
    OverlayObject_PrepareObject(0x2c80000, 0, 0x1ea0000, 0x11e);
    Call3(Func_02004eec, FIELD_AT(gPartyState, s32, 532), 6, 0x800);
    Call1(Func_02005014, 0x136);
    Call1(GameFlag_SetBit, 0x122);
    Call1(Func_02004f6c, 8);
}

