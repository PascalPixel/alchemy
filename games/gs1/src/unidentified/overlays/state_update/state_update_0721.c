#include "types.h"

typedef struct {
    u8 filler0[9];
    u8 unk9;
} Struct_Sub;

typedef struct {
    u8 filler0[8];
    s32 unk8;
    u8 fillerC[4];
    s32 unk10;
    u8 filler14[48];
    s32 unk44;
    u8 filler48[8];
    Struct_Sub *unk50;
} Struct_1a50;

extern s32 Func_02001ad4(s32, s32, s32, s32);
extern void Func_020023d0(Struct_1a50 *);
extern void Func_0200687a(s32);
extern void Func_02003454(void);

void Func_02001a50(Struct_1a50 *o)
{
    Struct_Sub *q;
    s32 v;
    s32 z;
    s32 t;
    s32 m;

    q = o->unk50;
    v = q->unk9;
    if ((v & 12) == 12) {
        m = -13;
        m &= v;
        m |= 4;
        {
            u8 *pq = &q->unk9;
            *pq = m;
        }
        z = 0;
        o->unk44 = z;
        t = Func_02001ad4(o->unk8, 0, 0x2000000, 223);
        Func_020023d0(o);
        o->unk8 = z;
        o->unk10 = z;
        Func_0200687a(t);
    } else {
        Func_02003454();
    }
}
