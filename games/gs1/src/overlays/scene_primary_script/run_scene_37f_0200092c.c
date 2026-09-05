#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_0200a69c[];
extern u8 Data_03001ebc[];
void Func_0200248a();
void Func_020024ca();
s32 Func_020024d6();
void Func_020024de();
s32 Func_020024f0();
void Func_02002502();
void Func_02002514();
void Func_02002540();
s32 Func_02002542();
void Func_02002552();
void Func_02002564();
void Func_020025c4();

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

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

void FieldScene_RunScene37f_0200092c(void)
{
    Call1(Func_020024ca, 0x144);
    *(s32 *)((*(u8 *volatile *)Data_03001ebc + 0x1c0)) = 0x100;
    if (Value1(Func_020024d6, 0x814) != 0) {
        s32 zero = 0;
        *(volatile s32 *)Data_0200a69c = zero;
        Call2(Func_0200248a, 0x2009ac9, 0xc80);
    }
    if (Value1(Func_020024f0, 0x879) != 0) {
        Call6(Func_020024de, 5, 6, 1, 1, 6, 6);
        Call6(Func_020024f0, 5, 6, 1, 1, 7, 6);
        Call6(Func_02002502, 5, 6, 1, 1, 8, 6);
        Call6(Func_02002514, 0, 1, 3, 1, 6, 5);
    }
    if (Value1(Func_02002542, 0x815) != 0) {
        Call3(Func_020025c4, 8, 0x780000, 0xe80000);
        Call6(Func_02002540, 2, 10, 1, 1, 6, 14);
        Call6(Func_02002552, 2, 10, 1, 1, 7, 14);
        Call6(Func_02002564, 2, 10, 1, 1, 8, 14);
    }
}
