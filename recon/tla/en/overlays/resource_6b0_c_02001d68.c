#include "types.h"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "call.h"
s32 Object_GetById();
s32 Func_02003398();
s32 Func_02003488();
s32 Object_SetModeById();
s32 Func_020033e0();
s32 Event_StartScreenTransition();
s32 Func_02003310();
s32 Func_02003318();
s32 GameFlag_SetBit();
s32 Func_020033a0();
extern u8 gPartyState[];

void Func_02001d68(void)
{
    s32 t6;
    s32 t0;

    t0 = Value1(Object_GetById, 9);
    Func_02003398();
    Call1(Func_02003488, 0);
    Call2(Object_SetModeById, 9, 2);
    FIELD_AT(t0, s32, 12) = -4194304;
    Call3(Func_020033e0, 9, 0x2d80000, 0x780000);
    Call3(Func_020033e0, FIELD_AT(gPartyState, s32, 532), 0x2d80000, 0x780000);
    t6 = Value1(Object_GetById, FIELD_AT(gPartyState, s32, 532));
    FIELD_AT(t6, s32, 12) = -3145728;
    Event_StartScreenTransition();
    FIELD_AT(t0, s32, 52) = 0x10000;
    FIELD_AT(t0, s32, 48) = 0x10000;
    Call4(Func_02003310, t0, FIELD_AT(t0, s32, 8), (FIELD_AT(t0, s32, 12) + 0x300000), FIELD_AT(t0, s32, 16));
    Call1(Func_02003318, t0);
    Call1(GameFlag_SetBit, 0x300);
    Func_020033a0();
}

