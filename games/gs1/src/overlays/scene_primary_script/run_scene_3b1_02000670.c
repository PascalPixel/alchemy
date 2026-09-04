#include "types.h"

#define FieldScene_RunScene3b1_02000670 Func_02000670

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_020025f6();
s32 Func_02006b1e();
s32 Func_02006b38();
void Func_02006b3c();
s32 Func_02006b92();
void Func_02006bbc();
void Func_02006c30();
void Func_02006c3c();
void Func_02006c48();
void Func_02006c4a();
s32 Func_02006c5a();
void Func_02006c84();
void Func_02006c88();
void Func_02006ca0();
void Func_02006ca8();
void Func_02006cc0();

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

void FieldScene_RunScene3b1_02000670(void)
{
    u32 i;
    s32 record;

    Func_02006b3c();
    Func_02006cc0();
    if (Value1(Func_02006b1e, 0x921) != 0) {
        Call1(Func_02006c30, 0x1dd4);
        Func_02006c48(10, 0);
    } else {
        if (Value1(Func_02006b38, 0x922) != 0) {
            Call1(Func_02006c4a, 0x1d91);
            Value2(Func_02006c5a, 10, 0);
            if (Value2(Func_02006b92, 0, 0) == 0) {
                Func_020025f6();
                goto L_020006ea;
            }
            Func_02006c3c(10, 2);
            Func_02006c84(10, 0);
            Call3(Func_02006ca8, 10, 0xd000, 0);
        } else {
            Call1(Func_02006c88, 0x1d31);
            Func_02006ca0(10, 0);
        }
    }
    L_020006ea:;
    Func_02006bbc();
}
