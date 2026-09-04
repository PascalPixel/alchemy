#include "types.h"

#define FieldScene_RunScene3b4_02000ccc Func_02000ccc

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_020031b0();
s32 Func_020031da();
s32 Func_020031e6();
void Func_020031fc();
void Func_02003218();
s32 Func_02003236();
s32 Func_0200324c();
void Func_02003256();
void Func_0200328a();
void Func_0200328c();
s32 Func_02003290();
void Func_020032b6();
void Func_020032b8();
void Func_020032c4();
void Func_020032c8();
void Func_020032d0();
void Func_020032ee();
void Func_020032fa();
void Func_02003306();
void Func_02003308();
void Func_02003314();
void Func_0200331a();
void Func_02003322();
void Func_0200332a();
void Func_0200332e();
void Func_0200333c();
void Func_02003348();
void Func_0200334a();
void Func_02003356();
void Func_0200335e();
s32 Func_02003366();
void Func_0200337c();
void Func_020033ae();
void Func_020033c8();

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

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunScene3b4_02000ccc(void)
{
    u32 i;
    s32 rec7;
    s32 record;

    if (Value1(Func_020031da, 0x9c9) == 0) {
    } else {
        rec7 = Value1(Func_020031e6, 0x9ca);
        if (rec7 != 0) {
        } else {
            Call1(Func_020031fc, 0x9ca);
            Func_02003218();
            record = Value1(Func_02003236, 15);
            *(volatile u16 *)(*(volatile s32 *)(record + 80) + 30) = rec7;
            Func_020031b0(record, 16);
            Func_02003306(152);
            record = Func_0200324c(15);
            *(volatile s32 *)(record + 40) = 0x80000;
            Call3(Func_020032b6, 15, 0x8000, 30);
            Call2(Func_020032d0, 15, 0x102);
            Func_020032b8(15, 2);
            Func_02003256(20);
            Call3(Func_0200328c, 15, 0x10000, 0x8000);
            Func_0200334a(152);
            record = Func_02003290(15);
            *(volatile s32 *)(record + 40) = 0x40000;
            Call3(Func_020032c4, 15, 0x370, 0x2a8);
            Func_0200328a(10);
            Call2(Func_0200331a, 15, 0x101);
            Call3(Func_020032c8, 15, 0x20000, 0x10000);
            Call3(Func_020032ee, 15, 0x370, 0x2b8);
            Call3(Func_020032fa, 15, 0x372, 0x2c0);
            Call3(Func_02003308, 15, 0x370, 0x2c8);
            Call3(Func_02003314, 15, 0x36e, 0x2d0);
            Call3(Func_02003322, 15, 0x370, 0x2d8);
            Call3(Func_0200332e, 15, 0x372, 0x2e0);
            Call3(Func_0200333c, 15, 0x370, 0x2e8);
            Call3(Func_02003348, 15, 0x36e, 0x2f0);
            Call3(Func_02003356, 15, 0x370, 0x2f8);
            Call3(Func_0200337c, 15, 0x3580000, 0x3380000);
            Func_0200332a(10);
            Call3(Func_020033ae, 15, 0xc000, 20);
            Call2(Func_020033c8, 15, 0x100);
            record = Func_02003366(15);
            *(volatile s32 *)(record + 108) = 0x2008aa9;
            Func_0200335e();
        }
    }
}
