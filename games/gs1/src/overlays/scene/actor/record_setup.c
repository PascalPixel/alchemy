#include "types.h"
#include "scene.h"
#include "abi/overlays/scene/actor/record_setup.h"

#define STEP_COUNTER (*(u16 *)(*(u8 **)0x03001ebc + 0x1d8))

struct SceneRecord {
    u8 unk_00[9];
    u8 flags;
    u8 unk_0a[20];
    u16 angle;
    u8 unk_20[6];
    u8 field_26;
};

/*
 * Byte and field views of one actor share this union so that stores through
 * the linked record do not make the pointer read disappear through alias
 * analysis.
 */
union SceneActor {
    struct {
        u8 unk_00[12];
        s32 y;
        u8 unk_10[64];
        struct SceneRecord *record;
        u8 unk_54;
        u8 mode;
        u8 unk_56[3];
        u8 flags;
    } fields;
    u8 bytes[90];
};

extern u8 SceneMessage_FacingChoiceBase;
extern u8 Value_00002624;
extern s16 gCell[];

u16 *Actor_Run(s32);

u16 *Actor_Run2(s32);

void SceneMessage_Show(s32);

u16 *Actor_Run3(s32);

union SceneActor *Actor_Run4(s32);
union SceneActor *Actor_Run5(s32);
union SceneActor *Actor_Run6(s32);
union SceneActor *Actor_Run7(s32);

/*
 * Each pseudo symbol above names the per-site call word the overlay image
 * holds, not a runtime address -- one word can serve two sites with different
 * targets -- and the macro names the function the site reaches through the
 * veneers, keeping the site's own calling form. Names without a repository
 * binding are provisional.
 */

/*
 * Returns the in-image script table at 0x02008c3c. The eight-byte owner
 * includes its one pool word, which holds that address and is returned
 * without being dereferenced.
 */
u8 *SceneData_GetScriptTable(void)
{
    extern u8 *gWork;

    return (u8 *)0x02008c3c;
}

/*
 * Returns the in-image table at 0x02008da4. The eight-byte owner includes its
 * one pool word, which holds that address and is returned without being
 * dereferenced.
 */
u8 *SceneData_GetTable8da4(void)
{
    extern u8 *gWork;

    return (u8 *)0x02008da4;
}

/*
 * Returns the in-image table at 0x02008dd4. The eight-byte owner includes its
 * one pool word, which holds that address and is returned without being
 * dereferenced.
 */
u8 *SceneData_GetTable8dd4(void)
{
    extern u8 *gWork;

    return (u8 *)0x02008dd4;
}

s32 SceneData_SelectTable8e08ByFlag96f(void)
{
    extern u8 *gWork;

    if (Actor_Check(0x96f) != 0) {
        return 0x02008f28;
    }
    return 0x02008e08;
}

void Dialogue_HandleFacingChoice(s32 no)
{
    extern u8 *gWork;

    u16 facing = (Actor_Run(0)[3] + 0x2000) & ~0x3fff;
    if (facing == 0xc000) {
        Actor_Apply(31, no);
    } else if (Actor_Check2(0x96f)) {
        s32 msg = (s32)&SceneMessage_FacingChoiceBase;
        Actor_Do(msg);
        Actor_Apply2(no, 0);
        if (Actor_Apply3(0, 0) == 0) {
            Actor_Do2(10);
            Actor_Do3(msg + 1);
        } else {
            Actor_Do4(msg + 2);
        }
        Actor_Apply4(no, 0);
    } else {
        Actor_Do5(0x25cf);
        Actor_Apply5(no, 0);
    }
}

void Dialogue_HandleFacingBranch(s32 no)
{
    extern u8 *gWork;

    u16 facing = (Actor_Run3(0)[3] + 0x2000) & ~0x3fff;
    if (facing == 0xc000) {
        Actor_Apply6(10, no);
    } else if (Actor_Check3(0x96f)) {
        Actor_Do6(0x2620);
        Actor_Apply7(no, 0);
    } else {
        Actor_Do7(0x25d1);
        Actor_Apply8(no, 0);
    }
}

void Dialogue_HandleFacingAction(s32 no)
{
    extern u8 *gWork;

    u16 facing = (Actor_Run2(0)[3] + 0x2000) & ~0x3fff;
    if (facing == 0xc000) {
        Actor_Do8(no);
    } else if (Actor_Check4(0x96f)) {
        Actor_Do9(0x262c);
        Actor_Apply9(no, 0);
    } else {
        SceneMessage_Show(0x25d5);
        Actor_Apply10(no, 0);
    }
}

void Scene_RunActorCueBranch(s32 obj)
{
    extern u8 *gWork;

    s32 cue = (s32)&Value_00002624;
    Actor_Do10(cue);
    Actor_Apply11(obj, 0);
    if (Actor_Apply12(0, 0) == 0) {
        Actor_Do11(10);
        Actor_Do12(cue + 1);
    } else {
        Actor_Do13(cue + 2);
    }
    Actor_Apply13(obj, 0);
}

void State_ApplyCounter16cThenCall7b(void)
{
    extern u8 *gWork;

    u8 *state = gWork;
    s16 *cnt = (s16 *)(state + 0x16C);

    Actor_Do14(*cnt);
    Actor_Do15(0x7B);
}

/*
 * Runs the primary script for this scene: a long fixed sequence driving
 * actors 10, 19, 20, 21, 30 and 40 through position, pose and timing steps,
 * guarded by an initial skip check. The calls are in machine order, and that
 * order is what reproduces the reference.
 */
void Scene_RunPrimaryScript(void)
{
    extern u8 *gWork;

    u8 *record;
    GameFlag_Set_1(2480);
    if (GameFlag_IsSet_1(2442) == 0) {
    Audio_PlayCue_1(30);
    Battle_Reset_1();
    Motion_CamBounds_1(24117248, -1, 6815744, 1);
    Motion_SetPosReset_1(0, 368, 160);
    Motion_ArmCb_1(0, 49152, 0);
    Motion_RealignToTrackedObjectAndArmCallback_1(19, 0, -16, 49152);
    Motion_CommitPos_1(19);
    Object_CommitPositionThenWaitIfModeZero_1();
    SceneWork_SetStepValue_1(9707);
    Battle_WaitMode0_1(10);
    Motion_SetVarCbObj_1(20, 2);
    Battle_WaitMode0_2(20);
    BattleEv_RunWait_1(20, 0);
    Battle_WaitMode0_3(10);
    Motion_SetVarCbObj_2(19, 2);
    Battle_WaitMode0_4(20);
    Motion_SetSpeed_1(19, 78643, 39321);
    Motion_CommitPositionAndActivate_1(19, 0, -16);
    Motion_ArmCb_2(19, 0, 0);
    Battle_WaitMode0_5(30);
    Motion_ArmCb_3(19, 57344, 0);
    Battle_WaitMode0_6(30);
    Motion_ArmCb_4(19, 0, 0);
    Battle_WaitMode0_7(30);
    BattleFx_SpawnLinked_1(19, 256, 40);
    BattleEv_RunWait_2(19, 0);
    Battle_WaitMode0_8(10);
    Motion_SetVarCbObj_3(21, 2);
    Battle_WaitMode0_9(20);
    BattleEv_RunWait_3(21, 0);
    Battle_WaitMode0_10(10);
    Motion_SetSpeed_2(19, 78643, 39321);
    Motion_CommitPositionAndActivate_2(19, 0, -24);
    Motion_CommitPositionAndActivate_3(19, 48, 0);
    Motion_ArmCb_5(19, 49152, 0);
    Battle_WaitMode0_11(30);
    BattleEv_RunWait_4(19, 0);
    Battle_WaitMode0_12(20);
    BattleFx_SpawnLinked_2(19, 256, 40);
    BattleEv_RunWait_5(19, 0);
    Battle_WaitMode0_13(20);
    BattleFx_SpawnLinked_3(20, 258, 40);
    BattleEv_RunWait_6(20, 0);
    Battle_WaitMode0_14(10);
    Motion_ArmCb_6(19, 8192, 0);
    Battle_WaitMode0_15(20);
    BattleEv_RunWait_7(19, 0);
    Battle_WaitMode0_16(10);
    Motion_SetVarCbObj_4(21, 2);
    Battle_WaitMode0_17(20);
    Motion_CallWaitAnim_1(21, 4);
    Battle_WaitMode0_18(20);
    BattleEv_RunWait_8(21, 0);
    Battle_WaitMode0_19(10);
    Motion_ArmCb_7(19, 49152, 0);
    Battle_WaitMode0_20(30);
    BattleFx_SpawnLinked_4(19, 263, 40);
    BattleEv_RunWait_9(19, 0);
    Battle_WaitMode0_21(10);
    Motion_SetVarCb_1(20, 2);
    Motion_SetVarCbObj_5(21, 2);
    Battle_WaitMode0_22(20);
    Motion_SetSpeed_3(0, 78643, 39321);
    Motion_SetPosReset_2(0, 368, 104);
    Motion_CommitPositionAndActivate_4(0, 16, 0);
    Motion_ArmCb_8(0, 0, 0);
    Battle_WaitMode0_23(20);
    Battle_WaitMode0_24(10);
    Motion_CallWaitAnim_2(19, 4);
    Battle_WaitMode0_25(20);
    BattleEv_RunWait_10(19, 0);
    Battle_WaitMode0_26(20);
    BattleFx_SpawnLinked_5(19, 258, 50);
    BattleEv_RunWait_11(19, 0);
    Battle_WaitMode0_27(10);
    Motion_SetVarCbObj_6(20, 2);
    Battle_WaitMode0_28(20);
    BattleEv_RunWait_12(20, 0);
    Battle_WaitMode0_29(10);
    Motion_SetAngleToward_1(0, 20, 0);
    Motion_ArmCb_9(19, 8192, 0);
    Battle_WaitMode0_30(20);
    BattleEv_RunWait_13(19, 0);
    Battle_WaitMode0_31(10);
    Motion_SetVarCbObj_7(21, 2);
    Battle_WaitMode0_32(20);
    BattleEv_RunWait_14(21, 0);
    Battle_WaitMode0_33(10);
    Motion_SetAngleToward_2(0, 21, 0);
    Motion_ArmCb_10(19, 49152, 0);
    Battle_WaitMode0_34(40);
    Motion_CallWaitAnim_3(19, 3);
    Battle_WaitMode0_35(30);
    BattleEv_RunWait_15(19, 0);
    Battle_WaitMode0_36(10);
    Motion_SetVarCbObj_8(21, 2);
    Battle_WaitMode0_37(20);
    Motion_CallWaitAnim_4(21, 4);
    Battle_WaitMode0_38(20);
    BattleEv_RunWait_16(21, 0);
    Battle_WaitMode0_39(10);
    Object_LinkPair_1(19, 0, 30);
    BattleEv_RunWait_17(19, 0);
    Battle_WaitMode0_40(10);
    Motion_SetVarCbObj_9(20, 2);
    Battle_WaitMode0_41(20);
    BattleEv_RunWait_18(20, 0);
    Battle_WaitMode0_42(10);
    Motion_SetAngleToward_3(0, 20, 0);
    Motion_ArmCb_11(19, 8192, 0);
    Battle_WaitMode0_43(40);
    Battle_WaitMode0_44(10);
    Motion_SetVarCbObj_10(21, 2);
    Battle_WaitMode0_45(20);
    BattleEv_RunWait_19(21, 0);
    Battle_WaitMode0_46(10);
    Motion_SetAngleToward_4(0, 21, 0);
    Motion_ArmCb_12(19, 49152, 0);
    Battle_WaitMode0_47(40);
    Object_SetModeById_1(0, 3);
    Motion_CallWaitAnim_5(19, 3);
    Battle_WaitMode0_48(30);
    Battle_WaitMode0_49(10);
    Motion_CallWaitAnim_6(20, 4);
    Battle_WaitMode0_50(20);
    BattleEv_RunWait_20(20, 0);
    Battle_WaitMode0_51(10);
    BattleFx_SpawnLinked_6(0, 256, 0);
    BattleFx_SpawnLinked_7(19, 256, 40);
    Battle_WaitMode0_52(10);
    Motion_SetAngleToward_5(0, 20, 0);
    Motion_ArmCb_13(19, 8192, 0);
    Battle_WaitMode0_53(40);
    Battle_WaitMode0_54(10);
    Motion_SetVarCbObj_11(21, 2);
    Battle_WaitMode0_55(20);
    BattleEv_RunWait_21(21, 0);
    Battle_WaitMode0_56(10);
    BattleFx_SpawnLinked_8(0, 256, 50);
    Motion_SetAngleToward_6(0, 21, 0);
    Battle_WaitMode0_57(30);
    Motion_SetVarCbObj_12(21, 2);
    Battle_WaitMode0_58(30);
    Battle_WaitMode0_59(10);
    Motion_ArmCb_14(19, 49152, 0);
    Battle_WaitMode0_60(30);
    BattleEv_RunWait_22(19, 0);
    Battle_WaitMode0_61(10);
    Motion_SetVarCbObj_13(21, 2);
    Battle_WaitMode0_62(20);
    Motion_CallWaitAnim_7(21, 3);
    Battle_WaitMode0_63(30);
    Battle_WaitMode0_64(10);
    BattleFx_SpawnLinked_9(19, 258, 40);
    BattleEv_RunWait_23(19, 0);
    Battle_WaitMode0_65(10);
    Motion_CallWaitAnim_8(20, 4);
    Battle_WaitMode0_66(20);
    BattleEv_RunWait_24(20, 0);
    Battle_WaitMode0_67(10);
    BattleFx_SpawnLinked_10(19, 257, 50);
    Battle_WaitMode0_68(10);
    Motion_SetAngleToward_7(0, 20, 0);
    Motion_ArmCb_15(19, 8192, 0);
    Battle_WaitMode0_69(20);
    Battle_WaitMode0_70(10);
    Motion_SetVarCbObj_14(20, 2);
    Battle_WaitMode0_71(20);
    BattleEv_RunWait_25(20, 0);
    Battle_WaitMode0_72(10);
    BattleFx_SpawnLinked_11(19, 256, 40);
    BattleEv_RunWait_26(19, 0);
    Battle_WaitMode0_73(10);
    Motion_SetVarCbObj_15(21, 2);
    Battle_WaitMode0_74(20);
    BattleEv_RunWait_27(21, 0);
    Motion_SetVarCbObj_16(21, 2);
    Battle_WaitMode0_75(20);
    Battle_WaitMode0_76(10);
    Object_LinkPair_2(19, 0, 0);
    Battle_WaitMode0_77(30);
    BattleEv_RunWait_28(19, 0);
    Battle_WaitMode0_78(10);
    Motion_CallWaitAnim_9(20, 4);
    Battle_WaitMode0_79(20);
    BattleEv_RunWait_29(20, 0);
    Battle_WaitMode0_80(10);
    BattleFx_SpawnLinked_12(0, 258, 0);
    BattleFx_SpawnLinked_13(19, 258, 80);
    BattleFx_SpawnLinked_14(21, 258, 50);
    BattleEv_RunWait_30(21, 0);
    Battle_WaitMode0_81(10);
    Motion_SetVarCbObj_17(21, 2);
    Battle_WaitMode0_82(20);
    BattleEv_RunWait_31(21, 0);
    Motion_SetVarCbObj_18(21, 3);
    Battle_WaitMode0_83(20);
    Battle_WaitMode0_84(10);
    Motion_ArmCb_16(19, 49152, 0);
    Battle_WaitMode0_85(30);
    Motion_SetVarCbObj_19(19, 2);
    Battle_WaitMode0_86(10);
    BattleEv_RunWait_32(19, 0);
    Battle_WaitMode0_87(10);
    Motion_SetVarCbObj_20(20, 2);
    Battle_WaitMode0_88(20);
    BattleEv_RunWait_33(20, 0);
    Battle_WaitMode0_89(10);
    Motion_SetAngleToward_8(0, 20, 0);
    Motion_ArmCb_17(19, 8192, 0);
    Battle_WaitMode0_90(30);
    Motion_CallWaitAnim_10(19, 3);
    Battle_WaitMode0_91(30);
    BattleEv_RunWait_34(19, 0);
    Battle_WaitMode0_92(10);
    Motion_SetVarCb_2(20, 2);
    Motion_SetVarCbObj_21(21, 2);
    Battle_WaitMode0_93(30);
    Battle_WaitMode0_94(10);
    Object_LinkPair_3(19, 0, 20);
    Motion_CommitPositionAndActivate_5(19, -12, 0);
    Battle_WaitMode0_95(20);
    Motion_SetSpeed_4(19, 0);
    if (UiWork_WaitThenFinalizeCapacity_1(0, 0) == 0) {
    Battle_WaitMode0_96(20);
    BattleEv_RunWait_35(19, 0);
    STEP_COUNTER++;
    } else {
    Battle_WaitMode0_97(10);
    STEP_COUNTER++;
    BattleEv_RunWait_36(19, 0);
    }
    Battle_WaitMode0_98(10);
    Motion_SetVarCbObj_22(19, 2);
    Battle_WaitMode0_99(20);
    BattleEv_RunWait_37(19, 0);
    Battle_WaitMode0_100(10);
    BattleFx_SpawnLinked_15(19, 258, 50);
    BattleEv_RunWait_38(19, 0);
    Battle_WaitMode0_101(10);
    Motion_CallWaitAnim_11(0, 3);
    Battle_WaitMode0_102(20);
    Motion_CallWaitAnim_12(19, 3);
    Battle_WaitMode0_103(30);
    Audio_PlayCue_2(30);
    Motion_SetSpeed_5(19, 78643, 39321);
    Object_SetModeById_2(19, 2);
    record = (u8 *)Scene_GetRecord_1(0);
    if (record != 0) {
    Motion_ResetAndSetPosition_1(19, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Motion_CommitPos_2(19);
    Motion_SetHPosTerrain_1(19, 0, 0);
    Battle_WaitMode0_104(10);
    Audio_PlayCueForPartyMember_1();
    Battle_SchedShoulder_1();
    }
}

s32 SceneData_SelectTable9090ByFlag96f(void)
{
    extern u8 *gWork;

    if (Actor_Check5(0x96F) != 0) {
        return 0x020091EC;
    }
    return 0x02009090;
}

s32 Scene_InitActorRecords(void)
{
    extern s32 *gWork;

    union SceneActor *work;
    if (gCell[225] == 90)
        Actor_Do16(0x96f);
    gWork[112] = 521;
    gWork[114] = 24;
    Actor_Run4(12)->bytes[89] |= 4;
    Actor_Run5(13)->bytes[89] |= 4;
    work = Actor_Run6(20);
    work->fields.record->field_26 = 0;
    work->fields.record->angle = 0x4000;
    {
        /* The mask is built in a local, not folded into the store. */
        struct SceneRecord *record = work->fields.record;
        s32 flags = ~12;

        flags = flags & record->flags;
        record->flags = flags | 4;
    }
    work = Actor_Run7(21);
    work->fields.record->field_26 = 0;
    work->fields.record->angle = 0x4000;
    work->bytes[85] = 2;
    work->fields.y = 0;
    return 0;
}
