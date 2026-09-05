#include "types.h"

extern u8 Value_00002233;

/* Supplemental scene sequence of the resource_3b8 overlay. */

void Func_020081ee();
void Func_0200822a();
void Func_0200825c();
void Func_0200827e();
s32 Func_02008294();
void Func_020082a8();
void Func_020082aa();
void Func_020082c4();
void Func_020082d2();
void Func_020082ea();
void Func_0200830a();
void Func_02008314();
void Func_02008316();
void Func_02008322();
void Func_02008324();
void Func_0200832e();
void Func_02008330();
s32 Func_02008334();
void Func_0200833e();
void Func_02008344();
void Func_02008356();
void Func_0200835a();
void Func_0200835c();
void Func_0200836a();
void Func_02008372();
void Func_02008374();
void Func_02008394();
void Func_02008396();
void Func_020083ac();
void Func_020083d6();
void Func_020083da();
void Func_020083e0();
void Func_020083e6();
void Func_020083fa();
void Func_02008416();

static __inline__ void Call0(void (*f)())
{
    f();
}

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

void RunSupplementalSequenceOne(void)
{
    s32 p;
    Call1(Func_020081ee, 2412);
    Call0(Func_0200822a);
    Call0(Func_02008356);
    Call3(Func_0200830a, 8, 20480, 0);
    Call3(Func_02008316, 9, 12288, 0);
    Call3(Func_020082aa, 0, 200, 272);
    Call3(Func_0200832e, 0, 49152, 0);
    Call1(Func_0200825c, 20);
    p = (s32)&Value_00002233;
    Func_02008324(p);
    Value2(Func_02008334, 8, 0);
    if (Value2(Func_02008294, 0, 0) == 0) {
        Call1(Func_0200827e, 20);
        Func_02008344(p + 1);
        Call2(Func_0200835c, 8, 0);
    } else {
        Value1(Func_02008294, 20);
        Func_0200835a(p + 2);
        Call2(Func_02008372, 8, 0);
        Call1(Func_020082a8, 20);
        Call3(Func_0200836a, 8, 9, 60);
        Call3(Func_02008396, 9, 12288, 0);
        Call1(Func_020082c4, 40);
        Call2(Func_02008374, 9, 2);
        Call1(Func_020082d2, 30);
        Call3(Func_02008394, 8, 9, 30);
        Call2(Func_02008374, 9, 3);
        Call1(Func_020082ea, 30);
        Call3(Func_020083d6, 8, 258, 50);
        Call3(Func_020083da, 8, 20480, 0);
        Call3(Func_020083e6, 9, 12288, 0);
        Call1(Func_02008314, 20);
        Call2(Func_020083ac, 8, 4);
        Call1(Func_02008322, 20);
        Call2(Func_020083fa, 8, 0);
        Call1(Func_02008330, 10);
        Call2(Func_020083e0, 8, 2);
        Call1(Func_0200833e, 20);
        Call2(Func_02008416, 8, 0);
    }
    Call0(Func_0200835a);
}
