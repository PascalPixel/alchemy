#include "types.h"

#define FieldScene_RunScene3c5_020024d0 Func_020024d0

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02004046();
s32 Func_020052f0();
void Func_020052fc();
void Func_02005302();
void Func_02005324();
s32 Func_02005346();
void Func_02005358();
void Func_02005364();
void Func_02005368();
void Func_0200538a();
void Func_02005390();
void Func_02005478();

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

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunScene3c5_020024d0(void)
{
    u32 i;
    s32 record;

    if (Value1(Func_020052f0, 0x985) == 0) {
        Call1(Func_02005302, 0x985);
        Func_02005478(157);
        Func_02005324();
        Call3(Func_02005358, 8, 0x118, 240);
        Call3(Func_02005364, 9, 0x148, 240);
        Func_0200538a(8);
        Func_02005390(9);
        Call6(Func_020052fc, 81, 14, 4, 1, 17, 14);
        Func_02005368();
        if (Value1(Func_02005346, 0x989) == 0) {
            Func_02004046();
        }
    }
}
