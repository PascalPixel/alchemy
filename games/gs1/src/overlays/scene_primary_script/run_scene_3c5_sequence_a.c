#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02001c0a();
void Func_02003c4e();
void Func_02003c78();
void Func_02003cea();
void Func_02003cf6();
s32 Func_02003d24();
s32 Func_02003d30();
s32 Func_02003d3c();
void Func_02003d44();
s32 Func_02003d46();
s32 Func_02003d50();
s32 Func_02003d70();
s32 Func_02003d7c();
void Func_02003d86();
void Func_02003d8c();
void Func_02003de4();
void Func_02003e0a();
void Func_02003e5a();
void Func_02003e6e();
void Func_02003e78();
void Func_02003eaa();

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

void FieldScene_RunScene3c5SequenceA(s32 a0)
{
    u32 i;
    s32 record;
    s32 v5;

    Func_02003cea();
    Call4(Func_02003de4, -1, -1, -1, 0);
    Func_02003c78();
    Func_02003c4e(1);
    record = Func_02003d24(0);
    *(volatile s32 *)(record + 12) = 0x820000;
    record = Func_02003d30(0);
    *(volatile s32 *)(record + 72) = 0x4000;
    v5 = 0;
    record = Func_02003d3c(0);
    *(volatile s32 *)(record + 68) = v5;
    *(u8 *)(Func_02003d46(0) + 85) = v5;
    record = Func_02003d50(0);
    Func_02003cf6(record, 0);
    Func_02003e5a();
    Func_02003e6e();
    Func_02003d44(10);
    Func_02003eaa(204);
    *(u8 *)(Func_02003d70(0) + 85) = 3;
    record = Func_02003d7c(0);
    *(volatile s32 *)(record + 40) = -0x50000;
    Func_02003d86(0);
    Func_02001c0a();
    Func_02003e0a(0, 15);
    Func_02003e78(a0);
    Func_02003d8c();
}
