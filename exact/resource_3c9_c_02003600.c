#include "types.h"

typedef struct {
    u8 filler0[8];
    s32 unk08;
    u8 filler0c[4];
    s32 unk10;
} Obj;

typedef struct {
    u8 filler0[8];
    s32 unk08;
    u8 filler0c[4];
    s32 unk10;
    u8 filler14[0x1c];
    s32 unk30;
    u8 filler34[4];
    s32 unk38;
    u8 filler3c[4];
    s32 unk40;
} Spr;

Obj *Func_0200935a(s32);
s32 Func_02009242(s32);
s32 Func_02009250(s32);

void Func_02003600(Spr *s)
{
    Obj *m = Func_0200935a(24);
    u16 *p = (u16 *)((u8 *)s + 0x64);
    s32 h = *p;

    {
        s32 r = Func_02009242(h);
        s32 t = s->unk30 + 3;
        s->unk08 = m->unk08 + r * t;
    }
    s->unk10 = m->unk10 + (Func_02009250(h) << 1);
    s->unk38 = s->unk08;
    s->unk40 = s->unk10;
    {
        s32 t = *p + (s32)0xfffff800;
        *p = t;
    }
}
