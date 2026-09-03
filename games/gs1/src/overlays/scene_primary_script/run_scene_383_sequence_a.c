#include "types.h"

#define FieldScene_RunSetupSequence Func_02000b48

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
void Func_02001b36();
void Func_020051f2();
void Func_02005222();
void Func_02005228();
void Func_02005232();
void Func_0200525c();
void Func_0200525e();
void Func_0200526c();
void Func_02005274();
void Func_02005290();
void Func_020052aa();
void Func_020052b6();
void Func_020052c6();
void Func_020052d0();
void Func_020052ec();
void Func_020052fe();
void Func_02005346();
void Func_02005890();
s32 Func_020058ce();
void Func_020058da();
void Func_020058e4();
void Func_020058ee();
void Func_02005912();
void Func_02005928();
void Func_02005932();
void Func_02005950();
void Func_0200595a();
void Func_0200595e();
void Func_02005964();
void Func_02005992();
void Func_020059a0();
void Func_020059b6();
void Func_020059de();
void Func_020059e6();
void Func_020059f4();
void Func_020059f8();
void Func_020059fa();
void Func_02005a0a();
void Func_02005a0c();
void Func_02005a3c();
void Func_02005a54();
void Func_02005a58();
void Func_02005a6c();
void Func_02005a70();
void Func_02005a74();
void Func_02005a7c();
void Func_02005a7e();
void Func_02005a94();
void Func_02005aa0();
s32 Func_02005abe();
u8 *ObjectMotion_ArmCallback_5();
void Func_02005af6();
void Func_02005b0a();
void Func_02005b12();
void Func_02005b14();
void Func_02005b16();
u8 *Scene_GetRecord_2();
void Func_02005b1e();
void Func_02005b34();
void Func_02005b42();
void Func_02005b48();
void Func_02005b4e();
void Func_02005b54();
void Func_02005b5e();
void Func_02005b6e();
void Func_02005b7c();
void Func_02005b8e();
void Func_02005bb6();
void Func_02005c58();
void Func_02005cca();

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */
#define BattleRuntime_Reset_1(args...) Func_02005890(args)
#define ObjectMotion_SetSpeedParameters_1(a0, a1, a2) Call3(Func_020058da, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_2(a0, a1, a2) Call3(Func_020058e4, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_3(a0, a1, a2) Call3(Func_020058ee, a0, a1, a2)
#define Audio_PlayCue_1(args...) Func_02005a94(args)
#define ObjectMotion_SetPositionAndReset_1(a0, a1, a2) Call3(Func_02005932, a0, a1, a2)
#define ObjectMotion_ArmCallback_1(a0, a1, a2) Call3(Func_020059b6, a0, a1, a2)
#define ObjectMotion_SetHorizontalPositionWithTerrain_1(a0, a1, a2) Call3(Func_02005964, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_2(a0, a1, a2) Call3(Func_0200595a, a0, a1, a2)
#define ObjectMotion_ArmCallback_2(a0, a1, a2) Call3(Func_020059de, a0, a1, a2)
#define GameFlag_IsSet_1(a0) Value1(Func_020058ce, a0)
#define GameFlag_Set_1(a0) Call1(Func_020058e4, a0)
#define BattleRuntime_WaitIfModeZero_1(args...) Func_02005912(args)
#define SceneWork_SetStepValue_1(a0) Call1(Func_020059f4, a0)
#define Audio_PlayCue_2(args...) Func_02005b0a(args)
#define BattleRuntime_WaitIfModeZero_2(args...) Func_02005928(args)
#define ObjectMotion_SetVariantCallback_1(args...) Func_020059fa(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_1(args...) Func_02005a0a(args)
#define BattleRuntime_WaitIfModeZero_3(args...) Func_02005950(args)
#define BattleRuntime_WaitIfModeZero_4(args...) Func_0200595e(args)
#define ObjectMotion_ArmCallback_3(a0, a1, a2) Call3(Func_02005a70, a0, a1, a2)
#define ObjectMotion_ArmCallback_4(a0, a1, a2) Call3(Func_02005a7c, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_5(args...) Func_02005992(args)
#define BattleRuntime_WaitIfModeZero_6(args...) Func_020059a0(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_2(args...) Func_02005a6c(args)
#define Object_SetModeById_1(args...) Func_02005a54(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_3(args...) Func_02005aa0(args)
#define BattleRuntime_WaitIfModeZero_7(args...) Func_020059e6(args)
#define BattleRuntime_WaitIfModeZero_8(args...) Func_020059f8(args)
#define ObjectMotion_SetPositionAndReset_3(a0, a1, a2) Call3(Func_02005a7e, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_9(args...) Func_02005a0c(args)
#define ObjectMotion_ArmCallback_5(a0, a1, a2) Call3(Func_02005b12, a0, a1, a2)
#define ObjectMotion_ArmCallback_6(a0, a1, a2) Call3(Func_02005b1e, a0, a1, a2)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_4(args...) Func_02005af6(args)
#define BattleRuntime_WaitIfModeZero_10(args...) Func_02005a3c(args)
#define ObjectMotion_ArmCallback_7(a0, a1, a2) Call3(Func_02005b42, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_11(args...) Func_02005a58(args)
#define BattleEvent_RunActionAndWait_1(args...) Func_02005b48(args)
#define ObjectMotion_SetSpeedParameters_4(args...) Func_02005b48(args)
#define Audio_PlayCue_3(args...) Func_02005c58(args)
#define SceneWork_SetStepValue_2(a0) Call1(Func_02005b4e, a0)
#define ObjectMotion_SetSpeedParameters_5(args...) Func_02005b5e(args)
#define UiWork_WaitThenFinalizeCapacity_1(args...) Func_02005abe(args)
#define GameFlag_Set_2(a0) Call1(Func_02005a74, a0)
#define Object_SetModeById_2(args...) Func_02005b34(args)
#define Scene_GetRecord_1(args...) Func_02005ae2(args)
#define ObjectMotion_ResetAndSetPosition_1(args...) Func_02005b14(args)
#define ObjectMotion_CommitCurrentPositionAndActivate_1(args...) Func_02005b42(args)
#define ObjectMotion_SetHorizontalPositionWithTerrain_2(args...) Func_02005b54(args)
#define BattleEvent_RunActionAndWait_2(args...) Func_02005bb6(args)
#define Object_SetModeById_3(args...) Func_02005b6e(args)
#define Scene_GetRecord_2(args...) Func_02005b1c(args)
#define ObjectMotion_ResetAndSetPosition_2(args...) Func_02005b4e(args)
#define ObjectMotion_CommitCurrentPositionAndActivate_2(args...) Func_02005b7c(args)
#define ObjectMotion_SetHorizontalPositionWithTerrain_3(args...) Func_02005b8e(args)
#define Audio_PlayCueForPartyMember_1(args...) Func_02005cca(args)
#define BattleRuntime_ScheduleShoulderButtonModeUpdate_1(args...) Func_02005b16(args)
u8 *Func_02005ae2();
u8 *Func_02005ae2();
u8 *Func_02005b1c();

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

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)0x03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

/* Configures actors 0, 1, 2 (position, movement, and animation timing), then
 * branches on whether actor 0 is already set up: one path sets up actors 0-2
 * with poses and movement, the other only advances actor 2's animation. Both
 * paths converge to check actor 0's record and, depending on that check,
 * configure either actor 2 or actor 1 from it before the scene finishes. */
void FieldScene_RunSetupSequence(void)
{
    u8 *record;

    BattleRuntime_Reset_1();
    ObjectMotion_SetSpeedParameters_1(0, 0xcccc, 0x6666);
    ObjectMotion_SetSpeedParameters_2(1, 0xcccc, 0x6666);
    ObjectMotion_SetSpeedParameters_3(2, 0xcccc, 0x6666);
    Audio_PlayCue_1(19);
    ObjectMotion_SetPositionAndReset_1(0, 0x180, 0x198);
    ObjectMotion_ArmCallback_1(0, 0xc000, 0);
    ObjectMotion_SetHorizontalPositionWithTerrain_1(1, 0x1800000, 0x1980000);
    ObjectMotion_SetPositionAndReset_2(1, 0x170, 0x198);
    ObjectMotion_ArmCallback_2(1, 0xd000, 20);
    if (GameFlag_IsSet_1(0x850) != 0) {
    } else {
        GameFlag_Set_1(0x850);
        Func_02005228(2, 0);
        BattleRuntime_WaitIfModeZero_1(40);
        Func_0200525e();
        SceneWork_SetStepValue_1(0x1256);
        Audio_PlayCue_2(60);
        BattleRuntime_WaitIfModeZero_2(30);
        Func_02005232(2, 3, 30);
        Func_020051f2(2, 30);
        ObjectMotion_SetVariantCallback_1(0, 1);
        ObjectMotion_SetVariantCallbackAndInvokeObject_1(1, 1);
        BattleRuntime_WaitIfModeZero_3(20);
        Func_02005274(2, 0);
        BattleRuntime_WaitIfModeZero_4(40);
        Func_020052aa();
        Func_02005222(2, 30);
        Func_0200525c(0, 1, 50);
        ObjectMotion_ArmCallback_3(0, 0xc000, 0);
        ObjectMotion_ArmCallback_4(1, 0xd000, 0);
        BattleRuntime_WaitIfModeZero_5(20);
        Func_020052b6(2, 0);
        BattleRuntime_WaitIfModeZero_6(40);
        Func_020052ec();
        ObjectMotion_SetVariantCallbackAndInvokeObject_2(2, 1);
        Func_0200526c(2, 50);
        Object_SetModeById_1(0, 3);
        Func_020052c6(1, 3, 20);
        Func_020052d0(2, 3, 20);
        Func_02005290(2, 40);
        ObjectMotion_SetVariantCallbackAndInvokeObject_3(2, 1);
        BattleRuntime_WaitIfModeZero_7(30);
        Call3((void (*)())Func_02005ae2, 2, 0xc000, 0);
        BattleRuntime_WaitIfModeZero_8(30);
        ObjectMotion_SetPositionAndReset_3(2, 0x178, 0x178);
        BattleRuntime_WaitIfModeZero_9(40);
        Func_020052fe(0, 1, 50);
        ObjectMotion_ArmCallback_5(0, 0xc000, 0);
        ObjectMotion_ArmCallback_6(1, 0xd000, 0);
        ObjectMotion_SetVariantCallbackAndInvokeObject_4(2, 1);
        BattleRuntime_WaitIfModeZero_10(50);
        Func_02005346(2, 3, 30);
        ObjectMotion_ArmCallback_7(2, 0x4000, 0);
        BattleRuntime_WaitIfModeZero_11(10);
        BattleEvent_RunActionAndWait_1(2, 0);
        ObjectMotion_SetSpeedParameters_4(2, 0);
        goto L_join_setup_paths;
    }
    Audio_PlayCue_3(60);
    SceneWork_SetStepValue_2(0x125d);
    ObjectMotion_SetSpeedParameters_5(2, 0);
    L_join_setup_paths:;
    if (UiWork_WaitThenFinalizeCapacity_1(0, 0) == 0) {
        Func_02001b36();
        GameFlag_Set_2(0x856);
        Object_SetModeById_2(2, 2);
        record = Scene_GetRecord_1(0);
        if (record != 0) {
            /* Read the two s16 fields at +10 and +18 of the record. */
            ObjectMotion_ResetAndSetPosition_1(2, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        ObjectMotion_CommitCurrentPositionAndActivate_1(2);
        ObjectMotion_SetHorizontalPositionWithTerrain_2(2, 0, 0);
    } else {
        BattleEvent_RunActionAndWait_2(2, 0);
    }
    Object_SetModeById_3(1, 2);
    record = Scene_GetRecord_2(0);
    if (record != 0) {
        /* Read the two s16 fields at +10 and +18 of the record. */
        ObjectMotion_ResetAndSetPosition_2(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    ObjectMotion_CommitCurrentPositionAndActivate_2(1);
    ObjectMotion_SetHorizontalPositionWithTerrain_3(1, 0, 0);
    Audio_PlayCueForPartyMember_1();
    BattleRuntime_ScheduleShoulderButtonModeUpdate_1();
}
