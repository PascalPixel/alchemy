#include "types.h"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "call.h"
s32 Object_GetById();
s32 ObjectMotion_SetSpeedParameters();
s32 ObjectMotion_SetPositionAndCommit();
s32 WaitFrames();
s32 Object_SetPositionAndResetMotion();
s32 Battle_WaitMode0();
s32 Func_0200592c();
extern u8 gPartyState[];

void Func_0200510c(void)
{
    s32 t0;
    s32 t6;

    t0 = Value1(Object_GetById, FIELD_AT(gPartyState, s32, 532));
    Call3(ObjectMotion_SetSpeedParameters, FIELD_AT(gPartyState, s32, 532), 0x40000, 0x20000);
    Call3(ObjectMotion_SetPositionAndCommit, FIELD_AT(gPartyState, s32, 532), (((FIELD_AT(t0, s32, 8) >> 20) << 4) + 8), (((FIELD_AT(t0, s32, 16) >> 20) << 4) + 8));
    Call1(WaitFrames, 1);
    Call4(Object_SetPositionAndResetMotion, t0, (((FIELD_AT(t0, s32, 8) >> 20) << 20) + 0x80000), FIELD_AT(t0, s32, 12), (((FIELD_AT(t0, s32, 16) >> 20) << 20) + 0x80000));
    Call1(Battle_WaitMode0, 4);
    t6 = Func_0200592c();
    FIELD_AT(t6, s32, 12) = (FIELD_AT(t6, s32, 12) + -524288);
}

