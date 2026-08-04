#include "types.h"

typedef struct {
    u8 filler0[8];
    s32 unk8;
    s32 unkC;
    s32 unk10;
    u8 filler14[4];
    s32 unk18;
    s32 unk1C;
} Sub;

typedef struct {
    u8 filler0[8];
    s32 unk8;
    s32 unkC;
    s32 unk10;
    u8 filler14[4];
    s32 unk18;
    s32 unk1C;
    u8 filler20[72];
    Sub *unk68;
} Ent;

extern void Func_020062d4(Ent *);
extern s32 Func_02006284(s32);
extern void Func_02005cb0(s32);

void Func_02002e5c(Ent *p)
{
    u16 *ctr;
    Sub *q;
    s32 n;
    s32 v;
    s32 d;

    ctr = (u16 *)((u8 *)p + 100);
    n = *ctr + 1;
    q = p->unk68;
    *ctr = n;
    v = (s16)n;
    if (v > 31) {
        Func_020062d4(p);
        return;
    }
    v = Func_02006284(v << 10);
    p->unk18 = v;
    p->unk1C = -v;
    p->unk8 = q->unk8;
    d = 0x10000;
    p->unkC += d;
    d = d - v;
    p->unk10 = q->unk10 - d * 5 + 0x100000;
}
