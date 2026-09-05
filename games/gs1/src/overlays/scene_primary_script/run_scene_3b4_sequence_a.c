#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_020039c8();
void Func_020039f2();
void Func_020039fc();
s32 Func_02003a00();
void Func_02003a0e();
void Func_02003a22();
void Func_02003a34();
void Func_02003a44();
void Func_02003a48();
void Func_02003a4a();
void Func_02003a5a();
void Func_02003a60();
void Func_02003a6e();
void Func_02003a80();
void Func_02003aba();
void Func_02003ae8();

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

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunScene3b4SequenceA(void)
{
    u32 i;
    s32 record;

    Func_020039c8();
    Call3(Func_020039f2, 0, 0x1b333, 0xd999);
    Call3(Func_020039fc, 9, 0x1b333, 0xd999);
    Func_02003aba(188);
    record = Value1(Func_02003a00, 0);
    if (record != 0) {
        Func_02003a22(9, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Func_02003a48(9);
    Func_02003a4a(0, 0, 24);
    Func_02003ae8(188);
    Func_02003a0e(4);
    Func_02003a60(9, 0, 16);
    Func_02003a6e(0);
    Call3(Func_02003a5a, 9, 168, 0x108);
    Func_02003a80(9);
    Func_02003a44();
    Call1(Func_02003a34, 0x220);
}
