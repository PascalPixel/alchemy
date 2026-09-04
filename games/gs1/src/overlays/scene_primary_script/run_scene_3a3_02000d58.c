#include "types.h"

#define FieldScene_RunScene3a3_02000d58 Func_02000d58

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_020015a4();
void Func_02001bc6();
void Func_02001c04();
void Func_02001c20();
void Func_02001c3c();
void Func_02001c7c();
void Func_02001c88();
void Func_02001c96();
void Func_02001cba();
void Func_02001cbe();
void Func_02001cd4();
void Func_02001ce6();
void Func_02001cee();
void Func_02001cfa();
void Func_02001d08();
void Func_02001d0a();
void Func_02001d14();
void Func_02001d20();
void Func_02001d38();
void Func_02001d40();
void Func_02001d5c();
void Func_02001d78();
void Func_02001d8e();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
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

void FieldScene_RunScene3a3_02000d58(void)
{
    u32 i;
    s32 record;

    Func_02001c04();
    Call2(Func_02001cd4, 0x6666, 0xccc);
    Call4(Func_02001cee, 0x3f0000, -1, 0x1c20000, 1);
    Func_02001cfa();
    Func_02001c20(30);
    Func_02001c88(18, 1);
    Call1(Func_02001d38, -1);
    Call1(Func_02001bc6, 0x2008d09);
    Func_02001c3c(20);
    Func_02001cbe(0, 18, 0);
    Call3(Func_02001d0a, 0, 0x4000, 0);
    Func_02001d14(18, 0, 20);
    Call3(Func_02001d20, 18, 0xd000, 40);
    Func_02001d8e(147);
    Func_02001ce6(18, 2);
    Func_02001c7c(20);
    Call3(Func_02001d40, 18, 0xb000, 40);
    Func_020015a4();
    Func_02001d5c(0, 1);
    Func_02001d78();
    Func_02001d08(14, 4);
    Call1(Func_02001c96, 0x8ff);
    Func_02001cba();
}
