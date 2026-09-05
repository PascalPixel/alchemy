#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
s32 Func_02001686();
void Func_02001690();
void Func_020016a4();
void Func_020016b6();
void Func_020016ca();
void Func_020016d2();
void Func_020016d6();
void Func_02001732();
void Func_020017bc();

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

void FieldScene_RunScene3a3SequenceC(void)
{
    u32 i;
    s32 record;

    record = Value1(Func_02001686, 18);
    {
        s32 shown = 0;
    
        *(volatile u16 *)(record + 100) = shown;
    }
    {
        s32 shown = 0;
    
        *(volatile u16 *)((record + 100) + 2) = shown;
    }
    *(volatile s32 *)(record + 72) = 0x6666;
    *(volatile s32 *)(record + 108) = 0x20086f1;
    Call3(Func_020016b6, 18, 0x13333, 0x9999);
    Call3(Func_020016ca, 18, 28, 0x1cc);
    Call3(Func_020016d6, 18, 24, 0x1c0);
    Func_020017bc(229);
    Func_020016d2(18);
    Call3(Func_02001690, 0, 0x10000, 0x10000);
    Func_020016b6(4);
    Call3(Func_020016a4, -1, -1, 0xe666);
    Func_020016ca(40);
    Func_02001732(18, 1);
}
