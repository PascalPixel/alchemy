#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_020087c8();
void Func_0200880e();
void Func_0200885a();
void Func_02008f02();
void Func_0200a3b8();
void Func_0200a428();
void Func_0200a442();
void Func_0200a452();
void Func_0200a46a();
void Func_0200a474();
void Func_0200a4d0();
void Func_0200a506();
void Func_0200a520();
void Func_0200a528();

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

void FieldScene_RunScene3b1_02003eec(void)
{
    u32 i;
    s32 record;

    Func_0200a3b8();
    Func_0200a442(0, 0, 0);
    Func_02008f02();
    Call3(Func_0200a452, 18, 0x960000, 0x24a0000);
    Call4(Func_020087c8, 0x9c0000, -1, 0x2180000, 0x1000001);
    Func_0200880e(8, 0, 0);
    Call3(Func_0200a428, 18, 0xcccc, 0x6666);
    Call3(Func_0200a46a, 18, 168, 0x242);
    Call3(Func_0200a474, 18, 168, 0x22a);
    Call3(Func_0200a520, 18, 0x8000, 20);
    Func_0200a4d0(18, 2);
    Call1(Func_0200a506, 0x1e3c);
    Func_0200a528(18, 0, 20);
    Func_0200885a(9, 12, 0);
}
