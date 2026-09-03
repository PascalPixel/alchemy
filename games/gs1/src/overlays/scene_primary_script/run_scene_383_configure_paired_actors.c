#include "types.h"

#define FieldScene_ConfigurePairedActors Func_02001e80

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_000012a0[];
void Func_02006498();
void Func_020064bc();
void Func_020064d2();
void Func_02006bc4();
void Func_02006bd2();
void Func_02006c12();
void Func_02006c20();
void Func_02006c70();
void Func_02006c7e();
void Func_02006c8c();
s32 Func_02006c8e();
u8 *Func_02006c92();
void Func_02006c96();
s32 Func_02006c9e();
void Func_02006ca4();
void Func_02006cac();
void Func_02006cb0();
void Func_02006cbc();
void Func_02006cda();
void Func_02006cfc();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
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

void FieldScene_ConfigurePairedActors(void)
{
    u32 i;
    u8 *record;

    Func_02006bc4(30);
    Func_02006c8c(24, 1);
    Func_02006bd2(20);
    Func_02006cb0((s32)Data_000012a0);
    Func_02006498(24, 20);
    Func_02006cda(25, 0, 20);
    Call2(Func_02006cfc, 25, 0x102);
    Func_02006cbc(25, 2);
    Func_020064bc(25, 20);
    Func_02006cac(24, 4);
    Func_02006c12(20);
    Func_020064d2(24, 20);
    Call3(Func_02006c70, 24, 0x40000, 0x20000);
    Call3(Func_02006c7e, 25, 0x38000, 0x1c000);
    Value2(Func_02006c8e, 25, 0x200d830);
    Call2(Func_02006c96, 24, 0x200d560);
    Func_02006ca4(24);
    {
        u8 *record = Func_02006c92(24);
        s32 shown = 1;
    
        *(u16 *)(record + 100) = shown;
    }
    {
        u8 *record = Value1(Func_02006c9e, 25);
        s32 shown = 3;
    
        *(u16 *)(record + 100) = shown;
    }
    Call6(Func_02006c20, 14, 48, 4, 1, 14, 44);
}
