#include "types.h"

typedef struct Obj {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
    u8 f14[28];
    s32 f30;
    s32 f34;
    u8 f38[29];
    u8 f55;
} Obj;

extern s32 Data_02000240[];

extern Obj *Func_02004212(s32);
extern Obj *Func_0200421a(s32);
extern void Func_020041ca(s32);
extern void Func_0200420e(void);
extern void Func_02004276(s32, s32);
extern void Func_02004214(s32);
extern void Func_02004386(s32);
extern void Func_0200413e(Obj *, s32);
extern void Func_0200417e(Obj *, s32, s32, s32);
extern void Func_02004244(s32);
extern void Func_020042b4(s32, s32);
extern s32 *Func_020040fc(s32, s32);
extern void Func_0200418a(s32, Obj *);
extern void Func_0200429c(s32, s32, s32);
extern void Func_020041be(Obj *, s32, s32, s32);
extern void Func_020042dc(s32);
extern void Func_020042f4(s32, s32);
extern void Func_020041da(Obj *);
extern void Func_020041aa(Obj *, s32);
extern void Func_02004402(s32);
extern void Func_02004408(s32);
extern void Func_020042ae(s32);
extern void Func_020042c2(void);
extern void Func_02004226(s32, s32, s32, s32, s32, s32);
extern void Func_02004238(s32, s32, s32, s32, s32, s32);
extern s32 Func_0200429e(s32);
extern void Func_020042f0(void);
extern void Func_020043cc(s32, s32);
extern void Func_020043e6(s32, s32, s32, s32);
extern void Func_020043f2(void);
extern void Func_02004274(s32, s32, s32, s32, s32, s32);
extern void Func_02004322(s32);
extern void Func_0200428a(s32, s32, s32, s32, s32, s32);
extern void Func_02004338(s32);
extern void Func_020042a0(s32, s32, s32, s32, s32, s32);
extern void Func_0200434e(s32);
extern void Func_020042b6(s32, s32, s32, s32, s32, s32);
extern void Func_02004364(s32);
extern void Func_020042cc(s32, s32, s32, s32, s32, s32);
extern void Func_0200437a(s32);
extern void Func_0200438e(void);
extern void Func_02004392(void);
extern void Func_0200446e(s32, s32);
extern void Func_02004488(s32, s32, s32, s32);
extern void Func_02004494(void);
extern Obj *Func_020043d2(s32);
extern void Func_02004314(Obj *, s32, s32, s32);
extern void Func_020042ec(Obj *, s32);
extern void Func_02004346(s32, s32, s32, s32, s32, s32);
extern void Func_020043f4(s32);
extern void Func_0200435c(s32, s32, s32, s32, s32, s32);
extern void Func_0200440a(s32);
extern void Func_02004372(s32, s32, s32, s32, s32, s32);
extern void Func_02004420(s32);
extern void Func_02004388(s32, s32, s32, s32, s32, s32);
extern void Func_02004436(s32);
extern void Func_0200439e(s32, s32, s32, s32, s32, s32);
extern Obj *Func_0200446c(s32);
extern void Func_020043aa(Obj *, s32, s32, s32);
extern void Func_020043b8(Obj *);
extern void Func_02004476(s32);
extern void Func_0200448a(void);
extern void Func_020043ee(s32, s32, s32, s32, s32, s32);

void Func_02000540(void)
{
    Obj *a;
    Obj *b;
    s32 g;
    s32 t;

    g = Data_02000240[125];
    a = Func_02004212(g);
    b = Func_0200421a(12);
    Func_020041ca(0x302);
    Func_0200420e();
    Func_02004276(g, 8);
    Func_02004214(6);
    b->f30 = 0x8000;
    b->f34 = 0x3333;
    Func_02004386(239);
    Func_0200413e(b, 2);
    Func_0200417e(b, b->f08 - 0x300000, 0, b->f10);
    Func_02004244(6);
    Func_020042b4(g, 2);
    Func_0200418a(Func_020040fc(27, 0xCCC)[120], b);
    Func_0200429c(g, 0x4CCC, 0x3333);
    Func_020041be(a, a->f08 - 0x180000, 0, a->f10);
    Func_020042dc(g);
    Func_020042f4(g, 1);
    Func_020041da(b);
    Func_020041aa(b, 1);
    Func_02004402(288);
    Func_02004408(213);
    Func_020042ae(15);
    Func_020042c2();
    Func_02004226(37, 7, 1, 4, 34, 7);
    Func_02004238(36, 7, 1, 4, 37, 7);
    t = Func_0200429e(0x301);
    if (t != 0) {
        Func_020042f0();
        Func_020043cc(0x20000, 0x4000);
        Func_020043e6(0x2280000, -1, 0xC80000, 1);
        Func_020043f2();
        Func_02004274(96, 29, 1, 3, 34, 38);
        Func_02004322(3);
        Func_0200428a(97, 29, 1, 3, 34, 38);
        Func_02004338(3);
        Func_020042a0(98, 29, 1, 3, 34, 38);
        Func_0200434e(3);
        Func_020042b6(99, 29, 1, 3, 34, 38);
        Func_02004364(3);
        Func_020042cc(100, 29, 1, 3, 34, 38);
        Func_0200437a(15);
        Func_0200438e();
    } else {
        Func_0200434e(0x301);
        Func_02004392();
        Func_0200446e(0x20000, 0x4000);
        Func_02004488(0x2580000, -1, 0xC80000, 1);
        Func_02004494();
        b = Func_020043d2(13);
        b->f55 = t;
        b->f34 = 0x6666;
        b->f30 = 0xCCCC;
        Func_02004314(b, b->f08, 0x80000, b->f10);
        Func_020042ec(b, 3);
        Func_02004346(96, 29, 1, 3, 34, 38);
        Func_020043f4(3);
        Func_0200435c(97, 29, 1, 3, 34, 38);
        Func_0200440a(3);
        Func_02004372(98, 29, 1, 3, 34, 38);
        Func_02004420(3);
        Func_02004388(99, 29, 1, 3, 34, 38);
        Func_02004436(3);
        Func_0200439e(100, 29, 1, 3, 34, 38);
        b = Func_0200446c(14);
        b->f55 = t;
        b->f34 = 0x6666;
        b->f30 = 0xCCCC;
        Func_020043aa(b, b->f08, 0x200000, b->f10);
        Func_020043b8(b);
        Func_02004476(15);
        Func_0200448a();
        {
            s32 x = 41;
            s32 y = 12;

            Func_020043ee(43, 12, 1, 1, x, y);
        }
    }
}
