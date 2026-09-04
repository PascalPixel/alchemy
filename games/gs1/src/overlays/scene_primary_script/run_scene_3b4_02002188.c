#include "types.h"

#define FieldScene_RunScene3b4_02002188 Func_02002188

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02003168();
void Func_02003178();
void Func_02003188();
void Func_02003198();
void Func_02003244();
void Func_0200425a();
void Func_02004262();
void Func_0200426a();
void Func_02004272();
void Func_0200427a();
void Func_020045f6();
s32 Func_020046c4();
s32 Func_020046dc();
s32 Func_020046f4();
s32 Func_0200470c();
s32 Func_02004726();
s32 Func_02004736();
s32 Func_02004746();
s32 Func_02004756();
void Func_0200475e();
s32 Func_02004766();
void Func_02004776();
void Func_0200478e();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

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

void FieldScene_RunScene3b4_02002188(void)
{
    u32 i;
    s32 record;

    Func_020045f6(1);
    Func_0200425a(12, 243);
    Func_02004262(11, 244);
    Func_0200426a(10, 244);
    Func_02004272(9, 244);
    Func_0200427a(8, 244);
    if (Value1(Func_020046c4, 0xee7) == 0) {
        Call3(Func_02004746, 8, 0xe80000, 0x3680000);
    }
    if (Value1(Func_020046dc, 0xee8) == 0) {
        Call3(Func_0200475e, 9, 0x1280000, 0x3380000);
    }
    if (Value1(Func_020046f4, 0xee9) == 0) {
        Call3(Func_02004776, 10, 0x1480000, 0x2f80000);
    }
    if (Value1(Func_0200470c, 0xeea) == 0) {
        Call3(Func_0200478e, 11, 0x1680000, 0x3680000);
    }
    if (Value1(Func_02004726, 0x9c0) != 0) {
        Func_02003168(0);
    }
    if (Value1(Func_02004736, 0x9c1) != 0) {
        Func_02003178(1);
    }
    if (Value1(Func_02004746, 0x9c2) != 0) {
        Func_02003188(2);
    }
    if (Value1(Func_02004756, 0x9c3) != 0) {
        Func_02003198(3);
    }
    if (Value1(Func_02004766, 0x9c4) != 0) {
        Func_02003244(0);
    }
}
