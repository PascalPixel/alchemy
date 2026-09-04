#include "types.h"

#define FieldScene_RunScene3c9_02003924 Func_02003924

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02008784();
void Func_020090a2();
void Func_020090d4();
void Func_02009106();
void Func_02009134();
void Func_0200917e();
void Func_0200918e();
void Func_020091a2();
void Func_020091b2();
void Func_020095b6();
void Func_020095cc();
void Func_020095d6();
void Func_020095ea();
void Func_020095fe();
void Func_0200961c();
void Func_0200962e();
void Func_02009640();
void Func_02009650();
void Func_0200966c();
s32 Func_0200967e();
void Func_020096a6();
void Func_020096da();
void Func_0200970c();
void Func_02009734();
void Func_0200973a();
void Func_0200973e();
s32 Func_02009740();
s32 Func_02009774();
s32 Func_02009780();
s32 Func_02009788();
void Func_02009790();
void Func_020097a2();
s32 Func_020097a6();
s32 Func_020097b2();
void Func_020097d6();
s32 Func_020097d8();
s32 Func_020097e4();
void Func_020097f2();
void Func_020097f6();
void Func_02009806();
void Func_02009808();
s32 Func_0200982a();
s32 Func_02009836();
void Func_0200983a();
s32 Func_02009848();
s32 Func_02009858();
void Func_02009866();
s32 Func_0200986c();
void Func_02009876();
void Func_0200987c();
s32 Func_0200988c();
s32 Func_02009898();
s32 Func_020098ac();
void Func_020098b2();
void Func_020098c0();
void Func_0200993e();
void Func_02009a00();
void Func_02009a1e();
void Func_02009a32();

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
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunScene3c9_02003924(void)
{
    u32 i;
    s32 rec4;
    s32 record;
    u8 *work;

    rec4 = Value1(Func_0200967e, 0);
    Func_0200966c();
    *(u8 *)(Func_02009788() + 85) = 0;
    Call6(Func_020095ea, 102, 4, 74, 4, 18, 23);
    Call6(Func_020095fe, 39, 72, 11, 72, 16, 20);
    Call6(Func_0200961c, 19, 6, 3, 7, 22, 6);
    Call6(Func_0200962e, 19, 6, 3, 7, 13, 6);
    Call6(Func_02009640, 19, 6, 3, 7, 22, 13);
    Call6(Func_02009650, 19, 6, 3, 7, 13, 13);
    Func_020095b6(1);
    Call4(Func_02009806, 0xc00000, -0x400000, 0xee0000, 0);
    Func_020095cc(1);
    Func_02009650();
    Func_020095d6(1);
    Func_02009734(20);
    Func_0200973a(19);
    Func_020097a2(0, 19);
    record = Func_02009740(0);
    Func_020096a6(record, 0);
    *(s32 *)(rec4 + 8) = 0x15a0000;
    *(s32 *)(rec4 + 16) = 0xcd0000;
    *(s32 *)(rec4 + 12) = 0x200000;
    {
        s32 shown = 0x6000;
    
        *(u16 *)(rec4 + 6) = shown;
    }
    Func_020090a2(rec4);
    Func_020097d6(1, 18);
    record = Func_02009774(1);
    Func_020096da(record, 0);
    record = Func_02009780(1);
    *(s32 *)(record + 8) = 0x1640000;
    *(s32 *)(record + 16) = 0xc00000;
    {
        s32 shown = 0xa000;
    
        *(u16 *)(record + 6) = shown;
    }
    *(s32 *)(record + 12) = 0x200000;
    Func_020090d4();
    Func_02009808(2, 18);
    record = Func_020097a6(2);
    Func_0200970c(record, 0);
    record = Func_020097b2(2);
    *(s32 *)(record + 8) = 0x1680000;
    {
        s32 shown = 0x2000;
    
        *(u16 *)(record + 6) = shown;
    }
    *(s32 *)(record + 12) = 0x200000;
    *(s32 *)(record + 16) = 0xde0000;
    Func_02009106();
    Func_0200983a(3, 18);
    record = Func_020097d8(3);
    Func_0200973e(record, 0);
    record = Func_020097e4(3);
    *(s32 *)(record + 8) = 0x14e0000;
    {
        s32 shown = 0x8000;
    
        *(u16 *)(record + 6) = shown;
    }
    *(s32 *)(record + 12) = 0x200000;
    *(s32 *)(record + 16) = 0xde0000;
    Func_02009134();
    Call3(Func_02009866, 21, 0xc40000, 0xdc0000);
    Func_02009876(21, 5);
    Call3(Func_0200987c, 6, 0xbc0000, 0x13c0000);
    Call2((void (*)())Func_0200988c, 6, 5);
    record = Func_0200982a(6);
    Func_02009790(record, 0);
    record = Func_02009836(8);
    *(s32 *)(record + 8) += -0x100000;
    Func_0200917e();
    record = Value1(Func_02009848, 9);
    *(s32 *)(record + 8) += -0x100000;
    Func_0200918e();
    record = Func_02009858(10);
    *(s32 *)(record + 8) += 0x100000;
    Func_020091a2();
    record = Value1(Func_0200986c, 11);
    *(s32 *)(record + 8) += 0x100000;
    Func_020091b2();
    Call3(Func_020097f6, 0x10000, 0x10000, 0x10000);
    record = Func_0200988c(23);
    Func_020097f2(record, 0);
    *(u8 *)(Func_02009898(23) + 85) = 4;
    Func_0200993e(23, 4);
    record = Func_020098ac(23);
    *(s32 *)(record + 12) = 0x280000;
    Call2(Func_02009774, 0x200da29, 0xc80);
    work = *(u8 *volatile *)Data_03001ebc;
    *(s32 *)(((s32)work + 0x1c0)) = 0x200;
    *(s32 *)(((s32)work + 0x1c8)) = 24;
    Func_02009a1e();
    Func_02009a32();
    Func_020098c0(40);
    Func_02008784();
    Call1(Func_020098b2, 0x9a7);
    Func_02009a00(2);
}
