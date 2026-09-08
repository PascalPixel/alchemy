#include "types.h"

extern u8 Data_0200e6a8[];

#define FieldScene_RunThreeActorPresentation Func_02001b34

void Func_020063f0();
void Func_02006412();
void Func_02006418();
void Func_0200641a();
void Func_02006448();
void Func_02006458();
void Func_02006476();
void Func_02006484();
void Func_02006498();
void Func_020064a6();
void Func_020064b4();
void Func_020064ba();
void Func_020064c8();
void Func_020064d6();
void Func_020064e4();
void Func_020064f2();
void Func_02006514();
void Func_02006554();
void Func_0200656a();
void Func_02006570();
void Func_0200658c();
void Func_0200659a();
void Func_020065b2();
void Func_020065ee();
void Func_020065fe();
void Func_02006622();
void Func_02006630();
void Func_0200664a();
void Func_02006658();
void Func_02006720();
s32 Func_02007fe6();
void Func_02008018();
void Func_020080bc();
void Func_020080f4();
void Func_02008120();
void Func_02008122();
void Func_02008136();
void Func_02008154();
void Func_0200815e();
void Func_0200817c();
void Func_02008180();
void Func_02008188();
void Func_02008192();
void Func_0200819c();
void Func_020081b6();
void Func_020081bc();
void Func_020081c0();
void Func_020081c2();
void Func_020081c6();
void Func_020081dc();
void Func_020081e0();
void Func_020081e6();
void Func_02008214();
void Func_02008228();
void Func_0200825a();
void Func_02008266();
void Func_02008268();
void Func_02008278();
void Func_02008280();
void Func_02008286();
void Func_020082a4();
void Func_020082a8();
void Func_020082b8();
void Func_020082c0();
void Func_020082c2();
void Func_020082f2();
void Func_020082f6();
void Func_020082fe();
void Func_02008308();
void Func_02008312();
void Func_0200831e();
void Func_02008320();
void Func_02008322();
void Func_02008324();
void Func_0200832a();
void Func_0200832c();
void Func_02008330();
void Func_02008336();
void Func_02008340();
void Func_02008346();
void Func_0200834e();
void Func_02008358();
void Func_0200835c();
void Func_02008366();
void Func_0200838c();
void Func_02008394();
void Func_020083b6();
void Func_020083b8();
void Func_020083c6();
void Func_020083e6();
void Func_0200840a();
void Func_0200840e();
void Func_02008426();
void Func_02008454();
void Func_02008460();
void Func_02008476();
void Func_020084a8();

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

void Func_02008320_a();
void Func_02008308_a();
void Func_02008312_a();
void Func_0200832c_a();
void Func_02008340_a();
u8 *Func_02008336_a();
u8 *Func_02008358_a();
void Func_020083c6_a();
void Func_0200838c_a();
void Func_020083b8_a();

void FieldScene_RunThreeActorPresentation(void)
{
    s32 request_a;
    s32 request_b;
    s32 action;

    if (Value1(Func_02007fe6, 0x911) == 0) {
    } else {
        Func_02008018();
        Func_0200819c();
        Call2(Func_02008154, 0x26666, 0x4ccc);
        Func_02006418( 0x5b70000, -1, 0x1d00000, 0x10000014);
        Func_020080f4(13, 1);
        Call1(Func_02008122, 0x1d56);
        Func_020063f0(0x200d);
        Func_02006412(12, 0xd000);
        request_a = 0x800c;
        Call3(Func_02008188, 12, 0x102, 20);
        Func_02008120(12, 2);
        Func_0200641a(request_a);
        Func_02008136(14, 1);
        Call3(Func_02008180, 0xa00e, 0, 20);
        Func_02006448(12, 0);
        request_b = 0xa00e;
        Call3(Func_020081bc, 12, 0x101, 40);
        Call3(Func_020081c6, 14, 0x103, 40);
        Func_0200815e(14, 3);
        Func_02006458(request_b);
        Call2(Func_020081e6, 12, 0x102);
        Func_020080bc(40);
        Func_0200817c(12, 3);
        Func_02006476(request_a);
        Func_02008192(14, 1);
        Func_02006484(request_b);
        Func_020064a6(14, 0xb000);
        Func_02006498(request_b);
        Func_020064b4(12, 0xd000);
        Call3(Func_02008228, 12, 0x100, 30);
        Func_020081c0(12, 1);
        Func_020064ba(request_a);
        Func_020081b6(13, 4);
        Func_020064c8(0x200d);
        Func_020081dc(13, 2);
        Func_020064d6(0x200d);
        Func_020081c2(12, 4);
        Func_020064e4(request_a);
        Func_020081e0(14, 4);
        Func_020064f2(request_b);
        Func_02006514(14, 0x8000);
        Func_02008214(14, 2);
        Func_02008266(request_b, 0, 20);
        Func_02008280(12, 0, 0);
        Call3(Func_020082a4, 12, 0x102, 80);
        Func_02008286(request_a, 0, 20);
        Call3(Func_020082b8, 14, 0x103, 0);
        Call3(Func_020082c2, 13, 0x103, 60);
        Func_0200825a(14, 2);
        Func_02006554(request_b);
        Func_02006570(14, 0xb000);
        Func_02008278(14, 1);
        Func_0200656a(request_b);
        Func_0200658c(13, 0x3000);
        Call3(Func_020082fe, 13, 0x101, 0);
        Call3(Func_02008308, 12, 0x101, 60);
        Func_020082a8(13, 1);
        Func_0200659a(13);
        Call3(Func_02008320, 14, 0x103, 40);
        Func_020082c0(14, 1);
        Func_020065b2(request_b);
        Func_02008320_a(12, 0xd000, 0);
        Call3(Func_0200832c, 13, 0x5000, 40);
        Call3(Func_02008336, 12, 0, 0);
        Func_020065ee(13, 0x3000);
        Func_020082f6(12, 2);
        Func_02008340(request_a, 0, 20);
        Call3(Func_0200835c, 14, 0x4000, 40);
        Func_020065fe(request_b);
        Func_02008312(12, 2);
        Func_02008322(13, 2);
        Func_02008268(60);
        Func_02008330(13, 1);
        Func_02006622(13);
        Func_0200831e(14, 3);
        Func_02006630(request_b);
        Call3(Func_020083b8, 12, 0x102, 40);
        Call2(Func_02008358, 12, 2);
        Func_0200664a(request_a);
        Func_02008346(13, 3);
        Func_02006658(13);
        Func_020083c6(14, 0xb000, 40);
        Func_0200834e(14, 3);
        Func_02008366(13, 3);
        Call3(Func_02008308_a, 14, 0x19999, 0xcccc);
        Call3(Func_02008312_a, 13, 0x19999, 0xcccc);
        action = (s32)Data_0200e6a8;
        Func_02008324(14, action);
        Func_0200832c_a(13, action);
        Func_020082f2(20);
        Call3(Func_0200840e, 12, 0x4000, 0);
        Call3(Func_02008340_a, 0, 0x26666, 0x13333);
        *(u8 *)(Func_02008336_a(0) + 90) &= 254;
        Call3(Func_02008394, 0, 184, 0x208);
        Func_0200832a(1);
        {
            u8 *record = Func_02008358_a(0);
            u32 flag = 1;

            flag = flag | record[90];
            record[90] = (u8)flag;
        }
        Func_02008454(0, 0x8000, 20);
        Call3(Func_02008460, 0, 0x4000, 0);
        Func_0200840a(12, 4, 20);
        Call3(Func_02008476, 0, 0xa000, 20);
        Func_02008426(12, 2);
        Func_02006720(12);
        Call3(Func_020083b6, 12, 0x19999, 0xcccc);
        Func_020083c6_a(12, action);
        Func_0200838c(40);
        Call3(Func_020084a8, 0, 0x4000, 0);
        Func_020083e6(12);
        Call1(Func_0200838c_a, 0x922);
        Func_020083b8_a();
    }
}
