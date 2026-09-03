#include "types.h"

#define FieldScene_RunPositionTransferPresentation Func_020050e4

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_00001e46[];
extern u8 Data_0200e7c8[];
void Func_020099a6();
void Func_02009ae0();
void Func_02009ae8();
void Func_02009b00();
void Func_02009b1a();
s32 Func_02009b26();
void Func_02009b3a();
void Func_02009ba6();
void Func_0200b5b6();
void Func_0200b60c();
s32 Func_0200b642();
void Func_0200b648();
void Func_0200b64e();
void Func_0200b654();
s32 Func_0200b656();
void Func_0200b660();
s32 Func_0200b66a();
void Func_0200b66c();
void Func_0200b698();
void Func_0200b6ae();
void Func_0200b6b0();
void Func_0200b6b2();
void Func_0200b6c4();
void Func_0200b6c8();
void Func_0200b6d4();
void Func_0200b6d8();
void Func_0200b6e8();
void Func_0200b6ea();
void Func_0200b70c();
void Func_0200b724();
void Func_0200b734();
void Func_0200b73a();
void Func_0200b73c();
void Func_0200b75c();
void Func_0200b76e();
void Func_0200b774();
void Func_0200b798();
s32 Func_0200b7a0();
void Func_0200b7b8();
void Func_0200b7c8();
void Func_0200b7d4();
void Func_0200b7dc();
void Func_0200b7e0();
void Func_0200b7ee();
void Func_0200b816();
void Func_0200b83e();
void Func_0200b84e();

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

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)0x03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunPositionTransferPresentation(void)
{
    u32 i;
    s32 record;
    s32 base5_1e46;
    s32 base5_200e7c8;

    Func_020099a6(0x1b80000, -1, 0xb00000, 0x1000001);
    Call3(Func_0200b64e, 0, 0x1b80000, 0x860000);
    Func_0200b73a();
    Call3(Func_0200b60c, 0, 0x19999, 0xcccc);
    Func_0200b66c(0, 5);
    Call3(Func_0200b648, 0, 0x198, 134);
    Call3(Func_0200b654, 0, 0x198, 152);
    Call3(Func_0200b660, 0, 0x1b0, 166);
    Func_0200b698(0, 1);
    Call3(Func_0200b724, 0, 0x4000, 0);
    record = Value1(Func_0200b642, 0);
    if (record != 0) {
        Func_0200b6b0(2, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    record = Value1(Func_0200b656, 0);
    if (record != 0) {
        Func_0200b6c4(1, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    record = Value1(Func_0200b66a, 1);
    if (record != 0) {
        Func_0200b6d8(3, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Func_0200b5b6(1);
    Call3(Func_0200b698, 2, 0x19999, 0xcccc);
    Call3(Func_0200b6d4, 2, 0x1a8, 152);
    Call3(Func_0200b6ae, 1, 0x19999, 0xcccc);
    Call3(Func_0200b6ea, 1, 0x1c0, 168);
    Call3(Func_0200b6c8, 3, 0x20000, 0x10000);
    Call3(Func_0200b70c, 3, 0x1ca, 152);
    Func_0200b734(1, 1);
    Func_0200b73c(2, 1);
    Call3(Func_0200b7c8, 2, 0x4000, 0);
    Call3(Func_0200b7d4, 1, 0x4000, 0);
    Call3(Func_0200b7e0, 3, 0x4000, 40);
    Func_0200b7ee(10);
    base5_1e46 = (s32)Data_00001e46;
    Func_0200b6b2(base5_1e46, 1, 10);
    Func_0200b6e8(10);
    Func_02009b1a(0, 0, 40);
    Value3(Func_02009b26, 1, 0x4000, 20);
    Call2(Func_0200b83e, 0x39999, 0x7333);
    Call4(Func_02009b00, 0x1b80000, -1, 0x1400000, 0x10000014);
    Func_0200b7dc(8, 2);
    Call2(Func_02009ae8, 8, 0xd000);
    Func_0200b816((base5_1e46 + 1));
    Func_02009ae0(8);
    Func_0200b84e(8, 0, 20);
    Call4(Func_02009b3a, 0x1b80000, -1, 0x860000, 0x10000000);
    base5_200e7c8 = (s32)Data_0200e7c8;
    Func_0200b798(1, base5_200e7c8);
    Value2(Func_0200b7a0, 2, base5_200e7c8);
    Func_0200b7b8(3, base5_200e7c8);
    Func_0200b76e(40);
    Call1(Func_0200b75c, 0x301);
    Func_02009ba6(23, 0, 0);
    Call1(Func_0200b774, 0x12f);
}
