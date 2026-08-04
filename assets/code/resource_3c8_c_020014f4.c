#include "types.h"

typedef struct {
    u8 filler0[8];
    s32 unk8;
    u8 fillerC[4];
    s32 unk10;
} Struct_A;

typedef struct {
    u8 filler0[10];
    s16 unkA;
    u8 fillerC[6];
    s16 unk12;
} Struct_B;

extern u8 Value_00000986;

extern void Func_0200638c(void);
extern Struct_A *Func_020063aa(s32);
extern s32 Func_02006382();
extern void Func_02006394();
extern Struct_A *Func_020063ca(s32);
extern void Func_02006420(s32, s32, s32);
extern void Func_020063ea();
extern void Func_0200641e(s32, s32, s32);
extern void Func_0200642a(s32, s32, s32);
extern void Func_02006484(s32, s32, s32);
extern void Func_02006472(s32, s32);
extern void Func_020063f8(s32);
extern void Func_020064b6(s32);
extern void Func_020064d0(s32, s32, s32);
extern void Func_020064e2(s32, s32, s32);
extern void Func_020064fe(s32, s32, s32);
extern void Func_020064c0(s32, s32, s32);
extern void Func_0200642e(s32);
extern void Func_020064be(s32, s32);
extern void Func_0200643c(s32);
extern void Func_0200650e(s32, s32, s32);
extern void Func_020064d6(s32, s32);
extern void Func_0200645c(s32);
extern void Func_020064b8(s32, s32, s32);
extern void Func_020064e0(s32, s32);
extern Struct_B *Func_02006496(s32);
extern void Func_020064c8(s32, s32, s32);
extern void Func_020064ee(s32);
extern void Func_02006500(s32, s32, s32);
extern void Func_020064ac(void);

void Func_020014f4(void)
{
    Struct_A *o;
    Struct_B *u;
    s32 g;
    s32 m1;
    s32 m2;
    s32 h;
    s32 k;

    g = 0x986;
    m1 = 0xcccc;
    m2 = 0x6666;
    h = 0x100;
    k = 0x338;
    Func_0200638c();
    o = Func_020063aa(12);
    if (o->unk8 >> 20 == 53) {
        if (Func_02006382(g) == 0) {
            Func_02006394(g);
            o = Func_020063ca(0);
            if (o != 0) {
                Func_02006420(1, o->unk8, o->unk10);
            }
            Func_020063ea(1, m1, m2);
            Func_0200641e(1, k, 88);
            Func_0200642a(1, k, 104);
            Func_02006484(1, 0, 0);
            Func_020063ea(20);
            Func_02006472(1, 4);
            Func_020063f8(20);
            Func_020064b6(0x2691);
            Func_020064d0(1, 0, 20);
            Func_020064e2(1, 0, 10);
            Func_020064fe(1, h, 60);
            Func_020064c0(1, 0, 0);
            Func_0200642e(20);
            Func_020064be(1, 2);
            Func_0200643c(20);
            Func_0200650e(1, 0, 20);
            Func_020064be(0, 3);
            Func_020064d6(1, 3);
            Func_0200645c(30);
            Func_020064b8(1, k, 88);
            Func_020064e0(1, 2);
            u = Func_02006496(0);
            if (u != 0) {
                Func_020064c8(1, u->unkA, u->unk12);
            }
            Func_020064ee(1);
            Func_02006500(1, 0, 0);
            Func_020064ac();
        }
    }
}
