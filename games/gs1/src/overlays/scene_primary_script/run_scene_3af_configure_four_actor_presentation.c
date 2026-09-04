#include "types.h"

#define FieldScene_ConfigureFourActorPresentation Func_02003a0c

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_00006014[];
extern u8 Data_0200db50[];
extern u8 Data_0200db60[];
extern u8 Data_03001ebc[];
void Func_020074c2();
void Func_020074da();
void Func_02007514();
void Func_020075a8();
s32 Func_020075ae();
void Func_020075c0();
void Func_020075ce();
void Func_020075da();
void Func_02007c86();
void Func_02007cf0();
void Func_02007cfc();
void Func_02007d20();
void Func_02007d32();
s32 Func_02007d48();
s32 Func_02007d62();
s32 Func_02007d7a();
s32 Func_02007d94();
void Func_02007da2();
void Func_02007dbc();
void Func_02007dd0();
void Func_02007dd4();
void Func_02007de6();
void Func_02007e1c();
void Func_02007e26();
void Func_02007e5c();
void Func_02007e74();
void Func_02007e7c();
void Func_02007e94();
void Func_02007e96();
void Func_02007e9e();
void Func_02007eb6();
void Func_02007ec0();
void Func_02007ece();
void Func_02007eda();
void Func_02007ede();
void Func_02007ee2();
void Func_02007ef6();
void Func_02007efc();
void Func_02007f02();
void Func_02007f06();
void Func_02007f12();
void Func_02007f26();
void Func_02007f30();
void Func_02007f3a();
void Func_02007f40();
void Func_02007f42();
void Func_02007f4a();
void Func_02007f56();
void Func_02007f5c();
void Func_02007f60();
void Func_02007f6c();
void Func_02007f78();
void Func_02007f7c();
void Func_02007fb4();
void Func_02007fd8();
void Func_02008060();
void Func_02008076();
void Func_02008082();

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */
#define BattleRuntime_Reset_1(args...) Func_02007cfc(args)
#define Object_NotifyLastActiveOfEvent_1(a0) Call1(Func_02007d20, a0)
#define ObjectMotion_SetHorizontalPositionWithTerrain_1(a0, a1, a2) Call3(Func_02007da2, a0, a1, a2)
#define Scene_GetRecord_1(args...) Func_02007d48(args)
#define ObjectMotion_SetHorizontalPositionWithTerrain_2(a0, a1, a2) Call3(Func_02007dbc, a0, a1, a2)
#define Scene_GetRecord_2(args...) Func_02007d62(args)
#define ObjectMotion_SetHorizontalPositionWithTerrain_3(a0, a1, a2) Call3(Func_02007dd4, a0, a1, a2)
#define Scene_GetRecord_3(args...) Func_02007d7a(args)
#define ObjectMotion_SetHorizontalPositionWithTerrain_4(args...) Func_02007de6(args)
#define ObjectGroup_ConfigureChildValue_1(args...) Func_02007e26(args)
#define Scene_GetRecord_4(args...) Func_02007d94(args)
#define BattleRuntime_WaitIfModeZero_1(args...) Func_02007ee2(args)
#define ObjectMotion_SetSpeedParameters_1(args...) Func_02007ef6(args)
#define ObjectMotion_SetSpeedParameters_2(a0, a1, a2) Call3(Func_02007dd0, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_1(a0, a1, a2) Call3(Func_02007e1c, a0, a1, a2)
#define ObjectMotion_ArmCallback_1(args...) Func_02007e9e(args)
#define SceneWork_SetStepValue_1(a0) Call1(Func_02007e7c, a0)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_1(args...) Func_02007e74(args)
#define Object_SetModeById_1(args...) Func_02007e5c(args)
#define ObjectMotion_ArmCallback_2(a0, a1, a2) Call3(Func_02007ece, a0, a1, a2)
#define ObjectMotion_ArmCallback_3(a0, a1, a2) Call3(Func_02007eda, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_1(a0, a1, a2) Call3(Func_02007efc, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_2(a0, a1, a2) Call3(Func_02007f06, a0, a1, a2)
#define Object_SetModeById_2(args...) Func_02007e96(args)
#define BattleRuntime_WaitIfModeZero_2(a0, a1) Call2(Func_02007f26, a0, a1)
#define BattleRuntime_WaitIfModeZero_3(a0, a1) Call2(Func_02007f30, a0, a1)
#define BattleRuntime_WaitIfModeZero_4(args...) Func_02007e26(args)
#define BattleEffect_SpawnLinkedResourceObject_3(a0, a1, a2) Call3(Func_02007f3a, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_3(a0, a1, a2) Call3(Func_02007e74, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_2(a0, a1, a2) Call3(Func_02007ec0, a0, a1, a2)
#define ObjectMotion_ArmCallback_4(args...) Func_02007f42(args)
#define ObjectMotion_SetSpeedParameters_4(a0, a1, a2) Call3(Func_02007e94, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_3(a0, a1, a2) Call3(Func_02007ede, a0, a1, a2)
#define ObjectMotion_ArmCallback_5(args...) Func_02007f60(args)
#define ObjectMotion_SetSpeedParameters_5(a0, a1, a2) Call3(Func_02007eb6, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_4(a0, a1, a2) Call3(Func_02007f02, a0, a1, a2)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_2(args...) Func_02007f4a(args)
#define ObjectMotion_CallThenWaitForAnimationChange_1(args...) Func_02007f40(args)
#define ObjectMotion_ArmCallback_6(a0, a1, a2) Call3(Func_02007fb4, a0, a1, a2)
#define Object_SetModeById_3(args...) Func_02007f5c(args)
#define ObjectMotion_SetPositionAndReset_5(a0, a1, a2) Call3(Func_02007f56, a0, a1, a2)
#define ObjectMotion_ArmCallback_7(args...) Func_02007fd8(args)
#define ObjectMotion_SetPositionAndReset_6(a0, a1, a2) Call3(Func_02007f6c, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_7(a0, a1, a2) Call3(Func_02007f78, a0, a1, a2)
#define ObjectMotion_ResetAndSetPositionInMode2_1(a0, a1, a2) Call3(Func_02007f7c, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_5(args...) Func_02007f12(args)
#define ObjectMotion_SetSpeedParameters_6(args...) Func_02008082(args)

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

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_ConfigureFourActorPresentation(void)
{
    u32 i;
    s32 record;
    s32 base6_6014;

    BattleRuntime_Reset_1();
    *(volatile s32 *)Data_0200db50 = 0x40000;
    *(volatile s32 *)Data_0200db60 = -0x8000;
    Object_NotifyLastActiveOfEvent_1(0x200d160);
    Func_02007c86(1);
    ObjectMotion_SetHorizontalPositionWithTerrain_1(21, 0xb60000, 0x26a0000);
    record = Scene_GetRecord_1(21);
    {
        s32 shown = 0xc000;

        *(volatile u16 *)(record + 6) = shown;
    }
    ObjectMotion_SetHorizontalPositionWithTerrain_2(20, 0xda0000, 0x2040000);
    record = Scene_GetRecord_2(20);
    {
        s32 shown = 0xb000;

        *(volatile u16 *)(record + 6) = shown;
    }
    ObjectMotion_SetHorizontalPositionWithTerrain_3(22, 0xcc0000, 0x20e0000);
    record = Scene_GetRecord_3(22);
    {
        s32 shown = 0xb000;

        *(volatile u16 *)(record + 6) = shown;
    }
    ObjectMotion_SetHorizontalPositionWithTerrain_4(23, 0, 0);
    ObjectGroup_ConfigureChildValue_1(0, 15);
    record = Scene_GetRecord_4(0);
    Func_02007d32(record, 0);
    Func_02007cf0(1);
    *(s32 *)((*(u8 *volatile *)Data_03001ebc + 0x1c0)) = 0x202;
    BattleRuntime_WaitIfModeZero_1();
    ObjectMotion_SetSpeedParameters_1();
    ObjectMotion_SetSpeedParameters_2(21, 0xcccc, 0x6666);
    ObjectMotion_SetPositionAndReset_1(21, 182, 0x214);
    ObjectMotion_ArmCallback_1(21, 0xb000, 40);
    SceneWork_SetStepValue_1(0x1f23);
    Func_020074c2(21);
    base6_6014 = (s32)Data_00006014;
    ObjectMotion_SetVariantCallbackAndInvokeObject_1(20, 2);
    Object_SetModeById_1(20, 4);
    Func_020074da(base6_6014);
    ObjectMotion_ArmCallback_2(21, 0xd000, 0);
    ObjectMotion_ArmCallback_3(22, 0xd000, 0);
    BattleEffect_SpawnLinkedResourceObject_1(21, 0x101, 0);
    BattleEffect_SpawnLinkedResourceObject_2(22, 0x101, 60);
    Object_SetModeById_2(20, 3);
    Func_02007514(base6_6014);
    BattleRuntime_WaitIfModeZero_2(21, 0x102);
    BattleRuntime_WaitIfModeZero_3(22, 0x102);
    BattleRuntime_WaitIfModeZero_4(80);
    BattleEffect_SpawnLinkedResourceObject_3(21, 0x100, 20);
    ObjectMotion_SetSpeedParameters_3(21, 0x19999, 0xcccc);
    ObjectMotion_SetPositionAndReset_2(21, 194, 0x1f4);
    ObjectMotion_ArmCallback_4(21, 0xb000, 20);
    ObjectMotion_SetSpeedParameters_4(22, 0xcccc, 0x6666);
    ObjectMotion_SetPositionAndReset_3(22, 192, 0x206);
    ObjectMotion_ArmCallback_5(22, 0xb000, 0);
    ObjectMotion_SetSpeedParameters_5(20, 0x10000, 0x8000);
    ObjectMotion_SetPositionAndReset_4(20, 210, 0x1fc);
    Value2(Func_020075ae, 20, 0xb000);
    ObjectMotion_SetVariantCallbackAndInvokeObject_2(21, 1);
    Call1(Func_020075a8, 0x5015);
    ObjectMotion_CallThenWaitForAnimationChange_1(20, 3);
    Call2(Func_020075ce, 22, 0xd000);
    Call1(Func_020075c0, 0x9016);
    ObjectMotion_ArmCallback_6(20, 0x8000, 20);
    Object_SetModeById_3(20, 4);
    Call1(Func_020075da, 0xa014);
    ObjectMotion_SetPositionAndReset_5(20, 204, 0x218);
    ObjectMotion_ArmCallback_7(22, 0xb000, 0);
    ObjectMotion_SetPositionAndReset_6(20, 182, 0x224);
    ObjectMotion_SetPositionAndReset_7(20, 182, 0x250);
    ObjectMotion_ResetAndSetPositionInMode2_1(20, 182, 0x298);
    BattleRuntime_WaitIfModeZero_5(40);
    Func_02008076();
    ObjectMotion_SetSpeedParameters_6();
    Func_02008060(16);
}
