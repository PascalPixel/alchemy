#include "types.h"

#define FieldScene_RunTwoActorCutsceneSequence Func_02002008

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
void Func_02005b3e();
void Func_02005b52();
void Func_02005bd0();
void Func_02005c06();
void Func_02005ca2();
void Func_02005cac();
void Func_02005cbc();
u8 *Func_02005cc0();
void Func_02005cca();
u8 *Func_02005ccc();
s32 Func_02005cde();
void Func_02005cea();
s32 Func_02005cf0();
void Func_02005d0e();
u8 *Func_02005d20();
u8 *Func_02005d30();
void Func_02005d38();
void Func_02005d3a();
void Func_02005d4c();
void Func_02005d52();
void Func_02005d6c();
void Func_02005d74();
s32 Func_02005d88();
void Func_02005d98();
void Func_02005dae();
void Func_02005db0();
void Func_02005dba();
void Func_02005dc8();
void Func_02005dd0();
void Func_02005dec();
void Func_02005dee();
void Func_02005e0e();
void Func_02005e24();
void Func_02005e28();
void Func_02005e2a();
void Func_02005e5a();
void Func_02005e64();
void Func_02005e7c();
void Func_02005e80();
void Func_02005e86();
void Func_02005e8c();
void Func_02005e96();
void Func_02005ea2();
void Func_02005eba();
void Func_02005ed4();
void Func_02005ee6();
void Func_02005ee8();
void Func_02005eec();
void Func_02005ef4();
void Func_02005ef6();
void Func_02005efa();
void Func_02005efe();
void Func_02005f08();
void Func_02005f14();
void Func_02005f16();
void Func_02005f26();
void Func_02005f28();
void Func_02005f2c();
void Func_02005f3e();
void Func_02005f44();
void Func_02005f4e();
void Func_02005f56();
void Func_02005f5a();
void Func_02005f6e();
void Func_02005f7e();
void Func_02005f80();
s32 Func_02005f84();
void Func_02005f98();
void Func_02005fbe();
void Func_02005fca();
void Func_02005fd4();
void Func_02005fda();
void Func_02005ff0();
void Func_02005ff6();
void Func_02005ffc();
void Func_0200600c();
void Func_02006010();
void Func_02006018();
void Func_0200601c();
s32 Func_02006020();
void Func_02006026();
void Func_02006030();
void Func_0200604c();
void Func_0200604e();
void Func_0200605a();
void Func_02006084();
void Func_0200608a();
void Func_020060b4();
void Func_020060c2();
void Func_020060c6();
void Func_020060d2();
void Func_020060e2();
void Func_020060e4();
void Func_020060ee();
void Func_020060fe();
void Func_02006104();
void Func_02006106();
void Func_02006118();
void Func_0200611a();
void Func_0200611c();
void Func_0200611e();
void Func_02006128();
void Func_02006148();
void Func_02006150();
u8 *Func_02006152();
void Func_02006156();
u8 *Func_0200615c();
void Func_02006160();
void Func_02006166();
s32 Func_02006168();
void Func_0200616c();
void Func_02006170();
void Func_02006174();
s32 Func_0200617a();
void Func_02006180();
void Func_02006184();
void Func_02006186();
void Func_0200618a();
void Func_0200618c();
void Func_02006194();
void Func_020061a0();
void Func_020061a8();
u8 *Func_020061aa();
u8 *Func_020061ba();
void Func_020061bc();
void Func_020061c4();
void Func_020061cc();
void Func_020061d6();
void Func_020061f8();
void Func_020061fe();
void Func_0200620c();
void Func_0200623a();
void Func_02006256();
void Func_02006274();
void Func_02006278();
void Func_02006290();
void Func_02006294();
void Func_02006296();
void Func_020062a0();
void Func_020062a2();
void Func_020062a4();
void Func_020062a6();
void Func_020062b6();
void Func_020062d8();
void Func_020062e2();
void Func_0200633e();
void Func_020063b8();
void Func_020063be();
void Func_020063ca();

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
    u8 *work = *(u8 **)0x03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunTwoActorCutsceneSequence(void)
{
    u32 i;
    u8 *record;

    Func_02005c06();
    Call4(Func_02005d38, -1, -1, -1, 0);
    Func_02005b3e(1);
    Call3(Func_02005cac, 0, 0xc00000, 0x1560000);
    Func_02005b52(1);
    Call2(Func_02005d52, 0x3333, 0x666);
    Call4(Func_02005d6c, 0xc00000, -1, 0xfc0000, 1);
    *(s32 *)((*(u8 **)0x03001ebc + 0x1c8)) = 40;
    Func_02005d98();
    Call3(Func_02005ca2, 0, 0xcccc, 0x6666);
    Call3(Func_02005cde, 0, 192, 0x116);
    Call3(Func_02005cbc, 8, 0x10000, 0x8000);
    Call3(Func_02005cca, 9, 0x10000, 0x8000);
    record = Func_02005cc0(8);
    {
        s32 shown = 0x3000;
    
        *(u16 *)(record + 6) = shown;
    }
    record = Func_02005ccc(9);
    {
        s32 shown = 0x5000;
    
        *(u16 *)(record + 6) = shown;
    }
    Func_02005bd0(1);
    *(u8 *)(Func_02005cde(8) + 90) &= 254;
    *(u8 *)(Func_02005cf0(9) + 90) &= 254;
    Func_02005d3a(8, 184, 232);
    Func_02005d4c(9, 198, 232);
    Func_02005d74(8, 1);
    Func_02005cea(20);
    {
        u8 *record = Func_02005d20(8);
        u8 value = *(volatile u8 *)&record[90];
    
        record[90] = (u8)(value | 1);
    }
    {
        u8 *record = Func_02005d30(9);
        u8 value = *(volatile u8 *)&record[90];
    
        record[90] = (u8)(value | 1);
    }
    Func_02005d0e(20);
    Func_02005dae(8, 4);
    Call1(Func_02005dec, 0x1b05);
    Func_02005e0e(8, 0, 10);
    Func_02005e28(9, 0x5000, 10);
    Func_02005dd0(9, 3);
    Func_02005e2a(9, 0, 10);
    Call2(Func_02005e64, 0, 0x102);
    Func_02005d52(60);
    record = Value1(Func_02005d88, 0);
    if (record != 0) {
        Func_02005dee(2, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Call3(Func_02005db0, 2, 0xcccc, 0x6666);
    Call3(Func_02005dec, 2, 212, 0x10c);
    Call3(Func_02005e80, 0, 0xe000, 0);
    Call3(Func_02005e8c, 2, 0x6000, 20);
    Call3(Func_02005e86, 0x4002, 0, 20);
    Call3(Func_02005ea2, 0, 0xc000, 0);
    Func_02005e24(2, 202, 254);
    Func_02005dba(20);
    Func_02005e5a(2, 3);
    Func_02005dc8(10);
    Func_02005eba(2, 0, 10);
    Call3(Func_02005eec, 8, 0x101, 0);
    Call3(Func_02005ef6, 9, 0x101, 40);
    Func_02005ee8(8, 0, 0);
    Call3(Func_02005ef4, 9, 0x8000, 40);
    Func_02005efe(8, 0x3000, 0);
    Func_02005f08(9, 0x3000, 20);
    Call3(Func_02005f2c, 2, 0x102, 60);
    Func_02005ed4(2, 1);
    Func_02005f16(2, 0, 10);
    Func_02005ee6(8, 2);
    Func_02005f28(8, 0, 10);
    Call3(Func_02005f5a, 8, 0x105, 60);
    Func_02005efa(8, 2);
    Func_02005f44(8, 0, 10);
    Func_02005f14(9, 2);
    Func_02005f56(9, 0, 20);
    Func_02005f26(2, 1);
    Func_02005e7c(10);
    Call3(Func_02005f80, 2, 0x6000, 10);
    Call2(Func_02005f80, 0x4002, 0);
    Func_02005e96(10);
    Func_02005f4e(8, 1);
    Func_02005f3e(8, 4);
    Func_02005f98(8, 0, 10);
    Call3(Func_02005fca, 2, 0x101, 40);
    Call3(Func_02005fbe, 2, 0xa000, 20);
    Func_02005f7e(9, 1);
    Func_02005f6e(9, 3);
    Call3(Func_02005fda, 2, 0xc000, 0);
    Func_02005fd4(9, 0, 10);
    Call3(Func_02005ff0, 2, 0x6000, 60);
    Call3(Func_02005ffc, 2, 0xc000, 10);
    Func_02005ff6(2, 0, 10);
    Func_02006010(8, 0, 0);
    Call3(Func_0200601c, 9, 0x8000, 40);
    Func_02006026(8, 0x3000, 0);
    Func_02006030(9, 0x3000, 10);
    Func_02005ff0(8, 1);
    Value2(Func_02006020, 8, 0);
    Call3(Func_0200604c, 2, 0x6000, 0);
    if (Value2(Func_02005f84, 0, 0) == 0) {
        Func_02006018(2, 2);
        Call3(Func_0200605a, 0x4002, 0, 10);
        bump_step(1);
    } else {
        bump_step(1);
        Func_0200605a(2, 4);
        Call3(Func_020060b4, 0x4002, 0, 10);
    }
    Func_02006084(9, 2);
    Func_020060c6(9, 0, 10);
    Call3(Func_020060e2, 2, 0xc000, 10);
    Func_0200608a(2, 4);
    Func_020060e4(2, 0, 10);
    Call2(Func_0200611e, 8, 0x102);
    Func_0200600c(60);
    Func_020060fe(8, 0, 10);
    Call3(Func_0200611a, 2, 0xa000, 10);
    Func_020060c2(2, 3);
    Func_0200611c(2, 0, 10);
    Call2(Func_02006156, 8, 0x102);
    Call2(Func_02006160, 9, 0x102);
    Func_0200604e(60);
    Func_02006106(8, 2);
    Func_02006148(8, 0, 10);
    Func_02006118(9, 2);
    Call3(Func_0200616c, 9, 0x8000, 10);
    Func_02006166(9, 0, 10);
    Func_02006180(8, 0, 10);
    Func_02006128(8, 3);
    Call3(Func_02006194, 8, 0x3000, 10);
    Func_02006184(8, 0);
    Call3(Func_020061a8, 9, 0x3000, 10);
    Func_02006150(2, 3);
    Call3(Func_020061bc, 2, 0x6000, 10);
    Call2(Func_020061bc, 0x4002, 0);
    Func_020060d2(10);
    Func_0200618a(2, 1);
    Call3(Func_020061cc, 0x4002, 0, 10);
    Func_02006184(0, 3);
    Func_0200618c(2, 3);
    Call3(Func_020061f8, 2, 0xc000, 10);
    Func_020061a0(2, 3);
    Call3((void (*)())Func_02006152, 8, 0xcccc, 0x6666);
    Call3((void (*)())Func_0200615c, 9, 0xcccc, 0x6666);
    record = Func_02006152(8);
    {
        s32 shown = 0;
    
        *(u16 *)(record + 6) = shown;
    }
    record = Func_0200615c(9);
    {
        s32 shown = 0x8000;
    
        *(u16 *)(record + 6) = shown;
    }
    *(u8 *)(Func_02006168(8) + 90) &= 254;
    *(u8 *)(Func_0200617a(9) + 90) &= 254;
    Func_020061c4(8, 168, 232);
    Func_020061d6(9, 212, 232);
    Func_020061fe(8, 1);
    Func_02006174(20);
    {
        u8 *record = Func_020061aa(8);
        u8 value = *(volatile u8 *)&record[90];
    
        record[90] = (u8)(value | 1);
    }
    {
        u8 *record = Func_020061ba(9);
        u8 value = *(volatile u8 *)&record[90];
    
        record[90] = (u8)(value | 1);
    }
    Func_0200620c(2, 192, 232);
    Call3(Func_020062a0, 2, 0xc000, 0);
    Func_0200633e(188);
    Call6(Func_02006170, 36, 23, 43, 12, 2, 2);
    Func_020060ee(5);
    Call6(Func_02006186, 39, 23, 43, 12, 2, 2);
    Func_02006104(5);
    Func_02006256(2, 192, 222);
    Func_02006278(2, 0, 0);
    Call3(Func_0200623a, 0, 0x13333, 0x9999);
    Func_02006274(0, 192, 222);
    Func_02006296(0, 0, 0);
    Func_020062a6(8, 3);
    Func_020062b6(9, 3);
    Func_02006290(8, 184, 232);
    Func_020062a2(9, 198, 232);
    Func_020062a4(8, 188, 212);
    Func_020062b6(9, 194, 212);
    Func_020062d8(8, 0, 0);
    Func_020062e2(9, 0, 0);
    *(s32 *)((*(u8 **)0x03001ebc + 0x1c8)) = 24;
    *(s32 *)((*(u8 **)0x03001ebc + 0x1c0)) = 0x201;
    Func_020063be();
    Func_020063ca();
    Func_020063b8(5);
    Func_02006294();
}
