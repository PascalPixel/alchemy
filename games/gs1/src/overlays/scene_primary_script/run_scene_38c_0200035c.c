#include "types.h"

#define FieldScene_RunScene38c_0200035c Func_0200035c

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
s32 Func_02000868();
void Func_02000878();
void Func_020008b6();
void Func_020008c2();
void Func_020008c4();
void Func_020008d6();
void Func_020008da();
void Func_020008ee();
void Func_020008fe();
void Func_02000904();
void Func_0200090e();
void Func_0200091c();
void Func_02000920();
void Func_02000934();
void Func_02000958();

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

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunScene38c_0200035c(void)
{
    u32 i;
    s32 record;

    Func_02000878();
    if (Value1(Func_02000868, 0x300) == 0) {
        Call1(Func_020008c2, 0x16ff);
        Func_020008da(21, 0);
        Call3(Func_020008fe, 21, 0x8000, 20);
        Func_020008ee(21, 0);
        Func_020008d6(22, 2);
        Call2(Func_02000920, 22, 0x102);
        Func_020008b6(60);
        Func_0200090e(22, 0);
        Func_020008c4(10);
        Call1(Func_020008c4, 0x300);
    }
    Func_0200090e(21, 0, 0);
    Call1(Func_0200091c, 0x1702);
    Func_02000934(21, 0);
    Call3(Func_02000958, 21, 0xc000, 10);
    Func_02000904();
}
