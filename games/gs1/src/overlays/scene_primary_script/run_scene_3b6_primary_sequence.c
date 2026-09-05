#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
struct Ent { s32 a; u16 b; u16 c; };
extern struct Ent Data_02009dcc[];
s32 Func_02000ab2();
s32 Func_02000ac6();
s32 Func_02000b0e();
void Func_02000b10();
void Func_02000b3c();
void Func_02000b78();
void Func_02000b8c();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

void FieldScene_RunPrimarySequence(void)
{
    u32 i;
    s32 record;
    u8 *p6;
    s32 n;

    p6 = *(u8 **)Data_03001ebc;
    for (i = 8; i < 66; i++) {
        record = Value1(Func_02000ac6, i);
        if (record != 0) {
            *(u8 *)(record + 85) = 0;
        }
    }
    p6 = p6 + 0x16c;
    n = *(s16 *)p6 - 14;
    Func_02000b78(158);
    Value3(Func_02000ab2,
           Data_02009dcc[n].a,
           Data_02009dcc[n].b,
           Data_02009dcc[n].c);
    Call3(Func_02000b10, 0, 0x8000, 0x4000);
    *(u8 *)(Func_02000b0e(0) + 85) = 0;
    Func_02000b3c(0, 2);
    Func_02000b8c(*(s16 *)p6);
}
