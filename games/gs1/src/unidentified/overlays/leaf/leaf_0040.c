#include "types.h"

#define Func_02003f84 Func_02003f84

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
s32 Func_0200832a();
void Func_0200833c();
void Func_02008432();
void Func_02008442();
void Func_0200844a();
void Func_0200845a();

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

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void Func_02003f84(void)
{
    u32 i;
    s32 record;

    if (Value1(Func_0200832a, 0x96d) == 0) {
        Call1(Func_0200833c, 0x96d);
        Call1(Func_02008432, 0x2239);
        Func_0200844a(9, 0);
    } else {
        Call1(Func_02008442, 0x223a);
        Func_0200845a(9, 0);
    }
}
