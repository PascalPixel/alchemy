#include "types.h"

#define FieldScene_RunScene371_02001680 Func_02001680

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_0200581a();
void Func_0200582e();
void Func_02005842();
void Func_02005856();
void Func_0200586a();
void Func_02005880();
void Func_02005896();
void Func_020058aa();
void Func_020058be();
void Func_020058d2();
void Func_020058e6();
void Func_020058fc();
void Func_02005a84();
void Func_02005a98();
void Func_02005aac();
void Func_02005ac0();
void Func_02005ad4();
void Func_02005aea();
void Func_02005b00();
void Func_02005b14();
void Func_02005b28();
void Func_02005b3c();
void Func_02005b50();
void Func_02005b66();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunScene371_02001680(void)
{
    u32 i;
    s32 record;

    Call4(Func_02005a84, 0x15ec0000, -1, 0x6c80000, 1);
    Func_0200581a(4);
    Call4(Func_02005a98, 0x15e40000, -1, 0x6cc0000, 1);
    Func_0200582e(4);
    Call4(Func_02005aac, 0x15ec0000, -1, 0x6c40000, 1);
    Func_02005842(4);
    Call4(Func_02005ac0, 0x15ec0000, -1, 0x6cc0000, 1);
    Func_02005856(4);
    Call4(Func_02005ad4, 0x15e40000, -1, 0x6c40000, 1);
    Func_0200586a(4);
    Call4(Func_02005aea, 0x15e80000, -1, 0x6c80000, 1);
    Func_02005880(4);
    Call4(Func_02005b00, 0x15ea0000, -1, 0x6c80000, 1);
    Func_02005896(4);
    Call4(Func_02005b14, 0x15e60000, -1, 0x6ca0000, 1);
    Func_020058aa(4);
    Call4(Func_02005b28, 0x15ea0000, -1, 0x6c60000, 1);
    Func_020058be(4);
    Call4(Func_02005b3c, 0x15ea0000, -1, 0x6ca0000, 1);
    Func_020058d2(4);
    Call4(Func_02005b50, 0x15e60000, -1, 0x6c60000, 1);
    Func_020058e6(4);
    Call4(Func_02005b66, 0x15e80000, -1, 0x6c80000, 1);
    Func_020058fc(4);
}
