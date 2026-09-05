#include "types.h"

#define FieldScene_RunPrimarySequence Func_020010e0

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02002d4e();
void Func_02002d60();
void Func_02002d74();
s32 Func_02002d90();
s32 Func_02002da8();
void Func_02002dac();
void Func_02002dd0();
void Func_02002dde();
void Func_02002dfc();
s32 Func_02002e3c();
void Func_02002e94();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

s32 Func_020010e0(s32 a0)
{
    s32 box[3];
    u8 *rec;
    u8 *flag;
    u8 *slot;
    s32 saved;

    rec = (u8 *)Value1(Func_02002da8, 0);
    flag = rec + 85;
    saved = *flag;
    slot = (u8 *)box;
    *(s32 *)(slot + 0) = (*(volatile s32 *)(rec + 8) & -0x100000) + 0x80000;
    *(s32 *)(slot + 4) = *(volatile s32 *)(rec + 12);
    *(s32 *)(slot + 8) = (*(volatile s32 *)(rec + 16) & -0x100000) + 0x280000;
    if (Value2(Func_02002d90, (s32)rec, (s32)slot) == 0) {
        Func_02002dd0();
        Func_02002d60((s32)rec, 6);
        Func_02002d4e(6);
        Func_02002e94(152);
        Func_02002d74((s32)rec, 7);
        *(volatile s32 *)(rec + 48) = 0x30000;
        *(volatile s32 *)(rec + 52) = 0x20000;
        *(volatile s32 *)(rec + 40) = 0x40000;
        *flag = *flag & 126;
        Func_02002dde((s32)rec, 0);
        Value3(Func_02002e3c, 0, *(s16 *)(slot + 2), *(s16 *)(slot + 10));
        Func_02002dac((s32)rec, 6);
        Func_02002dfc((s32)rec, 1);
        *flag = (u8)saved;
        ((void (*)())Func_02002e3c)();
        return 1;
    }
    return 0;
}
