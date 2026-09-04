#include "types.h"

#define FieldScene_RunScene3be_02001080 Func_02001080

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
s32 Func_0200258c();
void Func_020025a4();
void Func_020025a6();
void Func_020025ac();
void Func_020025b2();
void Func_020025b6();
void Func_020025b8();
void Func_020025bc();
void Func_020025f6();
void Func_02002672();
void Func_02002686();
void Func_02002692();

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

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunScene3be_02001080(void)
{
    u32 i;
    s32 record;
    u8 *p5;

    p5 = *(volatile s32 *)Data_03001ebc;
    Func_020025a4();
    if (Value1(Func_0200258c, 0x204) != 0) {
        Call1(Func_020025a6, 0x9a3);
        Call1(Func_020025ac, 0x9a5);
        Call1(Func_020025b2, 0x9a4);
        Call1(Func_020025b8, 0x9a6);
        Call1(Func_020025b6, 0x9a5);
        Call1(Func_020025bc, 0x9a4);
    }
    Func_02002672(*(s16 *)(((s32)p5 + 0x16c)));
    Func_02002686();
    Func_02002692();
    Func_020025f6();
}
