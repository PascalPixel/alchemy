#include "types.h"

#define FieldScene_RunScene3af_020011c8 Func_020011c8

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_020024d8();
void Func_02004c26();
s32 Func_0200549a();
s32 Func_020054a4();
void Func_020054cc();
void Func_020054fa();
s32 Func_02005504();
void Func_0200550e();
void Func_02005510();
s32 Func_02005528();
void Func_02005548();
s32 Func_02005556();
void Func_0200555c();
void Func_02005560();
void Func_0200556c();
u8 *Func_02005576();
void Func_0200559e();
void Func_020055b2();
void Func_020055ba();
void Func_020055c0();
void Func_020055e8();
void Func_020055f4();
void Func_02005612();
void Func_02005638();
void Func_02005684();
void Func_020056a6();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

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

void FieldScene_RunScene3af_020011c8(void)
{
    u32 i;
    u8 *record;

    if (Value1(Func_0200549a, 0x911) != 0) {
        if (Value1(Func_020054a4, 0x922) == 0) {
            Func_020054cc();
            Func_02005638();
            Func_020024d8();
            Call3(Func_0200550e, 20, 0x6666, 0x3333);
            *(u8 *)(Func_02005504(20) + 90) &= 254;
            Call3(Func_0200556c, 20, 202, 0x330);
            Func_020054fa(1);
            *(u8 *)(Func_02005528(20) + 90) |= 1;
            Func_02005510(20);
            Func_020055c0(20, 2);
            Func_02004c26(20);
            Call3(Func_02005560, 20, 0x13333, 0x9999);
            *(u8 *)(Func_02005556(20) + 90) &= 254;
            Call3(Func_020055ba, 20, 192, 0x324);
            Func_02005548(1);
            {
                u8 *record = Func_02005576(20);
                u8 value = *(volatile u8 *)&record[90];

                record[90] = (u8)(value | 1);
            }
            Func_0200555c(20);
            Call3(Func_0200559e, 20, 0x33333, 0x19999);
            Call3(Func_020055e8, 20, 180, 0x30a);
            Call3(Func_020055f4, 20, 180, 0x2bc);
            Call3(Func_02005612, 20, 0xf60000, 0x2000000);
            Func_02005684(20, 0, 0);
            Call3(Func_020056a6, 0, 0x101, 60);
            Func_020055b2();
        }
    }
}
