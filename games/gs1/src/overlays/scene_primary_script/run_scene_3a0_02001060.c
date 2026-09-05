#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02002424();
s32 Func_02002442();
s32 Func_0200244c();
s32 Func_02002458();
s32 Func_02002460();
s32 Func_02002468();
s32 Func_02002470();
s32 Func_02002478();
void Func_0200247a();
void Func_020024a8();
void Func_020024b6();
void Func_020024ea();
s32 Func_020024f2();
void Func_020024f4();
void Func_020024fc();
void Func_020024fe();
void Func_0200251c();
void Func_02002544();
void Func_02002588();
void Func_020025a4();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call0(void (*f)())
{
    f();
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

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunScene3a0_02001060(void)
{
    u32 i;
    s32 record;

    Func_02002424();
    Call2((void (*)())Func_0200244c, 18, 1);
    record = Func_02002442(18);
    *(volatile s32 *)(record + 108) = 0;
    record = Func_0200244c(18);
    *(volatile s32 *)(record + 56) = -0x80000000;
    record = Value1(Func_02002458, 18);
    *(volatile s32 *)(record + 64) = -0x80000000;
    record = Value1(Func_02002460, 18);
    *(volatile s32 *)(record + 36) = 0;
    record = Value1(Func_02002468, 18);
    *(volatile s32 *)(record + 44) = 0;
    record = Value1(Func_02002470, 18);
    *(volatile s32 *)(record + 48) = 0;
    record = Func_02002478(18);
    *(volatile s32 *)(record + 52) = 0;
    Call3(Func_02002544, 18, 0x103, 0);
    Func_020024f4(18, 2);
    Func_0200247a(60);
    Call3(Func_020024a8, 18, 0x18000, 0xc000);
    Call3(Func_020024b6, 0, 0x18000, 0xc000);
    Call3(Func_020024ea, 18, 0x118, 232);
    Call3(Func_020024fe, 0, 0x128, 232);
    Func_0200251c(18);
    Call3(Func_02002588, 0, 0x8000, 20);
    Call3(Func_020025a4, 0, 0x102, 60);
    Call2(Func_020024fc, 18, 0x20095b0);
    record = Func_020024f2(18);
    *(volatile s32 *)(record + 108) = 0x2008501;
    Call0((void (*)())Func_020024f2);
}
