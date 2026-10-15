#include "types.h"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "call.h"
s32 Object_GetById();
s32 Func_02001b1c();
s32 GameFlag_Test();
s32 Func_02001ab4();
s32 Func_02001a7c();
s32 Func_02001b0c();
s32 Func_02001acc();
s32 ObjectMotion_OffsetPositionAndResetMotion();
s32 ObjectMotion_CommitCurrentPositionAndActivate();
s32 Func_02001a84();
extern u8 gPartyState[];

void Func_020000f4(void)
{
    s32 t0;
    s32 t2;

    t0 = Value1(Object_GetById, FIELD_AT(gPartyState, s32, 532));
    Call7(Func_02001b1c, 0x38, 0, 3, 3, 0x4f, 0x19, 1);
    t2 = Value1(GameFlag_Test, 0xfe2);
    if (t2 == 0) {
        Call3(Func_02001ab4, 0x40, 0x1180000, 0x1b80000);
        if ((FIELD_AT(t0, s32, 8) >> 20) == 0x11) {
            if ((FIELD_AT(t0, s32, 16) >> 20) == 0x1b) {
                Func_02001a7c();
                Call1(Func_02001b0c, 0);
                Call2(Func_02001acc, FIELD_AT(gPartyState, s32, 532), 0x102);
                Call3(ObjectMotion_OffsetPositionAndResetMotion, FIELD_AT(gPartyState, s32, 532), -16, 0);
                FIELD_AT(t0, s32, 40) = 0x60000;
                Call1(ObjectMotion_CommitCurrentPositionAndActivate, FIELD_AT(gPartyState, s32, 532));
                Func_02001a84();
            }
        }
    }
}

