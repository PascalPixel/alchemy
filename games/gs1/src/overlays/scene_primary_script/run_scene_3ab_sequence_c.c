#include "types.h"

#define FieldScene_RunScene3abSequenceC Func_02000c10

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
s32 Func_020025d6();
s32 Func_020025e0();
void Func_0200262c();
s32 Func_02002636();
void Func_02002638();
void Func_02002676();
void Func_0200268a();
void Func_02002712();

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

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunScene3abSequenceC(void)
{
    u32 i;
    s32 rec7;
    s32 record;

    if (Value1(Func_020025d6, 0x202) != 0) {
        rec7 = Value1(Func_020025e0, 0x201);
        if (rec7 == 0) {
            Call3(Func_02002638, 0, 0x3333, 0x1999);
            *(u8 *)(Func_02002636(0) + 85) = rec7;
            Func_0200268a(0, 2);
            Call3(Func_02002676, 0, 2, -8);
            Func_0200262c(13);
            Func_02002712(12);
        }
    }
}
