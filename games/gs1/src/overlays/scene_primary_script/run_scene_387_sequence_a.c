#include "types.h"

#define FieldScene_RunScene387SequenceA Func_0200067c

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02000cb0();
void Func_02000cfc();
void Func_02000d6c();
void Func_02000d92();
void Func_02000dda();
void Func_02000e0a();
void Func_02000e4a();
void Func_02000eaa();
void Func_02000ec0();
void Func_02000eec();
void Func_02000f3a();
void Func_02000f60();
void Func_020016c2();
void Func_020017b0();
void Func_020017b2();
void Func_020017e8();
void Func_020017f2();
s32 Func_020017fc();
void Func_02001808();
s32 Func_02001814();
void Func_02001816();
void Func_0200182e();
void Func_02001836();
void Func_02001842();
void Func_02001856();
s32 Func_02001866();
void Func_02001878();
void Func_0200187c();
void Func_02001888();
void Func_0200188e();
void Func_02001890();
void Func_020018a0();
void Func_020018a6();
void Func_020018ae();
void Func_020018ba();
void Func_020018be();
void Func_020018c0();
void Func_020018c8();
void Func_020018d0();
void Func_020018e2();
void Func_020018e4();
void Func_020018ee();
void Func_020018f0();
void Func_02001942();
void Func_0200195c();
void Func_0200195e();
void Func_02001970();
void Func_02001972();
void Func_02001978();
void Func_02001982();
void Func_02001986();
void Func_02001996();
void Func_02001998();
void Func_020019a2();
void Func_020019bc();
void Func_020019c8();
void Func_020019dc();
void Func_020019e4();
void Func_020019f4();
void Func_020019fa();
void Func_02001a04();
void Func_02001a06();
void Func_02001a14();
void Func_02001a28();
void Func_02001a2c();
void Func_02001a3c();
void Func_02001a40();
void Func_02001a60();
void Func_02001a66();
void Func_02001a68();
void Func_02001a6e();
void Func_02001a82();
void Func_02001a86();
void Func_02001a8c();
void Func_02001aa0();
void Func_02001aaa();
void Func_02001aac();
void Func_02001ab2();
void Func_02001ab8();
void Func_02001aba();
void Func_02001ac2();
void Func_02001ac6();
void Func_02001ace();
void Func_02001ad0();
void Func_02001ad2();
void Func_02001ae2();
void Func_02001ae6();
void Func_02001af6();
void Func_02001afa();
void Func_02001b04();
void Func_02001b0c();
void Func_02001b22();
void Func_02001b32();
void Func_02001b3a();
void Func_02001b56();
void Func_02001b5c();
void Func_02001b64();
void Func_02001b66();
void Func_02001b68();
void Func_02001b6e();
void Func_02001b74();
void Func_02001b78();
void Func_02001b8c();
void Func_02001b90();
void Func_02001b9a();
void Func_02001bc8();
void Func_02001bd4();
void Func_02001bd6();
void Func_02001be0();
void Func_02001be2();
void Func_02001bea();
void Func_02001bf4();
void Func_02001bfc();
void Func_02001bfe();
void Func_02001c04();
void Func_02001c08();
void Func_02001c12();
void Func_02001c1a();
void Func_02001c26();
void Func_02001c34();
void Func_02001c3e();
void Func_02001c44();
void Func_02001c46();
void Func_02001c52();
void Func_02001c5c();
void Func_02001c74();
void Func_02001c76();
void Func_02001c82();
void Func_02001c8e();
void Func_02001c9a();
void Func_02001ca2();
void Func_02001ca4();
void Func_02001cb2();
void Func_02001cb4();
void Func_02001cbe();
void Func_02001cc8();
void Func_02001ce6();
void Func_02001cf4();
void Func_02001cfc();
void Func_02001d06();
void Func_02001d08();
void Func_02001d12();
void Func_02001d14();
void Func_02001d1c();
void Func_02001d20();
void Func_02001d26();
void Func_02001d28();
void Func_02001d2a();
void Func_02001d32();
void Func_02001d42();
void Func_02001d50();
void Func_02001d56();
void Func_02001d60();
void Func_02001d64();
void Func_02001d68();
void Func_02001d74();
void Func_02001d7c();
void Func_02001d94();
void Func_02001da0();
void Func_02001dac();
void Func_02001db2();
void Func_02001de4();
void Func_02001dee();
void Func_02001df6();
void Func_02001dfc();
void Func_02001e02();
void Func_02001e04();
void Func_02001e08();
void Func_02001e0c();
void Func_02001e0e();
void Func_02001e14();
void Func_02001e16();
void Func_02001e1a();
void Func_02001e22();
void Func_02001e3a();
void Func_02001e3c();
void Func_02001e4c();

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

void FieldScene_RunScene387SequenceA(void)
{
    u32 i;
    s32 record;

    Func_020018a6();
    Func_020017b2();
    Func_020017b0(30);
    Call1(Func_02001856, 0x138f);
    Call3(Func_020017e8, 0, 0xcccc, 0x6666);
    Call3(Func_020017f2, 1, 0xcccc, 0x6666);
    Call3(Func_0200188e, 0, 0xe000, 20);
    record = Value1(Func_020017fc, 0);
    if (record != 0) {
        Func_02001842(1, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
    }
    Call3(Func_02001836, 1, 0x108, 168);
    Call3(Func_020018ba, 1, 0x6000, 20);
    Func_02001808(20);
    Func_02001878(1, 4);
    Func_02001816(20);
    Func_020018d0(1, 0, 20);
    Func_02001890(0, 3);
    Func_0200182e(20);
    if (Value1(Func_02001814, 0x855) == 0) {
        Func_020018a0(1, 2);
        record = Value1(Func_02001866, 0);
        if (record != 0) {
            Func_02001888(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Func_020018ae(1);
        Func_020018c0(1, 0, 0);
        Func_0200187c();
    } else {
        Call3(Func_020018e4, 2, 0x1680000, 0xf80000);
        Call3(Func_020018be, 2, 0xcccc, 0x6666);
        Call3(Func_020018e2, 2, 0x110, 248);
        Call3(Func_020018ee, 2, 0x110, 208);
        Call3(Func_02001972, 2, 0xa000, 20);
        Func_02001942(2, 2);
        Func_020018c8(20);
        Func_02001982(2, 0, 20);
        Call3(Func_02001996, 0, 0x2000, 0);
        Call3(Func_020019a2, 1, 0x4000, 0);
        Func_020018f0(20);
        Call3(Func_020019bc, 0, 0x100, 0);
        Call3(Func_020019c8, 1, 0x100, 60);
        Func_02001978(2, 3);
        Call3(Func_0200195c, 2, 0x108, 200);
        Func_0200195e(0, 248, 168);
        Func_02001970(2, 248, 184);
        Func_02001986(0);
        Call3(Func_020019fa, 0, 0x6000, 0);
        Call3(Func_02001a06, 1, 0x6000, 0);
        Func_02001998(2, 232, 184);
        Func_0200195e(20);
        Call3(Func_02001a28, 2, 0x105, 60);
        Call3(Func_02001a2c, 2, 0xe000, 20);
        Func_020019e4(2, 4);
        Func_02001982(20);
        Func_02001a3c(2, 0, 20);
        Func_020019f4(0, 3);
        Func_02001a04(1, 3);
        Func_020019a2(20);
        Call3(Func_02001a66, 2, 0x8000, 20);
        Func_02001a68(2, 0, 120);
        Call3(Func_02001a82, 0, 0x105, 0);
        Call3(Func_02001a8c, 1, 0x105, 60);
        Func_02001a66(0, 1, 0);
        Func_020019dc(60);
        Call3(Func_02001aa0, 0, 0x6000, 0);
        Call3(Func_02001aac, 1, 0x6000, 0);
        Func_020019fa(60);
        Call3(Func_02001ac6, 2, 0x106, 0);
        Func_02001a86(2, 1);
        Func_02001a14(30);
        Func_02001ace(2, 0, 30);
        Call3(Func_02001ae2, 2, 0xe000, 20);
        Func_02001aaa(0, 2);
        Func_02001aba(1, 2);
        Func_02001a40(20);
        Func_02001afa(2, 0, 20);
        Func_02001ab2(0, 3);
        Func_02001ac2(1, 3);
        Func_02001a60(20);
        Func_02001ad0(2, 3);
        Func_02001a6e(20);
        Call3(Func_02001b32, 2, 0x8000, 20);
        Func_020016c2();
        Func_02001ae6(2, 1);
        Func_02001a8c(20);
        Func_02001b90();
        Call3(Func_02001b5c, 0, 0x100, 0);
        Call3(Func_02001b68, 1, 0x100, 60);
        Func_02001b22(1, 2, 0);
        Func_02001ab8(20);
        Func_02000cb0(1, 20);
        Call3(Func_02001b0c, 2, 0x108, 184);
        Func_02001ad2(10);
        Func_02001b64(2, 1, 0);
        Func_02001b6e(1, 2, 0);
        Func_02001b78(0, 2, 0);
        Func_02001af6(20);
        Func_02001b66(2, 3);
        Func_02001b04(20);
        Func_02000cfc(2, 60);
        Call3(Func_02001bd6, 0, 0x105, 0);
        Call3(Func_02001be0, 1, 0x105, 60);
        Call3(Func_02001bea, 0, 0x101, 0);
        Call3(Func_02001bf4, 1, 0x101, 0);
        Func_02001b3a(60);
        Call3(Func_02001bfe, 1, 0x8000, 0);
        Func_02001c08(0, 0, 0);
        Func_02001b56(60);
        Call3(Func_02001c1a, 1, 0x4000, 0);
        Call3(Func_02001c26, 0, 0x2000, 0);
        Func_02001b74(10);
        Func_02000d6c(1, 20);
        Call3(Func_02001c46, 2, 0x105, 0);
        Func_02001b8c(60);
        Func_02001bfc(2, 4);
        Func_02001b9a(20);
        Func_02000d92(2, 20);
        Func_02001c34(0, 2, 0);
        Func_02001c3e(1, 2, 0);
        Call3(Func_02001c82, 0, 0x102, 0);
        Call3(Func_02001c8e, 1, 0x102, 0);
        Func_02001bd4(60);
        Func_02001c5c(2, 2);
        Func_02001be2(20);
        Func_02000dda(2, 30);
        Call3(Func_02001cb4, 0, 0x101, 0);
        Call3(Func_02001cbe, 1, 0x101, 0);
        Func_02001c04(80);
        Func_02001c74(2, 3);
        Func_02001c12(20);
        Func_02000e0a(2, 20);
        Func_02001c9a(0, 1);
        Func_02001ca2(1, 1);
        Call2(Func_02001cfc, 0, 0x102);
        Call2(Func_02001d06, 1, 0x102);
        Func_02001c44(60);
        Func_02001cb4(2, 4);
        Func_02001c52(20);
        Func_02000e4a(2, 20);
        Func_02001d1c(0, 0, 0);
        Call3(Func_02001d28, 1, 0x8000, 0);
        Func_02001c76(80);
        Func_02001d08(0, 2, 0);
        Func_02001d26(1, 2, 0);
        Func_02001ca4(30);
        Func_02001d14(2, 3);
        Func_02001cb2(20);
        Func_02000eaa(2, 30);
        Func_02001d2a(2, 4);
        Func_02001cc8(20);
        Func_02000ec0(2, 20);
        Func_02001d50(0, 2);
        Func_02001d60(1, 2);
        Func_02001ce6(20);
        Func_02001d56(2, 3);
        Func_02001cf4(20);
        Func_02000eec(2, 40);
        Func_02001d64(0, 3);
        Func_02001d74(1, 3);
        Func_02001d12(20);
        Func_02001d32(2, 1);
        Func_02001d20(60);
        Func_02001bc8();
        Func_02001dac(2, 1);
        Func_02001d32(20);
        Func_02001d7c(2, 248, 184);
        Func_02001d42(20);
        Func_02000f3a(2, 20);
        Call3(Func_02001e0e, 0, 0x8000, 0);
        Call3(Func_02001e1a, 1, 0x8000, 0);
        Func_02001d68(120);
        Func_02000f60(2, 30);
        Func_02001e02(0, 2, 0);
        Func_02001e0c(1, 2, 0);
        Func_02001e16(2, 0, 0);
        Func_02001d94(20);
        Func_02001dfc(0, 3);
        Func_02001e04(1, 3);
        Func_02001e14(2, 3);
        Func_02001db2(50);
        Call3(Func_02001de4, 1, 0xcccc, 0x6666);
        Call3(Func_02001dee, 2, 0xcccc, 0x6666);
        Func_02001e08(1, 248, 168);
        Func_02001e1a(2, 248, 168);
        Func_02001e3c(2, 0, 0);
        Func_02001e3a(1);
        Func_02001e4c(1, 0, 0);
        Call6(Func_02001da0, 74, 11, 1, 1, 73, 11);
        Call1(Func_02001df6, 0x865);
        Func_02001e22();
    }
}
