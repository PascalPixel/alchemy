#include "types.h"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "call.h"
s32 Func_020043dc();
s32 Object_SetPartAttribute();
s32 ObjectDispatch_SetSingleChildFlags();
s32 Func_020043c4();
s32 Animation_SetStateFlags();
extern u8 Func_0200181c[];

void Func_02001860(s32 a0, s32 a1, s32 a2)
{
    s32 t0;

    t0 = Value4(Func_020043dc, 0x11d, a0, a1, a2);
    if (t0 != 0) {
        FIELD_AT((t0 + 0x5c), u8, 0) = 2;
        FIELD_AT((t0 + 0x55), u8, 0) = 0;
        FIELD_AT(((t0 + 0x55) + 0xf), u16, 0) = 0;
        FIELD_AT((t0 + 0x23), u8, 0) = (0xfe & FIELD_AT((t0 + 0x23), u8, 0));
        Call2(Object_SetPartAttribute, t0, 7);
        FIELD_AT(FIELD_AT(t0, s32, 80), s8, 9) = ((-13 & FIELD_AT(FIELD_AT(t0, s32, 80), s8, 9)) | 4);
        Call2(ObjectDispatch_SetSingleChildFlags, t0, 0);
        FIELD_AT(t0, s32, 24) = 0x8000;
        FIELD_AT(t0, s32, 28) = 0x8000;
        FIELD_AT(t0, s32, 104) = 0x3c;
        FIELD_AT(t0, s32, 108) = (s32)Func_0200181c;
        Call2(Func_020043c4, t0, 5);
        Call2(Animation_SetStateFlags, t0, 1);
    }
}

