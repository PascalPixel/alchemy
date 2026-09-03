#include "types.h"

#define FieldScene_ConfigureThreeActors Func_02002b7c

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_0200661a();
void Func_02006652();
void Func_02006dfc();
void Func_02006e38();
void Func_02006e6c();
void Func_02006e96();
s32 Func_02006e9a();
s32 Func_02006ec0();
s32 Func_02006eda();
void Func_02006ef2();
s32 Func_02006efc();
void Func_02006f1a();
void Func_02006f2c();
void Func_02006f34();
void Func_02006f56();
void Func_02006f60();
void Func_02006f9c();
void Func_02006fa6();
void Func_02006faa();
void Func_02006fb2();
s32 Func_02006fc0();
void Func_02006fd4();
void Func_02006ff4();
s32 Func_0200702c();
void Func_02007040();
void Func_02007054();
void Func_02007090();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_ConfigureThreeActors(void)
{
    u32 i;
    s32 record;

    Func_02006e6c();
    Func_02006f2c(0, 15);
    record = Func_02006e9a(0);
    Func_02006e38(record, 0);
    Call1(Func_02006e96, 0x200d160);
    Func_02006dfc(1);
    Call3(Func_02006f1a, 20, 0xc40000, 0x1f60000);
    record = Func_02006ec0(20);
    {
        s32 shown = 0xa000;
    
        *(volatile u16 *)(record + 6) = shown;
    }
    Call3(Func_02006f34, 22, 0xb80000, 0x20c0000);
    record = Func_02006eda(22);
    {
        s32 shown = 0xb000;
    
        *(volatile u16 *)(record + 6) = shown;
    }
    Value2(Func_02006fc0, 21, 1);
    Call3(Func_02006f56, 21, 0xb80000, 0x2780000);
    record = Func_02006efc(21);
    {
        s32 shown = 0xb000;
    
        *(volatile u16 *)(record + 6) = shown;
    }
    *(s32 *)((*(u8 *volatile *)Data_03001ebc + 0x1c0)) = 0x202;
    Func_02007040();
    Func_02007054();
    Func_02006ef2(20);
    Func_02006f9c(22, 4, 10);
    Func_02006fa6(22, 6, 20);
    Call1(Func_02006fd4, 0x1ee5);
    Func_0200661a(22);
    Func_02006fb2(20, 3);
    Call3(Func_02006f60, 21, 0x30000, 0x18000);
    Call3(Func_02006faa, 21, 180, 0x222);
    Value3(Func_0200702c, 21, 0xb000, 40);
    Func_02006ff4(21, 1);
    Func_02006652(21);
    Func_02007090(15);
}
