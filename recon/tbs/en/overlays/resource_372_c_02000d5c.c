#include "TYPES.H"


void Func_02001cf4();
void Func_020028d8();
void Func_020028ec();
void Func_020053de();
s32 Func_02005440();
void Func_02005450();
s32 Func_02005452();
void Func_0200546a();
u8 *Func_020054ac();
void Func_020054d8();
void Func_020054e4();
void Func_020054fc();
void Func_02005510();
s32 Func_0200551a();
void Func_0200551c();
s32 Func_02005524();
s32 Func_02005530();
void Func_02005558();
void Func_0200556e();
void Func_02005574();
s32 Func_0200558a();
void Func_02005598();
void Func_020055fa();
void Func_0200564e();
void Func_0200565e();
void Func_020056c2();
void Func_020056d0();

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

/* NONMATCHING: 360 of 360 bytes, 4 halfword edits (2026-09-24). The -13 mask is built
 * before the flag byte load, and the Sanctum flag read follows the p6[9]
 * store; the reference orders both the other way. */
void Func_02000d5c(void)
{
    u32 i;
    u8 *rec7;
    s32 record;
    s32 v3;
    s32 base5_396;
    u8 *p6;

    if (Value1(Func_02005440, 0x310) != 0) {
    } else {
        Func_0200546a();
        if (Value1(Func_02005452, 0x830) == 0) {
            rec7 = Func_020054ac(11);
            p6 = *(s32 *)((s32)rec7 + 80);
            Call3(Func_02005450, 0x40000, 0x40000, 0x10000);
            Func_0200564e(141);
            Func_020053de(40);
            Func_0200565e(145);
            rec7[35] &= 254;
            {
                s32 mask = -13;

                p6[9] = (p6[9] & mask) | 4;
            }
            Call3(Func_02005558, 11, 0x1d90000, 0x3a40000);
            *(s32 *)((s32)rec7 + 48) = 0x18000;
            *(s32 *)((s32)rec7 + 52) = 0x18000;
            v3 = (*(s32 *)((s32)rec7 + 12) + 0xf00000);
            *(s32 *)((s32)rec7 + 12) += 0xf00000;
            *(s32 *)((s32)rec7 + 60) = v3;
            *(s32 *)((s32)rec7 + 68) = 0x6666;
            Call3(Func_0200556e, 11, 0x158, 0x3a4);
            p6[9] |= 12;
            {
                u8 value = *(volatile u8 *)&rec7[35];
            
                rec7[35] = (u8)(value | 1);
            }
            Func_020054fc(40);
            Call1(Func_020056c2, 0x121);
            Call3(Func_020054d8, -1, -1, 0xe666);
            Func_020054e4();
            Func_020056d0();
            Call1(Func_02005510, 0x830);
        }
        Func_02001cf4();
        Call1(Func_0200551c, 0x310);
        if (Value1(Func_0200551a, 0x837) != 0) {
            if (Value1(Func_02005524, 0x841) == 0) {
                if (Value1(Func_02005530, 0x30c) == 0) {
                    record = Func_0200558a(0);
                    if (*(s32 *)(record + 12) > 0x800000) {
                        base5_396 = 0x396;
                        Call2(Func_020028d8, 0x146, base5_396);
                        Call3(Func_020055fa, 0, 0x123, base5_396);
                    } else {
                        Call2(Func_020028ec, 0x14f, 0x3bd);
                    }
                    Call1(Func_02005574, 0x30c);
                }
            }
        }
        Func_02005598();
    }
}
