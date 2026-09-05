#include "types.h"

#define FieldScene_RunScene3b9_02002820 Func_02002820

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_020053e0();
s32 Func_020053e2();
void Func_020053f4();
void Func_02005410();
void Func_02005412();
void Func_02005434();
void Func_0200547a();
s32 Func_02005482();
void Func_02005484();
void Func_020054a2();
void Func_020054d2();
void Func_020054e4();
void Func_020054e8();
void Func_020054ec();
void Func_020054ee();
void Func_02005504();
void Func_0200550a();
void Func_02005518();
void Func_02005530();
void Func_0200559c();
void Func_020055ee();
void Func_0200560c();
void Func_02005618();

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

void FieldScene_RunScene3b9_02002820(void)
{
    u32 i;
    s32 record;

    if (Value1(Func_020053e2, 5) != 0) {
        Call1(Func_020053f4, 0x16d);
        Func_02005412(5);
        Func_02005410(3);
    }
    Func_02005434();
    Call3(Func_020054a2, 11, 0x3640000, 0x24c0000);
    Func_020053e0(1);
    Func_02005530(11, 1);
    Call3(Func_0200547a, 11, 0x19999, 0xcccc);
    Call3(Func_02005484, 0, 0x19999, 0xcccc);
    record = Func_02005482(11);
    {
        s32 shown = 0x8000;

        *(volatile u16 *)(record + 6) = shown;
    }
    Func_0200559c();
    Func_020054e4(0, 2);
    Func_020054ec(11, 2);
    Call3(Func_020054d2, 0, 0x320, 0x24c);
    Call3(Func_020054e8, 11, 0x300, 0x24c);
    Call3(Func_020054ee, 0, 0x2bc, 0x24c);
    Call3(Func_02005504, 11, 0x29c, 0x24c);
    Call3(Func_0200550a, 0, 0x258, 0x24c);
    Call3(Func_02005518, 11, 0x238, 0x24c);
    Func_0200560c();
    Func_02005618();
    Func_020055ee(21);
}
