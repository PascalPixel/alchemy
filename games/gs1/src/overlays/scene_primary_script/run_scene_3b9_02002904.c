#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_020054fc();
void Func_02005526();
void Func_02005568();
void Func_02005576();
void Func_0200557a();
void Func_0200557c();
void Func_02005632();
void Func_02005652();
void Func_02005670();
void Func_0200567c();

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

void FieldScene_RunScene3b9_02002904(void)
{
    u32 i;
    s32 record;

    Func_020054fc();
    Call3(Func_02005526, 0, 0x19999, 0xcccc);
    Func_02005632();
    Func_0200557a(0, 2);
    Call3(Func_02005568, 0, 0x320, 0x1ac);
    Call3(Func_02005576, 0, 0x2bc, 0x1ac);
    Call3(Func_0200557c, 0, 0x258, 0x1ac);
    Func_02005670();
    Func_0200567c();
    Func_02005652(22);
}
