#include "types.h"

#define FieldScene_RunScene39f_02001818 Func_02001818

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_020025c4();
void Func_02002658();
void Func_020026cc();
void Func_020045b0();
void Func_020045c0();
void Func_020045d6();
void Func_020045f4();
void Func_020045f6();
s32 Func_0200460a();
void Func_0200461e();
void Func_02004662();
void Func_0200466c();

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

void FieldScene_RunScene39f_02001818(void)
{
    u32 i;
    s32 record;

    Func_020045c0();
    Func_020026cc(14, 1);
    Call4(Func_020025c4, 14, 0x1a8, 0x1e0, 0x79999);
    Func_020045d6(2);
    Func_02002658(14);
    Func_0200466c(14, 15);
    record = Func_0200460a(14);
    Func_020045b0(record, 0);
    Func_020045f6(30);
    Call1(Func_020045f4, 0x305);
    Call3(Func_02004662, 17, 0x1a80000, 0x1e00000);
    Func_0200461e();
}
