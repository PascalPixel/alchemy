#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02005b8c();
void Func_02005bac();
void Func_02005c10();
void Func_02005c26();
s32 Func_02005c58();
u8 *Func_02005c64();
u8 *Func_02005c76();
void Func_02005c88();
void Func_02005c8c();
void Func_02005c9a();
void Func_02005ca6();
void Func_02005caa();
void Func_02005cb2();
void Func_02005cbe();

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

void FieldScene_RunScene3af_02001920(void)
{
    u32 i;
    u8 *record;

    Func_02005c10();
    Call1(Func_02005c26, 0x200d160);
    Func_02005b8c(1);
    Func_02005ca6(20, 0, 0);
    Call3(Func_02005cb2, 23, 0xee0000, 0x2720000);
    Call3(Func_02005cbe, 22, 0x10c0000, 0x2a60000);
    record = Func_02005c64(22);
    {
        s32 shown = 0;

        *(volatile u16 *)((s32)record + 6) = shown;
    }
    Call2(Func_02005c88, 22, 0x200c980);
    {
        u8 *record = Func_02005c76(21);
        u8 value = *(volatile u8 *)&record[89];

        record[89] = (u8)(value | 128);
    }
    Call3(Func_02005c9a, 21, 0xcccc, 0x6666);
    Call2(Func_02005caa, 21, 0x200c628);
    if (Value1(Func_02005c58, 0x109) != 0) {
        Func_02005bac();
    }
    Func_02005c8c();
}
