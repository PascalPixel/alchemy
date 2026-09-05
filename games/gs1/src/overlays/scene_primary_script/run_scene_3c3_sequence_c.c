#include "types.h"

#define Lifted_02000124 Func_02000124

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_00000069[];
extern u8 Data_000000a9[];
extern u8 Data_000000aa[];
extern u8 Data_02000240[];
extern u8 Data_03001ebc[];
s32 Func_020009a8();
s32 Func_020009da();
s32 Func_020009fc();
s32 Func_02000a16();
void Func_02000a40();
s32 Func_02000a4c();
void Func_02000a4e();
void Func_02000a54();
void Func_02000a58();
void Func_02000a80();
void Func_02000ab6();
s32 Func_02000ae6();
void Func_02000b00();
void Func_02000b08();
void Func_02000b10();
void Func_02000b18();
void Func_02000b1e();
void Func_02000b20();
void Func_02000b34();
void Func_02000b36();
s32 Func_02000b52();
s32 Func_02000bc0();
void Func_02000c16();
void Func_02000c46();

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

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}


void Func_02000288(void)
{
    s32 arg;
    s32 v5;

    arg = *(s16 *)(*(u8 **)Data_03001ebc + 0x16c);
    *(u8 *)(Func_02000b52(0) + 85) = 0;
    v5 = 2;
    Func_02000c16(158);
    Call6(Func_02000b20, 66, 36, 71, 8, v5, v5);
    Func_02000b1e(4);
    Call6(Func_02000b36, 68, 36, 71, 8, v5, v5);
    Func_02000b34(4);
    Value3(Func_02000bc0, 0, 3, -16);
    Func_02000c46(arg);
}
