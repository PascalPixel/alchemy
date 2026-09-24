#include "TYPES.H"

#define FuneHeya_TurnActorToOpenSide Func_02001190

s32 Func_02002454();
s32 Func_02002474();
void Func_02005ae0();
s32 Func_02007686();
u8 *Func_0200769a();
s32 Func_020076fe();
void Func_02007726();
void Func_0200776c();
void Func_02007780();
void Func_02007786();
void Func_0200778c();
void Func_0200778e();
void Func_020077a6();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

extern s32 Data_0200e668[];

void FuneHeya_TurnActorToOpenSide(s32 a0)
{
    s32 p10;
    s32 rec4;
    u8 *record;
    s32 none;
    s32 v8;
    s32 v6;

    rec4 = Func_02007686(0);
    v8 = 1;
    Func_0200778c(a0, 2);
    {
        u8 *record = Func_0200769a(a0);
        /* FAKEMATCH: the flag byte is read through a volatile access. */
        u8 value = *(volatile u8 *)&record[35];
    
        record[35] = (u8)(value | 1);
    }
    v6 = (((*(u16 *)(rec4 + 6) + 0x4000) & 0xf000) >> 12);
    if (Value1(Func_02002454, (((*(u16 *)(rec4 + 6) + 0x4000) & 0xf000) >> 12)) != 0) {
        none = 0;
        v8 = none;
    }
    if (v8 != 0) {
        v6 = (((*(u16 *)(rec4 + 6) + -0x4000) & 0xf000) >> 12);
        if (Value1(Func_02002474, (((*(u16 *)(rec4 + 6) + -0x4000) & 0xf000) >> 12)) != 0) {
            none = 0;
            v8 = none;
        }
        if (v8 != 0) {
            v6 = (((*(u16 *)(rec4 + 6) + 0x8000) & 0xf000) >> 12);
        }
    }
    record = Value1(Func_020076fe, 0);
    if ((s32)record != 0) {
        Func_0200776c(a0, *(s32 *)((s32)record + 8), *(s32 *)((s32)record + 16));
    }
    Call3(Func_02007726, a0, 0x19999, 0xcccc);
    Func_02007786(a0, 2);
    {
        s32 w = Data_0200e668[v6];

        Func_02007780(a0, w >> 16, (w << 16) >> 16);
    }
    Func_0200778e(a0);
    Func_020077a6(a0, 1);
    Func_02005ae0(a0, *(u16 *)(rec4 + 6));
    p10 = a0;
}
