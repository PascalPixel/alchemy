#include "types.h"

#define FieldScene_RunScene386_0200011c Func_0200011c

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_0200075c();
void Func_02000784();
void Func_020007ac();
void Func_020007ba();
void Func_020007bc();
void Func_020007c2();
void Func_020007c6();
void Func_020007c8();
s32 Func_020007dc();
void Func_020007ee();
void Func_020007f4();
void Func_02000806();
s32 Func_02000814();
void Func_0200081c();
void Func_02000822();
void Func_0200084a();

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

void FieldScene_RunScene386_0200011c(void)
{
    u32 i;
    s32 record;

    Func_0200075c();
    Call1(Func_020007ba, 0x1cd4);
    Func_020007bc(16, 0, 2);
    Func_020007ac(16, 1);
    Func_020007ee(16, 0, 20);
    Func_020007c6(16, 4);
    Func_02000784(20);
    Func_02000806(16, 0, 20);
    Call3(Func_02000822, 16, 0x102, 60);
    Func_0200081c(16, 0, 30);
    Value2(Func_02000814, 16, 0);
    if (Value2(Func_020007dc, 0, 0) != 0) {
        bump_step(1);
    }
    Func_0200084a(16, 0, 20);
    Call1(Func_020007c2, 0x300);
    Call1(Func_020007c8, 0x868);
    Func_020007f4();
}
