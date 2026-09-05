#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
s32 Func_0200853e();
void Func_02008558();
void Func_02008566();
void Func_02008574();
s32 Func_0200857a();
s32 Func_02008582();
void Func_0200859c();
void Func_020085c0();
void Func_020085f6();
s32 Func_02008602();
void Func_0200861e();
void Func_02008622();
void Func_02008624();
void Func_0200862c();
void Func_0200862e();
void Func_0200864c();
void Func_02008656();
void Func_0200865a();
void Func_0200865c();
void Func_0200866a();
void Func_02008678();
void Func_02008682();
void Func_02008686();
void Func_02008690();
void Func_0200869a();
void Func_0200869c();
void Func_020086ae();
void Func_020086c8();
s32 Func_020086ca();
void Func_020086dc();
void Func_020086de();
void Func_020086e6();
void Func_020086fe();
void Func_02008706();
void Func_02008722();
void Func_02008724();
void Func_02008762();

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

void FieldScene_RunScene372_02003e48(void)
{
    u32 i;
    s32 rec7;
    s32 rec8;
    s32 record;

    rec8 = Value1(Func_0200857a, 0);
    rec7 = Value1(Func_02008582, 8);
    Func_02008558();
    if (Value1(Func_0200853e, 0x305) != 0) {
        Func_020085c0(8);
        Func_02008566(10);
        Func_0200862e(8, 2);
        Func_02008574(40);
        if (*(s16 *)(rec8 + 6) >= 0) {
            Func_02008624(8, 7);
        } else {
            Func_0200862e(8, 8);
        }
        Func_02008656(8, 2);
        Func_0200859c(20);
        Call1(Func_02008682, 0xed2);
        Func_0200869a(8, 0);
        Value2(Func_02008602, 8, 0x200cec8);
        Func_0200865a(8, 6);
    } else {
        Func_02008622(8);
        *(s32 *)(rec7 + 24) = 0x10000;
        *(s32 *)(rec7 + 28) = 0x10000;
        Call3(Func_020086de, 8, 0x1000, 0);
        if (*(s16 *)(rec8 + 6) >= 0) {
            Func_02008686(8, 7);
        } else {
            Func_02008690(8, 8);
        }
        Func_020085f6(20);
        Call1(Func_020086dc, 0xed1);
        Func_020086fe(8, 0, 20);
        Func_020086ae(8, 1);
        Func_020086c8(8, 4, 0);
        Func_0200861e(80);
        Func_020086e6(8, 2);
        Func_0200862c(40);
        if (*(s16 *)(rec8 + 6) >= 0) {
            Func_020086dc(8, 7);
        } else {
            Func_020086e6(8, 8);
        }
        Func_0200864c(2);
        Func_02008706(8, 2, 0);
        Func_0200865c(60);
        Func_02008724(8, 2);
        Func_0200866a(20);
        Func_02008762(8, 0);
        Value2(Func_020086ca, 8, 0x200cec8);
        Func_02008722(8, 6);
        Call1(Func_02008678, 0x305);
    }
    Func_0200869c();
}
