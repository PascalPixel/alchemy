#include "types.h"

typedef struct {
    u8 filler0[8];
    s32 unk8;
    s32 unkC;
    s32 unk10;
} T;

typedef struct {
    u8 filler0[23];
    u8 unk17;
} State;

extern T *Func_02003b12(s32);
extern s32 Func_02003b00(s32);
extern void Func_02003b42(s32);
extern void Func_02003b50(s32);
extern s32 Func_02003b48(s32);
extern void Func_02003b62(s32);
extern void Func_02003b72(s32);

void Func_020018c4(void)
{
    T *obj;
    s32 x;
    s32 cx;
    s32 y;
    s32 r;
    s32 g;
    s32 h;
    State *st;

    obj = Func_02003b12(0);
    x = obj->unk8;
    cx = x >> 19;
    g = 0x200;
    h = 0x201;
    if ((u32)(cx - 24) > 7) {
        y = obj->unk10;
        if ((u32)((y >> 19) - 36) > 9 || (u32)(cx - 22) > 9)
            goto rest;
    }
    r = Func_02003b00(g);
    if (r != 0)
        return;
    (*(State **)0x03001e70)->unk17 = r;
    Func_02003b42(g);
    Func_02003b50(h);
    return;

rest:
    if (x > 0xE80000 && obj->unkC > 0x1E0000 && y > 0xD40000) {
        st = *(State **)0x03001e70;
        st->unk17 = 0;
        Func_02003b42(g);
        Func_02003b50(h);
        return;
    }
    r = Func_02003b48(h);
    if (r != 0)
        return;
    st = *(State **)0x03001e70;
    st->unk17 = 1;
    Func_02003b62(h);
    Func_02003b72(g);
    return;
}
