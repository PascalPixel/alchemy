#include "types.h"

#define FieldScene_RunScene39a_02001b1c Func_02001b1c

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_0200a488[];
s32 Func_02003de6();
s32 Func_02003df4();
void Func_02003e1a();
s32 Func_02003e1c();
void Func_02003e7c();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

static __inline__ s32 Value4(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    return f(a0, a1, a2, a3);
}

void FieldScene_RunScene39a_02001b1c(void)
{
    u8 *rec;
    s32 rec7;
    s32 record;
    u8 *p6;

    record = 0;
    rec = Value4(Func_02003de6, 22, 0xf80000, 0x80000, 0x980000);
    if ((s32)rec != 0) {
        p6 = *(volatile s32 *)((s32)rec + 80);
        p6[38] = record;
        *(u8 *)(((s32)p6 + 38) + 1) = record;
        *((s8 *)p6 + 5) &= -33;
        p6[9] &= 15;
        rec[85] = record;
        rec[92] = 1;
        rec7 = Value2(Func_02003df4, 17, 0x608);
        Func_02003e7c(230);
        Value3(Func_02003e1c, p6[28], 128, (rec7 + 0x400));
        Func_02003e1a(17);
        *(volatile s32 *)Data_0200a488 = (s32)rec;
    }
}
