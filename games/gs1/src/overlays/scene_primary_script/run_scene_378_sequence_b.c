#include "types.h"

#define FieldScene_RunScene378SequenceB Func_020004e8

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02001d28();
void Func_02003954();
void Func_02003974();
void Func_0200397c();
s32 Func_02003982();
void Func_02003998();
s32 Func_020039ac();
void Func_020039b4();
s32 Func_020039c8();
u8 *Func_020039e2();
void Func_020039ee();
u8 *Func_020039f2();
void Func_02003a12();
s32 Func_02003a20();
s32 Func_02003a22();
void Func_02003a28();
void Func_02003a36();
u8 *Func_02003a40();
s32 Func_02003a4c();
void Func_02003a50();
s32 Func_02003a68();
void Func_02003a6c();
void Func_02003a7e();
void Func_02003aa0();
void Func_02003aa2();
void Func_02003aa8();
void Func_02003aaa();
void Func_02003ab0();
void Func_02003aba();
void Func_02003ac4();
void Func_02003ad0();
void Func_02003ad4();
void Func_02003ade();
void Func_02003af4();
void Func_02003af8();
s32 Func_02003b18();
void Func_02003b3e();
void Func_02003b56();
void Func_02003b62();
void Func_02003ba8();
s32 Func_02003bb8();
void Func_02003bbc();
void Func_02003bca();
void Func_02003bd4();
void Func_02003bd8();
void Func_02003bf6();
void Func_02003c02();
void Func_02003c12();
void Func_02003c1a();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call11(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7, s32 a8, s32 a9, s32 a10)
{
    f(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(void)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + 1);
}


void FieldScene_RunScene378SequenceB(void)
{
    u32 i;
    s32 record;

    Func_02003ade();
    Call1(Func_02003af4, 0x200bc9c);
    Call1((void (*)())Func_02003a22, 1);
    Call1(Func_02003ba8, 0x1bfd);
    Value2(Func_02003bb8, 9, 0);
    if (Value2(Func_02003b18, 0, 0) == 0) {
        Func_02003bd4(9, 0);
    } else {
        bump_step();
        Call11(Func_02003c1a, 2, 16, 1, 24, 1, 3, 7, 16, 1, 14, 0);
        Func_02003c12(9, 0);
    }
    Func_02003b56();
}
