#include "TYPES.H"

#define Function Func_02000c5c

extern u8 Data_00000091[];
extern u8 Data_02000240[];
void Func_02004920();
void Func_02004942();
s32 Func_02004a48();
void Func_02005664();
void Func_020056b4();
void Func_020056b6();
void Func_020056c4();
void Func_020056d0();
void Func_020056da();
void Func_020056e2();
void Func_02005704();
void Func_02005714();
void Func_02005718();
void Func_02005724();
void Func_02005726();
void Func_0200572c();
void Func_02005744();
void Func_02005752();
void Func_02005754();
void Func_02005766();
void Func_0200578e();
void Func_02005796();
s32 Func_02005850();
s32 Func_0200586c();
void Func_0200587c();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}
void Func_02005766_a();
void Func_02005766_b();

/* NONMATCHING: 276 of 276 bytes, 16 halfword edits (2026-09-24), psynergy
 * decompile with disambiguated call words; not yet reviewed by hand. */
void Function(void)
{
    u32 i;
    s32 rec8;
    s32 record;
    s32 v5;
    s32 base3_2000240;

    Func_02004920();
    Func_02005664();
    rec8 = Value2(Func_02004a48, 3, 17);
    v5 = 9;
    Func_02004942();
    do {
        Func_020056b4(8);
        v5 = (v5 - 1);
    } while (v5 >= 0);
    Call3(Func_020056b6, 8, 0x10000, 0x8000);
    Call3(Func_020056e2, 8, 0x5f8, 192);
    Call3(Func_020056d0, 0, 0x10000, 0x8000);
    Call3(Func_02005704, 0, 0x5d8, 192);
    Func_0200572c(8, 1);
    Func_02005766_a(0, 8, 0);
    Func_020056c4(10);
    Func_02005744(8, 3);
    Func_02005754(0, 3);
    Func_020056da(20);
    Call3(Func_02005718, 0, 0x20000, 0x10000);
    Call3(Func_02005726, 8, 0x20000, 0x10000);
    Call3(Func_02005752, 0, 0x5e0, 192);
    Call3(Func_02005766_b, 8, 0x5f0, 192);
    Func_0200578e(0, 16);
    Func_02005796(8, 9);
    Func_02005724(10);
    Value2(Func_02005850, 72, ((4 - rec8) + 1));
    base3_2000240 = (s32)Data_02000240;
    *(u8 *)((base3_2000240 + 0x22b)) = 3;
    Value2(Func_0200586c, (s32)Data_00000091, 4);
    Func_0200587c((s32)Data_00000091, 5);
    Call1(Func_02005714, 0x11a);
}
