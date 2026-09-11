#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02006a5e();
void Func_02006ab6();
void Func_02006ae4();
void Func_02006aee();
void Func_02006afc();
s32 Func_02006b00();
void Func_02006b3a();
void Func_02006b6e();
void Func_02006bb6();
void Func_02006c6c();

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

void FieldScene_RunScene39c_02000ffc(s32 a0)
{
    u32 i;
    s32 record;

    Func_02006ab6();
    Func_02006c6c(228);
    Call3(Func_02006aee, 0, 0x6666, 0x3333);
    Func_02006bb6(0, 2);
    Call3(Func_02006b3a, 0, 0, -8);
    record = Func_02006b00(0);
    Func_02006a5e(record, 0);
    Func_02006ae4(8);
    Func_02006b6e(0, ((a0 << 19) + 0x80000), 0);
    Func_02006afc(30);
}
