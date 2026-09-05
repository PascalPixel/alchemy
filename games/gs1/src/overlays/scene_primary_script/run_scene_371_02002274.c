#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_0200641c();
void Func_0200642c();
void Func_02006458();
void Func_020064d2();
void Func_0200655c();
s32 Func_02006576();
void Func_02006598();
void Func_020065f0();
void Func_020065fe();
void Func_02006612();
void Func_0200662a();
void Func_0200664a();
void Func_02006650();
void Func_02006654();
void Func_0200665e();
void Func_02006672();
void Func_02006686();
void Func_02006696();
void Func_020066a4();
void Func_020066b4();
void Func_020066c6();
void Func_020066cc();
void Func_020066d2();
void Func_020066dc();
void Func_020066ea();
void Func_020066ec();
void Func_020066f2();
void Func_020066f6();
void Func_020066fc();
void Func_02006700();
void Func_0200670a();
void Func_02006712();
void Func_02006716();
void Func_0200671c();
void Func_02006722();
void Func_0200673e();
void Func_0200674e();
void Func_02006762();
void Func_02006764();
void Func_02006778();
void Func_0200677e();
void Func_02006798();
void Func_020067dc();
void Func_02006800();
void Func_02006824();
void Func_0200683a();
void Func_02006868();
void Func_02006874();
void Func_0200689c();

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
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunScene371_02002274(void)
{
    u32 i;
    s32 rec7;
    s32 record;

    rec7 = Value1(Func_02006576, 10);
    Func_0200655c();
    Call4(Func_02006686, -1, -1, -1, 0);
    Func_0200641c(1);
    Func_020065fe(0, 0, 0);
    Func_0200642c(1);
    *(volatile s32 *)(rec7 + 24) = 0x18000;
    *(volatile s32 *)(rec7 + 28) = 0x18000;
    {
        s32 shown = 0x4000;

        *(volatile u16 *)(rec7 + 6) = shown;
    }
    Func_020066f6();
    Func_0200670a();
    Func_02006598(20);
    Call3(Func_0200662a, 10, 0x15680000, 0x8380000);
    Func_02006458(1);
    Func_0200677e(141);
    Call3(Func_020065f0, 10, 0x19999, 0x6666);
    Func_02006650(10, 2);
    Call3(Func_0200662a, 10, 0x156d, 0x858);
    Call2(Func_020066ea, 0x6666, 0xccc);
    Call4(Func_02006700, 0x15b80000, -1, 0x8580000, 1);
    Call3(Func_0200664a, 10, 0x159e, 0x858);
    Call3(Func_02006654, 10, 0x15a8, 0x86e);
    Call3(Func_0200665e, 10, 0x15e8, 0x878);
    Func_02006696(10, 1);
    Call1(Func_020067dc, 0x121);
    Func_02006612(20);
    Call3(Func_020066a4, 0, 0x15d80000, 0x8780000);
    Func_020064d2(1);
    Func_020066cc(0, 6, 0);
    Call3(Func_02006672, 0, 0x20000, 0x10000);
    Call3(Func_020066b4, 0, 0x15c8, 0x878);
    Func_0200673e(0, 0, 40);
    Func_02006824(141);
    Func_020066ec(10, 2);
    Call3(Func_020066c6, 10, 0x15f8, 0x878);
    Call3(Func_02006762, 0, 0xe000, 0);
    Call3(Func_020066dc, 10, 0x15f8, 0x838);
    Call3(Func_02006778, 0, 0xc000, 0);
    Call3(Func_020066f2, 10, 0x15bd, 0x838);
    Call3(Func_020066fc, 10, 0x15b8, 0x853);
    Call3(Func_02006798, 0, 0xa000, 0);
    Call3(Func_02006712, 10, 0x1572, 0x858);
    Call3(Func_0200671c, 10, 0x1568, 0x838);
    Func_0200674e(10, 0, 0);
    Call1(Func_0200689c, 0x121);
    Func_020066d2(40);
    Call4(Func_02006800, 0x15d80000, -1, 0x8580000, 1);
    Call3(Func_02006722, 0, 0xcccc, 0x6666);
    Call3(Func_02006764, 0, 0x15d8, 0x858);
    Func_02006868();
    Func_02006874();
    Func_0200683a(20);
    Func_02006716();
}
