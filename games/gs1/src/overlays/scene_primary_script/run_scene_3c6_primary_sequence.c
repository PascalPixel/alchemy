#include "types.h"

#define FieldScene_RunPrimarySequence Func_02000218

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
void Func_0200174c();
void Func_02001782();
void Func_0200179e();
void Func_020017c0();
void Func_020017ca();
void Func_020017dc();
void Func_020017e4();
void Func_020017e8();
void Func_020017ea();
void Func_020017ee();
void Func_020017f8();
void Func_020017fc();
void Func_0200180a();
void Func_0200181c();
void Func_02001832();
void Func_02001840();
void Func_0200184c();
void Func_0200184e();
void Func_02001850();
void Func_0200185c();
void Func_02001864();
void Func_02001866();
void Func_0200186e();
void Func_02001872();
void Func_02001878();
void Func_0200187c();
void Func_0200187e();
void Func_02001882();
void Func_0200188a();
void Func_02001898();
void Func_020018a6();
void Func_020018b4();
void Func_020018c2();
void Func_020018c4();
s32 Func_020018cc();
void Func_020018ce();
void Func_020018d0();
void Func_020018e6();
void Func_020018e8();
void Func_020018f2();
void Func_020018f8();
void Func_0200190c();
void Func_02001918();
void Func_0200191a();
void Func_0200191e();
void Func_02001920();
void Func_02001932();
void Func_02001940();
void Func_02001942();
void Func_02001944();
void Func_0200194e();
void Func_0200195c();
void Func_0200195e();
void Func_0200196a();
void Func_02001974();
void Func_0200197a();
void Func_0200198c();
void Func_0200198e();
void Func_0200199c();
void Func_020019aa();
void Func_020019b6();
void Func_020019bc();
void Func_020019c2();
void Func_020019c6();
void Func_020019ca();
void Func_020019d8();
void Func_020019de();
void Func_020019e4();
void Func_020019f8();
void Func_02001a08();
void Func_02001a10();
void Func_02001a14();
void Func_02001a18();
void Func_02001a1e();
void Func_02001a24();
void Func_02001a2a();
void Func_02001a38();
void Func_02001a3c();
void Func_02001a3e();
void Func_02001a4a();
void Func_02001a54();
void Func_02001a58();
void Func_02001a66();
void Func_02001a6e();
void Func_02001a74();
void Func_02001a84();
void Func_02001a92();
void Func_02001aa0();
void Func_02001aae();
void Func_02001ab2();
void Func_02001abc();
void Func_02001aca();
void Func_02001ada();
void Func_02001adc();
void Func_02001ae4();
void Func_02001aee();
void Func_02001af2();
void Func_02001af4();
void Func_02001af6();
void Func_02001b00();
void Func_02001b0e();
void Func_02001b10();
void Func_02001b12();
void Func_02001b1c();
void Func_02001b20();
void Func_02001b2e();
void Func_02001b36();
void Func_02001b3a();
void Func_02001b3c();
void Func_02001b3e();
void Func_02001b58();
void Func_02001b6c();
void Func_02001b7a();
void Func_02001b82();
void Func_02001b8c();
void Func_02001b96();
void Func_02001b9c();
void Func_02001ba0();
void Func_02001bb6();
void Func_02001bb8();
void Func_02001bc4();
void Func_02001bd2();
void Func_02001be0();
void Func_02001be6();
void Func_02001bee();
void Func_02001c08();
void Func_02001c16();
void Func_02001c24();
void Func_02001c2a();
void Func_02001c34();
void Func_02001c46();
void Func_02001c4c();
void Func_02001c4e();
void Func_02001c58();
void Func_02001c5e();
void Func_02001c62();
void Func_02001c66();
void Func_02001c68();
void Func_02001c6c();
void Func_02001c7a();
void Func_02001c7c();
void Func_02001c88();
void Func_02001c92();
void Func_02001c96();
void Func_02001c98();
void Func_02001cb0();
void Func_02001cca();
void Func_02001cce();
void Func_02001cd0();
u8 *Func_02001cd6();
void Func_02001ce8();
void Func_02001cea();
void Func_02001cee();
void Func_02001cfa();
void Func_02001cfe();
void Func_02001d00();
void Func_02001d08();
void Func_02001d10();
void Func_02001d18();
void Func_02001d24();
void Func_02001d26();
void Func_02001d38();

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

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)0x03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunPrimarySequence(void)
{
    u32 i;
    u8 *record;

    Func_0200174c();
    Call1(Func_020017ea, 0x2694);
    Call3(Func_020017c0, 0, 0xf80000, 0x1a80000);
    Call3(Func_0200181c, 0, 0xc000, 0);
    Func_020017dc(8, 0);
    Func_020017e4(9, 0);
    Func_02001850();
    Func_0200185c();
    Func_02001782(20);
    Call4(Func_02001882, 22, 8, -16, 0xc000);
    Func_020017f8(22);
    Func_0200179e(20);
    Call3(Func_02001872, 22, 0x102, 60);
    Call3(Func_020017e8, 22, 0x10000, 0x8000);
    Call3(Func_020018c4, 22, 0, -16);
    Func_020017ca(10);
    Func_02001864(22, 8, 40);
    Func_0200186e(22, 9, 40);
    Func_02001878(22, 8, 40);
    Func_020017ee(10);
    Func_0200187e(22, 2);
    Func_020017fc(20);
    Func_020018b4(22, 0);
    Func_0200180a(10);
    Call3(Func_020018ce, 22, 0x4000, 0);
    Func_0200181c(30);
    Value2(Func_020018cc, 22, 0);
    Func_0200184c(0, 0);
    Func_02001832(30);
    Func_020018c2(8, 2);
    Func_02001840(20);
    Func_020018f8(8, 0);
    Func_0200184e(10);
    Func_020018e8(22, 8, 30);
    Func_020018e8(22, 2);
    Func_02001866(20);
    Func_0200191e(22, 0);
    Func_020018e6(8, 1);
    Func_0200187c(20);
    Func_0200190c(8, 2);
    Func_0200188a(30);
    Func_02001942(8, 0);
    Func_02001898(10);
    Func_02001918(22, 3);
    Func_020018a6(20);
    Func_0200195e(22, 0);
    Func_020018b4(10);
    Func_02001944(9, 2);
    Func_020018c2(20);
    Func_0200197a(9, 0);
    Func_020018d0(10);
    Func_0200196a(22, 9, 0);
    Func_02001974(0, 9, 30);
    Func_0200195c(9, 1);
    Func_020018f2(10);
    Call3(Func_020019c6, 22, 0x108, 40);
    Func_020019b6(22, 0);
    Func_0200190c(10);
    Func_0200198c(0, 3);
    Func_0200191a(20);
    Func_02001920(10);
    Call3(Func_020019e4, 22, 0x4000, 0);
    Func_02001932(30);
    Func_020019c2(22, 2);
    Func_02001940(20);
    Func_020019f8(22, 0);
    Func_0200194e(10);
    Func_020019de(9, 2);
    Func_0200195c(30);
    Func_02001a14(9, 0);
    Func_0200196a(10);
    Call3(Func_02001a3e, 22, 0x102, 50);
    Func_02001a10(22, 9, 20);
    Func_02001a38(22, 0);
    Func_0200198e(10);
    Func_02001a1e(8, 2);
    Func_0200199c(20);
    Func_02001a54(8, 0);
    Func_020019aa(10);
    Call3(Func_02001a6e, 22, 0x4000, 0);
    Func_020019bc(30);
    Func_02001a74(22, 0);
    Func_020019ca(10);
    Func_02001a4a(0, 3);
    Func_020019d8(20);
    Func_020019de(20);
    Call3(Func_02001ab2, 22, 0x102, 50);
    Call3(Func_02001a24, 22, 0x1cccc, 0xe666);
    Call3(Func_02001a4a, 22, 0x100, 0x168);
    Func_02001a08(10);
    Func_02001aca(22, 0, 0);
    Func_02001a18(30);
    Call3(Func_02001adc, 22, 0x8000, 0);
    Func_02001a2a(30);
    Call3(Func_02001aee, 22, 0xc000, 0);
    Func_02001a3c(30);
    Func_02001af4(22, 0);
    Func_02001a4a(10);
    Func_02001ada(9, 2);
    Func_02001a58(20);
    Func_02001b10(9, 0);
    Func_02001a66(10);
    Call3(Func_02001b3a, 22, 0x100, 40);
    Call3(Func_02001b36, 22, 0x2000, 0);
    Func_02001a84(20);
    Func_02001b3c(22, 0);
    Func_02001a92(10);
    Func_02001b12(9, 3);
    Func_02001aa0(30);
    Func_02001b58(9, 0);
    Func_02001aae(20);
    Func_02001b3e(22, 2);
    Func_02001abc(20);
    Call3(Func_02001af6, 22, 0x19999, 0xcccc);
    Call3(Func_02001b1c, 22, 0x100, 0x180);
    Func_02001b96(22, 0, 0);
    Func_02001ae4(20);
    Func_02001b9c(22, 0);
    Func_02001af2(10);
    Func_02001b82(8, 2);
    Func_02001b00(20);
    Func_02001bb8(8, 0);
    Func_02001b0e(10);
    Call3(Func_02001bd2, 22, 0x8000, 0);
    Func_02001b20(40);
    Func_02001ba0(22, 3);
    Func_02001b2e(20);
    Func_02001be6(22, 0);
    Func_02001b3c(10);
    Call3(Func_02001b8c, 22, 0x13333, 0x9999);
    Func_02001c66(22, 0, 16);
    Func_02001b6c(10);
    Func_02001c24(22, 0);
    Func_02001b7a(10);
    Call3(Func_02001c4e, 0, 0x102, 80);
    Call3(Func_02001c58, 22, 0x101, 80);
    Func_02001c2a(22, 8, 40);
    Func_02001c34(22, 9, 40);
    Call3(Func_02001c68, 22, 0x4000, 0);
    Func_02001bb6(30);
    Func_02001c46(22, 2);
    Func_02001bc4(20);
    Func_02001c7c(22, 0);
    Func_02001bd2(10);
    Func_02001c62(9, 2);
    Func_02001be0(20);
    Func_02001c98(9, 0);
    Func_02001bee(10);
    Func_02001cb0(22, 0, 0);
    Func_02001c92(0, 9, 30);
    Func_02001c08(10);
    Func_02001c98(8, 2);
    Func_02001c16(20);
    Func_02001cce(8, 0);
    Func_02001c24(10);
    Call3(Func_02001ce8, 22, 0x8000, 0);
    Func_02001cca(0, 8, 70);
    Call3(Func_02001cfe, 22, 0x4000, 0);
    Func_02001c4c(40);
    Call3(Func_02001d10, 0, 0xc000, 0);
    Func_02001c5e(40);
    Func_02001cee(22, 2);
    Func_02001c6c(20);
    Func_02001d24(22, 0);
    Func_02001c7a(10);
    Func_02001cfa(0, 3);
    Func_02001c88(20);
    Func_02001d08(22, 3);
    Func_02001c96(30);
    Call3(Func_02001cd0, 22, 0x13333, 0x9999);
    Func_02001d18(22, 2);
    record = Func_02001cd6(0);
    if (record != 0) {
        Func_02001d00(22, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Func_02001d26(22);
    Func_02001d38(22, 0, 0);
    Func_02001cd6(10);
    Func_02001cea();
}
