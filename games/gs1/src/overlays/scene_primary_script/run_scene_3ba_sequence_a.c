#include "types.h"

#define FieldScene_RunScene3baSequenceA Func_02002aec

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_00000000[];
extern u16 Data_02000240[];
void Func_02006698();
void Func_0200672a();
void Func_02006732();
void Func_0200678a();
u8 *Func_020067ae();
void Func_020067d0();
void Func_02006812();
void Func_02006832();
void Func_0200687a();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)0x03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunScene3baSequenceA(s32 a0)
{
    u8 *rec7;
    u8 *obj;
    s32 idx;
    s32 zero;

    obj = *(u8 **)0x03001e68;
    rec7 = Func_020067ae();
    idx = 249;
    if (*(u8 *)(Data_02000240 + idx) == 1) {
        *(u8 *)(Data_02000240 + idx) = 0;
        Func_02006812(a0, 1);
    } else {
        Call3(Func_0200687a, a0, 0x4000, 30);
        Func_02006832(a0, 3);
        Func_020067d0(30);
    }
    zero = 0;
    obj[7] = zero;
    obj[6] = 15;
    *(s32 *)(rec7 + 8) = *(s32 *)0x0200c804;
    *(s32 *)(rec7 + 16) = *(s32 *)0x0200c75c;
    *(u16 *)(rec7 + 6) = *(s32 *)0x0200c788;
    *(s32 *)(rec7 + 56) = -0x80000000;
    *(s32 *)(rec7 + 64) = -0x80000000;
    *(s32 *)(rec7 + 36) = zero;
    *(s32 *)(rec7 + 44) = zero;
    {
        u16 p0[1];

        p0[0] = (u16)(s32)Data_00000000;
        rec7[85] = 3;
        rec7[34] = p0[0];
    }
    *(s32 *)(rec7 + 12) = zero;
    *(s32 *)(rec7 + 20) = zero;
    Func_0200678a(rec7, 1);
    Func_0200672a(rec7, 0);
    Func_02006732(rec7, 1);
    Func_02006698(1);
}
