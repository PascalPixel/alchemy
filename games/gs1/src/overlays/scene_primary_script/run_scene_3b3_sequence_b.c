#include "types.h"

#define FieldScene_RunFlaggedDisplayScene Func_0200175c

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
void Func_020026e0();
void Func_020026e6();
void Func_020026ec();
void Func_020026f2();
void Func_020026f8();
void Func_020026fe();
void Func_02002704();
void Func_02002822();
void Func_02002828();
void Func_0200282e();
void Func_02002834();
void Func_0200283a();
void Func_02002840();
void Func_02002846();
void Func_02002e0e();
void Func_02002ec0();
void Func_02004244();
void Func_0200426e();
s32 Func_0200427e();
void Func_0200428c();
void Func_02004296();
void Func_020042b2();
s32 Func_020042de();
u8 *Func_020042e8();
void Func_020042ea();
void Func_02004302();
void Func_02004306();
void Func_0200430e();
void Func_02004314();
void Func_0200432a();
void Func_02004330();
void Func_0200433a();
void Func_0200434c();
void Func_02004352();
void Func_0200435a();
void Func_0200435c();
void Func_02004368();
void Func_02004374();
s32 Func_02004398();
u8 *Func_020043a4();
void Func_020043c6();
void Func_020043e6();
void Func_020043f6();
void Func_02004406();
void Func_02004414();
void Func_02004422();
void Func_0200442a();

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
    u8 *work = *(u8 **)0x03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunFlaggedDisplayScene(void)
{
    u32 i;
    u8 *rec7;
    u8 *record;

    Func_0200426e();
    Call2(Func_020042ea, 0x10000, 0x2000);
    Call4(Func_02004302, 0x1190000, -1, 0x1b00000, 1);
    Func_0200430e();
    Call2(Func_0200426e, 0x1528, 1);
    rec7 = Value1(Func_0200427e, 0x200);
    if (rec7 == 0) {
        Func_02004352(232);
        Call3(Func_02004244, 0x200ada8, 84, 24);
        Func_020042b2(30);
        Func_02004368(240);
        Func_02004330(16, 1);
        *(u8 *)(Func_020042de(16) + 85) = rec7;
        record = Func_020042e8(16);
        *(s32 *)(record + 12) = -0x200000;
        Call3(Func_0200432a, 16, 0x1100000, 0x1a00000);
        Func_0200433a(16, 1);
        Call3(Func_0200428c, 0x200adfc, 80, 24);
        Call3(Func_02004296, 0x200ae50, 80, 28);
        Call6(Func_020042b2, 65, 40, 16, 27, 2, 4);
        Func_02002e0e();
        Func_020026e0(9);
        Func_020026e6(10);
        Func_020026ec(11);
        Func_020026f2(12);
        Func_020026f8(13);
        Func_020026fe(14);
        Func_02002704(15);
        Call6(Func_02004314, 24, 3, 1, 1, 24, 8);
        Call1(Func_0200434c, 0x200);
    } else {
        Func_02004414(232);
        Call3(Func_02004306, 0x200add2, 84, 24);
        Func_02004374(30);
        Func_0200442a(230);
        *(u8 *)(Func_02004398(16) + 85) = 0;
        record = Func_020043a4(16);
        *(s32 *)(record + 12) = -0x200000;
        Call3(Func_020043e6, 16, 0x1100000, 0x1b40000);
        Func_020043f6(16, 2);
        Call6(Func_0200435a, 65, 45, 16, 27, 2, 4);
        Call3(Func_0200435c, 0x200ae26, 80, 24);
        Func_02002ec0();
        Func_02002822(9);
        Func_02002828(10);
        Func_0200282e(11);
        Func_02002834(12);
        Func_0200283a(13);
        Func_02002840(14);
        Func_02002846(15);
        Call6(Func_020043c6, 24, 4, 1, 1, 24, 8);
        Call1(Func_02004406, 0x200);
    }
    Func_02004422();
}
