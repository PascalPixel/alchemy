#include "types.h"

#define RunEventScript02 Func_0200161c

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
void Func_020031cc();
void Func_020031e0();
s32 Func_0200320e();
void Func_0200322a();
void Func_02003232();
void Func_0200324c();
void Func_0200325e();
void Func_02003266();
void Func_0200326a();
void Func_0200326c();
void Func_02003284();
void Func_0200328a();
void Func_020032ac();
void Func_020032d6();
void Func_020032ea();
void Func_020032ee();
void Func_020032fa();
void Func_0200331e();
void Func_02003322();
s32 Func_0200332e();
void Func_02003334();
void Func_0200333c();
void Func_02003342();
void Func_02003346();
void Func_02003348();
void Func_02003350();
void Func_02003354();
void Func_02003360();
void Func_02003362();
void Func_0200336a();
void Func_0200336c();
void Func_0200336e();
void Func_02003376();
void Func_02003380();
void Func_02003382();
void Func_0200338e();
void Func_020033a0();
void Func_020033a2();
void Func_020033a6();

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

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)0x03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void RunEventScript02(void)
{
    u32 i;
    s32 record;

    Func_020031cc();
    *(s32 *)((*(u8 **)0x03001ebc + 0x1c0)) = 0x204;
    Func_020032d6();
    Func_020032ea();
    Func_020031e0(20);
    record = Value1(Func_0200320e, 0);
    if (record != 0) {
        Func_0200324c(8, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Call3(Func_02003232, 8, 0x10000, 0x8000);
    Func_0200326a(8, 2);
    Call3(Func_0200325e, 8, 24, -10);
    Func_0200326c(8);
    Func_02003284(8, 1);
    Func_0200322a(6);
    Call3(Func_020032ee, 8, 0xb000, 0);
    Call3(Func_020032fa, 0, 0xc000, 40);
    Call2(Func_02003322, 0x26666, 0x4ccc);
    Call4(Func_0200333c, 0x6880000, -1, 0x20c0000, 1);
    Func_02003348();
    Func_02003266(20);
    Call2(Func_02003346, 0x19999, 0x3333);
    Call4(Func_02003360, 0x7580000, -1, 0x20c0000, 1);
    Func_0200336c();
    Func_0200328a(20);
    Call2(Func_0200336a, 0x33333, 0x6666);
    Call4(Func_02003382, 0x6e90000, -1, 0x2240000, 1);
    Func_0200338e();
    Func_020032ac(20);
    Func_02003334(8, 2);
    Func_02003376(8, 0, 30);
    Call1(Func_02003354, 0x103a);
    Call3(Func_02003376, 0x4008, 0, 10);
    Call3(Func_020033a2, 8, 0x100, 40);
    Func_02003362(8, 1);
    Call3(Func_020033a6, 8, 0x5000, 20);
    Call3(Func_020033a0, 0x4008, 0, 10);
    Func_02003360(8, 2);
    record = Value1(Func_0200332e, 0);
    if (record != 0) {
        Func_02003350(8, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Func_0200336e(8);
    Func_02003380(8, 0, 0);
    Call1(Func_0200331e, 0x825);
    Func_02003342();
}
