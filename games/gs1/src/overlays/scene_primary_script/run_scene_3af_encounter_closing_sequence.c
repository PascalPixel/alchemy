#include "types.h"

#define FieldScene_RunEncounterClosingSequence Func_02003f30

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_00002014[];
extern u8 Data_0200c918[];
void Func_02004b8a();
void Func_02007982();
void Func_0200799a();
s32 Func_02007a2e();
void Func_02007a6e();
void Func_02007a9e();
void Func_02007aac();
void Func_02007ac4();
void Func_02007adc();
void Func_02007afa();
void Func_02007b14();
void Func_02007b32();
void Func_02007b38();
void Func_02007b88();
void Func_02007b96();
s32 Func_02007ba6();
void Func_020081a0();
void Func_02008224();
void Func_0200823a();
void Func_02008272();
void Func_020082a6();
s32 Func_020082a8();
void Func_020082be();
void Func_02008306();
void Func_0200830e();
void Func_02008334();
s32 Func_02008344();
void Func_0200834c();
void Func_02008352();
void Func_0200835c();
void Func_0200835e();
void Func_02008370();
void Func_0200837e();
void Func_02008384();
s32 Func_0200839a();
void Func_020083b4();
void Func_020083bc();
void Func_020083c0();
void Func_020083ca();
void Func_020083e2();
void Func_020083ee();
void Func_020083fa();
void Func_02008404();
void Func_02008440();
void Func_02008446();
void Func_02008450();
void Func_02008458();
void Func_0200845e();
void Func_02008462();
void Func_02008466();
void Func_0200846c();
void Func_0200846e();
void Func_02008470();
void Func_0200847e();
void Func_02008480();
void Func_02008490();
void Func_0200849c();
s32 Func_0200849e();
void Func_020084aa();
void Func_020084ac();
void Func_020084ae();
void Func_020084cc();
void Func_020084d4();
void Func_020084dc();
void Func_02008506();
void Func_0200852a();
void Func_02008532();
void Func_020085f6();
void Func_0200860c();
void Func_02008618();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)0x03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunEncounterClosingSequence(void)
{
    u32 i;
    s32 record;
    s32 base6_2014;
    s32 base5_200c918;

    Func_02008224();
    Call1(Func_0200823a, 0x200d1d8);
    Func_020081a0(1);
    Func_02008384();
    Call3(Func_02008272, 0, 0x10000, 0x8000);
    Call3(Func_020082be, 0, 148, 0x290);
    Call3(Func_0200835c, 22, 0x100, 0);
    Func_0200830e(22, 1);
    Func_02007982(22, 0x5000);
    Call1(Func_02008334, 0x1f69);
    Value2(Func_02008344, 0x2016, 0);
    Call3(Func_02008370, 0, 0xe000, 0);
    if (Value2(Func_020082a8, 0, 0) == 1) {
        Call1(Func_0200799a, 0x2016);
        Func_020082a6();
    } else {
        bump_step(1);
        Value2(Func_0200839a, 0x2016, 0);
        Func_02004b8a();
        Call3(Func_0200834c, 26, 0xd80000, 0x24c0000);
        Call3(Func_02008306, 26, 0x13333, 0x9999);
        Call3(Func_02008352, 26, 216, 0x254);
        Call3(Func_0200835e, 26, 188, 0x268);
        Call3(Func_020083e2, 0, 0xe000, 0);
        Call3(Func_020083ee, 21, 0xd000, 0);
        Call3(Func_020083fa, 22, 0xd000, 0);
        Value2(Func_02007a2e, 26, 0x5000);
        Func_020083bc(26, 2, 0);
        Func_020083b4(26, 4);
        Func_02008404(26, 0);
        Call3(Func_020083c0, 20, 0xb40000, 0x3090000);
        Call3(Func_0200837e, 20, 0x10000, 0x8000);
        Call3(Func_020083ca, 20, 180, 0x298);
        base6_2014 = (s32)Data_00002014;
        Call3(Func_02008450, 20, 0xd000, 0);
        Func_02007a6e(base6_2014);
        Call3(Func_02008462, 0, 0x2000, 0);
        Call3(Func_0200846e, 22, 0x3000, 0);
        Call3(Func_02008490, 26, 0x101, 60);
        Func_02008440(20, 1);
        Func_02007a9e(base6_2014);
        Func_02008446(21, 2);
        Func_02007aac(21);
        Value3(Func_0200849e, 20, 0x5000, 20);
        Func_02008446(20, 3);
        Call1(Func_02007ac4, 0x6014);
        Func_02008466(26, 2, 20);
        Func_0200845e(26, 4);
        Func_02007adc(26);
        Call3(Func_02008458, 20, 182, 0x280);
        Call3(Func_020084dc, 20, 0xd000, 0);
        Call1(Func_02007afa, 0x8014);
        Call3(Func_02008506, 26, 0x100, 20);
        Func_020084ae(26, 2);
        Func_02007b14(26);
        Func_020084ac(20, 3);
        Func_02007b38(22, 0);
        Func_020084d4(22, 1);
        Func_02007b32(22);
        Call3(Func_0200846c, 22, 0x19999, 0xcccc);
        base5_200c918 = (s32)Data_0200c918;
        Func_0200847e(22, base5_200c918);
        Call3(Func_02008480, 21, 0x19999, 0xcccc);
        Call3(Func_020084cc, 21, 168, 0x278);
        Func_0200849c(21, base5_200c918);
        Func_02008462(80);
        Func_020084aa(26, base5_200c918);
        Func_02008470(40);
        Call2(Func_02007b96, 20, 0x8000);
        Func_02007b88(base6_2014);
        Value2(Func_02007ba6, 0, 0xe000);
        Func_0200852a(0, 3);
        Func_02008532(20, 3);
        *(s32 *)((*(u8 **)0x03001ebc + 0x1c0)) = 0x201;
        Func_0200860c();
        Func_02008618();
        Func_020085f6(17);
    }
}
