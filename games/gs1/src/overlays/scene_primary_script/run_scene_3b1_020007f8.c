#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02005086();
void Func_020050b0();
s32 Func_020050ce();
s32 Func_02006ca2();
void Func_02006cc4();
void Func_02006cfe();
s32 Func_02006d14();
void Func_02006d60();
void Func_02006d86();
void Func_02006dac();
void Func_02006dbc();
s32 Func_02006ddc();
void Func_02006e0e();
void Func_02006e28();
void Func_02006e2c();
void Func_02006e44();
void Func_02006e4c();

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

void FieldScene_RunScene3b1_020007f8(void)
{
    u32 i;
    s32 record;

    Func_02006cc4();
    if (Value1(Func_02006ca2, 0x925) != 0) {
        Func_02006d86(8, 2);
        Call1(Func_02006dbc, 0x1e13);
        Func_02005086(8);
        Func_02006dac(8, 0, 10);
        Value2(Func_02006ddc, 8, 0);
        if (Value2(Func_02006d14, 0, 0) == 0) {
            Func_02006cfe(40);
            Func_020050b0(8);
            Value2(Func_020050ce, 8, 0x3000);
            Func_02006e0e(8, 0);
            goto L_0200088e;
        }
        bump_step(2);
        Func_02006e28(8, 0);
        Call3(Func_02006e4c, 8, 0x3000, 0);
    } else {
        Call1(Func_02006e2c, 0x1d4e);
        Func_02006e44(8, 0);
    }
    L_0200088e:;
    Func_02006d60();
}
