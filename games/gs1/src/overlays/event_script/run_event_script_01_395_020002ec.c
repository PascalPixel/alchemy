#include "types.h"

#define RunEventScript01 Func_020002ec

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_02009dd0[];
extern u8 Data_03001ebc[];
void Func_02001c1c();
void Func_02001c38();
void Func_02001c3a();
s32 Func_02001ca2();
void Func_02001cac();
void Func_02001cb8();
void Func_02001ce2();
void Func_02001cea();
s32 Func_02001d04();
void Func_02001d2c();
void Func_02001d3a();
s32 Func_02001d40();
void Func_02001d56();
void Func_02001d5a();
void Func_02001d70();
void Func_02001d80();
void Func_02001d82();
u8 *Func_02001d92();
void Func_02001d96();
void Func_02001da0();
void Func_02001da6();
void Func_02001db4();
void Func_02001dca();
void Func_02001dde();
void Func_02001df6();
void Func_02001e18();
void Func_02001e1a();
void Func_02001e2a();
void Func_02001e3a();
void Func_02001e48();
void Func_02001e50();
void Func_02001e58();
void Func_02001e6c();
void Func_02001e9e();
void Func_02001eb4();
void Func_02001ebc();

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

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
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

void RunEventScript01(void)
{
    u32 i;
    s32 rec8;
    u8 *record;
    s32 base5_2009dd0;

    rec8 = Func_02001ca2(0);
    Value3(Func_02001d40, 0, 0xc000, 0);
    Call2(Func_02001d80, 0x406218, 1);
    Func_02001d96(20);
    Func_02001c1c(40);
    Func_02001dca(17);
    base5_2009dd0 = (s32)Data_02009dd0;
    *(volatile s32 *)base5_2009dd0 = 1;
    Call2(Func_02001c3a, 0x2009219, 0xc80);
    Func_02001c38(30);
    *(volatile s32 *)base5_2009dd0 = 0;
    Call4(Func_02001da6, 0x1480000, -1, 0xeb0000, 1);
    Func_02001d96(0, 1);
    *(u8 *)(Func_02001d04(0) + 90) &= 254;
    Func_02001d56(0, 16);
    Call3(Func_02001d2c, 0, 0x20000, 0x20000);
    Func_02001e2a(133);
    *(volatile s32 *)(rec8 + 40) = 0x50000;
    *(volatile s32 *)(rec8 + 72) = 0x4000;
    *(volatile s32 *)(rec8 + 68) = 0xa000;
    Call3(Func_02001d70, 0, 0x14f, 0x102);
    if (*(volatile s32 *)(rec8 + 40) >= 0) {
        do {
            Func_02001cac(1);
        } while (*(volatile s32 *)(rec8 + 40) >= 0);
    }
    do {
        Func_02001cb8(1);
    } while (*(volatile s32 *)(rec8 + 40) <= 0);
    Func_02001e6c(161);
    Func_02001db4(0, 19);
    Func_02001d5a(120);
    Call1(Func_02001cea, 0x2009219);
    Func_02001ce2(40);
    *(volatile s32 *)(rec8 + 68) = 0x4000;
    {
        u8 *record = Func_02001d92(0);
        u8 value = *(volatile u8 *)&record[90];
    
        record[90] = (u8)(value | 1);
    }
    Func_02001d82(80);
    Call1(Func_02001e18, 0x14cc);
    Call3(Func_02001e3a, 0x200e, 0, 20);
    Func_02001e1a(0, 2);
    Func_02001da0(20);
    Call2(Func_02001e48, 0x200e, 0);
    Func_02001ebc();
    Call2(Func_02001e9e, 0x10000, 1);
    Func_02001eb4(20);
    Func_02001d3a(40);
    {
        s32 shown = 0xc000;
    
        *(volatile u16 *)(rec8 + 6) = shown;
    }
    *(volatile s32 *)(rec8 + 72) = 0x10000;
    *(volatile s32 *)(rec8 + 68) = 0x4000;
    Func_02001e58(0, 2);
    Func_02001dde(40);
    Func_02001e58(0, 4, 0);
    Func_02001e50(0, 1);
    Func_02001df6(20);
}
