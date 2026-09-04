#include "types.h"

#define FieldScene_RunScene3b1_02003e34 Func_02003e34

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02008720();
void Func_0200872a();
void Func_02008734();
void Func_02008766();
void Func_020087b2();
void Func_0200a300();
void Func_0200a380();
void Func_0200a39e();
void Func_0200a3aa();
void Func_0200a3c2();
void Func_0200a3cc();
void Func_0200a428();
void Func_0200a45e();
void Func_0200a478();
void Func_0200a480();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

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

void FieldScene_RunScene3b1_02003e34(void)
{
    u32 i;
    s32 record;

    Func_0200a300();
    Func_0200872a(24, 0, 0);
    Func_02008734(18, 0, 0);
    Func_0200a39e(0, 0, 0);
    Call3(Func_0200a3aa, 16, 0x960000, 0x24a0000);
    Call4(Func_02008720, 0x9c0000, -1, 0x2180000, 0x1000001);
    Func_02008766(8, 0, 0);
    Call3(Func_0200a380, 16, 0xcccc, 0x6666);
    Call3(Func_0200a3c2, 16, 168, 0x242);
    Call3(Func_0200a3cc, 16, 168, 0x22a);
    Call3(Func_0200a478, 16, 0x8000, 20);
    Func_0200a428(16, 2);
    Call1(Func_0200a45e, 0x1e3c);
    Func_0200a480(16, 0, 20);
    Func_020087b2(9, 12, 0);
}
