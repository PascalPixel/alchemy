#include "types.h"

#define FieldScene_RunActorThreeBranchSequence Func_02002684

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
void Func_0200813c();
void Func_0200816e();
void Func_02008178();
void Func_02008196();
s32 Func_020081b4();
void Func_020081ba();
void Func_020081c6();
s32 Func_020081d6();
void Func_020081e0();
s32 Func_020081f6();
void Func_02008206();
void Func_0200821a();
void Func_0200821e();
void Func_02008228();
void Func_02008232();
void Func_02008238();
void Func_02008266();
void Func_0200826a();
void Func_02008270();
void Func_0200827e();
void Func_02008280();
void Func_0200828c();
s32 Func_020082a6();
void Func_020082aa();
void Func_020082ae();
void Func_020082b2();
void Func_020082c6();
void Func_020082d2();
void Func_0200830c();
void Func_0200830e();
void Func_02008318();
void Func_02008326();
void Func_02008346();
void Func_02008360();
void Func_02008376();
void Func_0200837a();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
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

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)0x03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunActorThreeBranchSequence(void)
{
    u32 i;
    s32 record;

    Func_0200813c();
    Call3(Func_0200816e, 3, 0xcccc, 0x6666);
    Call3(Func_02008178, 0, 0xcccc, 0x6666);
    Call1(Func_0200821e, 0x1577);
    Func_02008238(3, 0, 20);
    Call3(Func_020081c6, 3, 0x348, 0x288);
    Call3(Func_0200826a, 3, 0x100, 60);
    Call3(Func_02008266, 3, 0x8000, 20);
    Func_02008206(3, 16);
    record = Func_020081b4(3);
    *(s32 *)(record + 24) = -0x10000;
    Func_02008196(20);
    Func_02008280(3, 0, 20);
    Func_02008228(3, 1);
    record = Func_020081d6(3);
    *(s32 *)(record + 24) = 0x10000;
    Func_020081ba(20);
    Call3(Func_020082ae, 3, 0x4000, 20);
    Value2(Func_020082a6, 3, 0);
    if (Value2(Func_020081f6, 0, 0) == 0) {
        Func_020081e0(20);
        Func_02008270(3, 3);
        Func_020082d2(3, 0, 20);
        *(u16 *)((*(u8 **)0x03001ebc + 0x1d8)) += 1;
    } else {
        *(u16 *)((*(u8 **)0x03001ebc + 0x1d8)) += 1;
        Func_0200821a(20);
        Func_020082aa(3, 4);
        Func_0200830c(3, 0, 20);
    }
    Func_02008232(20);
    Call3(Func_02008326, 3, 0xc000, 20);
    Call2(Func_02008346, 0xcccc, 0x1999);
    Call4(Func_02008360, 0x3480000, -1, 0x2780000, 1);
    Call3(Func_020082c6, 3, 0x348, 0x278);
    Func_0200837a();
    Func_02008270(20);
    Func_02008318(3, 2);
    Func_0200827e(10);
    Func_0200830e(3, 4);
    Func_0200828c(20);
    Func_02008376(3, 0, 20);
    Call1(Func_0200827e, 0x870);
    Func_020082b2();
}
