#include "types.h"

#define FieldScene_RunScene385_020009f8 Func_020009f8

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_020013bc();
s32 Func_02001ace();
void Func_02001ae4();
void Func_02001ae8();
void Func_02001b0c();
void Func_02001b1c();
void Func_02001b32();
void Func_02001b38();
void Func_02001b64();
void Func_02001b66();
void Func_02001b76();
void Func_02001b86();
void Func_02001b92();
void Func_02001bb2();
void Func_02001bb4();

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

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunScene385_020009f8(void)
{
    u32 i;
    s32 record;

    Func_02001ae4();
    Func_02001bb2(100);
    Func_02001ae8(40);
    if (Value1(Func_02001ace, 0x867) == 0) {
        Call2(Func_02001bb4, 21, 0x102);
        Func_02001b66(21, 4, 0);
        Func_02001b0c(12);
        Func_02001b76(21, 4, 0);
        Func_02001b1c(20);
        Call4(Func_020013bc, 21, 0x188, 104, 0x70000);
        Func_02001b32(20);
        Call3(Func_02001b86, 21, 0x198, 104);
        Call3(Func_02001b92, 21, 0x198, 120);
        Call1(Func_02001b38, 0x867);
    }
    Func_02001b64();
}
