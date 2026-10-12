#include "types.h"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "call.h"
s32 Engine_ObjectCreate();
s32 Object_SetSpritePriority();
s32 ObjectDispatch_SetSingleChildFlags();
s32 Object_SetPartAttribute();

s32 Func_02000160(s32 a0, s32 a1, s32 a2, s32 a3)
{
    s32 t0;

    t0 = Value4(Engine_ObjectCreate, a3, a0, a1, a2);
    if (t0 != 0) {
        Call2(Object_SetSpritePriority, t0, 1);
        FIELD_AT((t0 + 0x55), u8, 0) = 0;
        FIELD_AT(((t0 + 0x55) + 4), u8, 0) = 8;
        Call2(ObjectDispatch_SetSingleChildFlags, t0, 0);
        Call2(Object_SetPartAttribute, t0, 0xf);
        FIELD_AT((t0 + 0x23), u8, 0) = (0x22 | FIELD_AT((t0 + 0x23), u8, 0));
        return t0;
    }
    return 0;
}

