#include "types.h"

typedef struct {
    u8 filler0[12];
    s32 unkC;
    u8 filler10[8];
    s32 unk18;
    s32 unk1C;
} Ent;

extern Ent *Func_0200432a(s32);
extern void Func_02004378(s32, s32, s32);
extern void Func_020043fc();
extern void Func_02004332(s32);
extern void Func_020043a6(s32, s32, s32);
extern Ent *Func_02004364(s32);
extern s32 Func_0200436c(s32);
extern void Func_0200430a(s32, s32);
extern void Func_020044c2(s32);
extern void Func_0200437c(s32);
extern void Func_020044e4(s32);
extern void Func_020043b8(s32);
extern void Func_02004434(s32, s32, s32);
extern void Func_0200443e(s32, s32, s32);

void Func_02000e30(s32 a)
{
    Ent *b;
    Ent *p;
    s32 step;
    u8 i;

    b = Func_0200432a(8);
    b->unk18 = 0x10000;
    b->unk1C = 0x10000;
    Func_02004378(a, 0x1d7, 0x122);
    Func_020043fc(a, 0xc000, 0);
    Func_02004332(10);
    Func_020043a6(8, 0x1d70000, 0x1220000);
    p = Func_02004364(a);
    Func_0200430a(Func_0200436c(a), 0);
    Func_020043fc(a, 0x100);
    *((u8 *)p + 0x55) = 0;
    Func_020044c2(0xc9);
    i = 0;
    do {
        p->unkC += 0x8000;
        Func_0200437c(1);
        i++;
    } while (i != 60);
    Func_020044e4(0xbe);

    i = 0;
    do {
        p->unkC += 0x1999;
        p->unk18 -= 0x28f;
        p->unk1C -= 0x28f;
        b->unk18 -= 0x28f;
        b->unk1C -= 0x28f;
        Func_020043b8(1);
        i++;
    } while (i != 90);
    Func_02004434(a, 0, 0);
    Func_0200443e(8, 0, 0);
}
