#include "types.h"

#define Func_020028b0 Func_020028b0

void Func_020029b8();
void Func_020029c4();
s32 Func_02002d14();
void Func_02005486();
void Func_02007b30();
void Func_0200837c();
void Func_0200838a();
s32 Func_0200838c();
void Func_02008398();
void Func_020083b4();
void Func_020083e0();
s32 Func_020083e6();
void Func_020083fa();
void Func_02008432();
s32 Func_02008456();
s32 Func_02008462();
void Func_02008470();
void Func_0200847a();
void Func_02008484();
void Func_02008486();
void Func_0200848a();
void Func_0200848c();
s32 Func_02008496();
void Func_02008498();
void Func_020084b8();
void Func_020084d2();
void Func_020084d6();
void Func_020084da();
void Func_020084e8();
void Func_020084ec();
void Func_020084ec_a();
void Func_02008504();
void Func_0200850e();
void Func_02008520();
void Func_02008534();
void Func_02008552();
void Func_02008552_a();
void Func_02008564();
void Func_02008576();
void Func_0200857c();
void Func_02008580();
void Func_02008586();
void Func_02008592();
void Func_02008592_a();
void Func_0200859e();
void Func_020085a2();
void Func_020085a4();
void Func_020085aa();
void Func_020085b4();
void Func_020085b6();
void Func_020085b6_a();
void Func_020085c4();
void Func_020085ca();
void Func_020085d0();
void Func_020085d6();
s32 Func_020085e6();
s32 Func_020085e6_a();
void Func_020085f2();
void Func_020085f8();
void Func_02008604();
void Func_02008606();
void Func_0200861e();
void Func_02008624();
void Func_02008644();
void Func_02008644_a();
void Func_02008646();
void Func_02008652();
void Func_02008658();
void Func_0200865a();
void Func_0200865c();
void Func_02008660();
void Func_02008666();
void Func_0200867a();
void Func_0200867c();
void Func_02008694();
s32 Func_02008696();
void Func_0200869e();
void Func_0200869e_a();
void Func_020086b4();
void Func_020086be();
void Func_020086c2();
void Func_020086ca();
void Func_020086d6();
void Func_020086ec();
void Func_02008716();
void Func_02008718();
void Func_02008726();
void Func_0200873c();
void Func_0200875e();
void Func_02008764();
void Func_0200876a();
void Func_02008776();
void Func_020087a0();
void Func_020087a4();
void Func_020087a8();
void Func_020087b2();
void Func_020087c2();
void Func_020087ca();
void Func_020087ce();
void Func_020087e6();
void Func_020087e6_a();
void Func_020087e8();
void Func_020087f0();
void Func_020087fc();
void Func_02008808();
void Func_02008812();
s32 Func_0200881e();
s32 Func_0200881e_a();
void Func_0200882e();
s32 Func_0200883e();
void Func_02008850();
void Func_02008852();
void Func_0200885a();
void Func_0200885c();
void Func_02008862();
void Func_0200886a();
void Func_02008870();
void Func_02008870_a();
void Func_02008880();
void Func_02008890();
void Func_02008894();
void Func_0200889e();
void Func_020088a0();
void Func_020088a2();
void Func_020088ac();
void Func_020088ae();
void Func_020088b0();
void Func_020088c0();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ s32 Value4(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    return f(a0, a1, a2, a3);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

void Func_020028b0(void)
{
    u32 i;
    s32 record;
    u8 *p5;

    record = Value1(Func_0200838c, 10);
    p5 = *(s32 *)(record + 8);
    Func_0200837c();
    if (((s32)p5 / 0x100000) != 51) {
    } else {
        Call3(Func_020083b4, 3, 0xcccc, 0x6666);
        Func_0200838a(20);
        Func_02008432(3, 2);
        Func_02008398(20);
        Call3(Func_0200848c, 3, 0xd000, 0);
        Call3(Func_02008498, 0, 0x5000, 10);
        Call1(Func_02008486, 0x157f);
        Value2(Func_02008496, 3, 0);
        if (Value2(Func_020083e6, 0, 0) == 0) {
            *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
            Func_020083e0(20);
            Func_02008470(3, 3);
            Func_020084d2(3, 0, 20);
        } else {
            Func_020083fa(20);
            Func_0200848a(3, 4);
            Func_020084ec(3, 0, 20);
            *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
        }
        Call3(Func_02008520, 0, 0x100, 60);
        record = Func_02008456(1);
        Func_020029b8(record, 1);
        record = Func_02008462(2);
        Func_020029c4(record, 1);
        Call3(Func_0200847a, 1, 0xcccc, 0x6666);
        Call3(Func_02008484, 2, 0xcccc, 0x6666);
        Call3(Func_020084da, 1, 0x3680000, 0x2580000);
        Call3(Func_020084e8, 2, 0x3680000, 0x2580000);
        Call3(Func_020084d6, 2, 0x378, 0x278);
        Call3(Func_020084ec_a, 1, 0x370, 0x268);
        Call3(Func_02008580, 1, 0x5000, 0);
        Func_0200850e(2);
        Call3(Func_02008592, 2, 0x8000, 0);
        Func_02008552(1, 1);
        Func_020084b8(20);
        Func_020085a2(1, 0, 20);
        Call3(Func_020085c4, 3, 0x101, 60);
        Func_020085b6(3, 0, 20);
        Call3(Func_020085ca, 1, 0x3000, 0);
        Call3(Func_020085d6, 2, 0xb000, 20);
        Func_02008576(1, 3);
        Func_02008586(2, 3);
        Func_02008504(30);
        Call3(Func_020085f8, 1, 0x5000, 0);
        Call3(Func_02008604, 2, 0x8000, 20);
        Func_02008606(2, 0, 20);
        Func_020085b6_a(3, 3);
        Func_02008534(20);
        Func_0200861e(3, 0, 20);
        Call3(Func_020085a4, 3, 0x348, 0x290);
        Func_02008552_a(5);
        Call3(Func_02008646, 2, 0x5000, 0);
        Func_02008564(10);
        Call3(Func_02008658, 0, 0x4000, 0);
        ((void (*)())Func_020085e6)(3);
        Func_0200857c(10);
        Func_02008666(3, 0, 20);
        Call3(Func_0200867a, 3, 0xd000, 20);
        Func_0200867c(3, 0, 20);
        Func_02008644(1, 1);
        Func_020085aa(20);
        Call3(Func_0200869e, 1, 0xc000, 20);
        Value2(Func_02008696, 1, 0);
        if (Value2(Func_020085e6_a, 0, 0) == 0) {
            Func_020085d0(20);
            Func_02008660(1, 3);
            Func_020086c2(1, 0, 20);
            *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
        } else {
            *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
            Func_02008624(20);
            Func_020086b4(1, 4);
            Func_02008716(1, 0, 20);
        }
        Func_020086be(3, 16);
        Func_02008644_a(30);
        Func_020086ec(3, 1);
        Func_02008652(20);
        Func_0200873c(3, 0, 20);
        Call3(Func_020086ca, 3, 0x348, 0x278);
        Call3(Func_0200875e, 0, 0x5000, 0);
        Call3(Func_0200876a, 1, 0x5000, 0);
        Call3(Func_02008776, 2, 0x8000, 0);
        Func_02008694(30);
        Func_02007b30();
        Func_0200869e_a(50);
        Func_0200885c(131);
        Call2(Func_020087e6, 0x10000, 0);
        Call2(Func_020087e6_a, 0x207e9f, 0);
        Func_020087fc(10);
        Func_02008592_a(1);
        Func_02008880(220);
        Func_0200859e(40);
        Call2(Func_02008808, 0x10000, 0);
        ((void (*)())Func_0200881e)(60);
        Func_020085b4(60);
        Func_020088a2(209);
        Func_02005486();
        Call6(Func_0200865a, 126, 35, 116, 35, 1, 2);
        Call3(Func_0200865c, 0x200e0b8, 116, 35);
        Call1(Func_020085f2, 0x200a649);
        Func_02008718(20);
        Func_020087a8(3, 3);
        Func_02008726(20);
        Call3(Func_02008764, 3, 0x30000, 0x18000);
        Func_020087ce(3, 4, 0);
        Call3(Func_020087a4, 3, 0x348, 0x258);
        Func_020087c2(3);
        Call6(Func_020086d6, 116, 36, 3, 4, 52, 36);
        Value4(Func_02002d14, 0x3480000, 0x380000, 0x2600000, 223);
        Call3(Func_020087b2, 3, 0xcccc, 0x6666);
        Call3(Func_020087f0, 3, 0x348, 0x230);
        Func_02008812(3, 0, 0);
        Func_020087a0(20);
        Call3(Func_02008894, 0, 0x4000, 0);
        Call3(Func_020088a0, 1, 0x8000, 0);
        Call3(Func_020088ac, 2, 0xb000, 0);
        Func_020087ca(10);
        Func_02008852(0, 3);
        Func_0200885a(1, 3);
        Func_0200886a(2, 3);
        Func_020087e8(20);
        Func_02008870(1, 2);
        record = Value1(Func_0200881e_a, 0);
        if (record != 0) {
            Func_02008850(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Func_02008890(2, 2);
        record = Value1(Func_0200883e, 0);
        if (record != 0) {
            Func_02008870_a(2, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Func_0200889e(1);
        Func_020088b0(1, 0, 0);
        Func_020088ae(2);
        Func_020088c0(2, 0, 0);
        Call1(Func_0200882e, 0x871);
    }
    Func_02008862();
    /* unlifted: 0x020028c0..0x020028c4 (2) */
}
