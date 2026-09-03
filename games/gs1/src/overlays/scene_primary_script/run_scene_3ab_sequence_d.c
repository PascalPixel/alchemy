#include "types.h"

#define FieldScene_BuildFlaggedMapLayout Func_02000d30

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
void Func_02002686();
void Func_020026c8();
s32 Func_020026fe();
void Func_02002702();
s32 Func_0200270a();
void Func_02002716();
void Func_0200272a();
void Func_0200273c();
void Func_02002740();
void Func_02002752();
void Func_02002764();
void Func_02002776();
void Func_0200277a();
void Func_0200278c();
void Func_0200279a();
void Func_0200279e();
void Func_020027b0();
void Func_020027b4();
void Func_020027c6();
void Func_020027d8();
void Func_020027ee();
void Func_02002800();
void Func_02002812();
void Func_0200285e();
void Func_02002890();

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

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)0x03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_BuildFlaggedMapLayout(void)
{
    u32 i;
    u8 *record;

    if (Value1(Func_020026fe, 0x201) != 0) {
    } else {
        if (Value1(Func_0200270a, 0x202) != 0) {
        } else {
            Func_0200279a(19, 0, 0);
            Func_02002890(210);
            Func_02002686(1);
            Call6(Func_02002702, 32, 45, 3, 4, 1, 14);
            Call6(Func_02002716, 35, 45, 3, 4, 33, 14);
            Call6(Func_0200272a, 38, 45, 3, 4, 1, 46);
            Func_020026c8(10);
            Call6(Func_02002740, 41, 45, 3, 4, 1, 14);
            Call6(Func_02002752, 44, 45, 3, 4, 33, 14);
            Call6(Func_02002764, 47, 45, 3, 4, 1, 46);
            Func_02002702(10);
            Call6(Func_0200277a, 50, 45, 3, 4, 1, 14);
            Call6(Func_0200278c, 53, 45, 3, 4, 33, 14);
            Call6(Func_0200279e, 56, 45, 3, 4, 1, 46);
            Func_0200273c(10);
            Call6(Func_020027b4, 32, 49, 3, 4, 1, 14);
            Call6(Func_020027c6, 35, 49, 3, 4, 33, 14);
            Call6(Func_020027d8, 38, 49, 3, 4, 1, 46);
            Func_02002776(10);
            Call6(Func_020027ee, 41, 49, 3, 4, 1, 14);
            Call6(Func_02002800, 44, 49, 3, 4, 33, 14);
            Call6(Func_02002812, 47, 49, 3, 4, 1, 46);
            Func_020027b0(10);
            Call1(Func_0200285e, 0x202);
        }
    }
}
