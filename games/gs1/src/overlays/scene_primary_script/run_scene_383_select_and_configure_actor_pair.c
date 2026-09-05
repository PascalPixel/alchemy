#include "types.h"

#define FieldScene_SelectAndConfigureActorPair Func_02001f50

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
void Func_02004318();
void Func_020065f0();
void Func_0200661c();
void Func_02006654();
void Func_0200666a();
void Func_0200668c();
void Func_0200669c();
void Func_020066ca();
void Func_020066d6();
void Func_0200673c();
void Func_02006bdc();
void Func_02006c8a();
void Func_02006c92();
void Func_02006c9e();
void Func_02006d00();
void Func_02006d4a();
void Func_02006d52();
void Func_02006d5a();
void Func_02006d62();
void Func_02006d6a();
void Func_02006d8e();
void Func_02006da4();
void Func_02006daa();
void Func_02006db6();
void Func_02006db8();
void Func_02006dc2();
void Func_02006dc4();
void Func_02006df0();
void Func_02006df6();
s32 Func_02006e00();
void Func_02006e04();
void Func_02006e0a();
void Func_02006e1a();
void Func_02006e20();
void Func_02006e22();
void Func_02006e30();
void Func_02006e3e();
void Func_02006e48();
void Func_02006e5a();
void Func_02006e64();
void Func_02006e6c();
s32 Func_02006e76();
void Func_02006e86();
void Func_02006e8c();
s32 Func_02006e8e();
void Func_02006e9c();
s32 Func_02006ea0();
void Func_02006eb4();
void Func_02006ec4();
void Func_02006eca();
void Func_02006ee2();
void Func_02006ee4();
u8 *Func_02006eec();
void Func_02006ef0();
u8 *Func_02006ef8();
void Func_02006f7e();
void Func_02006fec();
void Func_02006ff8();

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

struct Bitf {
    unsigned char skip[9];
    unsigned char pad : 2;
    unsigned char vis : 2;
};

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)0x03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_SelectAndConfigureActorPair(void)
{
    u8 *work;
    s16 *step;
    s32 n;
    u8 *record;

    work = *(u8 **)0x03001ebc;
    Func_02006c9e();
    Call1(Func_02006bdc, 0x200c8c9);
    Call1(Func_02006c8a, 0x107);
    Call1(Func_02006c92, 0x250);
    Func_02006d4a(24, 1);
    Func_02006d52(25, 1);
    Func_02006d5a(2, 1);
    Func_02006d62(0, 1);
    Func_02006d6a(1, 1);
    Func_02006da4(1, 2, 0);
    Func_02006db6(0, 2, 0);
    Func_02006db8(24, 2, 0);
    Func_02006dc2(25, 2, 0);
    Func_02006d00(10);
    n = 24;
    step = (s16 *)(work + 0x182);
    switch (*step) {
    case 202:
    case 203:
        Call1(Func_02006df6, 0x12a4);
        Call2(Func_02006e30, 25, 0x102);
        Func_02006df0(25, 2);
        Func_020065f0(25, 20);
        n = 25;
        if (*step == 202)
            break;
    case 201:
        Call1(Func_02006e20, 0x12a3);
        Call2(Func_02006e5a, 24, 0x102);
        Func_02006e1a(24, 2);
        n = 24;
        Func_0200661c(24, 20);
        break;
    }
    Func_02006e04(2, 1);
    Func_02006e3e(2, n, 0);
    Func_02006e48(1, 2, 0);
    Func_02006e48(1, 2);
    Func_02006d8e(20);
    Call1(Func_02006e6c, 0x12a5);
    Func_02006654(1, 20);
    Func_02006e64(2, 2);
    Func_02006daa(20);
    Func_0200666a(2, 20);
    Func_020066d6(2, n);
    Func_02006f7e();
    Func_02006dc4(60);
    Func_02006e8c(24, 2);
    Func_0200668c(24, 20);
    Func_02006e9c(25, 2);
    Func_0200669c(25, 20);
    Call3(Func_02006ef0, 2, 0x100, 60);
    Func_0200673c();
    Func_02006eb4(1, 2);
    Func_02006ec4(0, 2);
    Func_02006e0a(20);
    Func_020066ca(1, 20);
    Func_02006ee4(2, 1, 0);
    Func_02006e22(20);
    Func_02006eca(2, 3);
    Func_02006e30(40);
    record = Value1(Func_02006e76, 0);
    ((struct Bitf *)*(s32 *)(record + 80))->vis = 1;
    record = Value1(Func_02006e8e, 1);
    ((struct Bitf *)*(s32 *)(record + 80))->vis = 1;
    record = Value1(Func_02006ea0, 2);
    ((struct Bitf *)*(s32 *)(record + 80))->vis = 1;
    *(s32 *)((*(u8 **)0x03001ebc + 0x1c8)) = 24;
    *(s32 *)(*(u8 **)0x03001ebc + 0x1c0) = 0x201;
    Func_02006fec();
    Func_02006ff8();
    Func_02004318();
    Call6(Func_02006e48, 14, 45, 3, 1, 14, 44);
    Call1(Func_02006e86, 0x853);
    {
        u8 *record = Func_02006eec(24);
        s32 shown = 5;
    
        *(u16 *)(record + 100) = shown;
    }
    {
        u8 *record = Func_02006ef8(25);
        s32 shown = 4;
    
        *(u16 *)(record + 100) = shown;
    }
    Value2(Func_02006e00, 0x200aba1, 0xc80);
    *(s32 *)(*(u8 **)0x03001ebc + 0x1c0) = 0x209;
    Func_02006ee2();
}
