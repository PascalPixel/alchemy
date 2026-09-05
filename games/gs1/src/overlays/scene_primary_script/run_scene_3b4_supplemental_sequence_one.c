#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_020038b8();
void Func_020038e2();
void Func_020038ec();
s32 Func_020038f0();
void Func_020038f8();
void Func_02003912();
void Func_02003924();
void Func_02003934();
void Func_02003938();
void Func_0200393a();
void Func_0200394a();
void Func_02003950();
void Func_0200395e();
void Func_02003970();
void Func_020039aa();
void Func_020039de();

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

void FieldScene_RunSupplementalSequenceOne(void)
{
    u32 i;
    s32 record;

    Func_020038b8();
    Call3(Func_020038e2, 0, 0x1e666, 0xf333);
    Call3(Func_020038ec, 8, 0x1e666, 0xf333);
    Func_020039aa(188);
    record = Value1(Func_020038f0, 0);
    if (record != 0) {
        Func_02003912(8, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Func_02003938(8);
    Func_0200393a(0, 0, 24);
    Func_020038f8(4);
    Func_020039de(188);
    Func_02003950(8, 0, 16);
    Func_0200395e(0);
    Call3(Func_0200394a, 8, 0x168, 152);
    Func_02003970(8);
    Func_02003934();
    Call1(Func_02003924, 0x220);
}
