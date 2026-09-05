#include "types.h"

#define FieldScene_RunSupplementalSequenceTwo Func_02001550

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
s32 Func_02001f74();
void Func_020020de();
void Func_020045cc();
s32 Func_020045ee();
s32 Func_02004600();
void Func_0200464e();
void Func_0200465c();
s32 Func_0200466c();
void Func_0200469e();
s32 Func_020046a0();
void Func_020046aa();
u8 *Func_020046ba();
void Func_020046e0();
void Func_020046f8();
void Func_02004700();
u8 *Func_02004726();
void Func_02004730();
void Func_02004736();
void Func_02004744();
void Func_0200474c();
void Func_0200476e();
void Func_02004770();
void Func_0200477a();
void Func_0200477c();
void Func_02004788();
void Func_0200478e();
void Func_020047ac();
void Func_020047b6();
void Func_020047d0();
void Func_02004804();
void Func_02004840();

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

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

static __inline__ void Call8(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7)
{
    f(a0, a1, a2, a3, a4, a5, a6, a7);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunSupplementalSequenceTwo(void)
{
    u32 i;
    s32 rec2;
    s32 rec7;
    u8 *record;
    s32 value;
    s32 v5;
    s32 v6;
    u8 slot16[40];
    u8 *frame;

    Func_0200464e();
    record = Value1(Func_0200466c, 18);
    if ((*(volatile s32 *)((s32)record + 8) >> 20) != 46) {
    } else {
        Func_0200465c(30);
        rec2 = Func_02001f74(0x2e80000, 0, 0xb80000, 253);
        frame = slot16;
        *(s32 *)(frame + 8) = 0x9999;
        *(s32 *)(frame + 12) = 0x9999;
        *(s32 *)(frame + 4) = 7;
        *(u8 *)(Func_020046a0(18) + 85) = 0;
        Func_0200474c(185);
        for (i = 0; i < 16; i++) {
            Func_020045cc(3);
            record = Func_020046ba(18);
            *(volatile s32 *)((s32)record + 12) += -0x10000;
            rec7 = Value0(Func_020045ee);
            rec7 = ((((u32)(rec7 << 4) >> 16) << 16) + 0x2e00000);
            value = Value0(Func_02004600);
            Func_020020de(rec7, 0, ((((u32)(((value << 3) + value) << 1) >> 16) << 16) + 0x800000), 0, 0, 0, 0x90000, frame);
        }
        Call6(Func_020046aa, 51, 8, 1, 1, 49, 8);
        Func_02004700(30);
        {
            u8 *record = Func_02004726(18);
            u8 value = *(volatile u8 *)&record[35];
        
            record[35] = (u8)(value | 2);
        }
        Func_02004770(18, 3);
        Func_0200469e(rec2);
        Call6(Func_020046e0, 45, 4, 1, 1, 46, 8);
        Call3(Func_0200478e, 20, 0x2e80000, 0x880000);
        v5 = 1;
        v6 = 3;
        Func_02004804(188);
        Call6(Func_020046f8, 58, 8, 49, 8, v5, v6);
        Call3(Func_02004736, 0, 0x50000, 0x10000);
        Call3(Func_02004744, -1, -1, 0xe666);
        Func_0200477a(20);
        Func_02004840(188);
        Call6(Func_02004730, 59, 8, 49, 8, v5, v6);
        Call3(Func_0200476e, 0, 0x50000, 0x10000);
        Call3(Func_0200477c, -1, -1, 0xe666);
        Func_02004788();
        Func_020047b6(10);
        Call1(Func_020047ac, 0x971);
    }
    Func_020047d0();
}
