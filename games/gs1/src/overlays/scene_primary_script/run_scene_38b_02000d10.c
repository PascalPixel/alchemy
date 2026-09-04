#include "types.h"

#define FieldScene_RunScene38b_02000d10 Func_02000d10

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u16 Data_02000240[];
extern u8 Data_03001ebc[];
void Func_02001cc2();
void Func_02001d84();
void Func_02001db2();
s32 Func_02001dd6();
void Func_02001e06();
s32 Func_02001e18();
void Func_02001e26();
void Func_02001ed6();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ u16 ReadU16Elem(u16 *base, s32 idx)
{
    return *(u16 *)(base + idx);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

void FieldScene_RunScene38b_02000d10(void)
{
    s32 arg0;
    s32 rec7;
    s32 record;

    *(s32 *)((*(u8 *volatile *)Data_03001ebc + 0x1c0)) = 0x204;
    Func_02001cc2();
    record = ReadU16Elem(Data_02000240, 225);
    if ((u32)((record - 3) << 16) <= 0x10000) {
        if (Value1(Func_02001dd6, 0x109) == 0) {
            rec7 = Value1(Func_02001e18, 0);
            Func_02001e06();
            arg0 = *(s32 *)(rec7 + 8);
            *(s32 *)(rec7 + 12) = 0x100000;
            Call4(Func_02001ed6, arg0, 0x100000, *(s32 *)(rec7 + 16), 0);
            Func_02001db2();
            Func_02001e26();
            Func_02001d84(1);
        }
    }
}
