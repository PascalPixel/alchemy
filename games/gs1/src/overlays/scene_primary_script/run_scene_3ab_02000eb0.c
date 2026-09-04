#include "types.h"

#define FieldScene_RunScene3ab_02000eb0 Func_02000eb0

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_02000240[];
extern u8 Data_03001ebc[];
extern s16 Data_02000240_t[][2];
void Func_020028a6();
void Func_020028bc();
void Func_020028f2();
void Func_02002904();
void Func_0200290e();
void Func_02002916();
void Func_02002918();
void Func_0200291a();
void Func_02002934();
void Func_02002936();
void Func_0200293c();
void Func_0200295a();
void Func_02002972();
void Func_02002974();
void Func_0200297e();
void Func_020029ae();
void Func_020029c6();
void Func_020029c8();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
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

void FieldScene_RunScene3ab_02000eb0(void)
{
    u32 i;
    s32 record;

    if (Data_02000240_t[147][0] == 0) {
        Func_020028a6();
        Call3(Func_02002972, 8, 0x100, 2);
        Call3(Func_0200297e, 9, 0x100, 15);
        Func_020028bc(30);
        Func_0200290e(8, 152, 168);
        Func_02002918(9, 168, 168);
        Func_02002936(8);
        Func_0200293c(9);
        Func_0200291a(8);
        Func_0200295a(8, 0);
        Call3(Func_020029ae, 8, 0x3000, 0);
        Func_02002934(9);
        Func_02002974(9, 0);
        Call3(Func_020029c8, 9, 0x5000, 0);
        Call1(Func_020029ae, 0x24da);
        Func_020029c6(8, 0);
        Call1(Func_02002916, 0x240);
        Call6(Func_020028f2, 6, 11, 1, 1, 7, 11);
        Call6(Func_02002904, 6, 11, 1, 1, 8, 11);
        Call6(Func_02002916, 6, 11, 1, 1, 9, 11);
        Func_02002972();
    }
}
