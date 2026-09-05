#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02001af0();
void Func_02001af6();
void Func_02001b2a();
void Func_02001b38();
s32 Func_02001b68();
void Func_02001b6a();
void Func_02001b80();
void Func_02001b88();
void Func_02001bae();
void Func_02001bb2();
void Func_02001bb4();
void Func_02001bbc();
void Func_02001be8();
void Func_02001bf4();
s32 Func_02001c08();
void Func_02001c1c();
void Func_02001c4c();

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

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunScene377_020001e0(void)
{
    u32 i;
    s32 record;

    Func_02001af0();
    Func_02001b80(16, 2);
    Func_02001af6(30);
    Call1(Func_02001bbc, 0xf5b);
    Func_02001bae(0, 16, 10);
    Func_02001be8(16, 0, 6);
    Call3(Func_02001c1c, 16, 0x102, 0);
    Func_02001bb4(16, 1);
    Func_02001b2a(20);
    Func_02001bb2(16, 4);
    Func_02001b38(20);
    Value2(Func_02001c08, 16, 0);
    if (Value2(Func_02001b68, 0, 0) == 1) {
        bump_step(1);
    }
    Func_02001bf4(16, 1);
    Func_02001b6a(20);
    Func_02001c4c(16, 0, 4);
    Func_02001b88();
}
