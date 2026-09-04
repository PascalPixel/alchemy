#include "types.h"

#define RunMiddleAuxiliarySequence Func_020003b0

void Func_020047a2();
void Func_020047ca();
void Func_020047d8();
void Func_020047de();
void Func_020047e6();
void Func_020047f4();
void Func_02004804();
void Func_0200480e();
void Func_0200481c();
void Func_0200482a();
void Func_0200484c();
s32 Func_02004856();
void Func_02004868();
void Func_0200486a();
void Func_0200486c();
void Func_0200486e();
void Func_0200487a();
void Func_02004882();
void Func_02004884();
void Func_02004888();
void Func_02004892();
void Func_020048a2();
void Func_020048b0();
void Func_020048be();
void Func_020048c6();
void Func_020048cc();
void Func_020048d4();
void Func_020048e0();
void Func_020048fc();
void Func_0200490a();
void Func_0200491e();
void Func_02004940();
void Func_02004958();
void Func_0200495e();
void Func_02004968();
void Func_0200498e();
void Func_02004990();
void Func_020049ac();
void Func_020049c0();
void Func_020049d4();

extern u8 Data_0200c570[];

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

void RunMiddleAuxiliarySequence(s32 a)
{
    u8 *obj;
    u8 *q;

    obj = Func_020047bc();
    Func_020047a2();
    q = Data_0200c570;
    Func_020047de(a, q);
    Func_0200486c(0x2009);
    Func_02004884(a, 0);
    Func_02004804(a);
    *(s32 *)(obj + 28) = 0x10000;
    *(s32 *)(obj + 24) = 0x10000;
    Func_020047ca(30);
    Func_0200487a(a, 2);
    Func_020047d8(30);
    Func_02004888(a, 2);
    Func_020047e6(60);
    Func_020048be(a, 0);
    Func_020047f4(20);
    Func_020048e0(a, 258, 60);
    Func_020048b0(a, 2);
    Func_0200480e(30);
    Func_020048be(a, 2);
    Func_0200481c(30);
    Func_020048cc(a, 2);
    Func_0200482a(30);
    Func_0200486a(a, q);
    Func_0200490a(a, 0);
    Call3(Func_0200491e, a, 0xe000, 0);
    Func_0200484c(10);
    *(s32 *)(obj + 28) = 0x10000;
    *(s32 *)(obj + 24) = 0x10000;
    Func_02004892(a, q);
    Func_0200486e();
}
