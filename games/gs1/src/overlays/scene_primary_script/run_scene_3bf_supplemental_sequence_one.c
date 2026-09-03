#include "types.h"

#define FieldScene_RunSupplementalSequenceOne Func_02001e94

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
extern u8 Data_000000a1[];
extern u8 Data_00002438[];
extern u8 Data_02000240[];
void Func_020074f4();
void Func_02007544();
void Func_0200754a();
void Func_0200754c();
void Func_02007554();
void Func_0200755c();
void Func_020075a6();
void Func_020075c6();
void Func_020075ce();
void Func_020075d8();
void Func_020075de();
void Func_020075e2();
void Func_020075e8();
void Func_020075ec();
void Func_02007600();
void Func_0200760a();
void Func_02007614();
void Func_02007616();
void Func_02007628();
void Func_02007636();
void Func_0200763a();
void Func_0200763c();
void Func_0200763e();
void Func_02007640();
void Func_02007642();
void Func_0200764c();
void Func_02007650();
void Func_02007656();
void Func_02007662();
void Func_02007664();
void Func_0200766a();
void Func_02007678();
void Func_0200767a();
void Func_0200767c();
void Func_02007680();
void Func_0200768a();
void Func_02007690();
void Func_02007694();
void Func_020076ac();
void Func_020076cc();
void Func_020076d6();
void Func_020076da();
void Func_020076e0();
void Func_020076f2();
void Func_02007778();
void Func_02007782();

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */
#define Object_SetModeById_1(args...) Func_02007544(args)
#define Object_SetModeById_2(args...) Func_0200754c(args)
#define Object_SetModeById_3(args...) Func_02007554(args)
#define Object_SetModeById_4(args...) Func_0200755c(args)
#define Audio_PlayCue_1(args...) Func_02007662(args)
#define BattleEffect_SpawnLinkedResourceObject_1(a0, a1, a2) Call3(Func_020075de, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_1(args...) Func_020074f4(args)
#define ObjectMotion_SetAngleToward_1(args...) Func_020075a6(args)
#define SceneWork_SetStepValue_1(args...) Func_020075c6(args)
#define BattleEvent_RunActionAndWait_1(args...) Func_020075de(args)
#define BattleEffect_SpawnLinkedResourceObject_2(a0, a1, a2) Call3(Func_0200760a, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_3(a0, a1, a2) Call3(Func_02007616, a0, a1, a2)
#define ObjectMotion_SetAngleToward_2(args...) Func_020075d8(args)
#define ObjectMotion_SetAngleToward_3(args...) Func_020075e2(args)
#define ObjectMotion_SetAngleToward_4(args...) Func_020075ec(args)
#define BattleRuntime_WaitIfModeZero_2(args...) Func_0200754a(args)
#define ObjectMotion_ArmCallback_1(a0, a1, a2) Call3(Func_02007636, a0, a1, a2)
#define ObjectMotion_ArmCallback_2(a0, a1, a2) Call3(Func_02007642, a0, a1, a2)
#define SceneWork_SetStepValue_2(args...) Func_02007628(args)
#define BattleEvent_RunActionAndWait_2(args...) Func_02007640(args)
#define ObjectMotion_CallThenWaitForAnimationChange_1(args...) Func_02007600(args)
#define SceneWork_SetStepValue_3(args...) Func_0200763e(args)
#define BattleEvent_RunActionAndWait_3(args...) Func_02007656(args)
#define SceneWork_SetStepValue_4(args...) Func_0200764c(args)
#define BattleEvent_RunActionAndWait_4(args...) Func_02007664(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_1(args...) Func_0200763c(args)
#define SceneWork_SetStepValue_5(args...) Func_02007662(args)
#define BattleEvent_RunActionAndWait_5(args...) Func_0200767a(args)
#define ObjectMotion_CallThenWaitForAnimationChange_2(args...) Func_0200763a(args)
#define SceneWork_SetStepValue_6(args...) Func_02007678(args)
#define BattleEvent_RunActionAndWait_6(args...) Func_02007690(args)
#define ObjectMotion_CallThenWaitForAnimationChange_3(args...) Func_02007650(args)
#define BattleRuntime_WaitIfModeZero_3(args...) Func_020075ce(args)
#define ObjectMotion_SetAngleToward_5(args...) Func_02007680(args)
#define ObjectMotion_SetAngleToward_6(args...) Func_0200768a(args)
#define BattleRuntime_WaitIfModeZero_4(args...) Func_020075e8(args)
#define ObjectMotion_ResetAndSetPositionInMode2_1(a0, a1, a2) Call3(Func_0200764c, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_1(args...) Func_0200766a(args)
#define ObjectMotion_SetAngleToward_7(args...) Func_020076ac(args)
#define ObjectMotion_CallThenWaitForAnimationChange_4(args...) Func_02007694(args)
#define BattleRuntime_WaitIfModeZero_5(args...) Func_02007614(args)
#define SceneWork_SetStepValue_7(args...) Func_020076da(args)
#define BattleEvent_RunActionAndWait_7(args...) Func_020076f2(args)
#define SharedWorkData_SetFirstAndSecondFields_1(args...) Func_02007778(args)
#define BattleEffect_ComputeWeightedResultAndDispatch_1(args...) Func_02007782(args)
#define ObjectMotion_SetHorizontalPositionWithTerrain_1(args...) Func_020076cc(args)
#define ObjectMotion_SetHorizontalPositionWithTerrain_2(args...) Func_020076d6(args)
#define ObjectMotion_SetHorizontalPositionWithTerrain_3(args...) Func_020076e0(args)
#define BattleRuntime_ScheduleShoulderButtonModeUpdate_1(args...) Func_0200767c(args)
#define GameFlag_Set_1(a0) Call1(Func_02007662, a0)

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

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

/* Phase/status word at 0x1c0 of the shared scene work record. */
#define SCENE_PHASE (*(s32 *)(*(u8 *volatile *)Data_03001ebc + 0x1c0))

/* Runs a scripted beat on the objects indexed 12, 13 and 14, stepping
 * through the entries at Data_00002438 as it goes, then sets the scene
 * phase word and a status byte at +0x22b of the record at Data_02000240
 * before handing off to the next step. */
void FieldScene_RunSupplementalSequenceOne(void)
{
    u32 i;
    s32 record;
    s32 sequence_2438;
    s32 status_record_2000240;

    Object_SetModeById_1(0, 1);
    Object_SetModeById_2(12, 1);
    Object_SetModeById_3(13, 1);
    Object_SetModeById_4(14, 1);
    Audio_PlayCue_1(113);
    BattleEffect_SpawnLinkedResourceObject_1(12, 0x100, 0);
    BattleRuntime_WaitIfModeZero_1(30);
    ObjectMotion_SetAngleToward_1(12, 0, 0);
    sequence_2438 = (s32)Data_00002438;
    SceneWork_SetStepValue_1(sequence_2438);
    BattleEvent_RunActionAndWait_1(12, 0);
    BattleEffect_SpawnLinkedResourceObject_2(13, 0x100, 0);
    BattleEffect_SpawnLinkedResourceObject_3(14, 0x100, 0);
    ObjectMotion_SetAngleToward_2(13, 0, 0);
    ObjectMotion_SetAngleToward_3(14, 0, 0);
    ObjectMotion_SetAngleToward_4(0, 13, 0);
    BattleRuntime_WaitIfModeZero_2(65);
    ObjectMotion_ArmCallback_1(13, 0x5000, 0);
    ObjectMotion_ArmCallback_2(14, 0xd000, 0);
    SceneWork_SetStepValue_2((sequence_2438 + 1));
    BattleEvent_RunActionAndWait_2(13, 0);
    ObjectMotion_CallThenWaitForAnimationChange_1(14, 3);
    SceneWork_SetStepValue_3((sequence_2438 + 2));
    BattleEvent_RunActionAndWait_3(14, 0);
    SceneWork_SetStepValue_4((sequence_2438 + 3));
    BattleEvent_RunActionAndWait_4(12, 0);
    ObjectMotion_SetVariantCallbackAndInvokeObject_1(13, 1);
    SceneWork_SetStepValue_5((sequence_2438 + 4));
    BattleEvent_RunActionAndWait_5(13, 0);
    ObjectMotion_CallThenWaitForAnimationChange_2(14, 3);
    SceneWork_SetStepValue_6((sequence_2438 + 5));
    BattleEvent_RunActionAndWait_6(14, 0);
    ObjectMotion_CallThenWaitForAnimationChange_3(14, 3);
    BattleRuntime_WaitIfModeZero_3(60);
    ObjectMotion_SetAngleToward_5(13, 0, 0);
    ObjectMotion_SetAngleToward_6(14, 0, 0);
    BattleRuntime_WaitIfModeZero_4(70);
    ObjectMotion_ResetAndSetPositionInMode2_1(12, 0x2a0, 88); /* object_id 12, x 0x2a0, z 88 */
    ObjectMotion_CommitCurrentPositionAndActivate_1(12);
    ObjectMotion_SetAngleToward_7(12, 0, 0);
    ObjectMotion_CallThenWaitForAnimationChange_4(12, 3);
    BattleRuntime_WaitIfModeZero_5(30);
    SceneWork_SetStepValue_7((sequence_2438 + 6));
    BattleEvent_RunActionAndWait_7(12, 0);
    SCENE_PHASE = 0x200;
    SharedWorkData_SetFirstAndSecondFields_1((s32)Data_000000a1, 31);
    status_record_2000240 = (s32)Data_02000240;
    /* Status byte at +0x22b of the record. */
    *(u8 *)((status_record_2000240 + 0x22b)) = 3;
    BattleEffect_ComputeWeightedResultAndDispatch_1(98, 3);
    ObjectMotion_SetHorizontalPositionWithTerrain_1(12, 0, 0);
    ObjectMotion_SetHorizontalPositionWithTerrain_2(13, 0, 0);
    ObjectMotion_SetHorizontalPositionWithTerrain_3(14, 0, 0);
    BattleRuntime_ScheduleShoulderButtonModeUpdate_1();
    GameFlag_Set_1(0x94a); /* main:080770c8 */
}
