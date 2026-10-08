#include "types.h"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "call.h"
s32 Object_GetById();
s32 Engine_ObjectCreate();
s32 ObjectDispatch_SetSingleChildFlags();
s32 Func_02004dac();
extern u8 Data_020050d0[];
extern u8 Func_0200062c[];

s32 Func_02000678(void)
{
    s32 t0;
    s32 t1;

    t0 = Value1(Object_GetById, 9);
    t1 = Value4(Engine_ObjectCreate, 0xe, (FIELD_AT(t0, s32, 8) + 0x80000), (FIELD_AT(t0, s32, 12) + -131072), FIELD_AT(t0, s32, 16));
    FIELD_AT((t1 + 0x55), u8, 0) = 0;
    Call2(ObjectDispatch_SetSingleChildFlags, t1, 0);
    Call2(Func_02004dac, t1, (s32)Data_020050d0);
    FIELD_AT(t1, s32, 24) = 0x6666;
    FIELD_AT(t1, s32, 28) = 0x6666;
    FIELD_AT(FIELD_AT(t1, s32, 80), s8, 9) = ((-13 & FIELD_AT(FIELD_AT(t1, s32, 80), s8, 9)) | 4);
    FIELD_AT(t1, s32, 68) = 0;
    FIELD_AT(t1, s32, 76) = 0;
    FIELD_AT(t1, s32, 108) = (s32)Func_0200062c;
    return 0;
}

