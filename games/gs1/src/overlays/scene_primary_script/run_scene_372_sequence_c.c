#include "types.h"

#define FieldScene_RunScene372SequenceC Func_020016cc

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_00000ed0[];
extern u8 Data_03001ebc[];
void Func_02002e5e();
s32 Func_02005dae();
void Func_02005dd6();
void Func_02005dde();
s32 Func_02005dec();
void Func_02005e0c();
void Func_02005e14();
void Func_02005e2a();
void Func_02005ea0();
void Func_02005ec4();
void Func_02005edc();
void Func_02005ef2();
void Func_02005f00();

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

void FieldScene_RunScene372SequenceC(void)
{
    u32 i;
    s32 record;

    if (Value1(Func_02005dae, 0x841) != 0) {
        Func_02005dd6();
        Func_02005ea0(22, 0, 0);
        Func_02005dde(20);
        Func_02005ec4((s32)Data_00000ed0);
        Func_02005edc(22, 0);
        Call3(Func_02005f00, 22, 0xe000, 10);
        Func_02005e0c();
    } else {
        if (Value1(Func_02005dec, 0x837) == 0) {
            Func_02005e14();
            Call1(Func_02005ef2, 0xe6e);
            Func_02002e5e();
            Func_02005e2a();
        }
    }
}
