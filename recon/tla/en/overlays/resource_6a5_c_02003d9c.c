#include "types.h"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "call.h"
s32 Func_020059b4();
s32 Object_GetById();
s32 GameFlag_Test();
s32 Func_02004644();
s32 Func_02005884();
s32 ObjectDispatch_SetSingleChildFlags();
s32 Scheduler_AddOrUpdateCallback();
s32 Func_02000cd0();
s32 Party_AddActiveOwner();
s32 Func_02001388();
s32 Func_020016e8();
s32 Func_02002ae4();
s32 Func_02001bc4();
extern u8 gPartyState[];
extern u8 Func_020005b8[];
extern u8 MsgRobe[];

s32 Func_02003d9c(void)
{
    s32 t11;
    s32 t2;
    s32 t3;
    s32 t4;
    s32 t5;
    s32 t6;
    s32 t7;
    s32 t8;
    s32 t9;
    s32 t25;

    FIELD_AT((FIELD_AT(0x3000000, s32, 108) + 0x1ac), s32, 0) = 0x201;
    Call2(Func_020059b4, 0xf, 1);
    Call2(Func_020059b4, 0x10, 1);
    t2 = Value1(Object_GetById, FIELD_AT(gPartyState, s32, 532));
    FIELD_AT((t2 + 0x23), u8, 0) = (0x40 | FIELD_AT((t2 + 0x23), u8, 0));
    t3 = Value1(Object_GetById, 0x11);
    FIELD_AT((t3 + 0x23), u8, 0) = (FIELD_AT((t3 + 0x23), u8, 0) | 0x80);
    t4 = Value1(Object_GetById, 0x12);
    FIELD_AT((t4 + 0x23), u8, 0) = (FIELD_AT((t4 + 0x23), u8, 0) | 0x80);
    t5 = Value1(Object_GetById, 0x13);
    FIELD_AT((t5 + 0x23), u8, 0) = (FIELD_AT((t5 + 0x23), u8, 0) | 0x80);
    t6 = Value1(Object_GetById, 0x14);
    FIELD_AT((t6 + 0x23), u8, 0) = (FIELD_AT((t6 + 0x23), u8, 0) | 0x80);
    t7 = Value1(Object_GetById, 0x15);
    FIELD_AT((t7 + 0x23), u8, 0) = (FIELD_AT((t7 + 0x23), u8, 0) | 0x80);
    t8 = Value1(Object_GetById, 0x16);
    FIELD_AT((t8 + 0x23), u8, 0) = (0x80 | FIELD_AT((t8 + 0x23), u8, 0));
    t9 = Value1(GameFlag_Test, 0xa21);
    if (t9 == 0) {
        Func_02004644();
    } else {
        t11 = Value1(Object_GetById, 0x18);
        FIELD_AT(t11, s32, 24) = 0;
    }
    if ((u32)((FIELD_AT(gPartyState, u16, 482) - 1) << 16) > 0x10000) {
        Call3(Func_02005884, 0x18, 0, 0);
    }
    Call2(ObjectDispatch_SetSingleChildFlags, Value1(Object_GetById, 0x24), 0);
    Call2(ObjectDispatch_SetSingleChildFlags, Value1(Object_GetById, 0x24), 0);
    Call2(Scheduler_AddOrUpdateCallback, (s32)Func_020005b8, 0x480);
    if (FIELD_AT(gPartyState, s16, 482) == 9) {
        Func_02000cd0();
        FIELD_AT(gPartyState, u16, 608) = (s32)MsgRobe;
        FIELD_AT(gPartyState, u16, 610) = 1;
    } else {
        if (FIELD_AT(gPartyState, s16, 482) == 0xa) {
            Call1(Party_AddActiveOwner, 5);
            Func_02001388();
        } else {
            if (FIELD_AT(gPartyState, s16, 482) == 0xb) {
                Call1(Party_AddActiveOwner, 6);
                Func_020016e8();
            } else {
                if (FIELD_AT(gPartyState, s16, 482) == 0xc) {
                    Func_02002ae4();
                } else {
                    if (FIELD_AT(gPartyState, s16, 482) == 0xd) {
                        Func_02001bc4();
                    } else {
                        t25 = Value1(GameFlag_Test, 0xa21);
                        if (t25 != 0) {
                            Call3(Func_02005884, 0x1b, 0, 0);
                            Call3(Func_02005884, 0x1c, 0, 0);
                        }
                    }
                }
            }
        }
    }
    return 0;
}

