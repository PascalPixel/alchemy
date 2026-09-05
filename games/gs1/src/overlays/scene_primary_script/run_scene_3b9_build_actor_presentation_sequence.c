#include "types.h"

#define FieldScene_BuildActorPresentationSequence Func_02001cd4

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
void Func_02003a00();
void Func_02003a36();
void Func_02003a44();
void Func_02003a56();
void Func_02003a90();
void Func_02003a94();
void Func_02003aa8();
void Func_02003aba();
void Func_02003ac8();
void Func_02003ae4();
void Func_02003aea();
void Func_02003af2();
void Func_02003b16();
void Func_02003b18();
s32 Func_02003b32();
void Func_02003b42();
void Func_02003b50();
void Func_02003b58();
void Func_02003b70();
void Func_02003b7e();
void Func_02003be4();
void Func_02003bf2();
void Func_02003c18();
void Func_02003c1e();
void Func_02003c4a();
void Func_02003c84();
void Func_02003cb8();
void Func_02003cbe();
void Func_02003d28();
void Func_02003d36();
void Func_02003da4();
void Func_02003dce();
void Func_02003ddc();
void Func_02003e52();
void Func_02003e60();
void Func_02003e98();
void Func_02003ea6();
void Func_02003eb4();
void Func_02003ec0();
void Func_02003eca();
s32 Func_02003ed0();
void Func_02003f5a();
void Func_0200489c();
void Func_020048ca();
void Func_020048d4();
void Func_02004942();
void Func_02004950();
void Func_0200495e();
void Func_02004966();
void Func_02004974();
void Func_02004982();
void Func_0200499e();
void Func_020049b4();
void Func_020049ce();
void Func_020049fe();
void Func_02004a06();
void Func_02004a0c();
void Func_02004a28();
void Func_02004a36();
void Func_02004a4c();
void Func_02004a58();
void Func_02004a62();
void Func_02004a72();
void Func_02004a76();
void Func_02004a7c();
void Func_02004a88();
void Func_02004a8a();
void Func_02004a98();
void Func_02004ac0();
void Func_02004acc();
void Func_02004ace();
void Func_02004ad6();
void Func_02004ae0();
void Func_02004ae6();
void Func_02004aec();
void Func_02004af4();
void Func_02004b02();
void Func_02004b06();
void Func_02004b16();
void Func_02004b20();
void Func_02004b30();
void Func_02004b3c();
void Func_02004b5e();
void Func_02004b66();
void Func_02004b78();
void Func_02004b96();
void Func_02004ba6();
void Func_02004bf6();
void Func_02004bf8();
void Func_02004c02();
void Func_02004c0a();
void Func_02004c0e();
void Func_02004c22();
void Func_02004c2e();
void Func_02004c36();
void Func_02004c3a();
void Func_02004c4a();
void Func_02004c62();
void Func_02004c6c();
void Func_02004c76();
void Func_02004c78();
void Func_02004ca0();
void Func_02004cac();
void Func_02004cb2();
void Func_02004cb8();
void Func_02004cc8();
void Func_02004cd4();
void Func_02004cd6();
void Func_02004cde();
void Func_02004cea();
void Func_02004d02();
void Func_02004d0c();
void Func_02004d16();
void Func_02004d36();
s32 Func_02004d3c();
void Func_02004d42();
void Func_02004d4c();
void Func_02004d4e();
void Func_02004d58();
void Func_02004d6a();
void Func_02004d74();
void Func_02004d7e();
void Func_02004d8c();
s32 Func_02004da2();
void Func_02004da6();
void Func_02004db0();
void Func_02004dc2();
void Func_02004dcc();
void Func_02004dd6();
void Func_02004de0();
void Func_02004de4();
void Func_02004dea();
void Func_02004e62();
void Func_02004e6c();
void Func_02004e78();
void Func_02004e84();
void Func_02004e96();
void Func_02004ea0();
void Func_02004eaa();
void Func_02004eb6();
void Func_02004ebe();
void Func_02004ed2();
void Func_02004ed4();
void Func_02004ede();
void Func_02004ee2();
void Func_02004ee8();
void Func_02004eea();
void Func_02004ef6();
void Func_02004efc();
void Func_02004efe();
s32 Func_02004f04();
void Func_02004f06();
void Func_02004f1c();
void Func_02004f1e();
void Func_02004f2e();
void Func_02004f3a();
void Func_02004f44();
void Func_02004f4a();
void Func_02004f56();
void Func_02004f72();
void Func_02004f74();
void Func_02004f7a();
void Func_02004f7e();
void Func_02004f80();
void Func_02004f86();
void Func_02004f8a();
void Func_02004fca();
void Func_02004fd0();
void Func_0200503c();
void Func_02005056();
void Func_020050b2();
void Func_020050d0();
void Func_020050dc();

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

/* The scene step counter at 0x1d8 of the shared scene work record. */
extern u8 Data_0200adac[];

static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)0x03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_BuildActorPresentationSequence(void)
{
    s32 flag;
    s32 k6000;
    s32 k8000;
    s32 base5_200adac;

    Func_020048d4();
    Call3(Func_02004942, 1, 0x3180000, 0x880000);
    Call3(Func_02004950, 2, 0x3380000, 0x880000);
    Call3(Func_0200495e, 3, 0x3280000, 0x980000);
    Func_0200489c(1);
    Call4(Func_02004a06, -1, -1, -1, 0);
    Func_02004a36(0, 0);
    Func_02004a36(0, 0);
    Func_02004a4c(1);
    Func_020048ca(1);
    *(s32 *)((*(u8 **)0x03001ebc + 0x1c0)) = 0x203;
    *(s32 *)((*(u8 **)0x03001ebc + 0x1c8)) = 1;
    Func_02004a76();
    Func_02004a8a();
    Func_02004a72(0, 0);
    Call2(Func_02004a72, 0x10002, 0);
    Func_02004a88(40);
    Func_02004966(80);
    Func_020049fe(8, 1);
    Func_02004974(20);
    Func_02004a0c(2, 2);
    Func_02004982(40);
    Func_02003a00(8, 0x3000);
    Func_02004a28(8, 2);
    Func_0200499e(20);
    Call2(Func_02004ac0, 0x10000, 0);
    Func_02004ad6(40);
    Func_020049b4(80);
    Call3(Func_02004a98, 2, 0x100, 0);
    Func_02004a58(2, 1);
    Func_020049ce(20);
    Call1(Func_02004a7c, 0x20f8);
    Func_02003a36(2);
    Func_02004a62(8, 3);
    Func_02003a44(8);
    Call3(Func_02004acc, 3, 0x102, 40);
    Func_02003a56(3);
    Call3(Func_02004ad6, 8, 0x8000, 0);
    Func_02004ae0(9, 0, 40);
    Call3(Func_02004aec, 8, 0x5000, 0);
    Func_02003a94(9, 0x3000);
    Call3(Func_02004b06, 9, 0x105, 20);
    Func_02003a90(9);
    Call3(Func_02004b16, 1, 0x103, 20);
    Func_02004ace(1, 2);
    Func_02003aa8(1);
    Call3(Func_02004b30, 10, 0x102, 40);
    Func_02003aba(10);
    Func_02004ae6(11, 3);
    Func_02003ac8(11);
    Func_02003aea(2, 0xa000);
    Func_02004b02(2, 4);
    Func_02003ae4(2);
    Func_02004b20(3, 1);
    Func_02003af2(3);
    Call3(Func_02004b78, 1, 0x103, 20);
    Func_02003b18(1, 0);
    k6000 = 0x6000;
    Func_02004b3c(1, 2);
    Func_02003b16(1);
    Value2(Func_02003b32, 0, k6000);
    Call3(Func_02004ba6, 0, 0x102, 60);
    Func_02004b66(2, 1);
    Call2(Func_02003b50, 2, 0x8000);
    Func_02003b42(2);
    Func_02004b5e(3, 4);
    Func_02004af4(20);
    Func_02003b58(3);
    Func_02004b96(1, 1);
    Func_02003b7e(1, 0x2000);
    Func_02003b70(1);
    Call3(Func_02004bf6, 2, 0x105, 60);
    Func_02004bf8(2, 0x2000, 0);
    Func_02004c02(1, 0, 0);
    Call3(Func_02004c0e, 3, 0xe000, 20);
    Call3(Func_02004c22, 0, 0x102, 0);
    Call3(Func_02004c2e, 1, 0x102, 0);
    Call3(Func_02004c3a, 3, 0x102, 80);
    Call3(Func_02004c4a, 2, 0x106, 0);
    Func_02004c0a(2, 1);
    Func_02003bf2(2, 0xc000);
    Func_02003be4(2);
    Func_02004c62(0, 0xc000, 0);
    Func_02004c6c(1, 0xc000, 0);
    Func_02004c76(3, 0xc000, 20);
    Func_02003c1e(8, 0x3000);
    Func_02004c36(8, 3);
    Func_02003c18(8);
    Call3(Func_02004ca0, 0, 0x100, 0);
    Call3(Func_02004cac, 1, 0x100, 0);
    Call3(Func_02004cb8, 3, 0x100, 40);
    Func_02004c78(9, 1);
    Func_02003c4a(9);
    Func_02004cc8(0, k6000, 0);
    Call3(Func_02004cd4, 1, 0xe000, 0);
    Func_02004cde(2, k6000, 0);
    Call3(Func_02004cea, 3, 0xe000, 40);
    Func_02004cb2(10, 1);
    Func_02003c84(10);
    Func_02004d02(0, 0xc000, 0);
    Func_02004d0c(1, 0xc000, 0);
    Func_02004d16(2, 0xc000, 0);
    Func_02003cbe(3, 0xc000);
    Func_02004cd6(11, 3);
    Func_02003cb8(11);
    Func_02004d36(0, k6000, 0);
    Call3(Func_02004d42, 1, 0xe000, 0);
    Func_02004d4c(2, k6000, 0);
    Call3(Func_02004d58, 3, 0xe000, 20);
    Call3(Func_02004d6a, 1, 0x105, 0);
    Call3(Func_02004d74, 2, 0x105, 0);
    Call3(Func_02004d7e, 3, 0x105, 80);
    Call3(Func_02004d8c, 2, 0x106, 0);
    Func_02004d4e(2, 1);
    Func_02003d36(2, 0xe000);
    Func_02003d28(2);
    Func_02004da6(0, 0, 0);
    Func_02004db0(1, 0, 20);
    Call3(Func_02004dc2, 11, 0x101, 60);
    Value2(Func_02004da2, 2, 0);
    Func_02004dcc(0, 0x2000, 0);
    Func_02004dd6(1, 0xe000, 0);
    Func_02004de0(2, 0xa000, 0);
    Func_02004dea(3, 0xc000, 0);
    flag = 0;
    if (Func_02004d3c(0, 0) == 1) {
        Func_02003da4(2);
        flag = 1;
    } else {
        bump_step(1);
        Func_02004de4(2, 3);
        Func_02003ddc(2, 0xc000);
        Func_02003dce(2);
    }
    if (flag) {
        bump_step(1);
    }
    Call3(Func_02004e62, 8, 0x8000, 0);
    Func_02004e6c(9, 0, 0);
    Call3(Func_02004e78, 10, 0x3000, 0);
    Call3(Func_02004e84, 11, 0xb000, 20);
    Call3(Func_02004e96, 8, 0x105, 0);
    Call3(Func_02004ea0, 9, 0x105, 0);
    Call3(Func_02004eaa, 10, 0x105, 0);
    Call3(Func_02004eb6, 11, 0x105, 60);
    Func_02004e78(8, 1);
    Func_02003e60(8, 0x3000);
    Func_02003e52(8);
    Call3(Func_02004ed2, 0, 0xc000, 0);
    Call3(Func_02004ede, 1, 0xc000, 0);
    Call3(Func_02004eea, 2, 0xc000, 0);
    Call3(Func_02004ef6, 3, 0xc000, 20);
    Func_02004ebe(9, 1);
    Func_02003ea6(9, 0x3000);
    Func_02003e98(9);
    Func_02004ed4(10, 1);
    Call2(Func_02003ec0, 10, 0x5000);
    k8000 = 0x8000;
    Func_02003eb4(10);
    Value2(Func_02003ed0, 11, k8000);
    Func_02004ee8(11, 3);
    Func_02003eca(11);
    Call3(Func_02004f4a, 1, 0xe000, 0);
    Call3(Func_02004f56, 2, 0xa000, 20);
    Func_02004efe(1, 3);
    Func_02004f06(2, 3);
    Func_02004f1e(3, 3);
    Call3(Func_02004ed2, 1, 0x10000, k8000);
    Call3(Func_02004ede, 2, 0x10000, k8000);
    Call3(Func_02004eea, 3, 0x10000, k8000);
    base5_200adac = (s32)Data_0200adac;
    Func_02004efc(1, base5_200adac);
    Value2(Func_02004f04, 2, base5_200adac);
    Func_02004f1c(3, base5_200adac);
    Func_02004ee2(20);
    Func_02003f5a(0, 0);
    Func_02004f72(0, 3);
    Func_02004f7a(11, 3);
    Call3(Func_02004f2e, 11, 0x10000, k8000);
    Call3(Func_02004f3a, 0, 0x10000, k8000);
    Func_02004f8a(11, 2);
    Call3(Func_02004f74, 11, 0x33e, 152);
    Call3(Func_02004f80, 11, 0x328, 164);
    Call3(Func_02004f86, 11, 0x328, 0x138);
    Func_02004f44(20);
    Call2(Func_0200503c, 0x6666, 0xccc);
    Call4(Func_02005056, 0x3280000, -1, 0x1380000, 1);
    Call3(Func_02004fca, 0, 0x328, 164);
    Call3(Func_02004fd0, 0, 0x328, 0x138);
    Func_02004f7e(60);
    *(s32 *)((*(u8 **)0x03001ebc + 0x1c0)) = 0x100;
    *(s32 *)((*(u8 **)0x03001ebc + 0x1c8)) = 40;
    Func_020050d0();
    Func_020050dc();
    Func_020050b2(64);
}
