#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
s32 Func_02004cfe();
s32 Func_02004d1e();
s32 Func_02004d38();
s32 Func_02004d98();
s32 Func_02004da6();
void Func_02004dc0();
s32 Func_02004dc6();
void Func_02004dca();
s32 Func_02004de0();
void Func_02004e08();
void Func_02004e0c();
void Func_02004e20();
void Func_02004e2a();
s32 Func_02004e40();
void Func_02004e42();
void Func_02004e46();
void Func_02004e68();
void Func_02004e6c();
void Func_02004e72();
void Func_02004e84();
void Func_02004e9e();
void Func_02004eb0();
void Func_02004eb4();
void Func_02004ec8();
void Func_02004ed2();
void Func_02004eea();
void Func_02004eee();
void Func_02004f14();
void Func_02004f2c();
void Func_02004f46();

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

void FieldScene_RunOpeningAuxiliarySequence(void)
{
    s32 rec7;
    s32 record;
    s32 shown;

    ((void (*)())Func_02004d38)();
    if (Value1(Func_02004d1e, 0x925) != 0) {
        Call1(Func_02004e08, 0x1e08);
        Func_02004e20(21, 0);
    } else {
        if (Value1(Func_02004d38, 0x922) != 0) {
            Func_02004e0c(21, 2);
            Call1(Func_02004e2a, 0x1d6f);
            Func_02004e42(21, 0);
            rec7 = Value1(Func_02004d98, 21);
            record = Func_02004cfe();
            shown = ((u32)(90 * record) >> 16) + 60;
            *(volatile u16 *)(rec7 + 100) = shown;
            Func_02004dca(21, 0x200c4d8);
        } else {
            Call3(Func_02004e9e, 21, 0x103, 0);
            Func_02004e46(21, 3);
            Call1(Func_02004e6c, 0x1d36);
            Func_02004e84(21, 0);
        }
    }
    Func_02004dc0();
}

