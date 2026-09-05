#include "types.h"

#define FieldScene_RunScene3bcSequenceB Func_02000c5c

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
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

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)0x03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunScene3bcSequenceB(void)
{
    s32 i;
    s32 rec8;
    s32 record;
    s32 base3_2000240;
    s32 four;

    four = 4;
    Func_02004920();
    Func_02005664();
    rec8 = Value2(Func_02004a48, 3, 17);
    Func_02004942();
    for (i = 9; i >= 0; i--) {
        Call1(Func_020056b4, 8);
    }
    Call3(Func_020056b6, 8, 0x10000, 0x8000);
    Call3(Func_020056e2, 8, 0x5f8, 192);
    Call3(Func_020056d0, 0, 0x10000, 0x8000);
    Call3(Func_02005704, 0, 0x5d8, 192);
    Func_0200572c(8, 1);
    Func_02005766(0, 8, 0);
    Func_020056c4(10);
    Func_02005744(8, 3);
    Func_02005754(0, 3);
    Func_020056da(20);
    Call3(Func_02005718, 0, 0x20000, 0x10000);
    Call3(Func_02005726, 8, 0x20000, 0x10000);
    Call3(Func_02005752, 0, 0x5e0, 192);
    Call3(Func_02005766, 8, 0x5f0, 192);
    Func_0200578e(0, 16);
    Func_02005796(8, 9);
    Func_02005724(10);
    record = four - rec8;
    Value2(Func_02005850, 72, record + 1);
    do {
        base3_2000240 = (s32)Data_02000240;
        *(u8 *)((base3_2000240 + 0x22b)) = 3;
    } while (0);
    {
        s32 p91 = (s32)Data_00000091;
        Value2(Func_0200586c, p91, four);
        Func_0200587c(p91, 5);
    }
    Call1(Func_02005714, 0x11a);
}
