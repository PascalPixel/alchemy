#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02000582();
void Func_02004a9c();
void Func_02004b14();
void Func_02004b8e();
void Func_02004cba();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

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

void FieldScene_RunScene372_02000400(void)
{
    u32 i;
    s32 record;

    Func_02004cba(158);
    Call3(Func_02004a9c, 0x200d7a0, 38, 72);
    Call3(Func_02004b8e, 0, 146, 0x49e);
    Func_02004b14(3);
    Func_02000582(13);
}
