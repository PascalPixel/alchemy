#include "types.h"

#define FieldScene_RunScene372SequenceB Func_02001600

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
s32 Func_02005ce2();
s32 Func_02005cec();
void Func_02005d14();
void Func_02005d48();
void Func_02005d4a();
void Func_02005d6c();
void Func_02005dba();
void Func_02005df2();
void Func_02005e04();
void Func_02005e14();
void Func_02005e3e();
void Func_02005e42();

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

void FieldScene_RunScene372SequenceB(void)
{
    u32 i;
    s32 record;

    if (Value1(Func_02005ce2, 0x836) == 0) {
        if (Value1(Func_02005cec, 0x837) == 0) {
            Func_02005d14();
            Call1(Func_02005df2, 0xe6c);
            Func_02005e14(22, 0, 20);
            Call3(Func_02005e3e, 0, 0x101, 40);
            Call3(Func_02005dba, 0, 0x17e, 0x26b);
            Func_02005e04(0, 22, 0);
            Func_02005e04(0, 2);
            Func_02005d4a(30);
            Func_02005e42(22, 0);
            Call1(Func_02005d48, 0x836);
            Func_02005d6c();
        }
    }
}
