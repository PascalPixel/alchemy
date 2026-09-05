#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_020022aa();
void Func_020022de();
void Func_020022ec();
s32 Func_0200230a();
void Func_02003240();
void Func_020032ae();
void Func_020032da();
void Func_020032f0();
void Func_0200330a();
void Func_02003314();
void Func_0200332a();
void Func_02003336();
void Func_02003342();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
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

void FieldScene_RunScene3b9_02000648(void)
{
    u32 i;
    s32 record;

    Func_02003240();
    Func_020032da(18, 0, 20);
    Call1(Func_020032f0, 0x2122);
    Func_020022aa(18);
    Call3(Func_0200332a, 18, 0xd000, 20);
    Call3(Func_02003336, 18, 0xb000, 20);
    Call3(Func_02003342, 18, 0x8000, 40);
    Func_02003314(18, 0, 20);
    Func_020022de(18);
    Func_0200330a(18, 3);
    Func_020022ec(18);
    Value2(Func_0200230a, 18, 0x5000);
    Func_020032ae();
}
