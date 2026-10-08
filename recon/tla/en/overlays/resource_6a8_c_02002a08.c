#include "types.h"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "call.h"
s32 Object_GetById();
s32 ObjectMotion_SetActionVariant();
extern u8 gPartyState[];

void Func_02002a08(void)
{
    s32 t0;

    t0 = Value1(Object_GetById, FIELD_AT(gPartyState, s32, 532));
    Call2(ObjectMotion_SetActionVariant, 8, ((u32)(FIELD_AT(FIELD_AT(t0, s32, 80), u8, 9) << 28) >> 30));
    Call2(ObjectMotion_SetActionVariant, 0xa, ((u32)(FIELD_AT(FIELD_AT(t0, s32, 80), u8, 9) << 28) >> 30));
}

