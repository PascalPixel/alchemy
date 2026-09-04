#include "types.h"

#define FieldScene_RunScene3bf_02002718 Func_02002718

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02007d7e();
void Func_02007d98();
void Func_02007db2();
void Func_02007dcc();
void Func_02007de6();
void Func_02007e04();
void Func_02007e16();
void Func_02007e28();
void Func_02007e80();
void Func_02007e92();
void Func_02007ea4();
void Func_02007eb0();

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

void FieldScene_RunScene3bf_02002718(void)
{
    u32 i;
    s32 record;

    Call3(Func_02007d7e, 2, 0x10000, 0x8000);
    Call3(Func_02007db2, 2, 0x1f8, 216);
    Call3(Func_02007d98, 3, 0x10000, 0x8000);
    Call3(Func_02007dcc, 3, 0x1b8, 232);
    Call3(Func_02007db2, 1, 0x10000, 0x8000);
    Call3(Func_02007de6, 1, 0x1e0, 224);
    Func_02007e04(1);
    Call3(Func_02007e80, 1, 0xc000, 0);
    Func_02007e16(2);
    Call3(Func_02007e92, 2, 0xc000, 0);
    Func_02007e28(3);
    Call3(Func_02007ea4, 3, 0xc000, 0);
    Call3(Func_02007eb0, 0, 0xc000, 0);
}
