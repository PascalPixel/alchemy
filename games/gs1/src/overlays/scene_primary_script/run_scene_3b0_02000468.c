#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02000950();
void Func_02001646();
void Func_02001650();
void Func_02001672();
void Func_02001680();
s32 Func_020016ba();
void Func_020016c0();
void Func_020016dc();
void Func_02001704();

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

void FieldScene_RunScene3b0_02000468(void)
{
    u32 i;
    s32 record;

    Func_02001680();
    Call3(Func_020016dc, 0, 0xa40000, 0x1410000);
    Func_02001704(0, 15);
    record = Func_020016ba(0);
    Func_02001680(record, 0);
    Func_02001646(1);
    Func_02001672();
    Func_02001650(1);
    Func_02000950();
    Func_020016c0();
}
