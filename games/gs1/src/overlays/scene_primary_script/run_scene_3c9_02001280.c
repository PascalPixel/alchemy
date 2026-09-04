#include "types.h"

#define FieldScene_RunScene3c9_02001280 Func_02001280

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02006f44();
void Func_02006f64();
s32 Func_02006fde();
s32 Func_02006ffe();
void Func_02007070();
void Func_02007090();

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

void FieldScene_RunScene3c9_02001280(s32 a0, s32 a1)
{
    u32 i;
    s32 record;

    if (a1 != 0) {
        Func_02007070(a0, 0);
        record = Func_02006fde(a0);
        Func_02006f44(record, 1);
        Call3(Func_02006ffe, a0, 0xcccc, 0x6666);
    } else {
        Func_02007090(a0, 15);
        record = Func_02006ffe(a0);
        Func_02006f64(record, 0);
    }
}
