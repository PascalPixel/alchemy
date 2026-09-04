#include "types.h"

#define FieldScene_RunScene3bf_02000bec Func_02000bec

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_020061fc();
void Func_0200620a();
void Func_0200620c();
void Func_0200621e();
void Func_02006240();
s32 Func_02006244();
void Func_0200627e();
s32 Func_0200629c();
void Func_020063c8();

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

void FieldScene_RunScene3bf_02000bec(void)
{
    u32 i;
    u8 *rec7;
    s32 record;

    record = Value1(Func_02006244, 12);
    if ((*(volatile s32 *)(record + 16) >> 20) > 22) {
        Call3(Func_020061fc, 0x40000, 0x40000, 0x10000);
        Call3(Func_0200620a, -1, -1, 0xe666);
        Func_020063c8(144);
        Call6(Func_0200620c, 15, 20, 1, 1, 15, 22);
        Call6(Func_0200621e, 17, 23, 1, 3, 15, 23);
        rec7 = Value1(Func_0200629c, 12);
        if ((s32)rec7 != 0) {
            Func_02006240((s32)rec7, 0);
            rec7[35] = 2;
        }
        Call1(Func_0200627e, 0x943);
    }
}
