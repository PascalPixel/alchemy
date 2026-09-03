#include "types.h"

#define FieldScene_RunThreeActorEncounter Func_02003c88

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
s32 Func_02007768();
void Func_02007770();
s32 Func_02007786();
void Func_020077b2();
void Func_020077ca();
s32 Func_020077d0();
s32 Func_020077e8();
void Func_02007822();
void Func_020078ba();
void Func_020078c8();
void Func_02007f78();
void Func_02007fb6();
s32 Func_02007fc2();
s32 Func_02007fd6();
s32 Func_02007fea();
void Func_02008000();
void Func_02008012();
void Func_02008020();
void Func_0200802a();
void Func_02008030();
void Func_02008044();
void Func_02008058();
void Func_0200806e();
void Func_02008078();
void Func_02008084();
void Func_0200808c();
void Func_020080ac();
void Func_020080b4();
s32 Func_020080b8();
s32 Func_02008104();
void Func_02008106();
void Func_02008122();
void Func_02008126();
void Func_02008132();
void Func_02008138();
void Func_0200813c();
void Func_0200814c();
void Func_02008154();
void Func_02008158();
s32 Func_02008160();
s32 Func_02008164();
void Func_0200816e();
void Func_02008184();
u8 *Func_02008186();
void Func_02008188();
s32 Func_02008196();
void Func_020081a0();
void Func_020081a6();
void Func_020081ac();
u8 *Func_020081b6();
void Func_020081be();
void Func_020081ca();
void Func_020081ce();
void Func_020081d8();
void Func_020081dc();
void Func_020081fa();
void Func_020081fc();
void Func_02008206();
void Func_0200820c();
void Func_02008212();
void Func_0200822c();
void Func_02008236();
void Func_02008248();
void Func_02008292();

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

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)0x03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunThreeActorEncounter(void)
{
    u32 i;
    u8 *record;

    Func_02007f78();
    Call3(Func_02007fb6, 0, 0x10000, 0x8000);
    Call3(Func_02008000, 0, 180, 0x28e);
    Call3(Func_02008084, 0, 0x8000, 0);
    record = Value1(Func_02007fc2, 0);
    if (record != 0) {
        Func_02008030(1, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    record = Value1(Func_02007fd6, 0);
    if (record != 0) {
        Func_02008044(2, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    record = Value1(Func_02007fea, 0);
    if (record != 0) {
        Func_02008058(3, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Call3(Func_02008012, 1, 0x13333, 0x9999);
    Call3(Func_02008020, 2, 0x10000, 0x8000);
    Call3(Func_0200802a, 3, 0x13333, 0x9999);
    Call3(Func_0200806e, 1, 194, 0x280);
    Call3(Func_02008078, 2, 198, 0x28e);
    Call3(Func_0200808c, 3, 194, 0x2a0);
    Func_020080ac(1, 1);
    Func_020080b4(2, 1);
    Func_0200802a(10);
    Call3(Func_02008126, 1, 0x8000, 0);
    Call3(Func_02008132, 2, 0x8000, 0);
    Value2(Func_02007768, 3, 0x8000);
    Func_02007770(22, 0);
    Call1(Func_02008122, 0x1f55);
    Call1((void (*)())Func_02007768, 22);
    Value2(Func_02007786, 21, 0xd000);
    Func_02008154(21, 0, 40);
    Call3(Func_02008188, 22, 0x100, 20);
    Func_02008138(22, 1);
    Value2(Func_02008160, 22, 0);
    if (Value2(Func_020080b8, 0, 0) == 1) {
        Func_0200813c(2, 4);
        Func_020077b2(2);
        Value2(Func_020077d0, 3, 0xa000);
        Func_0200814c(3, 3);
        Func_020077ca(3);
        Value2(Func_020077e8, 1, 0x6000);
        Func_02008184(1, 1);
        Func_020081ac(1, 0);
        L_02003dfa:;
        if (Value2(Func_02008104, 0, 0) == 1) {
            Func_020081a0(2, 1);
            Call1(Func_020081be, 0x1f53);
            Func_020081ce(2, 0);
            goto L_02003dfa;
        }
    }
    Func_02008106(20);
    Func_020081a6(22, 3);
    Call1(Func_020081dc, 0x1f5b);
    Func_02007822(22);
    Call3(Func_02008160, 22, 0x10000, 0x8000);
    Call3(Func_0200816e, 21, 0x10000, 0x8000);
    *(u8 *)(Func_02008164(22) + 90) &= 254;
    Call3(Func_020081ca, 22, 162, 0x27a);
    Func_02008158(1);
    {
        u8 *record = Func_02008186(22);
        u8 value = *(volatile u8 *)&record[90];
    
        record[90] = (u8)(value | 1);
    }
    *(u8 *)(Value1(Func_02008196, 21) + 90) &= 254;
    Call3(Func_020081fa, 21, 162, 0x2a4);
    Func_02008188(1);
    {
        u8 *record = Func_020081b6(21);
        u8 value = *(volatile u8 *)&record[90];
    
        record[90] = (u8)(value | 1);
    }
    Call3(Func_02008292, 22, 0x3000, 0);
    Call2(Func_020078c8, 21, 0xd000);
    Func_020078ba(22);
    Call3(Func_0200822c, 1, 180, 0x28e);
    Call3(Func_02008236, 2, 180, 0x28e);
    Call3(Func_02008248, 3, 180, 0x28e);
    Func_02008206(1);
    Func_0200820c(2);
    Func_02008212(3);
    Call1(Func_020081d8, 0x903);
    Func_020081fc();
}
