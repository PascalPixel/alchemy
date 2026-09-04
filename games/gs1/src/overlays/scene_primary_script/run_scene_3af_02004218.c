#include "types.h"

#define FieldScene_RunScene3af_02004218 Func_02004218

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_020084aa();
void Func_020084b2();
s32 Func_0200854e();
void Func_020085a8();
void Func_0200863e();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

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

void FieldScene_RunScene3af_02004218(void)
{
    u32 i;
    s32 record;

    Call4(Func_0200863e, 0xe80000, -1, 0x2a40000, 0);
    Func_020084b2();
    Call3(Func_020085a8, 0, 0xe80000, 0x2a40000);
    record = Func_0200854e(0);
    {
        s32 shown = 0x4000;
    
        *(volatile u16 *)(record + 6) = shown;
    }
    Func_020084aa(1);
}
