#include "types.h"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "call.h"
s32 Engine_ObjectCreate();
s32 Object_SetPartAttribute();
s32 Object_SetSpritePriority();
s32 Func_020032e0();
s32 ObjectDispatch_SetSingleChildFlags();
s32 Func_020032e8();
s32 Animation_SetStateFlags();
extern u8 Data_02003674[];

void Func_020006a4(s32 a0)
{
    s32 t0;

    t0 = Value4(Engine_ObjectCreate, 0x2a0, FIELD_AT(a0, s32, 8), FIELD_AT(a0, s32, 12), FIELD_AT(a0, s32, 16));
    FIELD_AT((t0 + 0x55), u8, 0) = 0;
    Call2(Object_SetPartAttribute, t0, FIELD_AT((a0 + 0x62), u8, 0));
    Call2(Object_SetSpritePriority, t0, 2);
    Call2(Func_020032e0, t0, 1);
    Call2(ObjectDispatch_SetSingleChildFlags, t0, 0);
    Call2(Func_020032e8, t0, (s32)Data_02003674);
    Call2(Animation_SetStateFlags, t0, 1);
}

