#include "types.h"

#define Func_02000ba8 Func_02000ba8

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
s32 Func_02006b4e();
void Func_02006b60();
void Func_02006b68();
void Func_02006c0e();
void Func_02006c46();
void Func_02006c4a();
void Func_02006c7c();
void Func_02006c7e();
void Func_02006c90();
void Func_02006c98();
void Func_02006c9c();
void Func_02006cba();
void Func_02006cc0();
void Func_02006cd8();
void Func_02006ce2();
void Func_02006cee();
void Func_02006cfa();

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

void Func_02000ba8(void)
{
    u32 i;
    s32 record;

    Func_02006b68();
    if (Value1(Func_02006b4e, 0x807) == 0) {
        Call1(Func_02006b60, 0x807);
        Call1(Func_02006c46, 0xf63);
        Call3(Func_02006c90, 18, 0x103, 0);
        Func_02006c4a(0, 18, 20);
        Func_02006c7c(18, 0, 6);
        Call3(Func_02006c98, 18, 0x8000, 30);
        Func_02006c4a(18, 2, 20);
        Func_02006c9c(18, 0, 6);
        Func_02006c7e(18, 0, 10);
        Call3(Func_02006cd8, 18, 0x103, 0);
        Func_02006cba(18, 0, 10);
        Call3(Func_02006cee, 0, 0x102, 60);
    } else {
        Call3(Func_02006cfa, 18, 0x103, 0);
        Call1(Func_02006cc0, 0xf66);
        Func_02006ce2(18, 0, 20);
    }
    Func_02006c0e();
}
