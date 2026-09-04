#include "types.h"

#define FieldScene_RunScene385_02000d84 Func_02000d84

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02001e62();
void Func_02001e74();
void Func_02001e76();
void Func_02001e98();
void Func_02001eae();
void Func_02001eb4();
void Func_02001ec4();
void Func_02001ed4();
void Func_02001eda();
void Func_02001ee0();
void Func_02001ee4();
void Func_02001ee6();
void Func_02001f0c();
void Func_02001f0e();
void Func_02001f1e();
void Func_02001f28();
void Func_02001f2e();
void Func_02001f4c();
void Func_02001f9a();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
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

void FieldScene_RunScene385_02000d84(void)
{
    u32 i;
    s32 record;

    Func_02001e62(231);
    Func_02001e76();
    Func_02001e74(10);
    Func_02001ee4(18, 2);
    Call3(Func_02001eae, 18, 0xcccc, 0x6666);
    Call3(Func_02001eda, 18, 216, 0x198);
    Func_02001e98(10);
    Call3(Func_02001f4c, 18, 0x4000, 20);
    Func_02001f0e(18, 6, 0);
    Func_02001eb4(30);
    Func_02001f1e(18, 6, 0);
    Func_02001ec4(30);
    Func_02001f2e(18, 6, 0);
    Func_02001ed4(30);
    Call3(Func_02001f28, 18, 216, 0x188);
    Func_02001ee6(10);
    Call3(Func_02001f9a, 18, 0x4000, 20);
    Call1(Func_02001ee0, 0x858);
    Func_02001f0c();
}
