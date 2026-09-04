#include "types.h"

#define FieldScene_RunScene37b_020022f4 Func_020022f4

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02002424();
s32 Func_02004734();
s32 Func_0200473e();
void Func_0200474e();
void Func_0200476a();
s32 Func_02004784();
s32 Func_020047b8();
void Func_020047ba();
void Func_020047c4();
void Func_020047ec();
void Func_0200482e();
void Func_02004848();
void Func_02004854();
void Func_02004866();
void Func_0200489a();

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

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunScene37b_020022f4(void)
{
    u32 i;
    s32 record;

    Func_0200474e();
    if (Value1(Func_02004734, 0x818) == 0) {
        if (Value1(Func_0200473e, 0x817) == 0) {
            Call2(Func_0200482e, 0x20000, 0x4000);
            Call4(Func_02004848, 0x11e0000, -1, 0x920000, 1);
            Func_02004854();
            Func_0200489a(186);
            Call6(Func_0200474e, 4, 59, 17, 38, 4, 3);
            if (Value1(Func_02004784, 0x816) != 0) {
                Call6(Func_0200476a, 8, 60, 17, 39, 2, 2);
            }
            Call3(Func_02004866, 0, 0x8000, 0);
            Func_020047c4(30);
            Call1(Func_020047ba, 0x817);
            if (Value1(Func_020047b8, 0x816) != 0) {
                Func_02002424();
            }
        }
    }
    Func_020047ec();
}
