#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_0200515a();
void Func_0200517e();
void Func_0200519e();
void Func_020051a2();
void Func_020051c6();
s32 Func_020051d0();
s32 Func_020051d8();
void Func_020051ea();
s32 Func_020051f4();
s32 Func_020051fc();
void Func_0200520e();
s32 Func_02005218();
s32 Func_02005220();
s32 Func_0200523c();
s32 Func_02005244();
s32 Func_02005260();
s32 Func_02005268();
s32 Func_02005284();
s32 Func_02005294();
s32 Func_020052a4();
s32 Func_020052b2();
s32 Func_020052c2();
s32 Func_020052d0();
s32 Func_020052e0();
void Func_020052e2();
s32 Func_020052e8();
s32 Func_020052ee();
s32 Func_020052fe();
s32 Func_0200530c();
void Func_0200530e();
void Func_02005310();
void Func_02005318();
void Func_0200532a();
void Func_02005334();
void Func_02005348();
s32 Func_02005354();
void Func_02005364();
void Func_02005374();
void Func_02005376();
void Func_02005398();
void Func_0200539a();
void Func_020053a0();
void Func_020053ba();
void Func_020053c6();
void Func_020053ca();
void Func_020053de();
void Func_02005400();
void Func_02005406();
void Func_02005420();
void Func_0200542c();
void Func_02005466();

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

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunScene3c4SequenceA(s32 a0)
{
    s32 record;
    s32 v6;
    s32 p5;
    s32 q;

    v6 = 0;
    Func_0200519e(a0);
    Call6(Func_0200515a, 83, 45, 11, 8, 19, 45);
    record = Value1(Func_020051d0, 19);
    p5 = *(volatile s32 *)(record + 8);
    q = *(volatile s32 *)(Value1(Func_020051d8, 19) + 16);
    q >>= 20;
    p5 >>= 20;
    Func_0200517e(20, 56, 1, 1, p5, q);
    record = Value1(Func_020051f4, 20);
    p5 = *(volatile s32 *)(record + 8);
    q = *(volatile s32 *)(Value1(Func_020051fc, 20) + 16);
    q >>= 20;
    p5 >>= 20;
    Func_020051a2(20, 56, 1, 1, p5, q);
    record = Value1(Func_02005218, 21);
    p5 = *(volatile s32 *)(record + 8);
    q = *(volatile s32 *)(Value1(Func_02005220, 21) + 16);
    q >>= 20;
    p5 >>= 20;
    Func_020051c6(20, 56, 1, 1, p5, q);
    record = Value1(Func_0200523c, 22);
    p5 = *(volatile s32 *)(record + 8);
    q = *(volatile s32 *)(Value1(Func_02005244, 22) + 16);
    q >>= 20;
    p5 >>= 20;
    Func_020051ea(20, 56, 1, 1, p5, q);
    record = Value1(Func_02005260, 23);
    p5 = *(volatile s32 *)(record + 8);
    q = *(volatile s32 *)(Value1(Func_02005268, 23) + 16);
    q >>= 20;
    p5 >>= 20;
    Func_0200520e(20, 56, 1, 1, p5, q);
    record = Value1(Func_02005284, 19);
    if ((*(volatile s32 *)(record + 8) >> 20) == 25) {
        record = Value1(Func_02005294, 19);
        if ((*(volatile s32 *)(record + 16) >> 20) == 49) {
            v6 = 1;
        }
    }
    record = Value1(Func_020052a4, 20);
    if ((*(volatile s32 *)(record + 8) >> 20) == 23) {
        record = Value1(Func_020052b2, 20);
        if ((*(volatile s32 *)(record + 16) >> 20) == 49) {
            v6 = (v6 + 1);
        }
    }
    record = Value1(Func_020052c2, 21);
    if ((*(volatile s32 *)(record + 8) >> 20) == 25) {
        record = Value1(Func_020052d0, 21);
        if ((*(volatile s32 *)(record + 16) >> 20) == 47) {
            v6 = (v6 + 1);
        }
    }
    record = Value1(Func_020052e0, 22);
    if ((*(volatile s32 *)(record + 8) >> 20) == 23) {
        record = Value1(Func_020052ee, 22);
        if ((*(volatile s32 *)(record + 16) >> 20) == 47) {
            v6 = (v6 + 1);
        }
    }
    record = Value1(Func_020052fe, 23);
    if ((*(volatile s32 *)(record + 8) >> 20) == 24) {
        record = Value1(Func_0200530c, 23);
        if ((*(volatile s32 *)(record + 16) >> 20) == 48) {
            v6 = (v6 + 1);
        }
    }
    if (v6 == 5) {
        if (Value1(Func_020052e8, 0x984) != 0) {
            Func_02005318();
            goto L_020022e4;
        }
        Func_02005310(20);
        Call2(Func_020053a0, 0xcccc, 0x1999);
        Call4(Func_020053ba, 0x1d80000, -1, 0x30c0000, 1);
        Func_020053c6();
        Func_02005334(30);
        Call1(Func_0200532a, 0x984);
        Func_02005400(158);
        Call3(Func_020052e2, 0x200b3ec, 32, 46);
        Call6(Func_0200530e, 24, 60, 1, 1, 32, 47);
        Func_02005364(40);
    } else {
        if (Value1(Func_02005354, 0x984) != 0) {
            Func_02005376(20);
            Call2(Func_02005406, 0xcccc, 0x1999);
            Call4(Func_02005420, 0x1d80000, -1, 0x30c0000, 1);
            Func_0200542c();
            Func_0200539a(30);
            Call1(Func_02005398, 0x984);
            Func_02005466(159);
            Call3(Func_02005348, 0x200b40c, 32, 46);
            Call6(Func_02005374, 31, 47, 1, 1, 32, 47);
            Func_020053ca(40);
        }
    }
    Func_020053de();
    L_020022e4:;
}
