#include "types.h"

struct Ent {
    s32 a;
    u16 b;
    u16 c;
};

extern u8 *Data_03001ebc;
extern struct Ent Data_02009d00[];

extern void Func_020013a8(void);
extern s32 Func_020013d2(s32);
extern void Func_020014dc(s32);
extern s32 Func_0200139e(s32, s32, s32);
extern void Func_0200141c(s32, s32, s32);
extern s32 Func_0200141a(s32);
extern void Func_02001468(s32, s32);
extern void Func_02001460(s32, s32, s32);
extern void Func_02001416(s32);
extern void Func_020014fe(s32);
extern void Func_0200150a(void);
extern void Func_02001516(void);
extern void Func_0200143a(void);

void Func_02000644(void) {
    u8 *base;
    s16 *h;
    u32 i;
    s32 o;
    s32 k;
    u8 *p;

    base = Data_03001ebc;
    Func_020013a8();
    for (i = 8; i <= 65; i++) {
        o = Func_020013d2(i);
        if (o != 0) {
            *(u8 *)(o + 85) = 0;
        }
    }
    h = (s16 *)(base + 364);
    k = *h - 1;
    Func_020014dc(158);
    { s32 t1 = Data_02009d00[k].b; s32 t2 = Data_02009d00[k].c; Func_0200139e(Data_02009d00[k].a, t1, t2); }
    Func_0200141c(0, 0x8000, 0x4000);
    p = (u8 *)(Func_0200141a(0) + 85);
    *p = 0;
    Func_02001468(0, 2);
    if (k != 6) {
        Func_02001460(0, 2, -8);
        Func_02001416(10);
    }
    Func_020014fe(*h);
    Func_0200150a();
    Func_02001516();
    Func_0200143a();
}
