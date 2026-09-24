#include "TYPES.H"

s32 Func_02001910();
void Func_0200221c();
void Func_02006678();
void Func_020066aa();
s32 Func_020066bc();
s32 Func_020066da();
void Func_020066e8();
void Func_020066f6();
void Func_020066fc();
void Func_020066fe();
void Func_0200670e();
void Func_0200672a_a();
void Func_02006760();
void Func_02006786();
void Func_0200678c();
void Func_02006790();
void Func_0200679c();
void Func_020067ac();
void Func_020067e8();
void Func_0200680c();
void Func_02006882();
void Func_020068ae();
void Func_020068ba();

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

void VinasuHeya_Func02001808(void)
{
    u32 i;
    s32 rec7;
    s32 record;
    s32 zero;

    zero = 0;
    rec7 = Value1(Func_020066bc, 0);
    Func_020066aa();
    Call4(Func_020067ac, -1, -1, -1, 0);
    record = Func_020066da(0);
    Func_02006678(record, 0);
    {
        s32 shown = 0x4000;
    
        *(u16 *)(rec7 + 6) = shown;
    }
    Call3(Func_020066fc, 0, 0x30000, 0x18000);
    Call3(Func_0200672a_a, 0, *(s16 *)(rec7 + 10), 0x228);
    Func_020066e8(10);
    Func_02006760(0, 22);
    Func_020066f6(30);
    Call2(Func_020067e8, 0, 0x102);
    Func_02006790(0, 2);
    Func_0200670e(20);
    {
        s32 shown = 0xc000;
    
        *(u16 *)(rec7 + 6) = shown;
    }
    Func_0200678c(0, 5);
    Func_0200679c(0, 24);
    Func_020066e8(40);
    *(s32 *)(rec7 + 72) = 0x9999;
    {
        s32 z = *(s32 *)(rec7 + 16) + 0x480000;

        *(s32 *)(rec7 + 68) = zero;
        Func_02001910(*(s32 *)(rec7 + 8), 0, z, 223);
    }
    Call6(Func_020066fe, 34, 35, 5, 1, 34, 34);
    Func_0200221c(0);
    Func_0200680c(0, 15);
    Func_02006882(20);
    Func_020068ae();
    Func_020068ba();
    Func_02006786();
}
