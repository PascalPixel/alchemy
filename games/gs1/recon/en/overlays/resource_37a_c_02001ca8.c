#include "types.h"

#define FieldScene_RunScene37aSequenceH Func_02001ca8

void Func_020044e6();
void Func_020045d6();
s32 Func_02004668();
void Func_02004692();
void Func_020046f2();
void Func_02004708();
void Func_02004712();
s32 Func_0200471a();
void Func_02004722();
void Func_0200472c();
void Func_0200472c_a();
void Func_0200472c_b();
s32 Func_02004730();
s32 Func_02004730_a();
void Func_0200473e();
void Func_02004742();
void Func_02004744();
void Func_02004756();
void Func_0200475e();
void Func_02004764();
void Func_02004766();
void Func_0200476c();
void Func_02004776();
void Func_0200477c();
void Func_0200477e();
void Func_0200478e();
void Func_0200479e();
void Func_020047a4();
void Func_020047c8();
void Func_020047ce();
void Func_020047d4();
void Func_020047dc();
void Func_020047de();
void Func_020047ee();
void Func_02004804();
void Func_02004814();
void Func_0200482c();
void Func_0200482c_a();
void Func_02004840();
void Func_02004846();
void Func_02004852();
void Func_02004864();
void Func_0200486a();
void Func_0200486c();
void Func_02004898();
void Func_020048a4();
void Func_020048dc();
void Func_020048e6();
void Func_020048f6();
void Func_020048f6_a();
void Func_020048fe();
void Func_02004900();
u8 * Func_02004902();
u8 * Func_02004902_a();
void Func_0200490e();
void Func_0200491e();
void Func_0200491e_a();
void Func_02004926();
void Func_0200492e();
void Func_02004936();
void Func_02004948();
void Func_0200494c();
void Func_02004966();
void Func_020049b4();
void Func_020049ba();
void Func_020049c0();
void Func_020049c2();
void Func_020049c8();
void Func_020049cc();
void Func_020049de();
void Func_020049e4();
void Func_020049f2();
s32 Func_020049fc();
void Func_020049fe();
void Func_02004a06();
void Func_02004a2a();
void Func_02004a52();
void Func_02004a5c();
void Func_02004a66();
void Func_02004a74();
void Func_02004a82();
void Func_02004a94();
void Func_02004a9c();
void Func_02004ad6();
void Func_02004b20();
void Func_02004b4c();
void Func_02004ba0();
void Func_02004ba2();
void Func_02004bb4();
void Func_02004bb6();
void Func_02004bc0();

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

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

void FieldScene_RunScene37aSequenceH(void)
{
    u32 i;
    u8 *rec8;
    s32 record;
    s32 base5_8010;
    s32 base5_0;
    s32 v5;
    u8 *p7;

    if (Value1(Func_02004668, 0x811) == 0) {
    } else {
        Func_02004692();
        Call2(Func_0200475e, 0x10000, 0x2000);
        Call4(Func_02004776, 0x11f0000, -1, 0x940000, 1);
        Call3(Func_020046f2, 0, 0x120, 120);
        Func_02004712(0, 0);
        Func_0200472c(0, 4, 30);
        Call3(Func_02004722, 16, 0x1200000, 0x780000);
        Call3(Func_02004708, 16, 0x10000, 0x8000);
        Call3(Func_0200472c_a, 16, 0x114, 136);
        Call3(Func_02004730, 16, 0x108, 136);
        Call3(Func_02004744, 0, 0x138, 136);
        Func_02004764(0, 1);
        Func_0200476c(16, 1);
        Call3(Func_020047c8, 0, 0xb000, 0);
        Call3(Func_020047d4, 16, 0xd000, 20);
        if (Value1(Func_0200471a, 0x819) == 0) {
            Func_02004864(220);
        }
        Func_02004742(40);
        if (Value1(Func_02004730_a, 0x819) != 0) {
        } else {
            Call6(Func_0200472c_b, 36, 62, 17, 36, 2, 3);
            Call6(Func_0200473e, 44, 59, 17, 38, 2, 1);
            Func_0200477c(10);
            Call6(Func_02004756, 38, 62, 17, 36, 2, 3);
            Call6(Func_02004766, 44, 59, 17, 39, 2, 1);
            Func_020047a4(10);
            Call6(Func_0200477e, 40, 62, 17, 36, 2, 3);
            Call6(Func_0200478e, 0, 32, 17, 39, 2, 1);
            Call6(Func_0200479e, 44, 59, 17, 40, 2, 1);
            Func_020047dc(10);
            Call3(Func_02004898, 0, 0x100, 0);
            Call3(Func_020048a4, 16, 0x100, 0);
            Call6(Func_020047ce, 42, 62, 17, 36, 2, 3);
            Call6(Func_020047de, 0, 32, 17, 40, 2, 1);
            Call6(Func_020047ee, 44, 59, 17, 41, 2, 1);
            Func_0200482c(10);
            Call6(Func_02004804, 0, 32, 17, 41, 2, 1);
            Call6(Func_02004814, 44, 59, 17, 42, 2, 1);
            Func_02004852(10);
            Call6(Func_0200482c_a, 0, 32, 17, 42, 2, 3);
            Func_0200486a(80);
            Func_02004840(9);
            Func_02004846(10);
            Call1(Func_0200486c, 0x819);
        }
        Func_020048fe(16, 0, 30);
        Func_020048e6(16, 3);
        Func_020048f6(0, 3);
        Func_020048f6_a(16, 1);
        base5_8010 = 0x8010;
        Func_02004900(0, 0);
        Call1(Func_02004936, 0x102e);
        Func_020044e6(base5_8010, 6);
        Func_0200491e(16, 3);
        Func_0200491e_a(16, 1);
        Func_02004966(base5_8010, 0);
        Func_0200492e(0, 3);
        Func_020048dc(60);
        rec8 = Func_02004902(0);
        Call2(Func_020049b4, 0x9999, 0x1333);
        Call4(Func_020049cc, 0x11f0000, -1, 0x720000, 1);
        Call3(Func_02004948, 0, 0x120, 120);
        Func_0200490e(20);
        Call3(Func_020049c2, 0, 0xc000, 20);
        Call3(Func_0200494c, 0, 0x4ccc, 0x2666);
        rec8[90] &= 254;
        rec8[85] = 0;
        Func_02004a66(201);
        Call2(Func_020049c8, 0, 0x100);
        p7 = *(s32 *)((s32)rec8 + 80);
        base5_0 = 0;
        p7[38] = 0;
        do {
            *(s32 *)((s32)rec8 + 12) += 0x3333;
            base5_0 = (base5_0 + 1);
            Call1((void (*)())Func_02004902_a, 1);
        } while (base5_0 != 120);
        v5 = 0;
        Func_02004a94(190);
        do {
            *(s32 *)((s32)rec8 + 12) += 0x1999;
            *(s32 *)((s32)p7 + 24) += -0x400;
            v5 = (v5 + 1);
            Func_02004926(1);
        } while (v5 != 60);
        Func_020049e4(0, 0, 0);
        Func_02004a06(16, 4, 20);
        Func_020045d6(16, 6);
        Call3(Func_020049f2, 16, 0x120, 120);
        Func_02004a2a(16, 2);
        Func_020049c0(20);
        Call3(Func_02004a74, 16, 0xc000, 20);
        Call3(Func_020049fe, 16, 0x4ccc, 0x2666);
        rec8 = Value1(Func_020049fc, 16);
        base5_0 = 0;
        rec8[90] &= 254;
        rec8[85] = 0;
        Func_02004b20(201);
        Call2(Func_02004a82, 16, 0x100);
        p7 = *(s32 *)((s32)rec8 + 80);
        p7[38] = 0;
        do {
            *(s32 *)((s32)rec8 + 12) += 0x3333;
            base5_0 = (base5_0 + 1);
            Func_020049ba(1);
        } while (base5_0 != 120);
        v5 = 0;
        Func_02004b4c(190);
        do {
            *(s32 *)((s32)rec8 + 12) += 0x1999;
            *(s32 *)((s32)p7 + 24) += -0x400;
            v5 = (v5 + 1);
            Func_020049de(1);
        } while (v5 != 60);
        Func_02004a9c(16, 0, 0);
        Func_02004a52(80);
        *(s32 *)((*(s32 *)0x03001ebc + 0x1c0)) = 0x203;
        *(s32 *)((*(s32 *)0x03001ebc + 0x1c8)) = 24;
        Func_02004bb4();
        Func_02004bc0();
        Func_02004ba0(0, 0);
        Func_02004bb6(1);
        Func_02004a5c(1);
        Func_02004ba2(7);
        Func_02004ad6();
    }
}
