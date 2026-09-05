#include "types.h"

#define FieldScene_RunActorAvailabilityPresentation Func_020038ac

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
void Func_02008188();
void Func_0200818e();
void Func_0200819e();
s32 Func_020081ae();
void Func_020081b8();
void Func_020081c6();
void Func_02008204();
void Func_02008212();
void Func_02008220();
void Func_0200823c();
void Func_020082fa();
void Func_02008312();
void Func_02008332();
void Func_02008348();
void Func_02008352();
void Func_0200835c();
void Func_02008366();
void Func_0200839c();
void Func_020083b6();
void Func_020083c4();
void Func_020083f6();
void Func_02008400();
s32 Func_0200840a();
void Func_02008438();
void Func_02008446();
s32 Func_02008456();
void Func_02008478();
s32 Func_0200847e();
void Func_02008498();
s32 Func_0200849e();
void Func_020084ae();
s32 Func_020084ca();
void Func_020084cc();
void Func_020084fe();
void Func_02008506();
void Func_0200858c();
void Func_02008596();
void Func_020085dc();
void Func_02009d84();
void Func_02009e4c();
void Func_02009e76();
void Func_02009e90();
void Func_02009e9c();
void Func_02009ea8();
s32 Func_02009eaa();
void Func_02009ed2();
s32 Func_02009ede();
void Func_02009f06();
void Func_02009f0c();
void Func_02009f0e();
s32 Func_02009f14();
void Func_02009f18();
void Func_02009f20();
void Func_02009f3a();
void Func_02009f3c();
void Func_02009f4a();
void Func_02009f4c();
void Func_02009f54();
void Func_02009f7a();
void Func_02009f80();
void Func_02009f82();
void Func_02009fc0();
void Func_02009ff6();
void Func_0200a00e();
void Func_0200a020();
void Func_0200a026();
void Func_0200a02e();
void Func_0200a034();
void Func_0200a03c();
void Func_0200a044();
void Func_0200a04c();
void Func_0200a04e();
void Func_0200a058();
void Func_0200a064();
void Func_0200a074();
void Func_0200a088();
void Func_0200a08a();
void Func_0200a0a0();
void Func_0200a0b2();
void Func_0200a0bc();
void Func_0200a0e4();
void Func_0200a0f6();
void Func_0200a10c();
void Func_0200a10e();
void Func_0200a124();
void Func_0200a132();
void Func_0200a13c();
void Func_0200a15e();
void Func_0200a160();
void Func_0200a162();
void Func_0200a166();
void Func_0200a16c();
void Func_0200a18a();
void Func_0200a194();
void Func_0200a19e();
void Func_0200a1a2();
s32 Func_0200a1a6();
void Func_0200a1aa();
void Func_0200a1b2();
void Func_0200a1b4();
void Func_0200a1c8();
void Func_0200a1da();
void Func_0200a1de();
void Func_0200a1ea();
void Func_0200a1ec();
void Func_0200a214();
void Func_0200a25c();

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

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ s32 Value4(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    return f(a0, a1, a2, a3);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)0x03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunActorAvailabilityPresentation(void)
{
    s32 c_1b8;
    s32 c_5000;
    s32 c_d000;
    s32 c_b000;
    s32 c_8000;
    s32 c_100;
    s32 record;

    Func_02009d84();
    Call3((void (*)())Func_020081ae, 25, 0, 0);
    Func_020081b8(24, 1, 0);
    Call4(Func_0200818e, 0x1b80000, -1, 0xa80000, 0x1000001);
    c_1b8 = 0x1b8;
    c_5000 = 0x5000;
    Func_02008188(27, c_1b8, 164, c_5000);
    c_d000 = 0xd000;
    Func_0200819e(8, 0x1ac, 190, c_d000);
    c_b000 = 0xb000;
    Value4(Func_020081ae, 9, 0x1c4, 190, c_b000);
    Func_02009e76(9, 1);
    c_8000 = 0x8000;
    Func_020081c6(0, c_1b8, 134, c_8000);
    c_100 = 0x100;
    *(s32 *)((*(u8 **)0x03001ebc + 0x1c0)) = c_100;
    Func_02009f7a();
    Call3(Func_02009e4c, 0, 0xcccc, 0x6666);
    Call3(Func_02009e90, 0, 0x198, 134);
    Call3(Func_02009e9c, 0, 0x198, 148);
    Call3(Func_02009ea8, 0, 0x1a8, 148);
    Call3(Func_02009f54, 0, 0x4000, 20);
    Func_02009f0c(27, 1);
    Call1(Func_02009f3a, 0x1e27);
    Func_02008204(27);
    Func_02009f20(8, 1);
    Func_02008212(8);
    Func_02009f0e(27, 3);
    Func_02008220(27);
    Func_0200823c(27, c_d000);
    record = Value1(Func_02009eaa, 0);
    if (record != 0) {
        Func_02009f18(1, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Call3(Func_02009ed2, 1, 0xcccc, 0x6666);
    Call3((void (*)())Func_02009f14, 1, c_1b8, 148);
    Call3(Func_02009fc0, 1, 0x4000, 0);
    record = Value1(Func_02009ede, 1);
    if (record != 0) {
        Func_02009f4c(2, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Call3(Func_02009f06, 2, 0xcccc, 0x6666);
    Call3(Func_02009f4a, 2, 0x1c8, 148);
    Call3(Func_02009ff6, 2, 0x4000, 0);
    record = Value1(Func_02009f14, 2);
    if (record != 0) {
        Func_02009f82(3, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Call3(Func_02009f3c, 3, 0xcccc, 0x6666);
    Call3(Func_02009f80, 3, 0x1d8, 148);
    Call3(Func_0200a02e, 3, 0x4000, 20);
    Func_02008352(0, 0, 60);
    Func_0200835c(1, 0x4000, 20);
    Func_02008366(2, 1, 20);
    Func_0200a058(27, c_5000, 20);
    Func_020082fa(27);
    Func_0200a00e(9, 1);
    Func_0200a088(9, c_100, 40);
    Func_02008312(9);
    Func_0200a026(1, 3);
    Call3(Func_0200a0a0, 1, 0x103, 60);
    Func_0200a020(27, 3);
    Func_02008332(27);
    Func_0200a04e(10, 1);
    Func_0200a026(10, 3);
    Func_02008348(10);
    Func_0200a034(8, 3);
    Func_0200a03c(9, 3);
    Func_0200a044(11, 3);
    Func_0200a04c(12, 3);
    Func_0200a064(13, 3);
    Func_020083f6(0, 0, 40);
    Func_02008400(2, 1, 0);
    Value3(Func_0200840a, 1, 0x4000, 20);
    Func_0200a08a(27, 4);
    Func_0200839c(27);
    Call3(Func_0200a124, 8, 0x102, 60);
    Func_0200a0bc(8, 1);
    Func_020083b6(8);
    Func_0200a0b2(27, 3);
    Func_020083c4(27);
    Func_0200a132(8, 0, 0);
    Func_0200a13c(9, c_8000, 40);
    Call3(Func_0200a160, 8, 0x102, 0);
    Call3(Func_0200a16c, 8, 0x102, 40);
    Func_0200a10c(27, 1);
    Func_0200a0e4(27, 3);
    Func_0200a15e(27, 0, 20);
    Func_0200a0f6(8, 3);
    Func_0200a10e(9, 3);
    Func_0200a074(40);
    Value3(Func_0200a1a6, 9, c_100, 20);
    Func_02008446(9, c_b000);
    Func_02008438(9);
    Value2(Func_02008456, 27, 0x3000);
    Call3(Func_0200a1c8, 27, 0x101, 60);
    Func_0200a1aa(27, 0, 60);
    Call3(Func_0200a1de, 27, 0x106, 20);
    Value2(Func_0200847e, 27, c_b000);
    Func_0200a166(27, 3);
    Func_02008478(27);
    Func_020084fe(3, 2, 80);
    Value2(Func_0200849e, 8, c_d000);
    Func_0200a19e(8, 2);
    Func_02008498(8);
    Func_0200a194(9, 3);
    Func_0200a1b4(9, 2);
    Func_020084ae(9);
    Value2(Func_020084ca, 27, c_5000);
    Func_0200a1b2(27, 3);
    Func_0200a1da(27, 1);
    Func_020084cc(27);
    Call3(Func_0200a162, 27, 0xcccc, 0x6666);
    Call3(Func_0200a1a6, 27, 0x198, 158);
    Call3(Func_0200a1b2, 27, 0x198, 148);
    Func_0200a25c(27, 0, 20);
    Func_0200a214(27, 1);
    Func_02008506(27);
    Func_0200858c(1, c_8000, 20);
    Func_02008596(2, 1, 0);
    Call3(Func_0200a1ea, 27, 0x198, 134);
    Func_0200a1ec(27, c_1b8, 134);
    Func_0200a18a(40);
    Func_020085dc(9, 10, 0);
    Call1(Func_0200a1a2, 0x926);
}
