#include "types.h"

#define FieldScene_RunScene39f_02000d90 Func_02000d90

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02003b36();
void Func_02003b5e();
s32 Func_02003b62();
void Func_02003b82();
void Func_02003bb8();
void Func_02003bec();
void Func_02003bee();
void Func_02003c78();

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
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunScene39f_02000d90(s32 a0, s32 a1, s32 a2, s32 a3)
{
    u32 i;
    s32 p10;
    s32 p10b;
    s32 p10c;
    s32 p8;
    s32 p8b;
    s32 p9;
    s32 rec7;
    s32 record;

    p10 = a2;
    p9 = a3;
    p8 = a1;
    rec7 = Func_02003b62();
    Func_02003bec(a0, 1);
    Call3(Func_02003b82, a0, 0x30000, 0x18000);
    Func_02003c78(152);
    *(volatile s32 *)(rec7 + 40) = p9;
    *(volatile s32 *)(rec7 + 72) = 0x8000;
    *(volatile s32 *)(rec7 + 68) = 0;
    Func_02003b36(rec7, 0);
    Func_02003bb8(a0, p8, p10);
    p8b = ((s32)p8 << 16);
    p10b = ((s32)p10 << 16);
    Func_02003bee(a0, p8b, p10b);
    Func_02003b5e(rec7, 1);
    *(volatile s32 *)(rec7 + 72) = 0x10000;
    p10c = a0;
}
