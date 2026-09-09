#include "types.h"

#define FieldScene_RunScriptedStep1472 Func_020001ec
#define FieldScene_RunScriptedStep146E Func_02000208
#define SceneDialogue_RunLine1470 Func_02000224
#define FieldScene_RunScene38b_02000240 Func_02000240
#define SceneDialogue_RunActorTwelveDialogue Func_02000288
#define SceneDialogue_RunActorFourteenDialogue Func_020002a8
#define SceneDialogue_ShowLine16BF Func_020002c8
#define SceneDialogue_RunActorSixteenDialogue Func_020002e8
#define SceneDialogue_ShowLine16CC Func_02000308
extern u8 Data_03001ebc[];

void Func_020090b8(void);
void Func_02009090(s32, s32);
void Func_020090c0(void);
s32 Func_02009098();
void Func_020090b8_unprototyped();
void Func_020090a0();
void Func_020090c0_unprototyped();
void Func_02009130();
void Func_02009138();
void Func_02009130_args1(s32);
s32 Func_02009148(s32, s32);

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunScriptedStep1472(void)
{
    Func_020090b8();
    Func_02009090(0x1472, 1);
    Func_020090c0();
}

void FieldScene_RunScriptedStep146E(void)
{
    Func_020090b8();
    Func_02009090(0x146E, 1);
    Func_020090c0();
}

void SceneDialogue_RunLine1470(void)
{
    Func_020090b8();
    Func_02009090(0x1470, 1);
    Func_020090c0();
}

void FieldScene_RunScene38b_02000240(void)
{
    Func_020090b8_unprototyped();
    Call1(Func_02009130, 0x13ae);
    if (Value1(Func_02009098, 0x301) != 0) {
        bump_step(1);
    }
    Func_02009138(9, 0);
    Call1(Func_020090a0, 0x301);
    Func_020090c0_unprototyped();
}

void SceneDialogue_RunActorTwelveDialogue(void)
{
    Func_020090b8();
    Func_02009130_args1(0x13B3);
    Func_02009148(12, 0);
    Func_020090c0();
}

void SceneDialogue_RunActorFourteenDialogue(void)
{
    Func_020090b8();
    Func_02009130_args1(0x13B7);
    Func_02009148(14, 0);
    Func_020090c0();
}

void SceneDialogue_ShowLine16BF(void)
{
    Func_020090b8();
    Func_02009130_args1(0x16BF);
    Func_02009148(21, 0);
    Func_020090c0();
}

void SceneDialogue_RunActorSixteenDialogue(void)
{
    Func_020090b8();
    Func_02009130_args1(0x16C8);
    Func_02009148(16, 0);
    Func_020090c0();
}

void SceneDialogue_ShowLine16CC(void)
{
    Func_020090b8();
    Func_02009130_args1(0x16CC);
    Func_02009148(18, 0);
    Func_020090c0();
}
