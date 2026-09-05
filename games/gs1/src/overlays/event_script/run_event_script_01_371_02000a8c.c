#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02004d36();
void Func_02004d6c();
void Func_02004d98();
void Func_02004db4();
void Func_02004dd2();
void Func_02004de8();
void Func_02004dfa();
void Func_02004e04();
void Func_02004e0a();
void Func_02004e12();
void Func_02004e26();
void Func_02004e2c();
s32 Func_02004e30();
s32 Func_02004e66();
void Func_02004e6a();
void Func_02004e72();
void Func_02004e7c();
void Func_02004e92();
s32 Func_02004e9c();
void Func_02004ea0();
void Func_02004ea8();
void Func_02004eb0();
void Func_02004eb2();
void Func_02004ebc();
void Func_02004ec8();
void Func_02004ed0();
void Func_02004ed6();
void Func_02004ede();
void Func_02004ee2();
void Func_02004ee4();
void Func_02004ee8();
void Func_02004ef8();
void Func_02004efe();
void Func_02004f0c();
void Func_02004f1e();
void Func_02004f2c();
void Func_02004f3a();
void Func_02004f3c();
void Func_02004f4a();
void Func_02004f4c();
void Func_02004f5a();
void Func_0200501c();
void Func_0200504e();

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

void RunEventScript01(void)
{
    u32 i;
    s32 record;

    Func_02004d6c();
    Func_02004ed0();
    Func_02004ee4();
    Func_02004ef8();
    Call1(Func_02004d36, 0x94f);
    Call3(Func_02004e0a, 11, 0x16e00000, 0x49c0000);
    Func_02004e04(11, 24, 8);
    Func_02004e12(11);
    Func_02004d98(60);
    Call3(Func_02004e2c, 12, 0x16e00000, 0x49c0000);
    Func_02004e26(12, 12, 24);
    Func_02004db4(30);
    Call3(Func_02004eb0, 11, 0x5000, 0);
    Call3(Func_02004ebc, 12, 0xd000, 0);
    Func_02004dd2(60);
    Func_02004e6a(11, 3);
    Func_02004e72(12, 3);
    Func_02004de8(120);
    Call3(Func_02004e7c, 8, 0x16f80000, 0x4b80000);
    Func_02004dfa(60);
    Func_02004e92(12, 2);
    record = Value1(Func_02004e30, 8);
    if (record != 0) {
        Func_02004e72(12, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Func_02004ea0(12);
    Func_02004eb2(12, 0, 0);
    ((void (*)())Func_02004e30)(60);
    Func_02004ec8(11, 2);
    record = Value1(Func_02004e66, 8);
    if (record != 0) {
        Func_02004ea8(11, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Func_02004ed6(11);
    Func_02004ee8(11, 0, 0);
    ((void (*)())Func_02004e66)(60);
    Func_02004efe(0, 2);
    record = Value1(Func_02004e9c, 8);
    if (record != 0) {
        Func_02004ede(0, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Func_02004f0c(0);
    Func_02004f1e(0, 0, 0);
    ((void (*)())Func_02004e9c)(60);
    Call3(Func_02004ee2, 8, 0x8000, 0x4000);
    Func_02004f2c(8, 56, 8);
    Func_02004f3a(8);
    Func_02004f3c(8, 40, 40);
    Func_02004f4a(8);
    Func_02004f4c(8, 8, 88);
    Func_02004f5a(8);
    Func_0200504e();
    Func_0200501c(108);
    Func_02004ef8();
}
