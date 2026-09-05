#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02005ac8();
void Func_02005b02();
void Func_02005b4c();
void Func_02005b62();
u8 *Func_02005ba0();
s32 Func_02005bac();
s32 Func_02005bae();
u8 *Func_02005bce();
void Func_02005bd0();
void Func_02005be0();
void Func_02005be2();
void Func_02005bee();
void Func_02005bf0();
void Func_02005bfa();
void Func_02005c00();

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

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunScene3af_0200185c(void)
{
    u32 i;
    u8 *record;

    Func_02005b4c();
    Call1(Func_02005b62, 0x200d160);
    Func_02005ac8(1);
    Func_02005be2(20, 0, 0);
    Call3(Func_02005bee, 23, 0xee0000, 0x2720000);
    Call3(Func_02005bfa, 22, 0xcc0000, 0x2090000);
    record = Func_02005ba0(22);
    *(volatile s32 *)((s32)record + 12) = 0x100000;
    *(u8 *)(Func_02005bac(22) + 89) |= 128;
    Call3(Func_02005bd0, 22, 0x9999, 0x4ccc);
    Call2(Func_02005be0, 22, 0x200c58c);
    {
        u8 *record = Func_02005bce(21);
        u8 value = *(volatile u8 *)&record[89];

        record[89] = (u8)(value | 128);
    }
    Call3(Func_02005bf0, 21, 0xcccc, 0x6666);
    Call2(Func_02005c00, 21, 0x200c628);
    if (Value1(Func_02005bae, 0x109) != 0) {
        Func_02005b02();
    }
    Func_02005be2();
}
