#include "types.h"

#define FieldScene_RunOpeningAuxiliarySequence Func_02000d24

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_000000a5[];
extern u8 Data_02000240[];
extern u8 Data_02009a00[];
extern u8 Data_03001ebc[];
extern s16 Data_02000240_t[][1];
void Func_02001fa2();
s32 Func_02001fa8();
void Func_02001fbe();
s32 Func_02001fca();
s32 Func_02001fd2();
s32 Func_02001fd6();
void Func_02002026();
void Func_020020d4();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

static __inline__ s32 Value6(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    return f(a0, a1, a2, a3, a4, a5);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

s32 Func_02000d24(s32 a0)
{
    u32 i;
    s32 record;

    if (Data_02000240_t[224][0] == (s32)Data_000000a5) {
        *(u8 *)(Func_02001fca(14) + 35) = 2;
        *(u8 *)(Func_02001fd6(14) + 85) = 3;
        Func_02002026(14, 0, 0);
        Call6(Func_02001fa2, 16, 44, 1, 1, 15, 44);
        Func_020020d4(100, 0, 0);
        Call6(Func_02001fbe, 12, 71, 1, 1, 127, 127);
        Value6(Func_02001fd2, 11, 71, 1, 1, 12, 71);
        record = Value1(Func_02001fa8, 0x2008ce5);
        do {
            s32 shown = *(u16 *)Data_02009a00;

            *(volatile u16 *)0x0500019e = shown;
        } while (0);
        return record;
    }
    return a0;
}
