#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02004a68();
s32 Func_02004a6e();
s32 Func_02004a76();
s32 Func_02004a7e();
s32 Func_02004a86();
void Func_02004aac();
void Func_02004acc();
void Func_02004ae2();
void Func_02004b08();
void Func_02004b12();
void Func_02004b24();
void Func_02004b2e();
void Func_02004b38();
void Func_02004b48();
void Func_02004b4c();
void Func_02004b62();
void Func_02004b6a();
void Func_02004b84();
void Func_02004b90();
void Func_02004b9e();
void Func_02004ba4();
s32 Func_02004bbe();
void Func_02004bfa();
void Func_02004c10();
void Func_02004c14();
u8 *Func_02004c1c();
void Func_02004c4a();
void Func_02004c56();
void Func_02004c5e();
void Func_02004c60();
void Func_02004c66();
void Func_02004c76();
void Func_02004c7c();
void Func_02004c84();
void Func_02004c92();
u8 *Func_02004c98();
void Func_02004c9a();
void Func_02004c9e();
void Func_02004cb2();
void Func_02004cba();
void Func_02004cd2();
void Func_02004cda();
void Func_02004cec();
void Func_02004d00();
void Func_02004d0c();
void Func_02004d0e();
void Func_02004d1e();
void Func_02004d20();
void Func_02004d26();
void Func_02004d28();
void Func_02004d2c();
void Func_02004d38();
void Func_02004d42();
void Func_02004d44();
s32 Func_02004d4c();
void Func_02004d52();
void Func_02004d5e();
s32 Func_02004d86();
void Func_02004d9c();
void Func_02004da6();
void Func_02004dae();
void Func_02004dc4();
s32 Func_02004de6();
void Func_02004dea();
void Func_02004df8();
void Func_02004e08();
void Func_02004e30();
void Func_02004e32();
void Func_02004e60();
s32 Func_02004e74();
void Func_02004e9c();
void Func_02004e9e();
void Func_02004ea6();
void Func_02004eaa();
void Func_02004eae();
void Func_02004ec6();
void Func_02004ed2();
void Func_02004ed4();
void Func_02004ee6();
void Func_02004f00();
void Func_02004f02();
void Func_02004f0c();
void Func_02004f1c();
void Func_02004f32();
void Func_02004f3e();
void Func_02004faa();
void Func_0200500a();
void Func_0200abe1();

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

void FieldScene_RunSharedSetPiece(s32 a0)
{
    u32 i;
    s32 rec;
    s32 rec2;
    u8 *rec4;
    s32 rec8;
    u8 *record;
    s32 none;
    s32 base5_200abe1;
    s32 base6_ffc00000;
    s32 slot8;
    s32 zero;

    slot8 = a0;
    rec2 = Value1(Func_02004a6e, 0);
    rec = Value1(Func_02004a76, 8);
    rec4 = Value1(Func_02004a7e, 9);
    rec8 = Func_02004a86(10);
    Call2(Func_02004b62, 0, 0x102);
    Func_02004a68(40);
    Call2(Func_02004b84, 0x10000, 0x2000);
    Call4(Func_02004b9e, 0x3100000, -1, 0x740000, 1);
    Call3(Func_02004acc, 0, 0x20000, 0x10000);
    Func_02004b24(0, 6);
    Call3(Func_02004b08, 0, 0x318, 140);
    Func_02004b38(0, 1);
    Call3(Func_02004ba4, 0, 0xc000, 100);
    Call3(Func_02004bbe, 0, 0x101, 60);
    Func_02004c84(183);
    Call3(Func_02004aac, 0x30000, 0x30000, 0x10000);
    Func_02004ae2(20);
    *(s32 *)((s32)rec4 + 24) = 0x13333;
    *(s32 *)((s32)rec4 + 28) = 0x13333;
    {
        u8 *p = &rec4[35];
        u8 two = 2;
        u8 value = *(volatile u8 *)p;
    
        none = 0;
        *p = (u8)(value | two);
    }
    *(s32 *)((s32)rec4 + 108) = 0x2008099;
    Func_02004b90(8, 4);
    *(volatile s32 *)(rec + 68) = 0x8000;
    *(volatile s32 *)(rec + 8) = 0x3120000;
    *(volatile s32 *)(rec + 12) = 0x200000;
    *(volatile s32 *)(rec + 16) = 0x5a0000;
    *(volatile s32 *)(rec + 24) = 0x20000;
    *(volatile s32 *)(rec + 28) = 0x20000;
    Func_02004b2e(10);
    Func_02004cec(183);
    Call3(Func_02004b12, 0x40000, 0x20000, 0x10000);
    Func_02004b48(20);
    *(s32 *)(rec8 + 8) += 0xe0000;
    *(s32 *)(rec8 + 12) += -0x80000;
    {
        s32 target = *(s32 *)(rec8 + 80);
        s32 shown = 0xc000;
    
        *(volatile u16 *)(target + 30) = shown;
    }
    Func_02004d20(107);
    Call3(Func_02004b48, 0x10000, 0x10000, 0x10000);
    Call3(Func_02004c7c, 0, 0x102, 80);
    Func_02004d42(55);
    Call3(Func_02004b6a, 0x10000, 0x30000, 0x10000);
    Func_02004c92(8, 0);
    Func_02004c9a(0, 0);
    Call2(Func_02004cb2, 0, 0x101);
    Call3(Func_02004bfa, 0, 0x28000, 0x14000);
    *(volatile u16 *)(rec2 + 100) = none;
    Call2(Func_02004c10, 0, 0x200bdec);
    if (Value1(Func_02004bbe, 0x205) != 0) {
        Call3(Func_02004c66, 1, 0x36e0000, 0x2100000);
        record = Func_02004c1c(1);
        {
            s32 shown = 0x5000;
        
            *(u16 *)((s32)record + 6) = shown;
        }
    }
    Call2(Func_02004d0e, 0x14000, 0x2800);
    Call4(Func_02004d26, 0x3120000, -1, 0x22c0000, 1);
    Func_02004c14(slot8);
    Func_02004d0c(8, 1);
    Call3(Func_02004c60, 8, 0x195c2, 0xcae1);
    *(volatile u16 *)(rec + 100) = none;
    Func_02004c76(8, 0x200bd78);
    do {
        Func_02004b4c(1);
    } while (*(s16 *)(rec2 + 100) == 0);
    ((void (*)())Func_02004d4c)(0, 0);
    do {
        Func_02004b62(1);
    } while (*(s16 *)(rec + 100) == 0);
    Func_02004d52(0, 2);
    {
        u8 *record = Func_02004c98(0);
        u8 value = *(volatile u8 *)&record[35];
    
        record[35] = (u8)(value | 1);
    }
    Call1(Func_02004e30, 0x121);
    zero = 0;
    Call3(Func_02004c56, -1, -1, 0xe666);
    *(s32 *)((s32)rec4 + 8) = 0x3120000;
    base6_ffc00000 = -0x400000;
    *(s32 *)((s32)rec4 + 108) = zero;
    *(s32 *)((s32)rec4 + 16) = 0x26a0000;
    *(s32 *)((s32)rec4 + 12) = -0x400000;
    Call3(Func_02004cda, 8, 0x19999, 0xcccc);
    *(volatile s32 *)(rec + 68) = 0x1999;
    *(volatile s32 *)(rec + 72) = 0x3333;
    *(volatile s32 *)(rec + 40) = 0x40000;
    Call3(Func_02004d1e, 8, 0x312, 0x25c);
    Call3(Func_02004d00, 8, 0x33333, 0x19999);
    Call3(Func_02004d2c, 8, 0x312, 0x284);
    Func_02004cda(15);
    Call3(Func_02004cba, 0x50000, 0x70000, 0x10000);
    Call6(Func_02004c9e, 25, 36, 43, 36, 11, 9);
    Call6(Func_02004cd2, 25, 35, 10, 5, 43, 35);
    Func_02004d9c(8, 0, 0);
    Func_02004da6(9, 0, 0);
    base5_200abe1 = (s32)Func_0200abe1;
    Call2(Func_02004c4a, base5_200abe1, 0xc80);
    Func_02004d38(80);
    Func_02004c5e(base5_200abe1);
    Func_02004d44(60);
    Func_02004f02(17);
    Call3(Func_02004d28, -1, -1, 0xe666);
    Func_02004d5e(120);
    if (Value1(Func_02004d4c, 0x205) != 0) {
        Call3(Func_02004dae, 1, 0x10000, 0x8000);
        Call3(Func_02004dea, 1, 0x338, 0x22e);
    }
    Call3(Func_02004dc4, 0, 0x9999, 0x4ccc);
    Call3(Func_02004e08, 0, 0x356, 0x248);
    if (Value1(Func_02004d86, 0x205) != 0) {
        Func_02004e32(1, 1);
        Call3(Func_02004e9e, 1, 0x4000, 0);
    }
    Call3(Func_02004eaa, 0, 0x6000, 40);
    Call3(Func_02004ec6, 1, 0x102, 0);
    Call3(Func_02004ed2, 0, 0x102, 60);
    Call4(Func_02004f00, 0x3140000, base6_ffc00000, 0x2620000, 1);
    Func_02004f0c();
    Func_02004faa(148);
    Func_02004df8(240);
    if (Value1(Func_02004de6, 0x205) != 0) {
        Call2(Func_02004f1c, 0x40000, 0x8000);
        Call4(Func_02004f32, 0x3560000, 0, 0x2480000, 1);
        Func_02004f3e();
        Call3(Func_02004e9c, 1, 0x348, 0x228);
        Call3(Func_02004ea6, 1, 0x356, 0x232);
        Func_02004ec6(1, 2);
        record = Value1(Func_02004e74, 0);
        if ((s32)record != 0) {
            Func_02004eae(1, *(s16 *)((s32)record + 10), *(s16 *)((s32)record + 18));
        }
        Func_02004ed4(1);
        Func_02004ee6(1, 0, 0);
    }
    Func_0200500a();
    Call1(Func_02004e60, 0x908);
}
