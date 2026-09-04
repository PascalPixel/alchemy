#include "types.h"

#define FieldScene_RunScene3b2_02001494 Func_02001494

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_0000007e[];
extern u8 Data_02000240[];
extern u8 Data_03001ebc[];
extern s16 Data_02000240_t[][1];
void Func_020044f2();
void Func_02004508();
void Func_02004522();
void Func_02004530();
void Func_02004532();
void Func_02004560();
void Func_0200457e();
void Func_02004596();
void Func_020045a2();
void Func_020045bc();

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

void FieldScene_RunScene3b2_02001494(void)
{
    u32 i;
    s32 record;

    Func_02004508();
    Call3(Func_02004532, 0, 0x6666, 0x3333);
    Func_02004596();
    Func_020045a2();
    Func_02004522((Data_02000240_t[224][0] + (0x8c8 - (s32)Data_0000007e)));
    Func_02004530(30);
    Call3(Func_020044f2, 0x200b2bc, 44, 7);
    Call3(Func_0200457e, 0, 3, -16);
    Func_020045bc(3);
    Func_02004560();
}
