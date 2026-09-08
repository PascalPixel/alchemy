#include "types.h"

#define FieldScene_RunMultiActorPresentation Func_02003e9c

void Func_0200478e();
void Func_020047be();
void Func_020047fa();
void Func_02004808();
void Func_02004834();
void Func_02004842();
void Func_0200484e();
void Func_02004874();
void Func_02004888();
void Func_020048da();
void Func_020048fe();
void Func_02004902();
void Func_02004926();
void Func_0200497c();
void Func_02004990();
void Func_0200499e();
void Func_020049b0();
void Func_020049d0();
void Func_020049e6();
void Func_020049fe();
void Func_02004a04();
void Func_02004a16();
void Func_02004a24();
void Func_02004a34();
void Func_02004a42();
void Func_02004a5a();
void Func_02004a68();
void Func_02009bdc();
void Func_02009bf4();
void Func_02009c26();
s32 Func_02009c34();
void Func_02009c38();
void Func_02009c86();
void Func_02009c9e();
void Func_02009ca6();
void Func_02009cbe();
void Func_02009cc6();
s32 Func_02009ccc();
void Func_02009cf0();
void Func_02009cf0_a();
void Func_02004834_a();
void Func_02009e1e_a();
void Func_02009cf4();
void Func_02009d86();
void Func_02009d92();
void Func_02009da2();
void Func_02009dc8();
void Func_02009dce();
void Func_02009dda();
void Func_02009ddc();
void Func_02009dee();
void Func_02009df8();
void Func_02009dfe();
void Func_02009e06();
void Func_02009e12();
void Func_02009e1e();
void Func_02009e22();
void Func_02009e2a();
void Func_02009e34();
void Func_02009e36();
void Func_02009e3a();
void Func_02009e58();
void Func_02009e5a();
void Func_02009e5c();
void Func_02009e60();
void Func_02009e78();
void Func_02009e88();
void Func_02009e8a();
void Func_02009ea6();
void Func_02009eb2();
void Func_02009ec0();
void Func_02009ecc();
void Func_02009eec();
void Func_02009f04();
void Func_02009f0a();
void Func_02009f0e();
void Func_02009f10();
void Func_02009f20();
void Func_02009f2a();
void Func_02009f40();
void Func_02009f46();
void Func_02009f4e();
void Func_02009f52();
void Func_02009f5c();
void Func_02009f82();
void Func_02009f88();
void Func_02009f94();
void Func_02009fa4();
void Func_02009fb6();
void Func_02009fc6();
void Func_02009fdc();
void Func_02009ff4();
void Func_0200a000();
void Func_0200a022();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 */

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

void FieldScene_RunMultiActorPresentation(void)
{
    s32 count_flag;
    s32 request_a;
    s32 request_b;

    Func_02009c86(1, 1);
    Func_02009bdc(20);
    Func_02009cc6(1, 0, 20);
    Func_02009c9e(2, 1);
    Func_02009bf4(20);
    Value2(Func_02009ccc, 2, 0);
    Call3(Func_02009cf0, 2, 0xc000, 0);
    Func_0200478e(0, 0x4000);
    count_flag = 0;
    if (Value2(Func_02009c34, 0, 0) == 0) {
        Func_02009c26(20);
        Func_02009cbe(2, 4);
        count_flag = 1;
    } else {
        Func_02009c38(20);
        Func_02009cf0_a(2, 1);
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    }
    Func_020047be(2);
    if (count_flag != 0) {
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    }
    Call3(Func_02009ca6, 1, 0x19999, 0xcccc);
    request_a = 0x1001;
    Call3(Func_02009cf4, 1, 0x141, 174);
    Func_02004808(1, 0x2000);
    Func_020047fa(request_a);
    Call3(Func_02009d86, 0, 0xa000, 0);
    Call3(Func_02009d92, 2, 0xa000, 0);
    Func_02004834(3, 0xc000);
    Func_02004842(1, 0x4000);
    Func_02004834_a(request_a);
    Call3(Func_02009dce, 1, 0x105, 40);
    Func_02009dc8(1, 0x2000, 20);
    Func_0200484e(request_a);
    Call3(Func_02009dda, 1, 0xa000, 40);
    Call3(Func_02009ddc, 0x8001, 0, 40);
    Func_02009dee(1, 0x2000, 20);
    Func_02004874(request_a);
    Func_02009eb2(17);
    Func_02009da2(3, 4);
    Func_02004888(3);
    Func_02009e12(1, 0x4000, 80);
    Call3(Func_02009e1e, 3, 0x6000, 80);
    Call3(Func_02009e3a, 3, 0x100, 40);
    Call3(Func_02009e36, 0, 0x6000, 60);
    Func_02009e06(0, 2);
    Func_020048da(2, 0xc000);
    Call3(Func_02009e60, 2, 0x101, 60);
    Call3(Func_02009e5c, 2, 0x6000, 40);
    Func_02009e1e_a(2, 4, 60);
    Func_02004902(1, 0x2000);
    Call3(Func_02009e88, 1, 0x101, 40);
    Func_020048fe(request_a);
    Call3(Func_02009e8a, 1, 0x6000, 40);
    Call3(Func_02009ea6, 1, 0x100, 0);
    Func_02009e58(1, 4, 40);
    Func_02004926(1);
    Call3(Func_02009df8, 21, 0xcccc, 0x6666);
    Func_02009e2a(21, 200, 188);
    Func_02009e34(6, 200, 204);
    Call2(Func_02009eec, 0x33333, 0x6666);
    Call4(Func_02009f04, 0xfc0000, 0, 0xbe0000, 1);
    Func_02009f10();
    Func_02009dfe(40);
    Func_02009fa4(23);
    Call2(Func_02009f0e, 21, 0x102);
    Func_0200497c(21);
    Func_02009ecc(21, 1);
    Func_02009e22(20);
    Func_02004990(21);
    Func_02009ec0(21, 4);
    Func_0200499e(21);
    Call3(Func_02009f2a, 21, 0x3000, 20);
    Func_020049b0(21);
    Call2(Func_02009f52, 6, 0x102);
    request_b = 0x2003;
    Func_02009e5a(20);
    Func_02009f0a(3, 2);
    Func_020049d0(request_b);
    Func_02009f20(21, 2);
    Func_02009e78(20);
    Func_020049e6(0x2002);
    Func_02004a04(21, 0xe000);
    Func_02009f40(1, 1);
    Func_020049fe(1);
    Func_02009f4e(21, 1);
    Func_02009f88(21, 0, 20);
    Func_02004a16(request_b);
    Func_02009f46(21, 4);
    Func_02004a24(21);
    Call2(Func_02009fc6, 2, 0x102);
    Func_02004a34(0x2002);
    Func_02009f5c(21, 3);
    Func_02004a42(21);
    Call3(Func_02009fdc, 1, 0x103, 40);
    Func_02009f94(1, 2);
    Func_02004a5a(1);
    Func_02009f82(21, 4);
    Func_02004a68(21);
    Call3(Func_02009ff4, 21, 0x5000, 20);
    Func_02009fb6(3, 4, 20);
    Func_0200a000(request_b, 0, 20);
    Call3(Func_0200a022, 21, 0x103, 40);
}
