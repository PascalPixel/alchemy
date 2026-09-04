#include "types.h"

#define FieldScene_RunScene3b1_02003d10 Func_02003d10

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
s32 Func_0200833a();
void Func_02008606();
s32 Func_0200862c();
void Func_020086b6();
void Func_0200a1d8();
void Func_0200a1dc();
void Func_0200a1ec();
void Func_0200a22e();
void Func_0200a232();
void Func_0200a246();
void Func_0200a254();
void Func_0200a272();
void Func_0200a280();
void Func_0200a2a6();
void Func_0200a2d2();
void Func_0200a2d6();
void Func_0200a2ee();
void Func_0200a32c();
void Func_0200a362();
void Func_0200a384();
void Func_0200a3d8();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ s32 Value0(s32 (*f)())
{
    return f();
}

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

void FieldScene_RunScene3b1_02003d10(void)
{
    u32 i;
    s32 rec7;
    s32 record;

    Func_0200a1dc();
    Func_02008606(15, 0, 1);
    Func_0200a2a6(8, 1);
    Func_0200a1ec(20);
    Call3(Func_0200a22e, 8, 0xcccc, 0x6666);
    Call3(Func_0200a272, 8, 0x1d4, 0x266);
    Call3(Func_0200a280, 8, 0x1d8, 0x254);
    Call3(Func_0200a32c, 8, 0x8000, 20);
    Func_0200a2d6(8, 4, 20);
    rec7 = Value0(Func_0200833a);
    Func_0200a232(20);
    Func_0200a3d8(214);
    Func_0200a1d8(rec7, 0x200e738);
    Func_0200a246(40);
    Func_0200a2ee(8, 3);
    Func_0200a254(20);
    Call3(Func_0200a2d2, 8, 0x1d2, 0x270);
    Value2(Func_0200862c, 8, 0x5000);
    Func_0200a32c(8, 2);
    Call1(Func_0200a362, 0x1e3b);
    Func_0200a384(8, 0, 20);
    Func_020086b6(9, 11, 0);
}
