#include "types.h"

#define FieldScene_RunScene3c3SequenceA Func_020003c4

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02000c62();
void Func_02000c66();
void Func_02000c90();
s32 Func_02000ca6();
void Func_02000cb4();
void Func_02000ccc();
void Func_02000cec();
void Func_02000cee();
void Func_02000d30();
void Func_02000d6a();
void Func_02000d74();
void Func_02000d7e();
void Func_02000d82();
void Func_02000d88();
void Func_02000d8c();
void Func_02000d96();
void Func_02000d98();
void Func_02000da2();
void Func_02000daa();
void Func_02000dc4();
void Func_02000dc8();
void Func_02000dce();
void Func_02000ddc();
void Func_02000df0();
s32 Func_02000df6();
void Func_02000dfc();
void Func_02000dfe();
void Func_02000e14();
void Func_02000e16();
void Func_02000e28();
void Func_02000e2e();
void Func_02000e44();
void Func_02000e46();
void Func_02000e4c();
void Func_02000e58();
void Func_02000e62();
void Func_02000e76();
void Func_02000e92();
void Func_02000ea0();
void Func_02000eae();
void Func_02000ec4();
void Func_02000ece();
void Func_02000ed8();
void Func_02000edc();
void Func_02000ee8();
void Func_02000ef6();
void Func_02000f00();
void Func_02000f06();
void Func_02000f12();
void Func_02000f20();
void Func_02000f22();
void Func_02000f24();
void Func_02000f28();
void Func_02000f36();
void Func_02000f4a();
void Func_02000f4c();
void Func_02000f4e();
void Func_02000f5c();
void Func_02000f68();
s32 Func_02000f6e();
void Func_02000f76();
void Func_02000f80();
void Func_02000f8c();
void Func_02000f90();
void Func_02000f92();
void Func_02000f98();
void Func_02000fa0();
void Func_02000fa8();
void Func_02000fb0();
void Func_02000fb2();
void Func_02000fb6();
void Func_02000fc8();
void Func_02000fca();
void Func_02000fe0();
void Func_02000fe6();

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

void FieldScene_RunScene3c3SequenceA(void)
{
    u32 i;
    s32 record;
    s32 v5;

    Func_02000c66();
    Call3(Func_02000c90, 0, 0x19999, 0xcccc);
    Call3(Func_02000cb4, 0, 120, 0x1b6);
    Call3(Func_02000d30, 0, 0xc000, 0);
    record = Value1(Func_02000ca6, 0);
    if (record != 0) {
        Func_02000cec(11, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
    }
    Func_02000c62(1);
    Call3(Func_02000ccc, 11, 0x19999, 0xcccc);
    Call3(Func_02000cee, 11, 108, 0x1af);
    Call3(Func_02000d6a, 11, 0xd000, 10);
    Call3(Func_02000d7e, 11, 0x100, 20);
    Call3(Func_02000d82, 11, 0xd000, 20);
    Func_02000d8c(11, 0, 40);
    Call3(Func_02000d98, 11, 0xd000, 40);
    Func_02000da2(11, 0, 20);
    Func_02000d6a(11, 2);
    Call1(Func_02000d88, 0x2654);
    Func_02000daa(11, 0, 40);
    Call3(Func_02000dce, 8, 0x100, 0);
    Func_02000d96(8, 2);
    Func_02000dc8(8, 0, 10);
    Call3(Func_02000d74, 11, 132, 0x1a4);
    Call3(Func_02000df0, 11, 0xd000, 0);
    Call3(Func_02000dfc, 0, 0xe000, 0);
    Call3(Func_02000d98, 11, 138, 0x1a0);
    Call3(Func_02000e14, 11, 0xb000, 10);
    Func_02000ddc(11, 2);
    Func_02000e16(11, 0, 40);
    ((void (*)())Func_02000df6)(8, 2);
    Func_02000e28(8, 0, 40);
    Call3(Func_02000e4c, 9, 0x100, 20);
    Func_02000e14(9, 2);
    Func_02000e46(9, 0, 20);
    Call3(Func_02000e62, 0, 0xc000, 0);
    Call3(Func_02000dfe, 11, 144, 0x1a4);
    Func_02000dc4(20);
    Func_02000e44(9, 2);
    Func_02000e76(9, 0, 20);
    Call2(Func_02000ea0, 9, 0x102);
    Func_02000e58(9, 3);
    Func_02000e92(9, 0, 20);
    Call3(Func_02000eae, 11, 0x5000, 20);
    Func_02000eae(11, 0);
    if (Value1(Func_02000df6, 0x9b0) != 0) {
        Call3(Func_02000ece, 11, 0xd000, 40);
        Call2(Func_02000ee8, 11, 0x102);
        Func_02000e2e(40);
        Func_02000ed8(11, 0, 10);
    } else {
        bump_step(1);
    }
    Call3(Func_02000f06, 11, 0x5000, 10);
    Func_02000f00(11, 0, 40);
    Call3(Func_02000f24, 11, 0x100, 40);
    Call3(Func_02000f28, 11, 0xb000, 10);
    Func_02000f22(11, 0, 10);
    Call3(Func_02000ece, 11, 138, 0x1a0);
    Call3(Func_02000f4a, 11, 0xb000, 20);
    Func_02000f12(8, 2);
    Func_02000f4c(8, 0, 10);
    Call2(Func_02000f76, 11, 0x102);
    Func_02000f36(11, 1);
    Func_02000ec4(20);
    ((void (*)())Func_02000f6e)(11, 0, 20);
    Func_02000f4e(9, 2);
    Func_02000edc(20);
    Call3(Func_02000f98, 0, 0xe000, 10);
    Func_02000f68(9, 1);
    Func_02000ef6(20);
    Func_02000fa0(9, 0, 10);
    Func_02000f80(8, 2);
    Func_02000fb2(8, 0, 20);
    Func_02000f92(11, 1);
    Func_02000f20(20);
    Func_02000fca(11, 0, 20);
    Call3(Func_02000fe6, 11, 0x5000, 10);
    Func_02000fe0(11, 0, 10);
    Func_02000fa8(11, 2);
    record = Value1(Func_02000f6e, 0);
    if (record != 0) {
        Func_02000f90(11, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Func_02000fb6(11);
    v5 = 7;
    Func_02000fc8(11, 0, 0);
    Call6(Func_02000f5c, 6, 27, 1, 1, v5, 27);
    Call6(Func_02000f6e, 9, 26, 2, 1, v5, 26);
    Call1(Func_02000f8c, 0x89f);
    Func_02000fb0();
}
