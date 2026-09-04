#include "types.h"

#define FieldScene_RunScene37b_02002244 Func_02002244

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02002372();
s32 Func_02004684();
s32 Func_0200468e();
void Func_0200469e();
void Func_020046ba();
s32 Func_020046d4();
s32 Func_02004706();
void Func_02004708();
void Func_02004712();
void Func_0200473a();
void Func_0200477e();
void Func_02004798();
void Func_020047a4();
void Func_020047b4();
void Func_020047ea();

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

void FieldScene_RunScene37b_02002244(void)
{
    u32 i;
    s32 record;

    Func_0200469e();
    if (Value1(Func_02004684, 0x818) == 0) {
        if (Value1(Func_0200468e, 0x816) == 0) {
            Call2(Func_0200477e, 0x20000, 0x4000);
            Call4(Func_02004798, 0x11e0000, -1, 0x920000, 1);
            Func_020047a4();
            Func_020047ea(186);
            Call6(Func_0200469e, 0, 59, 15, 38, 4, 3);
            if (Value1(Func_020046d4, 0x817) != 0) {
                Call6(Func_020046ba, 8, 60, 17, 39, 2, 2);
            }
            Func_020047b4(0, 0, 0);
            Func_02004712(30);
            Call1(Func_02004708, 0x816);
            if (Value1(Func_02004706, 0x817) != 0) {
                Func_02002372();
            }
        }
    }
    Func_0200473a();
}
