#include "types.h"
#include "scene.h"
#include "abi/overlays/scene/dialogue/flagged_cue.h"

#define SCENE_STEP (*(u16 *)(*(u8 **)0x03001ebc + 0x1d8))
#define REC_S16(rec, off) (*(s16 *)((rec) + (off)))

extern u8 Value_000028be;
extern s16 gOv[][2];
extern u8 gOv2[];
extern u8 gOv3[];

u8 *Scene_GetRecord_1();
u8 *Battle_WaitMode0_71();

u8 *Scene_GetRecord_3();

u8 *Talk_Run(s32);

/* A signed 16-bit field of an actor record returned by one of the record
 * lookups. */

/*
 * Each pseudo symbol above names the per-site call word the overlay image
 * holds, not a runtime address -- one word can serve two sites with different
 * targets -- and the macro names the function the site reaches through the
 * veneers, keeping the site's own calling form. Names without a repository
 * binding are provisional.
 */

static __inline__ void PlaceActor(s32 actor, s32 x, s32 y)
{
    void Talk_Run2(s32, s32, s32);

    Talk_Run2(actor, x, y);
}

static __inline__ void SetOffset(s32 actor, s32 offset, s32 zero)
{
    void Talk_Run3(s32, s32, s32);

    Talk_Place4(actor, offset, zero);
}

static __inline__ s32 Scene_Value3(s32 (*fn)(), s32 a, s32 b, s32 c)
{
    return fn(a, b, c);
}

static __inline__ void Scene_Call3(void (*fn)(), s32 a, s32 b, s32 c)
{
    fn(a, b, c);
}

void Actor_SetActor23Params2And6(void)
{
    Talk_Place5(0x17, 2, 6);
}

/*
 * Returns the in-image table at 0x0200975c. The eight-byte owner includes its
 * one pool word, which holds that address and is returned without being
 * dereferenced.
 */
u8 *SceneData_GetTable975c(void)
{
    return (u8 *)0x0200975c;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

/*
 * Returns the in-image table at 0x020098c4. The eight-byte owner includes its
 * one pool word, which holds that address and is returned without being
 * dereferenced.
 */
u8 *SceneData_GetTable98c4(void)
{
    return (u8 *)0x020098c4;
}

s32 SceneData_SelectTableByFlag9a7(void)
{
    if (Talk_Check2(0x9A7) != 0) {
        return 0x02009A98;
    }
    return 0x02009900;
}

void Actor_SetupActorForTable9638(s32 a0)
{
    extern u8 gWork[];
    void Talk_Run3();

    u32 i;
    s32 record;

    record = Talk_Check3();
    *(volatile s32 *)(record + 24) = 0x10000;
    record = Talk_Check(a0);
    *(volatile s32 *)(record + 28) = 0x10000;
    Talk_Do(0x26af);
    Talk_Run4(a0, 0);
    Talk_Place(a0, 0xc000, 0);
    Talk_Run5(20);
    Talk_Run6(a0, 0x2009638);
}

void Actor_UpdateObjectWithCue28be(s32 obj)
{
    s32 cue = (s32)&Value_000028be;
    Talk_Do4(cue);
    Talk_Apply(obj, 0);
    if (Talk_Apply2(0, 0) == 0) {
        Talk_Do5(10);
        Talk_Do6(cue + 1);
    } else {
        Talk_Do7(cue + 2);
    }
    Talk_Apply3(obj, 0);
}

void Actor_SetupActor18Event(void)
{
    void Talk_Run3(s32, s32, s32);

    Talk_Run7(2491);
    Talk_Run8(0x28b8);
    Talk_Run9(18, 0);
    PlaceActor(18, 65536, 32768);
    SetOffset(18, -16, 0);
    Talk_Run10(18, 0, 0);
    Talk_Do8(10);
}

void Scene_RunTableTransition(void)
{
    extern u8 *gWork;

    s32 no = *(s16 *)(gWork + 0x16c);
    s32 x = gOv[no][0];
    s32 y = gOv[no][1];

    Talk_Run(0)[85] = 2;
    Talk_Do9(158);
    if (no == 6) {
        Scene_Value3(Talk_Check4, (s32)gOv2, (u16)x, (u16)y);
        Scene_Call3(Talk_Run11, 0, 0, -16);
    } else {
        Scene_Value3(Talk_Check5, (s32)gOv3, (u16)x, (u16)y);
        Scene_Call3(Talk_Run12, 0, 2, -16);
    }
    Talk_Do10(10);
    *(s32 *)(gWork + 0x1c8) = 16;
    Talk_Do11(no);
}

void State_SetWord1c8To16AndForward16c(void)
{
    extern u8 *gWork;

    u8 *work = gWork;
    s16 *p = (s16 *)(work + 0x16C);
    s32 n = *p;

    *(s32 *)(work + 0x1C8) = 16;
    Talk_Do12(n);
}

/*
 * Runs the fixed call sequence for this scene: a paced series of setup and
 * per-entity calls -- position, pose and property triples keyed by entity id,
 * interleaved with timed single-argument steps -- ending with a record lookup
 * whose s16 fields at +10 and +18 feed the last positioning call.
 */
void Scene_RunPrimarySequence(void)
{
    u8 *Scene_GetRecord_1();

    u32 i;
    u8 *record;

    Battle_Reset_1();
    SceneWork_SetStepValue_1(0x2694);
    Motion_SetHPosTerrain_1(0, 0xf80000, 0x1a80000);
    Motion_ArmCb_1(0, 0xc000, 0);
    Object_SetModeById_1(8, 0);
    Object_SetModeById_2(9, 0);
    Battle_WaitMode0_1(); /* main:0808a360 */
    Motion_SetSpeed_1(); /* main:0808a370 */
    Battle_WaitMode0_2(20);
    Motion_RealignToTrackedObjectAndArmCallback_1(22, 8, -16, 0xc000);
    Motion_CommitPos_1(22);
    Battle_WaitMode0_3(20);
    BattleFx_SpawnLinked_1(22, 0x102, 60);
    Motion_SetSpeed_2(22, 0x10000, 0x8000);
    Motion_CommitPositionAndActivate_1(22, 0, -16);
    Battle_WaitMode0_4(10);
    Motion_SetAngleToward_1(22, 8, 40);
    Motion_SetAngleToward_2(22, 9, 40);
    Motion_SetAngleToward_3(22, 8, 40);
    Battle_WaitMode0_5(10);
    Motion_SetVarCbObj_1(22, 2);
    Battle_WaitMode0_6(20);
    BattleEv_RunWait_1(22, 0);
    Battle_WaitMode0_7(10);
    Motion_ArmCb_2(22, 0x4000, 0);
    Battle_WaitMode0_8(30);
    Motion_SetSpeed_3(22, 0); /* main:0808a178 */
    UiWork_WaitThenFinalizeCapacity_1(0, 0);
    Battle_WaitMode0_9(30);
    Motion_SetVarCbObj_2(8, 2);
    Battle_WaitMode0_10(20);
    BattleEv_RunWait_2(8, 0);
    Battle_WaitMode0_11(10);
    Motion_SetAngleToward_4(22, 8, 30);
    Motion_SetVarCbObj_3(22, 2);
    Battle_WaitMode0_12(20);
    BattleEv_RunWait_3(22, 0);
    Object_SetModeById_3(8, 1);
    Battle_WaitMode0_13(20);
    Motion_SetVarCbObj_4(8, 2);
    Battle_WaitMode0_14(30);
    BattleEv_RunWait_4(8, 0);
    Battle_WaitMode0_15(10);
    Motion_CallWaitAnim_1(22, 3);
    Battle_WaitMode0_16(20);
    BattleEv_RunWait_5(22, 0);
    Battle_WaitMode0_17(10);
    Motion_SetVarCbObj_5(9, 2);
    Battle_WaitMode0_18(20);
    BattleEv_RunWait_6(9, 0);
    Battle_WaitMode0_19(10);
    Motion_SetAngleToward_5(22, 9, 0);
    Motion_SetAngleToward_6(0, 9, 30);
    Object_SetModeById_4(9, 1);
    Battle_WaitMode0_20(10);
    BattleFx_SpawnLinked_2(22, 0x108, 40);
    BattleEv_RunWait_7(22, 0);
    Battle_WaitMode0_21(10);
    Motion_CallWaitAnim_2(0, 3);
    Battle_WaitMode0_22(20);
    Battle_WaitMode0_23(10);
    Motion_ArmCb_3(22, 0x4000, 0);
    Battle_WaitMode0_24(30);
    Motion_SetVarCbObj_6(22, 2);
    Battle_WaitMode0_25(20);
    BattleEv_RunWait_8(22, 0);
    Battle_WaitMode0_26(10);
    Motion_SetVarCbObj_7(9, 2);
    Battle_WaitMode0_27(30);
    BattleEv_RunWait_9(9, 0);
    Battle_WaitMode0_28(10);
    BattleFx_SpawnLinked_3(22, 0x102, 50);
    Motion_SetAngleToward_7(22, 9, 20);
    BattleEv_RunWait_10(22, 0);
    Battle_WaitMode0_29(10);
    Motion_SetVarCbObj_8(8, 2);
    Battle_WaitMode0_30(20);
    BattleEv_RunWait_11(8, 0);
    Battle_WaitMode0_31(10);
    Motion_ArmCb_4(22, 0x4000, 0);
    Battle_WaitMode0_32(30);
    BattleEv_RunWait_12(22, 0);
    Battle_WaitMode0_33(10);
    Motion_CallWaitAnim_3(0, 3);
    Battle_WaitMode0_34(20);
    Battle_WaitMode0_35(20);
    BattleFx_SpawnLinked_4(22, 0x102, 50);
    Motion_SetSpeed_4(22, 0x1cccc, 0xe666);
    Motion_SetPosReset_1(22, 0x100, 0x168);
    Battle_WaitMode0_36(10);
    Motion_ArmCb_5(22, 0, 0);
    Battle_WaitMode0_37(30);
    Motion_ArmCb_6(22, 0x8000, 0);
    Battle_WaitMode0_38(30);
    Motion_ArmCb_7(22, 0xc000, 0);
    Battle_WaitMode0_39(30);
    BattleEv_RunWait_13(22, 0);
    Battle_WaitMode0_40(10);
    Motion_SetVarCbObj_9(9, 2);
    Battle_WaitMode0_41(20);
    BattleEv_RunWait_14(9, 0);
    Battle_WaitMode0_42(10);
    BattleFx_SpawnLinked_5(22, 0x100, 40);
    Motion_ArmCb_8(22, 0x2000, 0);
    Battle_WaitMode0_43(20);
    BattleEv_RunWait_15(22, 0);
    Battle_WaitMode0_44(10);
    Motion_CallWaitAnim_4(9, 3);
    Battle_WaitMode0_45(30);
    BattleEv_RunWait_16(9, 0);
    Battle_WaitMode0_46(20);
    Motion_SetVarCbObj_10(22, 2);
    Battle_WaitMode0_47(20);
    Motion_SetSpeed_5(22, 0x19999, 0xcccc);
    Motion_SetPosReset_2(22, 0x100, 0x180);
    Motion_ArmCb_9(22, 0, 0);
    Battle_WaitMode0_48(20);
    BattleEv_RunWait_17(22, 0);
    Battle_WaitMode0_49(10);
    Motion_SetVarCbObj_11(8, 2);
    Battle_WaitMode0_50(20);
    BattleEv_RunWait_18(8, 0);
    Battle_WaitMode0_51(10);
    Motion_ArmCb_10(22, 0x8000, 0);
    Battle_WaitMode0_52(40);
    Motion_CallWaitAnim_5(22, 3);
    Battle_WaitMode0_53(20);
    BattleEv_RunWait_19(22, 0);
    Battle_WaitMode0_54(10);
    Motion_SetSpeed_6(22, 0x13333, 0x9999);
    Motion_CommitPositionAndActivate_2(22, 0, 16);
    Battle_WaitMode0_55(10);
    BattleEv_RunWait_20(22, 0);
    Battle_WaitMode0_56(10);
    BattleFx_SpawnLinked_6(0, 0x102, 80);
    BattleFx_SpawnLinked_7(22, 0x101, 80);
    Motion_SetAngleToward_8(22, 8, 40);
    Motion_SetAngleToward_9(22, 9, 40);
    Motion_ArmCb_11(22, 0x4000, 0);
    Battle_WaitMode0_57(30);
    Motion_SetVarCbObj_12(22, 2);
    Battle_WaitMode0_58(20);
    BattleEv_RunWait_21(22, 0);
    Battle_WaitMode0_59(10);
    Motion_SetVarCbObj_13(9, 2);
    Battle_WaitMode0_60(20);
    BattleEv_RunWait_22(9, 0);
    Battle_WaitMode0_61(10);
    Motion_ArmCb_12(22, 0, 0);
    Motion_SetAngleToward_10(0, 9, 30);
    Battle_WaitMode0_62(10);
    Motion_SetVarCbObj_14(8, 2);
    Battle_WaitMode0_63(20);
    BattleEv_RunWait_23(8, 0);
    Battle_WaitMode0_64(10);
    Motion_ArmCb_13(22, 0x8000, 0);
    Motion_SetAngleToward_11(0, 8, 70);
    Motion_ArmCb_14(22, 0x4000, 0);
    Battle_WaitMode0_65(40);
    Motion_ArmCb_15(0, 0xc000, 0);
    Battle_WaitMode0_66(40);
    Motion_SetVarCbObj_15(22, 2);
    Battle_WaitMode0_67(20);
    BattleEv_RunWait_24(22, 0);
    Battle_WaitMode0_68(10);
    Motion_CallWaitAnim_6(0, 3);
    Battle_WaitMode0_69(20);
    Motion_CallWaitAnim_7(22, 3);
    Battle_WaitMode0_70(30);
    Motion_SetSpeed_7(22, 0x13333, 0x9999);
    Object_SetModeById_5(22, 2);
    record = Scene_GetRecord_1(0);
    if (record != 0) {
        /* Pass the record's s16 fields at +10 and +18 through to entity 22. */
        Motion_ResetAndSetPosition_1(22, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Motion_CommitPos_2(22);
    Motion_SetHPosTerrain_2(22, 0, 0);
    Battle_WaitMode0_71(10);
    Battle_SchedShoulder_1();
}

/*
 * Drives actors 0 to 3, 22 and 25 through a long timed sequence of pose, move
 * and sprite-flag calls, gated by two condition checks that each pick one of
 * two call sequences and both bump the shared scene step counter.
 */
void Scene_RunSecondarySequence(void)
{
    s32 Scene_GetRecord_1();

    u32 i;
    u8 *record;

    GameFlag_Set_1(0x9ba);
    BattleRuntime_Reset_1();
    SceneWork_SetStepValue_1(0x288e);
    ObjectMotion_SetPositionAndReset_1(0, 104, 0x178);
    ObjectMotion_ArmCallback_1(0, 0, 0);
    ObjectMotion_RealignToTrackedObjectAndArmCallback_1(1, -32, 0, 0);
    Motion_RealignToTrackedObjectAndArmCallback_2(3, -16, 16, 0xe000);
    Motion_RealignToTrackedObjectAndArmCallback_3(2, 0, 16, 0xc000);
    ObjectMotion_CommitCurrentPositionAndActivate_1(1);
    BattleRuntime_WaitIfModeZero_1(30);
    ObjectMotion_SetSpeedParameters_1(1, 0);
    BattleRuntime_WaitIfModeZero_2(10);
    ObjectMotion_ArmCallback_2(0, 0x8000, 0);
    BattleRuntime_WaitIfModeZero_3(10);
    /* Either path bumps the step counter once, at a different point in its
     * four calls. */
    if (UiWork_WaitThenFinalizeCapacity_2(0, 0) == 0) {
        BattleRuntime_WaitIfModeZero_4(20);
        ObjectMotion_CallThenWaitForAnimationChange_1(3, 4);
        BattleRuntime_WaitIfModeZero_5(20);
        BattleEvent_RunActionAndWait_1(3, 0);
        SCENE_STEP += 1;
    } else {
        BattleRuntime_WaitIfModeZero_6(20);
        ObjectMotion_CallThenWaitForAnimationChange_2(3, 4);
        BattleRuntime_WaitIfModeZero_7(20);
        SCENE_STEP += 1;
        BattleEvent_RunActionAndWait_2(3, 0);
    }
    BattleRuntime_WaitIfModeZero_8(10);
    BattleEffect_SpawnLinkedResourceObject_1(2, 0x108, 40);
    BattleEvent_RunActionAndWait_3(2, 0);
    BattleRuntime_WaitIfModeZero_9(10);
    Object_SetModeById_1(0, 3);
    BattleRuntime_WaitIfModeZero_10(40);
    Object_SetModeById_2(1, 3);
    Object_SetModeById_3(2, 3);
    ObjectMotion_CallThenWaitForAnimationChange_3(3, 3);
    BattleRuntime_WaitIfModeZero_11(30);
    ObjectMotion_CommitPositionAndActivate_1(0, -16, 0);
    Motion_OffsetPositionAndReset_1(2, 0, -16);
    Motion_OffsetPositionAndReset_2(3, 0, -8);
    ObjectMotion_CommitPositionAndActivate_2(0, 0, -16);
    Object_SetModeById_4(3, 1);
    Object_SetModeById_5(2, 1);
    ObjectMotion_ArmCallback_3(0, 0xc000, 0);
    ObjectMotion_ArmCallback_4(1, 0xc000, 0);
    ObjectMotion_ArmCallback_5(3, 0xc000, 0);
    ObjectMotion_ArmCallback_6(2, 0xc000, 0);
    ObjectMotion_ArmCallback_7(22, 0x4000, 0);
    BattleRuntime_WaitIfModeZero_12(80);
    ObjectMotion_SetSpeedParameters_2(22, 0xcccc, 0x6666);
    /* Set the byte at offset 85 of actor 22's record to 2. */
    *(u8 *)(Scene_GetRecord_1(22) + 85) = 2;
    Motion_SetActionVariant_1(22, 2);
    Talk_SetRect(34, 0, 1, 2, 4, 18);
    Audio_PlayCue_1(158);
    BattleRuntime_WaitIfModeZero_13(20);
    ObjectMotion_SetHorizontalPositionWithTerrain_1(22, 0x480000, 0x1380000);
    BattleRuntime_WaitIfModeZero_14(20);
    Motion_CommitPositionAndActivate_3(22, 0, 16);
    Talk_SetRect2(32, 0, 1, 2, 4, 18);
    Audio_PlayCue_2(159);
    BattleRuntime_WaitIfModeZero_15(20);
    BattleEvent_RunActionAndWait_4(22, 0);
    BattleRuntime_WaitIfModeZero_16(10);
    Object_SetModeById_6(0, 3);
    Object_SetModeById_7(1, 3);
    Object_SetModeById_8(2, 3);
    ObjectMotion_CallThenWaitForAnimationChange_4(3, 3);
    BattleRuntime_WaitIfModeZero_17(40);
    Motion_CommitPositionAndActivate_4(22, 16, 0);
    ObjectMotion_ArmCallback_8(22, 0x4000, 0);
    BattleRuntime_WaitIfModeZero_18(20);
    BattleRuntime_WaitIfModeZero_19(10);
    ObjectMotion_CallThenWaitForAnimationChange_5(22, 3);
    BattleRuntime_WaitIfModeZero_20(30);
    BattleEvent_RunActionAndWait_5(22, 0);
    BattleRuntime_WaitIfModeZero_21(10);
    ObjectMotion_CallThenWaitForAnimationChange_6(3, 3);
    BattleRuntime_WaitIfModeZero_22(30);
    BattleEvent_RunActionAndWait_6(3, 0);
    BattleRuntime_WaitIfModeZero_23(10);
    ObjectMotion_SetVariantCallbackAndInvokeObject_1(22, 2);
    BattleRuntime_WaitIfModeZero_24(20);
    BattleEvent_RunActionAndWait_7(22, 0);
    BattleRuntime_WaitIfModeZero_25(10);
    ObjectMotion_SetVariantCallbackAndInvokeObject_2(1, 2);
    BattleRuntime_WaitIfModeZero_26(20);
    BattleEvent_RunActionAndWait_8(1, 0);
    BattleRuntime_WaitIfModeZero_27(10);
    BattleEffect_SpawnLinkedResourceObject_2(22, 0x102, 40);
    BattleEvent_RunActionAndWait_9(22, 0);
    BattleRuntime_WaitIfModeZero_28(10);
    BattleEffect_SpawnLinkedResourceObject_3(2, 0x100, 40);
    BattleEvent_RunActionAndWait_10(2, 0);
    BattleRuntime_WaitIfModeZero_29(10);
    ObjectMotion_CallThenWaitForAnimationChange_7(22, 3);
    BattleRuntime_WaitIfModeZero_30(30);
    BattleEvent_RunActionAndWait_11(22, 0);
    BattleRuntime_WaitIfModeZero_31(10);
    Object_LinkPair_1(1, 0, 0);
    Object_LinkPair_2(3, 2, 0);
    BattleRuntime_WaitIfModeZero_32(60);
    ObjectMotion_ArmCallback_9(0, 0xc000, 0);
    ObjectMotion_ArmCallback_10(1, 0xc000, 0);
    ObjectMotion_ArmCallback_11(2, 0xc000, 0);
    ObjectMotion_ArmCallback_12(3, 0xc000, 0);
    BattleRuntime_WaitIfModeZero_33(40);
    BattleRuntime_WaitIfModeZero_34(10);
    Motion_CallWaitAnim_8(22, 3);
    BattleRuntime_WaitIfModeZero_35(30);
    BattleEvent_RunActionAndWait_12(22, 0);
    BattleRuntime_WaitIfModeZero_36(10);
    BattleEffect_SpawnLinkedResourceObject_4(3, 0x102, 40);
    BattleEvent_RunActionAndWait_13(3, 0);
    BattleRuntime_WaitIfModeZero_37(10);
    Motion_CallWaitAnim_9(22, 3);
    BattleRuntime_WaitIfModeZero_38(20);
    BattleEvent_RunActionAndWait_14(22, 0);
    BattleRuntime_WaitIfModeZero_39(10);
    ObjectMotion_ArmCallback_13(1, 0xe000, 0);
    BattleRuntime_WaitIfModeZero_40(20);
    ObjectMotion_SetSpeedParameters_3(1, 0);
    /* The same branch-and-bump shape as above. */
    if (UiWork_WaitThenFinalizeCapacity_4(0, 0) == 0) {
        BattleRuntime_WaitIfModeZero_41(20);
        Motion_CallWaitAnim_10(22, 4);
        BattleRuntime_WaitIfModeZero_42(20);
        BattleEvent_RunActionAndWait_15(22, 0);
        SCENE_STEP += 1;
    } else {
        BattleRuntime_WaitIfModeZero_43(20);
        Motion_CallWaitAnim_11(22, 4);
        BattleRuntime_WaitIfModeZero_44(20);
        SCENE_STEP += 1;
        BattleEvent_RunActionAndWait_16(22, 0);
    }
    BattleRuntime_WaitIfModeZero_45(10);
    BattleEffect_SpawnLinkedResourceObject_5(2, 0x102, 40);
    BattleEvent_RunActionAndWait_17(2, 0);
    BattleRuntime_WaitIfModeZero_46(10);
    ObjectMotion_ArmCallback_14(1, 0xc000, 0);
    BattleEffect_SpawnLinkedResourceObject_6(22, 0x106, 50);
    BattleEvent_RunActionAndWait_18(22, 0);
    BattleRuntime_WaitIfModeZero_47(20);
    ObjectMotion_SetHorizontalPositionWithTerrain_2(25, 0x580000, 0x14c0000);
    BattleEvent_RunActionAndWait_19(-1, 0);
    BattleRuntime_WaitIfModeZero_48(10);
    BattleEffect_SpawnLinkedResourceObject_7(3, 0x100, 40);
    Motion_Launch_1(3, 4, 13);
    Motion_Launch_2(3, 4, 30);
    BattleEvent_RunActionAndWait_20(3, 0);
    BattleRuntime_WaitIfModeZero_49(10);
    Motion_CallWaitAnim_12(22, 3);
    BattleRuntime_WaitIfModeZero_50(30);
    BattleEvent_RunActionAndWait_21(22, 0);
    /* One extra call, run only when GameFlag_IsSet_1(0x9bf) is non-zero. */
    if (GameFlag_IsSet_2(0x9bf) != 0) {
        Talk_Run13();
    }
    SceneWork_SetStepValue_2(0x28a5);
    BattleRuntime_WaitIfModeZero_51(10);
    ObjectMotion_SetVariantCallbackAndInvokeObject_3(22, 2);
    BattleRuntime_WaitIfModeZero_52(20);
    BattleEvent_RunActionAndWait_22(22, 0);
    BattleRuntime_WaitIfModeZero_53(10);
    Motion_CallWaitAnim_13(22, 3);
    BattleRuntime_WaitIfModeZero_54(30);
    BattleEvent_RunActionAndWait_23(22, 0);
    BattleRuntime_WaitIfModeZero_55(10);
    BattleFx_SpawnLinked_8(1, 0x100, 40);
    BattleEvent_RunActionAndWait_24(1, 0);
    BattleRuntime_WaitIfModeZero_56(10);
    ObjectMotion_SetVariantCallbackAndInvokeObject_4(22, 2);
    BattleRuntime_WaitIfModeZero_57(20);
    BattleEv_RunWait_25(22, 0);
    BattleRuntime_WaitIfModeZero_58(20);
    Motion_CallWaitAnim_14(2, 3);
    BattleRuntime_WaitIfModeZero_59(30);
    BattleEv_RunWait_26(2, 0);
    BattleRuntime_WaitIfModeZero_60(10);
    Motion_CallWaitAnim_15(22, 3);
    BattleRuntime_WaitIfModeZero_61(30);
    ObjectMotion_SetSpeedParameters_4(25, 0xcccc, 0x6666);
    Motion_OffsetPositionAndResetMotion_1(25, 0, 16);
    Motion_CommitPositionAndActivate_5(22, 0, 16);
    BattleRuntime_WaitIfModeZero_62(30);
    Talk_Place2(25, 0, 0);
    SCENE_STEP += 1;
    Talk_Run14(242, 0);
    BattleRuntime_WaitIfModeZero_63(10);
    /* Clear bit 0 of the flag byte at offset 90 of the record, then set it
     * back through a second record accessor. */
    *(u8 *)(Scene_GetRecord_2(22) + 90) &= 254;
    Motion_CommitPositionAndActivate_6(22, 0, -16);
    {
        u8 *record = Scene_GetRecord_3(22);
        u8 value = *(volatile u8 *)&record[90];

        record[90] = (u8)(value | 1);
    }
    ObjectMotion_ArmCallback_15(22, 0x4100, 0);
    BattleRuntime_WaitIfModeZero_64(30);
    BattleRuntime_WaitIfModeZero_65(10);
    BattleFx_SpawnLinked_9(22, 0x100, 40);
    BattleEv_RunWait_27(22, 0);
    BattleRuntime_WaitIfModeZero_66(10);
    Motion_CallWaitAnim_16(22, 3);
    BattleRuntime_WaitIfModeZero_67(30);
    BattleEv_RunWait_28(22, 0);
    BattleRuntime_WaitIfModeZero_68(20);
    Object_SetModeById_9(0, 3);
    Object_SetModeById_10(1, 3);
    Object_SetModeById_11(2, 3);
    Motion_CallWaitAnim_17(3, 3);
    BattleRuntime_WaitIfModeZero_69(50);
    BattleRuntime_WaitIfModeZero_70(10);
    Motion_CallWaitAnim_18(22, 3);
    BattleRuntime_WaitIfModeZero_71(30);
    Motion_CommitPositionAndActivate_7(22, -16, 0);
    Motion_ArmCb_16(22, 0xc000, 0);
    Battle_WaitMode0_72(20);
    Talk_SetRect3(34, 0, 1, 2, 4, 18);
    Audio_PlayCue_3(158);
    Battle_WaitMode0_73(10);
    Motion_CommitPositionAndActivate_8(22, 0, -16);
    Motion_SetHPosTerrain_3(22, 0, 0);
    Battle_WaitMode0_74(10);
    Talk_SetRect4(32, 0, 1, 2, 4, 18);
    Audio_PlayCue_4(159);
    Battle_WaitMode0_75(50);
    Motion_ArmCb_17(0, 0x4000, 0);
    ObjectMotion_SetAngleToward_1(1, 0, 0);
    ObjectMotion_SetAngleToward_2(2, 0, 0);
    Battle_WaitMode0_76(20);
    BattleEv_RunWait_29(1, 0);
    Battle_WaitMode0_77(10);
    ObjectMotion_SetVariantCallbackAndInvokeObject_5(3, 2);
    Battle_WaitMode0_78(20);
    BattleEv_RunWait_30(3, 0);
    Battle_WaitMode0_79(10);
    Motion_CallWaitAnim_19(2, 3);
    Battle_WaitMode0_80(30);
    BattleEv_RunWait_31(2, 0);
    Battle_WaitMode0_81(10);
    Object_SetModeById_12(0, 3);
    Battle_WaitMode0_82(40);
    Object_SetModeById_13(1, 3);
    Object_SetModeById_14(2, 3);
    Motion_CallWaitAnim_20(3, 3);
    Battle_WaitMode0_83(30);
    ObjectMotion_SetSpeedParameters_5(1, 0x13333, 0x9999);
    ObjectMotion_SetSpeedParameters_6(3, 0x13333, 0x9999);
    ObjectMotion_SetSpeedParameters_7(2, 0x13333, 0x9999);
    Object_SetModeById_15(1, 2);
    record = Scene_GetRecord_4(0);
    if (record != 0) {
        ObjectMotion_ResetAndSetPosition_1(1, REC_S16(record, 10), REC_S16(record, 18));
    }
    ObjectMotion_CommitCurrentPositionAndActivate_2(1);
    Motion_SetHPosTerrain_4(1, 0, 0);
    Object_SetModeById_16(3, 2);
    record = Scene_GetRecord_5(0);
    if (record != 0) {
        Motion_ResetAndSetPosition_2(3, REC_S16(record, 10), REC_S16(record, 18));
    }
    Motion_CommitPos_3(3);
    Motion_SetHPosTerrain_5(3, 0, 0);
    Object_SetModeById_17(2, 2);
    record = Scene_GetRecord_6(0);
    if (record != 0) {
        Motion_ResetAndSetPosition_3(2, REC_S16(record, 10), REC_S16(record, 18));
    }
    Motion_CommitPos_4(2);
    Motion_SetHPosTerrain_6(2, 0, 0);
    Talk_Do2(10);
    BattleRuntime_ScheduleShoulderButtonModeUpdate_1();
}

/* A fixed, unbranching sequence of overlay calls with constant arguments: no
 * loop, no stored result, no use of the scene work record. */
void Scene_RunScene3c6SequenceA(void)
{
    u32 i;
    u8 *record;

    SceneWork_SetStepValue_1(0x28b0);
    BattleRuntime_WaitIfModeZero_1(20);
    ObjectMotion_SetVariantCallbackAndInvokeObject_1(22, 2);
    BattleRuntime_WaitIfModeZero_2(20);
    BattleEvent_RunActionAndWait_1(22, 0);
    BattleRuntime_WaitIfModeZero_3(10);
    ObjectMotion_CallThenWaitForAnimationChange_1(22, 4);
    BattleRuntime_WaitIfModeZero_4(20);
    BattleEvent_RunActionAndWait_2(22, 0);
    BattleRuntime_WaitIfModeZero_5(10);
    BattleEffect_SpawnLinkedResourceObject_1(0, 0x100, 0);
    BattleEffect_SpawnLinkedResourceObject_2(1, 0x100, 0);
    BattleEffect_SpawnLinkedResourceObject_3(3, 0x100, 0);
    BattleEffect_SpawnLinkedResourceObject_4(2, 0x100, 55);
    BattleRuntime_WaitIfModeZero_6(10);
    ObjectMotion_SetVariantCallbackAndInvokeObject_2(1, 2);
    BattleRuntime_WaitIfModeZero_7(20);
    BattleEvent_RunActionAndWait_3(1, 0);
    BattleRuntime_WaitIfModeZero_8(10);
    ObjectMotion_CallThenWaitForAnimationChange_2(22, 4);
    BattleRuntime_WaitIfModeZero_9(20);
    BattleEvent_RunActionAndWait_4(22, 0);
    BattleRuntime_WaitIfModeZero_10(10);
    BattleEffect_SpawnLinkedResourceObject_5(3, 0x102, 40);
    BattleEvent_RunActionAndWait_5(3, 0);
    BattleRuntime_WaitIfModeZero_11(10);
    ObjectMotion_CallThenWaitForAnimationChange_3(1, 3);
    BattleRuntime_WaitIfModeZero_12(30);
    BattleEvent_RunActionAndWait_6(1, 0);
    BattleRuntime_WaitIfModeZero_13(10);
    ObjectMotion_ArmCallback_1(2, 0x8000, 0);
    BattleRuntime_WaitIfModeZero_14(65);
    ObjectMotion_ArmCallback_2(2, 0xc000, 0);
    BattleRuntime_WaitIfModeZero_15(40);
    ObjectMotion_CallThenWaitForAnimationChange_4(2, 3);
    BattleRuntime_WaitIfModeZero_16(30);
    BattleEvent_RunActionAndWait_7(2, 0);
}

void Scene_RunSequenceB(void)
{
    extern u8 gWork[];

    u32 i;
    s32 record;

    Talk_Run15();
    Talk_Do3(0x28b7);
    Talk_Run16(1, 0);
    Talk_Place3(0, 0, -16);
    Talk_Run17();
}

s32 SceneData_SelectSecondaryTableByFlag9a7(void)
{
    if (Talk_Check6(0x9A7) != 0) {
        return 0x02009EE4;
    }
    return 0x02009D04;
}
