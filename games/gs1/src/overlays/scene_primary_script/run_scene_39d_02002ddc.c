#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_0200619e();
void Func_0200642c();
void Func_0200646e();
void Func_020064c4();
void Func_020064e6();
s32 Func_020064f2();
s32 Func_0200650c();
s32 Func_02006516();
void Func_02006524();
void Func_0200653e();
void Func_0200654e();
void Func_02006560();
void Func_0200657a();
void Func_020065e0();
s32 Func_020065ec();
void Func_020065fa();
void Func_020065fe();
void Func_02006606();
void Func_02006612();
void Func_0200661a();

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

void FieldScene_RunScene39d_02002ddc(void)
{
    u32 i;
    s32 record;
    u8 *p5;
    u8 *work;

    p5 = *(volatile s32 *)0x03001e70;
    work = p5 + 0x164;
    Func_020064c4();
    *(volatile s32 *)(work + 12) = 0x3800000;
    Func_0200646e();
    Func_0200642c(1);
    *(u8 *)(Func_020064f2(9) + 85) = 0;
    Call3(Func_0200653e, 9, 0x680000, 0x1080000);
    record = Func_0200650c(9);
    *(volatile s32 *)(record + 12) = -0x200000;
    record = Value1(Func_02006516, 9);
    *(volatile s32 *)(record + 60) = -0x200000;
    *(u8 *)(Func_020065ec() + 85) = 0;
    Call2(Func_020065e0, 0xcccc, 0x1999);
    Call4(Func_020065fa, 0x800000, -1, 0xb80000, 1);
    Func_02006606();
    Func_02006524(30);
    Call6(Func_020064e6, 29, 74, 4, 74, 5, 4);
    Func_020065fe(17, 0);
    Func_02006606(18, 0);
    Func_0200619e();
    Func_02006612(17, 1);
    Func_0200661a(18, 1);
    Func_02006560(20);
    Call1(Func_0200654e, 0x251);
    Func_0200657a();
}
