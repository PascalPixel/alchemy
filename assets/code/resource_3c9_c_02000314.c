#include "types.h"

typedef struct {
    u8 filler0[0x18];
    s32 unk18;
    s32 unk1c;
    u8 filler20[0x44];
    u16 unk64;
} Spr;

s32 Func_02005f8e(Spr *);
s32 Func_02005f74(s32, s32);

s32 Func_02000314(Spr *s)
{
    s16 *p = (s16 *)((u8 *)s + 100);

    switch (*p) {
    case 6:
        s->unk18 += -0x2000;
        s->unk1c += 0x1000;
        break;
    case 4:
        s->unk18 += 0x1000;
        s->unk1c += -0x800;
        break;
    case 2:
        s->unk18 += 0x800;
        s->unk1c += -0x400;
        break;
    case 0:
        s->unk18 += 0x800;
        s->unk1c += -0x400;
        *p = Func_02005f74(Func_02005f8e(s), 80) + 80;
        break;
    }
    {
        s32 t = s->unk64;
        t = t - 1;
        s->unk64 = t;
    }
    return 1;
}
