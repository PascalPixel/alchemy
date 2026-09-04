#include "types.h"

#define FieldScene_RunScene3b9_02000468 Func_02000468

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_020020d8();
void Func_02003068();
s32 Func_0200307a();
void Func_0200309e();
void Func_020030b8();
void Func_020030d4();
void Func_02003108();
void Func_02003114();
void Func_0200311e();
void Func_02003144();

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

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunScene3b9_02000468(void)
{
    u32 i;
    s32 rec7;
    s32 record;

    rec7 = Value1(Func_0200307a, 13);
    Func_02003068();
    Func_0200309e(13);
    Func_02003108(13, 0, 20);
    Call1(Func_0200311e, 0x2114);
    Func_020020d8(13);
    Func_02003114(13, 1);
    Func_02003144(13, 0);
    {
        u16 *target = (u16 *)(rec7 + 100);
        s32 shown = 0x2d0;

        *target = shown;
    }
    {
        u16 *target = (u16 *)(rec7 + 102);
        s32 shown = 112;

        *target = shown;
    }
    Func_020030d4(13, 2);
    Func_020030b8();
}
