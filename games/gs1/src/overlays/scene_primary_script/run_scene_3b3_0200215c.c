#include "types.h"

#define FieldScene_RunScene3b3_0200215c Func_0200215c

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
s32 Func_02004c7e();
void Func_02004c80();
s32 Func_02004c86();
void Func_02004c98();

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

void FieldScene_RunScene3b3_0200215c(void)
{
    u32 i;
    s32 rec7;
    s32 record;
    u8 *p6;

    rec7 = Value1(Func_02004c7e, 0);
    record = Func_02004c86(13);
    p6 = *(s32 *)0x03001f30;
    if ((*(s32 *)(record + 8) >> 20) == (*(s32 *)(rec7 + 8) >> 20)) {
        if ((*(s32 *)(record + 16) >> 20) != (*(s32 *)(rec7 + 16) >> 20)) {
            goto L_02002198;
        }
        Call1(Func_02004c80, 0x203);
        p6[53] = 1;
    } else {
        L_02002198:;
        Call1(Func_02004c98, 0x203);
    }
}
