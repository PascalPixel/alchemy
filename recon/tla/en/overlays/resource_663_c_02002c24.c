#include "types.h"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "call.h"
s32 Object_GetById();
s32 GameFlag_Test();
s32 ObjectDispatch_SetSingleChildFlags();
s32 Object_AttachWorkTargetToObject();
s32 Func_02003b50();
s32 WaitFrames();
extern u8 gPartyState[];

void Func_02002c24(s32 a0)
{
    s32 t0;
    s32 t1;

    t0 = Value1(Object_GetById, FIELD_AT((0x214 + (s32)gPartyState), s32, 0));
    t1 = Value1(GameFlag_Test, 0x109);
    if (t1 == 0) {
        FIELD_AT((t0 + 0x55), u8, 0) = t1;
        Call2(ObjectDispatch_SetSingleChildFlags, t0, 0);
        FIELD_AT(t0, s32, 12) = (FIELD_AT(t0, s32, 12) + a0);
        FIELD_AT(t0, s32, 48) = 0x10000;
        FIELD_AT(t0, s32, 40) = t1;
        FIELD_AT((0x212 + (s32)gPartyState), u8, 0) = 2;
        FIELD_AT((t0 + 0x5a), u8, 0) = 1;
        Call2(Object_AttachWorkTargetToObject, FIELD_AT((0x214 + (s32)gPartyState), s32, 0), 0);
        Func_02003b50();
        Call1(WaitFrames, 1);
    }
}

