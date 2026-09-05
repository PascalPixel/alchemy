#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02001728();
void Func_020061fa();
void Func_02006200();
void Func_02006212();
void Func_0200623e();
void Func_02006264();
void Func_0200637a();

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

void FieldScene_RunScene3bf_02000bac(void)
{
    u32 i;
    s32 record;

    Call3(Func_02006212, 12, 0x10000, 0x8000);
    Call3(Func_0200623e, 12, 248, 0x178);
    Func_02006264(12);
    Func_0200637a(215);
    Func_02006200(60);
    Func_02001728();
    Call1(Func_020061fa, 0x943);
}
