#include "TYPES.H"

#define Function Func_020032b0

u8 *Func_020092a8();
void Func_020092c2();
void Func_020092d0();
void Func_020092f6();
void Func_02009306();
void Func_0200931e();
void Func_0200932a();
void Func_02009344();
void Func_02009352();
void Func_02009368();
void Func_0200939a();
void Func_0200939e();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}
void Func_02009306_a();
void Func_02009306_b();

/* NONMATCHING: 208 of 208 bytes, 2 halfword edits (2026-09-24), psynergy
 * decompile with disambiguated call words; not yet reviewed by hand. */
void Function(s32 a0, s32 a1, s32 a2, s32 a3)
{
    u32 i;
    s32 p10;
    s32 p10b;
    s32 p8;
    s32 p9;
    s32 p9b;
    u8 *rec8;
    s32 record;
    u8 *p5;

    p9 = a3;
    p8 = a1;
    p10 = a2;
    rec8 = Func_020092a8();
    p5 = *(s32 *)((s32)rec8 + 80);
    Call3(Func_020092c2, a0, 0x10000, 0x8000);
    Call3(Func_02009306_a, a0, 0x188, 0x376);
    Call3(Func_0200939a, 0, 0xc000, 10);
    rec8[85] = 0;
    p5[38] = 0;
    Func_02009344(a0, p8);
    Call3(Func_020092f6, a0, 0x4ccc, 0x2666);
    Call3(Func_0200932a, a0, 0x188, 0x36b);
    Func_020092d0(10);
    Func_02009368(a0, p10);
    Call3(Func_0200931e, a0, 0x10000, 0x8000);
    Call3(Func_02009352, a0, 0x188, 0x35b);
    p5[38] = 1;
    if (p9 != 0) {
        rec8[85] = 3;
    }
    Func_02009306_b(10);
    Func_0200939e(a0, 1);
    p9b = (s32)p5 + 38;
    p10b = (s32)rec8 + 85;
}
