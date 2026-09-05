#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
s32 Func_020066f4();
void Func_02006d58();
void Func_02006d68();
void Func_02006d76();
void Func_02006da2();
void Func_02006db6();
void Func_02006dc8();
void Func_02006dcc();
void Func_02006e16();
void Func_02006e30();
void Func_02006e52();
void Func_02006e68();
void Func_02006e8c();
void Func_02006e9c();
void Func_02006eac();
s32 Func_02006eba();
void Func_02006ec2();
void Func_02006ec4();
void Func_02006ece();
void Func_02006ed8();
void Func_02006eda();
void Func_02006ee0();
void Func_02006ee2();
void Func_02006ee6();
void Func_02006ef6();
void Func_02006ef8();
void Func_02006f06();
void Func_02006f0e();
void Func_02006f10();
void Func_02006f14();
void Func_02006f20();
void Func_02006f66();
void Func_02006f70();
void Func_02006f94();
void Func_02006fea();
void Func_02006ff6();
void Func_02007018();
void Func_02007022();
void Func_02007026();
void Func_02007030();
void Func_0200703a();
void Func_0200703c();
void Func_02007044();
void Func_02007048();
void Func_0200704a();
void Func_02007054();
void Func_02007060();
void Func_02007094();
void Func_020070fe();
void Func_02007100();
s32 Func_02007126();
void Func_02007132();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ s32 Value0(s32 (*f)())
{
    return f();
}

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

static __inline__ s32 Value4(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    return f(a0, a1, a2, a3);
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

void FieldScene_RunSixPassEffectSequence(void)
{
    u32 i;
    s32 rec7;
    s32 record;
    s32 v5;

    Func_02006da2();
    Func_02006f20(141);
    v5 = 0;
    do {
        Call2(Func_02006ee2, 0x4039d2, 1);
        Func_02006ef8(8);
        Func_02006db6(8);
        Call2(Func_02006ef8, 0x10000, 1);
        Func_02006f0e(8);
        Func_02006dcc(8);
        if (v5 == 1) {
            Call3(Func_02006d58, 0x10000, 0x10000, 0x10000);
        }
        v5 = ((u32)((v5 + 1) << 24) >> 24);
    } while (v5 != 6);
    Call1(Func_02006f70, 0x121);
    Call3(Func_02006d76, -1, -1, 0xe666);
    Call6(Func_02006d68, 0, 40, 13, 46, 3, 3);
    Func_02006e16(20);
    rec7 = Value4(Func_020066f4, 222, 0xe80000, 0x100000, 0x900000);
    Func_02006e30(40);
    Func_02006dc8(rec7, 1);
    Call2(Func_02006dc8, 0x1078, 1);
    Call3(Func_02006eba, 5, 0x1330000, 0x1150000);
    Call3(Func_02006ec4, 9, 0x1330000, 0x1150000);
    Call3(Func_02006ece, 11, 0x1330000, 0x1150000);
    Call3(Func_02006ed8, 10, 0x1330000, 0x1150000);
    Call3(Func_02006ee2, 14, 0x1330000, 0x1150000);
    Call3(Func_02006eac, 0, 0x13333, 0x9999);
    Func_02006ee6(0, 232, 156);
    Func_02006e8c(10);
    record = Value1(Func_02006eba, 0);
    if (record != 0) {
        Func_02006f10(1, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
    }
    Call3(Func_02006eda, 1, 0x13333, 0x9999);
    Func_02006f14(1, 218, 172);
    Func_02006f66(1, 0, 0);
    Func_02006ec4(20);
    Func_0200704a(145);
    Call3(Func_02006e52, 0x40000, 0x40000, 0x10000);
    Func_02006ee0(20);
    Call3(Func_02006e68, 0x10000, 0x10000, 0x10000);
    Func_02006ef6(40);
    Call3(Func_02006fea, 0, 0xd000, 0);
    Call3(Func_02006ff6, 1, 0x5000, 50);
    Func_02007094(144);
    Call3(Func_02006e9c, 0x30000, 0x30000, 0x10000);
    Call3(Func_02007018, 0, 0x8000, 0);
    Func_02007022(1, 0, 50);
    Call3(Func_02006ec2, 0x10000, 0x10000, 0x10000);
    Func_0200703c(0, 0, 0);
    Call3(Func_02007048, 1, 0x8000, 50);
    Call3(Func_02007054, 0, 0xb000, 0);
    Call3(Func_02007060, 1, 0xd000, 0);
    Func_020070fe(144);
    Call3(Func_02006f06, 0x30000, 0x30000, 0x10000);
    Func_02006f94(30);
    Func_02007026(0, 2, 0);
    Func_02007030(1, 2, 20);
    Func_0200703a(0, 6, 0);
    Func_02007044(1, 6, 40);
    *(s32 *)((*(u8 *volatile *)Data_03001ebc + 0x1c0)) = 0x100;
    Value0(Func_02007126);
    Func_02007132();
    Func_02007100(2);
}
