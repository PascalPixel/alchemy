#include "types.h"

#define FieldScene_ScaleActorPairAndPrompt Func_02001ed8

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_0200d144[];
extern u8 Data_03001ebc[];
s32 Func_02005a18();
void Func_02005b14();
void Func_02005b1e();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ s32 Call2R(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

static __inline__ s32 get_record(void)
{
    return 0;
}

void FieldScene_ScaleActorPairAndPrompt(s32 a0, s32 a1)
{
    u32 i;
    s32 record = 0;

    *(volatile s32 *)Data_0200d144 = 0;
    Call3(Func_02005b14, 20, 0x19999, 0xcccc);
    record = 0;
    Call3(Func_02005b1e, 21, 0x19999, 0xcccc);
    record = Call2R(Func_02005a18, 0x2009f15, 0xc80);
}
