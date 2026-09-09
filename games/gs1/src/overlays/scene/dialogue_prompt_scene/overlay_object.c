#include "types.h"

#define OverlayObject_GetObjectTwoByte118 Func_02002f84
#define OverlayObject_RunObjectTwoWhenFlagged Func_02002f98
#define OverlayObject_ConfigureObject22WithResource17 Func_02004698
u8 *Func_02007c8e(s32);
void Func_02007eb8(void);
u8 *Func_02007ca6(s32);
void Func_02007ce6(s32, s32, s32, s32);
void Func_02007ee4(s32);
void Func_02007cd2(s32);
s32 Func_02007cd8(s32);
u8 *Func_02009356(s32);
u8 *Func_02009370(s32, s32);
void Func_020093e0(s32);
void Func_02009398(s32, s32, u8 *);
void Func_02009396(s32);

s32 OverlayObject_GetObjectTwoByte118(void)
{
    return Func_02007c8e(2)[0x118];
}

s32 OverlayObject_RunObjectTwoWhenFlagged(void)
{
    u8 *o;

    Func_02007eb8();
    o = Func_02007ca6(2);
    o += 0xf8;
    if (*(s32 *)o & 1) {
        Func_02007ce6(2, 0, 0, 0);
        Func_02007ee4(126);
        Func_02007cd2(0);
        return Func_02007cd8(2);
    }
}

void OverlayObject_ConfigureObject22WithResource17(s32 a)
{
    u8 *o;
    u8 *q;
    u8 *p;
    u8 *v;
    s32 z;
    s32 m;

    z = 0;
    o = Func_02009356(22);
    if (o != 0) {
        q = *(u8 **)(o + 0x50);
        p = q + 38;
        *p = z;
        p += 1;
        *p = z;
        m = 33;
        m = -m;
        q[5] &= m;
        q[9] &= 15;
        o[0x55] = z;
        o[0x5c] = 1;
        v = Func_02009370(17, 0x608);
        Func_020093e0(a);
        v += 0x400;
        Func_02009398(q[28], 0x80, v);
        Func_02009396(17);
    }
}
