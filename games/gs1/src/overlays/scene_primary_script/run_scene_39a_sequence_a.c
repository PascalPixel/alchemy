#include "types.h"

#define FieldScene_RunScene39aSequenceA Func_020017a8

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02003ab0();
void Func_02003ae4();
s32 Func_02003b02();
void Func_02003b18();
void Func_02003b20();
void Func_02003b34();
void Func_02003b84();

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

void FieldScene_RunScene39aSequenceA(void)
{
    u32 i;
    s32 record;

    Func_02003ae4();
    record = Func_02003b02(8);
    Func_02003ab0(record, 0);
    Func_02003b84();
    Call3(Func_02003b20, 0, 0x20000, 0x1999);
    Call3(Func_02003b34, 0, 0x108, 196);
    Func_02003b18();
}
