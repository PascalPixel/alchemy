#include "types.h"

#define FieldScene_ConfigureLeadActors Func_020029d4

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_020036be();
void Func_02006476();
s32 Func_02006480();
void Func_020064bc();
void Func_020064ca();
void Func_02006502();
void Func_02006c40();
void Func_02006cc4();
void Func_02006cda();
u8 *Func_02006d1a();
u8 *Func_02006d2c();
void Func_02006d3c();
void Func_02006d3e();
void Func_02006d50();
void Func_02006d5c();
void Func_02006d60();
void Func_02006d68();
void Func_02006d74();
void Func_02006d7e();
void Func_02006d86();
void Func_02006dca();
void Func_02006df0();
void Func_02006e02();
void Func_02006e10();
void Func_02006e18();
void Func_02006e1c();
void Func_02006e2c();
void Func_02006e3a();
void Func_02006e48();
void Func_02006e54();
void Func_02006e5c();
void Func_02006e74();
void Func_02006e82();
void Func_02006e86();
void Func_02006e8a();
void Func_02006e8e();
void Func_02006e9e();
void Func_02006ea4();

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

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_ConfigureLeadActors(void)
{
    u32 i;
    u8 *record;

    Func_02006cc4();
    Call1(Func_02006cda, 0x200d160);
    Func_02006c40(1);
    Call3(Func_02006d5c, 20, 0xb60000, 0x26a0000);
    Call3(Func_02006d68, 23, 0xee0000, 0x2720000);
    Call3(Func_02006d74, 22, 0x10c0000, 0x2a60000);
    record = Func_02006d1a(22);
    {
        s32 shown = 0;
    
        *(volatile u16 *)(record + 6) = shown;
    }
    Call2(Func_02006d3e, 22, 0x200c980);
    {
        u8 *record = Func_02006d2c(21);
        u8 value = *(volatile u8 *)&record[89];
    
        record[89] = (u8)(value | 128);
    }
    Call3(Func_02006d50, 21, 0xcccc, 0x6666);
    Call2(Func_02006d60, 21, 0x200c628);
    *(s32 *)(*(u8 *volatile *)Data_03001ebc + 0x1c0) = 0x100;
    Func_02006e8a();
    Func_02006e9e();
    Func_02006d3c(20);
    Call3(Func_02006d7e, 20, 0x19999, 0xcccc);
    Call3(Func_02006dca, 20, 182, 0x224);
    Func_02006476(20, 0);
    Value2(Func_02006480, 0, 0x8000);
    Func_02006e1c(20, 1);
    Call1(Func_02006e3a, 0x1ee1);
    ((void (*)())Func_02006480)(20);
    Func_02006e18(0, 3);
    Func_02006d86(40);
    Call3(Func_02006e82, 20, 0x5000, 20);
    Call3(Func_02006ea4, 20, 0x105, 60);
    Func_02006e86(20, 0, 40);
    Func_020064ca(20, 0);
    Func_020064bc(20);
    Func_02006e54(0, 3);
    Func_02006e5c(20, 3);
    Call3(Func_02006e48, 20, 182, 0x258);
    Call3(Func_02006e54, 20, 216, 0x258);
    Call2(Func_02006502, 20, 0xc000);
    Func_020036be();
    Func_02006df0(10);
    Call3(Func_02006e74, 20, 216, 0x244);
    Func_02006e8e(20, 0, 0);
    *(s32 *)(*(u8 *volatile *)Data_03001ebc + 0x1c0) = 0x209;
    Call1(Func_02006e02, 0x92b);
    Call1(Func_02006e10, 0x302);
    Func_02006e2c();
}
