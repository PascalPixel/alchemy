#include "types.h"

#define FieldScene_RunScene382_02001238 Func_02001238

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_0200243c();
s32 Func_02002bd6();
void Func_02002bf4();
void Func_02002bf8();
void Func_02002c1c();
void Func_02002c2c();
void Func_02002c40();
void Func_02002c42();
void Func_02002c74();
void Func_02002c96();
void Func_02002ca6();
void Func_02002cb2();
void Func_02002cdc();
void Func_02002d0a();

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

void FieldScene_RunScene382_02001238(void)
{
    u32 i;
    s32 record;

    Func_02002bf4();
    Func_02002d0a(100);
    Func_02002bf8(40);
    if (Value1(Func_02002bd6, 0x867) == 0) {
        Call2(Func_02002cdc, 23, 0x102);
        Func_02002c96(23, 4, 0);
        Func_02002c1c(12);
        Func_02002ca6(23, 4, 0);
        Func_02002c2c(20);
        Call4(Func_0200243c, 23, 0x188, 104, 0x70000);
        Func_02002c42(20);
        Call3(Func_02002ca6, 23, 0x198, 104);
        Call3(Func_02002cb2, 23, 0x198, 120);
        Call1(Func_02002c40, 0x867);
    }
    Func_02002c74();
}
