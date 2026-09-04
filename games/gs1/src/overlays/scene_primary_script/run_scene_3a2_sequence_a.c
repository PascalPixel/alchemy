#include "types.h"

#define FieldScene_RunScene3a2SequenceA Func_020001ec

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_020016e0();
void Func_0200174e();
void Func_02001768();
void Func_02001776();
void Func_02001784();
void Func_02001792();
void Func_020017b2();
void Func_020017ba();
void Func_020017c8();
void Func_020017e0();
void Func_020017f6();
void Func_020017f8();
void Func_02001802();
void Func_0200180c();
void Func_0200180e();
void Func_02001814();
void Func_0200181a();
void Func_0200181c();
void Func_02001822();
void Func_0200182c();
void Func_02001832();
void Func_0200183a();
void Func_02001840();
void Func_02001844();
void Func_02001852();
void Func_02001866();
void Func_0200186c();
s32 Func_02001874();
void Func_02001878();
void Func_02001882();
void Func_0200189a();
void Func_020018b2();
void Func_020018b4();
void Func_020018ca();
void Func_020018d0();
void Func_020018d6();
s32 Func_020018f4();
void Func_020018fa();
void Func_020018fc();
void Func_0200190c();
void Func_02001912();
void Func_0200192c();
void Func_02001930();
void Func_02001934();
void Func_0200194a();
void Func_02001950();
void Func_02001958();
void Func_0200195c();
void Func_0200197a();
void Func_02001980();
void Func_02001984();
void Func_02001986();
void Func_0200198e();
void Func_020019b2();
void Func_020019c0();
void Func_020019c4();
void Func_020019cc();
void Func_020019ce();
void Func_020019e2();
void Func_020019e6();
void Func_020019f4();
void Func_020019f8();
void Func_020019fa();
void Func_02001a00();
void Func_02001a16();
void Func_02001a24();
void Func_02001a2e();
void Func_02001a30();
void Func_02001a32();
void Func_02001a3c();
void Func_02001a40();
void Func_02001a48();
void Func_02001a54();
void Func_02001a5a();
void Func_02001a68();
void Func_02001a70();
void Func_02001a72();
void Func_02001a74();
void Func_02001a78();
void Func_02001a86();
void Func_02001a88();
void Func_02001a96();
void Func_02001aa0();
void Func_02001aa4();
void Func_02001ab6();
void Func_02001ac6();
void Func_02001aee();
void Func_02001af0();
void Func_02001afc();
void Func_02001b10();
void Func_02001b30();
void Func_02001b3e();
void Func_02001b40();
void Func_02001b4c();
void Func_02001b52();
void Func_02001b56();
void Func_02001b64();
void Func_02001b66();
void Func_02001b68();
void Func_02001b7a();
void Func_02001b7c();
void Func_02001b82();
void Func_02001b86();
void Func_02001b88();
void Func_02001b94();
void Func_02001b98();
void Func_02001ba0();
void Func_02001ba2();
void Func_02001baa();
void Func_02001bb0();
void Func_02001bba();
void Func_02001bc2();
void Func_02001bc8();
void Func_02001be0();
void Func_02001bec();
void Func_02001bf8();
void Func_02001bfa();
void Func_02001c08();
void Func_02001c10();
void Func_02001c2c();
void Func_02001c30();
void Func_02001c34();
void Func_02001c42();
void Func_02001c58();
void Func_02001c66();
void Func_02001c6e();
void Func_02001c74();
void Func_02001c7a();
void Func_02001c7c();
void Func_02001c86();
void Func_02001c98();
void Func_02001c9e();
void Func_02001ca2();
void Func_02001ca8();
void Func_02001caa();
void Func_02001cae();
void Func_02001cb4();
void Func_02001cba();
void Func_02001cc2();
void Func_02001ce4();
void Func_02001cf2();
void Func_02001cf8();
void Func_02001d08();
void Func_02001d10();
void Func_02001d14();
void Func_02001d1c();
void Func_02001d20();
void Func_02001d26();
void Func_02001d2c();
void Func_02001d30();
void Func_02001d38();
void Func_02001d40();
void Func_02001d4e();
void Func_02001d5e();
void Func_02001d60();
void Func_02001d6e();
void Func_02001d70();
void Func_02001d80();

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

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunScene3a2SequenceA(void)
{
    u32 i;
    s32 record;

    Func_020016e0();
    Call3(Func_0200174e, 8, 0x880000, 0xa80000);
    Call3(Func_020017b2, 8, 0x5000, 0);
    Call3(Func_02001768, 0, 0x900000, 0xc80000);
    Call3(Func_02001776, 1, 0xa00000, 0xc00000);
    Call3(Func_02001784, 2, 0x800000, 0xc80000);
    Call3(Func_02001792, 3, 0x700000, 0xc00000);
    Call3(Func_020017f6, 0, 0xc000, 0);
    Call3(Func_02001802, 1, 0xa000, 0);
    Call3(Func_0200180e, 2, 0xc000, 0);
    Call3(Func_0200181a, 3, 0xe000, 0);
    *(s32 *)((*(u8 *volatile *)Data_03001ebc + 0x1c0)) = 0x201;
    Func_0200186c();
    Func_02001878();
    Func_02001776(60);
    Func_020017f6(8, 3);
    Func_02001784(20);
    Call1(Func_02001822, 0x19e9);
    Func_02001844(8, 0, 20);
    Func_0200180c(0, 3);
    Func_02001814(1, 3);
    Func_0200181c(2, 3);
    Func_0200182c(3, 3);
    Func_020017ba(20);
    Func_0200183a(8, 4);
    Func_020017c8(20);
    Func_02001882(8, 0, 20);
    Func_02001852(3, 4);
    Func_020017e0(20);
    Func_0200189a(3, 0, 20);
    Func_02001882(2, 2);
    Func_020017f8(20);
    Func_020018b2(2, 0, 20);
    Func_0200189a(8, 2);
    Call3(Func_020018d6, 8, 0x5000, 20);
    Func_020018d0(8, 0, 20);
    Call3(Func_020018fc, 2, 0x102, 60);
    Func_02001832(120);
    Func_020018ca(1, 2);
    Func_02001840(20);
    Call3(Func_0200190c, 1, 0x6000, 20);
    Value2(Func_020018f4, 1, 0);
    if (Value2(Func_02001874, 0, 0) == 0) {
        Func_02001866(20);
        Func_02001930(3, 0, 20);
        Call3(Func_0200194a, 3, 0x101, 60);
        Func_02001934(3, 0, 20);
        Call3(Func_02001950, 0, 0xe000, 0);
        Call3(Func_0200195c, 2, 0xe000, 0);
        Func_0200192c(1, 2);
        Call2(Func_02001986, 1, 0x102);
        Func_020018b4(60);
        Call3(Func_02001980, 1, 0x8000, 20);
        Func_0200197a(1, 0, 20);
        bump_step(2);
    } else {
        bump_step(2);
        Func_020018fa(20);
        Func_020019c4(3, 0, 20);
        Func_02001984(3, 3);
        Func_02001912(20);
        Func_020019cc(3, 0, 20);
        Call3(Func_020019f8, 1, 0x102, 60);
        Call3(Func_020019f4, 0, 0xe000, 0);
        Call3(Func_02001a00, 2, 0xe000, 0);
        Func_020019fa(1, 0, 20);
    }
    Func_020019e2(8, 2);
    Func_02001958(20);
    Call3(Func_02001a24, 0, 0xc000, 0);
    Call3(Func_02001a30, 1, 0xa000, 0);
    Call3(Func_02001a3c, 2, 0xc000, 0);
    Call3(Func_02001a48, 3, 0xe000, 0);
    Func_0200198e(20);
    Call3(Func_02001a5a, 8, 0x3000, 20);
    Func_02001a54(8, 0, 20);
    Func_02001a3c(1, 2);
    Func_020019b2(20);
    Func_02001a32(1, 3);
    Func_020019c0(30);
    Func_02001a40(8, 3);
    Func_020019ce(20);
    Func_02001a88(8, 0, 20);
    Func_02001a70(8, 2);
    Func_020019e6(20);
    Func_02001aa0(8, 0, 20);
    Func_02001a68(0, 3);
    Func_02001a70(1, 3);
    Func_02001a78(2, 3);
    Func_02001a88(3, 3);
    Func_02001a16(20);
    Func_02001a96(8, 3);
    Func_02001a24(20);
    Call3(Func_02001af0, 8, 0xc000, 30);
    Func_02001b66(188);
    Call3(Func_02001a00, 0x2009788, 67, 6);
    Call3(Func_02001a72, 8, 0xcccc, 0x6666);
    Func_02001aa4(8, 136, 136);
    Func_02001ac6(8, 0, 0);
    Func_02001b94(188);
    Call3(Func_02001a2e, 0x200979e, 67, 6);
    Func_02001a74(60);
    Func_02001b98();
    Func_02001b10(1, 2);
    Func_02001a86(20);
    Call3(Func_02001b52, 1, 0x6000, 20);
    Func_02001b4c(1, 0, 20);
    Call3(Func_02001b68, 0, 0x8000, 20);
    Func_02001b40(2, 2);
    Func_02001ab6(20);
    Call3(Func_02001b82, 3, 0x2000, 20);
    Func_02001b7c(3, 0, 20);
    Func_02001b64(1, 1);
    Call3(Func_02001ba0, 0, 0xe000, 20);
    Func_02001ba0(1, 0);
    Func_02001aee(20);
    Func_02001b86(2, 2);
    Func_02001afc(20);
    Call3(Func_02001bc8, 0, 0x8000, 20);
    Func_02001bc2(2, 0, 20);
    Func_02001ba2(0, 1);
    Func_02001baa(1, 1);
    Func_02001bba(3, 1);
    Func_02001b30(20);
    Func_02001bb0(2, 4);
    Func_02001b3e(20);
    Func_02001bf8(2, 0, 20);
    Func_02001be0(3, 2);
    Func_02001b56(20);
    Func_02001c10(3, 0, 20);
    Call3(Func_02001c2c, 2, 0xb000, 20);
    Func_02001bec(2, 3);
    Func_02001b7a(20);
    Func_02001bfa(2, 3);
    Func_02001b88(20);
    Func_02001c42(2, 0, 20);
    Call3(Func_02001c6e, 1, 0x102, 60);
    Func_02001c58(1, 0, 20);
    Call3(Func_02001c74, 2, 0xe000, 20);
    Func_02001c34(2, 4);
    Func_02001bc2(20);
    Func_02001c7c(2, 0, 20);
    Call3(Func_02001ca8, 0, 0x102, 0);
    Call3(Func_02001cb4, 3, 0x102, 60);
    Func_02001c9e(3, 0, 20);
    Call3(Func_02001cba, 2, 0xa000, 20);
    Func_02001c7a(2, 3);
    Func_02001c08(20);
    Func_02001cc2(2, 0, 20);
    Func_02001ca2(0, 1);
    Func_02001caa(1, 1);
    Func_02001cba(3, 1);
    Func_02001c30(20);
    Call3(Func_02001c66, 2, 0x8000, 0x4000);
    Func_02001c98(2, 128, 184);
    Call3(Func_02001d14, 2, 0x4000, 20);
    Call3(Func_02001d20, 0, 0xc000, 0);
    Call3(Func_02001d2c, 1, 0xa000, 0);
    Call3(Func_02001d38, 3, 0xe000, 20);
    Func_02001cf8(2, 3);
    Func_02001c86(20);
    Func_02001d40(2, 0, 20);
    Func_02001d08(0, 3);
    Func_02001d10(1, 3);
    Func_02001d20(3, 3);
    Func_02001cae(20);
    Call3(Func_02001ce4, 1, 0x8000, 0x4000);
    Call3(Func_02001cf2, 3, 0x8000, 0x4000);
    Func_02001d1c(1, 144, 200);
    Func_02001d26(2, 144, 200);
    Func_02001d30(3, 144, 200);
    Func_02001d4e(1);
    Func_02001d60(1, 0, 0);
    Func_02001d5e(2);
    Func_02001d70(2, 0, 0);
    Func_02001d6e(3);
    Func_02001d80(3, 0, 0);
    Func_02001d2c();
}
