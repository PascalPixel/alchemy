#include "types.h"

#define Scene_RunScene380SequenceA Func_02000390

s32 Func_02004852();
void Func_020048b8();
void Func_02004cf2();
void Func_02004d08();
void Func_02004d2c();
void Func_02004d3c();
void Func_02004d40();
void Func_02004d50();
void Func_02004d54();
void Func_02004d60();
void Func_02004d66();
void Func_02004d90();
void Func_02004d96();
void Func_02004da8();
void Func_02004db8();
void Func_02004dba();
void Func_02004dcc();
void Func_02004dd4();
void Func_02004df0();
void Func_02004e04();
void Func_02004e0e();
void Func_02004e16();
void Func_02004e28();
void Func_02004e3a();
void Func_02004e42();
void Func_02004e48();
void Func_02004e5a();
void Func_02004e7c();
void Func_02004e82();
void Func_02004e92();
void Func_02004e92_a();
void Func_02004ea8();
void Func_02004ea8_a();
void Func_02004eac();
void Func_02004eae();
void Func_02004eb6();
void Func_02004eba();
void Func_02004ec6();
void Func_02004ec6_a();
void Func_02004eca();
void Func_02004ed2();
void Func_02004ed6();
void Func_02004ef0();
void Func_02004ef6();
void Func_02004f16();
void Func_02004f26();
s32 Func_02004f28();
void Func_02004f30();
void Func_02004f3e();
void Func_02004f46();
void Func_02004f4e();
void Func_02004f52();
void Func_02004f5c();
void Func_02004f60();
void Func_02004f74();
void Func_02004f8e();
void Func_02004fb4();
void Func_02004fc0();
void Func_02004fce();
void Func_02004fea();
void Func_0200500c();
void Func_0200502e();
void Func_0200503c();
void Func_02005048();
void Func_0200505a();
void Func_02005066();
void Func_0200507e();
void Func_02005084();
void Func_02005094();
void Func_02005098();
void Func_020050a0();
void Func_020050bc();
void Func_020050c4();
void Func_020050ea();
void Func_02005114();
void Func_02005120();
void Func_02005130();
void Func_02005134();
void Func_02005144();
void Func_02005160();
void Func_0200516c();
void Func_0200517a();
void Func_0200518e();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

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

static __inline__ s32 Value4(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    return f(a0, a1, a2, a3);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

void Scene_RunScene380SequenceA(void)
{
    u32 i;
    s32 rec7;
    s32 record;
    s32 base5_1075;

    Func_02004d3c();
    Func_02004eba(141);
    for (i = 0; i != 6; i++) {
        Call2(Func_02004e7c, 0x403a52, 1);
        Func_02004e92(8);
        Func_02004d50(8);
        Call2(Func_02004e92_a, 0x10000, 1);
        Func_02004ea8(8);
        Func_02004d66(8);
        if (8 == 0) {
            Call3(Func_02004cf2, 0x10000, 0x10000, 0x10000);
        }
    }
    Call3(Func_02004d08, 0x10000, 0x20000, 0x10000);
    Func_02004d96(30);
    Call2(Func_02004eae, 0x26666, 0x4ccc);
    Call4(Func_02004ec6, 0x2980000, -1, 0x1f10000, 1);
    Func_02004ed2();
    Func_02004db8(20);
    Call3(Func_02004d40, 0x10000, 0x20000, 0x10000);
    Func_02004f4e(144);
    Func_02004d2c(0x200d088, 96, 29);
    Call6(Func_02004d54, 0, 0, 1, 1, 41, 29);
    Call6(Func_02004d60, 87, 42, 41, 31, 1, 2);
    Func_02004e0e(40);
    Func_02004d90(0, 0, 0);
    Call2(Func_02004f30, 0x66666, 0xcccc);
    Call4(Func_02004f46, 0x1370000, -1, 0x1f10000, 1);
    Func_02004f52();
    Call3(Func_02004dba, 0x10000, 0x20000, 0x10000);
    Func_02004e48(20);
    Func_02004fce(144);
    Func_02004da8(0x200d088, 74, 29);
    Call6(Func_02004dcc, 0, 0, 1, 1, 19, 29);
    Call6(Func_02004dd4, 87, 42, 19, 31, 1, 2);
    Func_02004e82(40);
    Func_02004e04(0, 0, 0);
    Call4(Func_02004fb4, 0x2970000, -1, 0xc00000, 1);
    Func_02004fc0();
    Call3(Func_02004e28, 0x10000, 0x20000, 0x10000);
    Func_02004eb6(20);
    Func_0200503c(144);
    Func_02004e16(0x200d088, 96, 10);
    Call6(Func_02004e3a, 0, 0, 1, 1, 41, 10);
    Call6(Func_02004e42, 87, 42, 41, 12, 1, 2);
    Func_02004ef0(40);
    *(s32 *)((*(s32 *)0x03001ebc + 0x1c0)) = 0x202;
    Func_0200505a();
    Func_02005066();
    Call4(Func_0200502e, 0x2c60000, -1, 0x1da0000, 0);
    Func_02004e5a();
    Func_02004df0(1);
    Call3(Func_02004ea8_a, 0x20000, 0x10000, 0x10000);
    Func_02005084();
    Func_02005098();
    Func_02004f3e(40);
    Call1(Func_020050c4, 0x121);
    Call3(Func_02004eca, -1, -1, 0xe666);
    Func_02004ed6();
    Func_02004f5c(20);
    Call6(Func_02004ec6_a, 0, 40, 43, 66, 3, 3);
    Func_02004f74(20);
    rec7 = Value4(Func_02004852, 220, 0x2c80000, 0x100000, 0x1d00000);
    Func_02004f8e(40);
    Func_02004f26(rec7, 1);
    base5_1075 = 0x1075;
    Value2(Func_02004f28, base5_1075, 1);
    Call3(Func_02005094, 9, 0x2000, 0);
    Call3(Func_020050a0, 5, 0x2000, 20);
    Func_02005114();
    Func_02005120();
    Call4(Func_020050ea, 0x1ce0000, -1, 0x15e0000, 0);
    Func_02004f16();
    Func_02004eac(1);
    Func_02005130();
    Func_02005144();
    Func_02004fea(40);
    Func_0200507e(9, 4, 30);
    Func_020050bc(0x1074);
    Func_020048b8(9, 20);
    Func_02005160();
    Func_0200516c();
    Call4(Func_02005134, 0x2c60000, -1, 0x1da0000, 0);
    Func_02004f60();
    Func_02004ef6(1);
    Func_0200517a();
    Func_0200518e();
    Call1(Func_0200500c, 0x83c);
    Func_02005048();
}
