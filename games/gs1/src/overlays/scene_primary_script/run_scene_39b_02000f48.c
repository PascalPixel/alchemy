#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_0200340a();
void Func_0200342a();
void Func_02003452();
void Func_02003458();
s32 Func_02003464();
void Func_02003470();
void Func_0200348e();
void Func_020034c2();
void Func_020034d2();
void Func_02003568();

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

void FieldScene_RunScene39b_02000f48(s32 a0)
{
    u32 i;
    s32 record;

    Func_0200342a();
    Func_02003568(228);
    Call3(Func_02003452, 0, 0x6666, 0x3333);
    Func_020034d2(0, 2);
    Call3(Func_0200348e, 0, 0, -8);
    record = Func_02003464(0);
    Func_0200340a(record, 0);
    Func_02003458(8);
    Func_020034c2(0, ((a0 << 19) + 0x80000), 0);
    Func_02003470(30);
}
