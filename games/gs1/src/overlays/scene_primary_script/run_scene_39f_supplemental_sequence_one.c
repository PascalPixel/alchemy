#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_02000240[];
extern u8 Data_03001ebc[];
s32 Func_02001eb0();
void Func_02001ec2();
void Func_02001ed2();
s32 Func_02001ee2();
s32 Func_02001ef4();
void Func_02001f30();
void Func_02002834();
void Func_02002abc();
void Func_02002b32();
void Func_02002bc4();
void Func_020049e4();
s32 Func_02004a02();
s32 Func_02004a0a();
void Func_02004a6c();
s32 Func_02004a72();
s32 Func_02004a80();
void Func_02004a9c();
void Func_02004ab8();
s32 Func_02004ac8();
void Func_02004ace();
void Func_02004b00();
void Func_02004b1e();
void Func_02004b42();
u8 *Func_02004b44();
u8 *Func_02004b4e();
void Func_02004b70();
void Func_02004b7a();
void Func_02004ba8();
void Func_02004bba();
void Func_02004c30();
void Func_02004c3c();

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

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

static __inline__ void Call8(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7)
{
    f(a0, a1, a2, a3, a4, a5, a6, a7);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunSupplementalSequenceOne(void)
{
    s32 one;
    s32 x;
    s32 y;
    u8 *record;

    Func_020049e4();
    record = Value1(Func_02004a02, 14);
    x = *(volatile s32 *)((s32)record + 8);
    record = Value1(Func_02004a0a, 14);
    y = *(volatile s32 *)((s32)record + 16);
    x >>= 20;
    y >>= 20;
    one = 1;
    Func_02001eb0(2, x, y, 1, one, 255);
    Call6(Func_02001ec2, 2, x + 1, y, 1, one, 0);
    Call6(Func_02001ed2, 2, x - 1, y, 1, one, 0);
    Func_02001ee2(2, x, y + 1, 1, one, 0);
    Func_02001ef4(2, x, y - 1, 1, one, 0);
    record = Value1(Func_02004a72, 14);
    if ((*(volatile s32 *)((s32)record + 16) >> 20) == 27) {
        record = Value1(Func_02004a80, 14);
        record[85] = 0;
        *(volatile s32 *)((s32)record + 20) = -0x20000;
        *(volatile s32 *)((s32)record + 12) = -0x20000;
        Call1(Func_02004a6c, 0x214);
        Call6(Func_02001f30, 2, 43, 23, 1, one, 255);
    }
    Func_02004a9c();
}
