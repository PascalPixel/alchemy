#include "types.h"

#define FieldScene_RunScene3ce_02000b10 Func_02000b10

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02001cc8();
s32 Func_02001d62();
s32 Func_02001d6c();
s32 Func_02001d76();
s32 Func_02001d80();
s32 Func_02001d86();
s32 Func_02001d92();
s32 Func_02001d9a();
s32 Func_02001d9c();
s32 Func_02001da6();
void Func_02001db0();
void Func_02001db4();
void Func_02001dba();
void Func_02001dc0();
void Func_02001dc6();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunScene3ce_02000b10(void)
{
    u32 i;
    s32 record;

    Call2(Func_02001cc8, 0xc1b, 1);
    Value2(Func_02001d62, 0, -100);
    Value2(Func_02001d6c, 1, -100);
    Value2(Func_02001d76, 2, -33);
    Value2(Func_02001d80, 3, -100);
    Value2(Func_02001d92, 0, -50);
    Value2(Func_02001d9c, 1, -40);
    Value2(Func_02001da6, 2, -35);
    Call2(Func_02001db0, 3, -20);
    record = Func_02001d86(0);
    *(u8 *)(record + 0x131) = 1;
    record = record + 0x140;
    *(u8 *)record = 1;
    record = Func_02001d9a(1);
    *(u8 *)((record + 0x130)) = 1;
    *(u8 *)(record + 0x131) = 2;
    Func_02001db4(0);
    Func_02001dba(1);
    Func_02001dc0(3);
    Func_02001dc6(2);
}
