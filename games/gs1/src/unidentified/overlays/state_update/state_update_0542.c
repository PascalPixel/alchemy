#include "types.h"

typedef struct {
    u8 filler0[6];
    u16 unk6;
    u8 filler8[82];
    u8 unk5A;
    u8 filler5B[9];
    u16 unk64;
} T;

extern s16 Data_0200a3f0[];
extern void Func_02004100(T *, s32);
extern void Func_02004128(T *, s32);
extern void Func_02004138(T *, s32);

void Func_02001f24(s32 z)
{
    T *o;
    s32 t;
    s32 d;
    u16 prev;
    s32 n;

    o = (T *)z;
    n = o->unk64;
    z = 0;
    t = ((s16 *)&o->unk64)[z];
    if (t != 0) {
        o->unk64 = n - 1;
        return;
    }
    o->unk5A = t;
    z = 1;
    d = Data_0200a3f0[(*(u32 *)0x03001ae8 >> 4) & 0xF];
    z = -z;
    if (d == z) {
        Func_02004100(o, 9);
        return;
    }
    prev = o->unk6;
    d = (s16)(d - prev);
    if (d > 0x1000)
        d = 0x1000;
    if (d < (s32)0xFFD0F002)
        d = (s32)0xFFD0F002;
    o->unk6 = prev + d;
    Func_02004128(o, 2);
    Func_02004138(o, 0x30);
}
