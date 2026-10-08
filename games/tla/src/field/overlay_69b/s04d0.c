#include "types.h"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "call.h"
s32 Object_GetById();
s32 ObjectMotion_SetActionVariant();
extern u8 gPartyState[];

void Func_020004d0(s32 a0, s32 a1)
{
    s32 t0;

    t0 = Value1(Object_GetById, a1);
    FIELD_AT((t0 + 0x55), u8, 0) = 0;
    Call2(ObjectMotion_SetActionVariant, a1, 1);
    Call2(ObjectMotion_SetActionVariant, FIELD_AT(gPartyState, s32, 532), 1);
}

