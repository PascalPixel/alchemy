#include "types.h"

#define FieldScene_RunScene3b8_02000264 Func_02000264

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_00002241[];
extern u8 Data_03001ebc[];
s32 Func_02004614();
void Func_02004626();
void Func_0200462c();
void Func_0200464a();
void Func_02004684();
void Func_020046a6();
s32 Func_020046bc();
void Func_020046c2();
void Func_020046d0();
void Func_020046d2();
void Func_0200470e();
void Func_02004718();
void Func_02004740();
void Func_0200474c();
void Func_02004756();
void Func_0200475a();
s32 Func_0200475c();
void Func_0200476c();
void Func_02004774();
void Func_02004776();
void Func_0200478c();
void Func_020047bc();
void Func_020047cc();
void Func_02004842();
void Func_0200484c();

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

void FieldScene_RunScene3b8_02000264(s32 a0)
{
    u32 i;
    s32 record;
    s32 base6_2241;

    Func_0200464a();
    Func_02004776();
    if (Value1(Func_02004614, 0x966) == 0) {
        Call1(Func_02004626, 0x966);
        Call1(Func_0200462c, 0x967);
        Call3(Func_02004740, a0, 0x4000, 0);
        Func_020046d2(0, 120, 96);
        Call3(Func_02004756, 0, 0xc000, 0);
        Func_02004684(20);
        base6_2241 = (s32)Data_00002241;
        Func_0200474c(base6_2241);
        Value2(Func_0200475c, a0, 0);
        if (Value2(Func_020046bc, 0, 0) == 0) {
            Func_020046a6(10);
            Func_0200476c((base6_2241 + 1));
        } else {
            Func_02004774((base6_2241 + 2));
        }
        Func_0200478c(a0, 0);
        Func_020046c2(10);
        Func_0200475a(a0, 3);
        Func_020046d0(20);
        Call3(Func_0200470e, a0, 0x10000, 0x8000);
        Call3(Func_02004842, a0, -64, 0);
        Func_0200484c(a0, 0, 48);
    } else {
        Call1(Func_020047bc, 0x2245);
        Func_020047cc(a0, 0);
    }
    Func_02004718();
}
