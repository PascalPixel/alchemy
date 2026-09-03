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

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */
#define BattleRuntime_Reset_1(args...) Func_02008224(args)
#define Object_NotifyLastActiveOfEvent_1(a0) Call1(Func_0200823a, a0)
#define BattleRuntime_WaitIfModeZero_1(args...) Func_02008384(args)
#define ObjectMotion_SetSpeedParameters_1(a0, a1, a2) Call3(Func_02008272, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_1(a0, a1, a2) Call3(Func_020082be, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_1(a0, a1, a2) Call3(Func_0200835c, a0, a1, a2)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_1(args...) Func_0200830e(args)
#define SceneWork_SetStepValue_1(a0) Call1(Func_02008334, a0)
#define ObjectMotion_SetSpeedParameters_2(a0, a1) Value2(Func_02008344, a0, a1)
#define ObjectMotion_ArmCallback_1(a0, a1, a2) Call3(Func_02008370, a0, a1, a2)
#define UiWork_WaitThenFinalizeCapacity_1(a0, a1) Value2(Func_020082a8, a0, a1)
#define BattleRuntime_ScheduleShoulderButtonModeUpdate_1(args...) Func_020082a6(args)
#define BattleEventRuntime_ProcessAction_1(a0, a1) Value2(Func_0200839a, a0, a1)
#define ObjectMotion_SetHorizontalPositionWithTerrain_1(a0, a1, a2) Call3(Func_0200834c, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_3(a0, a1, a2) Call3(Func_02008306, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_2(a0, a1, a2) Call3(Func_02008352, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_3(a0, a1, a2) Call3(Func_0200835e, a0, a1, a2)
#define ObjectMotion_ArmCallback_2(a0, a1, a2) Call3(Func_020083e2, a0, a1, a2)
#define ObjectMotion_ArmCallback_3(a0, a1, a2) Call3(Func_020083ee, a0, a1, a2)
#define ObjectMotion_ArmCallback_4(a0, a1, a2) Call3(Func_020083fa, a0, a1, a2)
#define ObjectMotion_Launch_1(args...) Func_020083bc(args)
#define Object_SetModeById_1(args...) Func_020083b4(args)
#define BattleEvent_RunActionAndWait_1(args...) Func_02008404(args)
#define ObjectMotion_SetHorizontalPositionWithTerrain_2(a0, a1, a2) Call3(Func_020083c0, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_4(a0, a1, a2) Call3(Func_0200837e, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_4(a0, a1, a2) Call3(Func_020083ca, a0, a1, a2)
#define ObjectMotion_ArmCallback_5(a0, a1, a2) Call3(Func_02008450, a0, a1, a2)
#define ObjectMotion_ArmCallback_6(a0, a1, a2) Call3(Func_02008462, a0, a1, a2)
#define ObjectMotion_ArmCallback_7(a0, a1, a2) Call3(Func_0200846e, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_2(a0, a1, a2) Call3(Func_02008490, a0, a1, a2)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_2(args...) Func_02008440(args)
#define ObjectMotion_SetVariantCallback_1(args...) Func_02008446(args)
#define ObjectMotion_ArmCallback_8(a0, a1, a2) Value3(Func_0200849e, a0, a1, a2)
#define Object_SetModeById_2(args...) Func_02008446(args)
#define ObjectMotion_Launch_2(args...) Func_02008466(args)
#define Object_SetModeById_3(args...) Func_0200845e(args)
#define ObjectMotion_SetPositionAndReset_5(a0, a1, a2) Call3(Func_02008458, a0, a1, a2)
#define ObjectMotion_ArmCallback_9(a0, a1, a2) Call3(Func_020084dc, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_3(a0, a1, a2) Call3(Func_02008506, a0, a1, a2)
#define ObjectMotion_SetVariantCallback_2(args...) Func_020084ae(args)
#define ObjectMotion_CallThenWaitForAnimationChange_1(args...) Func_020084ac(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_3(args...) Func_020084d4(args)
#define ObjectMotion_SetSpeedParameters_5(a0, a1, a2) Call3(Func_0200846c, a0, a1, a2)
#define ObjectMotion_EnableActionAndSetCallback_1(args...) Func_0200847e(args)
#define ObjectMotion_SetSpeedParameters_6(a0, a1, a2) Call3(Func_02008480, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_6(a0, a1, a2) Call3(Func_020084cc, a0, a1, a2)
#define ObjectMotion_EnableActionAndSetCallback_2(args...) Func_0200849c(args)
#define BattleRuntime_WaitIfModeZero_2(args...) Func_02008462(args)
#define ObjectMotion_EnableActionAndSetCallback_3(args...) Func_020084aa(args)
#define BattleRuntime_WaitIfModeZero_3(args...) Func_02008470(args)
#define ObjectMotion_CallThenWaitForAnimationChange_2(args...) Func_0200852a(args)
#define ObjectMotion_CallThenWaitForAnimationChange_3(args...) Func_02008532(args)
#define ObjectMotion_SetSpeedParameters_7(args...) Func_02008618(args)

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

/* Pointer, held at fixed address 0x03001ebc, to the shared scene work
 * record. The phase/status word lives at offset 0x1c0 of that record. */
#define SCENE_WORK (*(u8 **)0x03001ebc)
#define SCENE_PHASE (*(s32 *)(SCENE_WORK + 0x1c0))

/* Sets up background/camera parameters and either takes the short branch
 * (branch value 1 from Func_020082a8) or runs a longer sequence that moves
 * and reconfigures actors 20, 21, 22 and 26 before advancing the scene
 * phase to 0x201 and finishing the scene. */
void FieldScene_RunEncounterClosingSequence(void)
{
    u32 i;
    s32 record;
    s32 data_00002014_addr;
    s32 data_0200c918_addr;

    BattleRuntime_Reset_1();
    Object_NotifyLastActiveOfEvent_1(0x200d1d8);
    Func_020081a0(1);
    BattleRuntime_WaitIfModeZero_1();
    ObjectMotion_SetSpeedParameters_1(0, 0x10000, 0x8000);
    ObjectMotion_SetPositionAndReset_1(0, 148, 0x290);
    BattleEffect_SpawnLinkedResourceObject_1(22, 0x100, 0);
    ObjectMotion_SetVariantCallbackAndInvokeObject_1(22, 1);
    Func_02007982(22, 0x5000);
    SceneWork_SetStepValue_1(0x1f69);
    ObjectMotion_SetSpeedParameters_2(0x2016, 0);
    ObjectMotion_ArmCallback_1(0, 0xe000, 0);
    if (UiWork_WaitThenFinalizeCapacity_1(0, 0) == 1) {
        Call1(Func_0200799a, 0x2016);
        BattleRuntime_ScheduleShoulderButtonModeUpdate_1();
    } else {
        bump_step(1);
        BattleEventRuntime_ProcessAction_1(0x2016, 0);
        Func_02004b8a();
        ObjectMotion_SetHorizontalPositionWithTerrain_1(26, 0xd80000, 0x24c0000);
        ObjectMotion_SetSpeedParameters_3(26, 0x13333, 0x9999);
        ObjectMotion_SetPositionAndReset_2(26, 216, 0x254);
        ObjectMotion_SetPositionAndReset_3(26, 188, 0x268);
        ObjectMotion_ArmCallback_2(0, 0xe000, 0);
        ObjectMotion_ArmCallback_3(21, 0xd000, 0);
        ObjectMotion_ArmCallback_4(22, 0xd000, 0);
        Value2(Func_02007a2e, 26, 0x5000);
        ObjectMotion_Launch_1(26, 2, 0);
        Object_SetModeById_1(26, 4);
        BattleEvent_RunActionAndWait_1(26, 0);
        ObjectMotion_SetHorizontalPositionWithTerrain_2(20, 0xb40000, 0x3090000);
        ObjectMotion_SetSpeedParameters_4(20, 0x10000, 0x8000);
        ObjectMotion_SetPositionAndReset_4(20, 180, 0x298);
        data_00002014_addr = (s32)Data_00002014;
        ObjectMotion_ArmCallback_5(20, 0xd000, 0);
        Func_02007a6e(data_00002014_addr);
        ObjectMotion_ArmCallback_6(0, 0x2000, 0);
        ObjectMotion_ArmCallback_7(22, 0x3000, 0);
        BattleEffect_SpawnLinkedResourceObject_2(26, 0x101, 60);
        ObjectMotion_SetVariantCallbackAndInvokeObject_2(20, 1);
        Func_02007a9e(data_00002014_addr);
        ObjectMotion_SetVariantCallback_1(21, 2);
        Func_02007aac(21);
        ObjectMotion_ArmCallback_8(20, 0x5000, 20);
        Object_SetModeById_2(20, 3);
        Call1(Func_02007ac4, 0x6014);
        ObjectMotion_Launch_2(26, 2, 20);
        Object_SetModeById_3(26, 4);
        Func_02007adc(26);
        ObjectMotion_SetPositionAndReset_5(20, 182, 0x280);
        ObjectMotion_ArmCallback_9(20, 0xd000, 0);
        Call1(Func_02007afa, 0x8014);
        BattleEffect_SpawnLinkedResourceObject_3(26, 0x100, 20);
        ObjectMotion_SetVariantCallback_2(26, 2);
        Func_02007b14(26);
        ObjectMotion_CallThenWaitForAnimationChange_1(20, 3);
        Func_02007b38(22, 0);
        ObjectMotion_SetVariantCallbackAndInvokeObject_3(22, 1);
        Func_02007b32(22);
        ObjectMotion_SetSpeedParameters_5(22, 0x19999, 0xcccc);
        data_0200c918_addr = (s32)Data_0200c918;
        ObjectMotion_EnableActionAndSetCallback_1(22, data_0200c918_addr);
        ObjectMotion_SetSpeedParameters_6(21, 0x19999, 0xcccc);
        ObjectMotion_SetPositionAndReset_6(21, 168, 0x278);
        ObjectMotion_EnableActionAndSetCallback_2(21, data_0200c918_addr);
        BattleRuntime_WaitIfModeZero_2(80);
        ObjectMotion_EnableActionAndSetCallback_3(26, data_0200c918_addr);
        BattleRuntime_WaitIfModeZero_3(40);
        Call2(Func_02007b96, 20, 0x8000);
        Func_02007b88(data_00002014_addr);
        Value2(Func_02007ba6, 0, 0xe000);
        ObjectMotion_CallThenWaitForAnimationChange_2(0, 3);
        ObjectMotion_CallThenWaitForAnimationChange_3(20, 3);
        SCENE_PHASE = 0x201;
        Func_0200860c();
        ObjectMotion_SetSpeedParameters_7();
        Func_020085f6(17);
    }
}
