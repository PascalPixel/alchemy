#include "types.h"

#define FieldScene_RunScene399_020005dc Func_020005dc

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02002810();
void Func_02002836();
void Func_020028e6();
void Func_020028f6();
void Func_0200291a();

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

void FieldScene_RunScene399_020005dc(void)
{
    u32 i;
    s32 record;

    Func_02002810();
    Call1(Func_020028e6, 0x156d);
    Func_020028f6(8, 0);
    Call3(Func_0200291a, 8, 0x3000, 10);
    Func_02002836();
}
