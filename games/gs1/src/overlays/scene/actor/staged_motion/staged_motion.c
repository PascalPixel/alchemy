#include "types.h"
#include "scene.h"
#include "abi/overlays/scene/actor/staged_motion/staged_motion.h"
#include "staged_actor_movement.h"
#include "run_staged_actor_movement_and_redraw_body.inc"

/* overlays/scene/actor/staged_motion/actor_motion.c */

s32 Actor_RunStep18WhenTargetSet(s32 *p)
{
    s32 t = Actor_Check22(0);
    if (p[14] == (s32)0x80000000 && p[16] == (s32)0x80000000)
        return 0;
    Actor_SetMode((s32)p, t, 18, 0);
    return 0;
}

/* overlays/scene/actor/staged_motion/actor_position.c */
extern u8 Value_0000087a;

s32 *Actor_Run43(s32);

u8 *Actor_Run44(s32);
u8 *Actor_Run45(s32);

void Actor_SetFlagByteBySlotZeroPosition(void)
{
    s32 *g = Actor_Run43(0);
    u8 *q;
    if (Actor_Check23((s32)&Value_0000087a) != 0)
        q = Actor_Run44(21);
    else
        q = Actor_Run45(20);
    if (q != 0) {
        if (g[3] > 0xc80000)
            q[0x23] = 3;
        else
            q[0x23] = 1;
    }
}

/* overlays/scene/actor/staged_motion/actor_presentation.c */
extern char Value_00000806;

void Dialogue_RunActorFourteenTalk(void)
{
    s32 flag = 0x806;
    Actor_Run46();
    if (Actor_Check24(0x815) != 0) {
        Actor_Do19(0x11c9);
        Actor_Apply(14, 0);
    } else if (Actor_Check25(flag) == 0) {
        Actor_Do20(flag);
        Actor_Do21(0xf7c);
        Actor_Place260(14, 0, 4);
        Actor_Apply2(14, 0);
    } else {
        Actor_Do22(0xf7e);
        Actor_Place261(14, 0, 4);
        Actor_Apply3(14, 0);
    }
    Actor_Run47();
}

/* overlays/scene/actor/staged_motion/effect.c */
extern volatile u32 gIw;

void Effect_UpdateObjectOnOddFrames(s32 p)
{
    if ((gIw & 1) != 0)
        Actor_Apply4(p, Actor_Apply5(gIw >> 1, 6));
    if ((gIw & 0xf) == 0)
        Actor_Do23(p);
}

void Effect_UpdateObjectOnOddFramesOnly(s32 p)
{
    if ((gIw & 1) != 0)
        Actor_Apply6(p, Actor_Apply7(gIw >> 1, 6));
}

/* overlays/scene/actor/staged_motion/field_scene.c */
/* Actor IDs the scene sets up: three actors share a common data pointer,
 * a fourth actor (11) is driven through its own sequence of position/speed
 * calls. */
#define ACTOR_A 12
#define ACTOR_B 13
#define ACTOR_C 14
#define ACTOR_D 11
/* Shared data used for actors A, B, C. */

extern u8 gWork[];
extern u8 Value;
extern u8 Value4;
extern u8 gVal[];
extern u8 gVal2[];
extern u8 gVal3[]; /* Buffer passed by address to SceneWork_SetStepValue_2. */
extern u8 gOv[];
extern u8 gVal4[];
extern u8 gOv2[];
extern u8 Value_00000ee4;

u8 *Actor_Run48();
u8 *Actor_Run49();

u8 *Actor_Run50();

u8 *Actor_Run51();

/* Runs four fixed steps in order: a single-argument call, a call that takes
 * the address of the byte at Value4 plus two constants, another
 * three-constant call, and a final single-argument call. */

/* Newly identified engine calls: each macro below names the site's own
 * engine function from its reconstructed C source, and keeps the site's
 * own calling form. */

void Scene_RunFlag807BranchSequence(void)
{
    u32 i;
    s32 record;

    Actor_Run52();
    if (Actor_Check(0x807) == 0) {
        Actor_Do(0x807);
        Actor_Do2(0xf63);
        Actor_Place(18, 0x103, 0);
        Actor_Run53(0, 18, 20);
        Actor_Run54(18, 0, 6);
        Actor_Place2(18, 0x8000, 30);
        Actor_Run55(18, 2, 20);
        Actor_Run56(18, 0, 6);
        Actor_Run57(18, 0, 10);
        Actor_Place3(18, 0x103, 0);
        Actor_Run58(18, 0, 10);
        Actor_Place4(0, 0x102, 60);
    } else {
        Actor_Place5(18, 0x103, 0);
        Actor_Do3(0xf66);
        Actor_Run59(18, 0, 20);
    }
    Actor_Run60();
}

void Scene_RunScene373(void)
{
    u32 i;
    s32 record;

    Actor_Run61();
    Actor_Place6(0x30000, 0x30000, 0x10000);
    Actor_Run62(10);
    Actor_Place7(-1, -1, 0xe666);
    Actor_Do4(0x1c9a);
    Actor_Run63(17, 0, 20);
    Actor_Run64(17, 0, 20);
    Actor_Run65(17, 0);
    Actor_Run66();
}

void Scene_RunScene373(void)
{
    u32 i;
    s32 record;

    Actor_Run67(188);
    Actor_Place8(0x200f544, 45, 11);
    Actor_Place9(0, 0x101, 0x1a4);
    Actor_Run68(11);
}

void Scene_RunScene373(void)
{
    u32 i;
    s32 record;

    Actor_Run69(158);
    Actor_Place10(0x200f55a, 54, 32);
    Actor_Place11(0, 0x196, 0x2d7);
    Actor_Run70(5);
}

void Scene_RunScene373(void)
{
    u32 i;
    s32 record;

    Actor_Run71(158);
    Actor_Place12(0x200f570, 45, 39);
    Actor_Place13(0, 0x106, 0x325);
    Actor_Run72(6);
}

/* Runs four scene primitives in sequence: one single-argument call, one call
 * passing the address of Value plus two small constants, one call
 * with a byte-flag-sized first argument (0) and two larger constants, and a
 * final single-argument call. */
void Scene_RunPrimarySequence(void)
{
    Audio_PlayCue_1(158);
    Actor_Place14((s32)&Value, 49, 69); /* main:08009178 */
    Motion_ResetPosMode2_1(0, 326, 0x466); /* object_id 0, x 326, z 0x466 */
    Actor_Do5(8); /* main:0808a248 */
}

/* Runs a short scripted step, then two 3-argument setup calls, then another
 * short scripted step; none of the callees' effects are visible here. */
void Scene_RunScene373SequenceA(void)
{
    Audio_PlayCue_1(158);
    Actor_Place15(0x200f586, 52, 76); /* main:08009178 */
    ObjectMotion_ResetAndSetPositionInMode2_1(0, 0x176, 0x4d6); /* object_id 0, x 0x176, z 0x4d6 */
    Actor_Run73(9); /* main:0808a248 */
}

void Scene_RunPrimarySequenceSecond(void)
{
    Audio_PlayCue_1(158);
    Actor_Place16((s32)&Value4, 35, 74); /* main:08009178 */
    ObjectMotion_ResetAndSetPositionInMode2_1(0, 102, 0x4b6);
    Actor_Do6(10); /* main:0808a248 */
}

void Scene_RunScene373SequenceC(void)
{
    u32 i;
    s32 rec7;
    s32 rec8;
    s32 record;

    rec8 = Actor_Check2(0);
    rec7 = Actor_Check3(5);
    Actor_Run74();
    *(volatile s32 *)(rec7 + 8) = *(volatile s32 *)(rec8 + 8);
    *(volatile s32 *)(rec7 + 12) = *(volatile s32 *)(rec8 + 12);
    *(volatile s32 *)(rec7 + 16) = *(volatile s32 *)(rec8 + 16);
    *(volatile s32 *)(rec7 + 56) = -0x80000000;
    *(volatile s32 *)(rec7 + 60) = -0x80000000;
    *(volatile s32 *)(rec7 + 64) = -0x80000000;
    *(volatile s32 *)(rec7 + 36) = 0;
    *(volatile s32 *)(rec7 + 40) = 0;
    *(volatile s32 *)(rec7 + 44) = 0;
    *(volatile s32 *)(rec7 + 20) = *(volatile s32 *)(rec8 + 12);
    Actor_Run75(1);
    Actor_Place17(5, 0x10000, 0x8000);
    Actor_Place18(5, 110, 0x11b);
    Actor_Run76(0, 5, 2);
    Actor_Do7(0xf39);
    if (*(volatile s32 *)(rec8 + 8) < *(volatile s32 *)(rec7 + 8)) {
        Actor_Place19(0xa005, 0, 2);
    } else {
        Actor_Place20(0x8005, 0, 2);
    }
    Actor_Run77(0, 3);
    Actor_Run78(2);
    Actor_Run79(5, 2);
    record = Actor_Check4(0);
    if (record != 0) {
        Actor_Run80(5, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Actor_Run81(5);
    Actor_Run82(5, 0, 0);
    Actor_Place21(0, 110, 0x12f);
    Actor_Run83();
}

void Scene_RunScene373SequenceD(void)
{
    u32 i;
    s32 record;
    s32 base5_f4d;

    if (Actor_Check5(0x808) == 0) {
        Actor_Run84();
        Actor_Place22(0, 0x10000, 0x8000);
        base5_f4d = (s32)gVal;
        Actor_Run85(base5_f4d);
        Actor_Run86(15, 0, 2);
        Actor_Run87(16, 0, 2);
        Actor_Run88((base5_f4d + 2), 1);
        Actor_Run89(6);
        Actor_Place23(0, 69, 0x366);
        Actor_Run90();
    }
}

void Scene_RunScene373SequenceE(void)
{
    u32 i;
    u8 *rec7;
    s32 record;

    rec7 = Actor_Check6(22);
    Actor_Run91();
    Actor_Place24(0, 0x20000, 0x20000);
    Actor_Run92(0, 5, 0);
    Actor_Place25(0, 215, 0x193);
    rec7[90] |= 1;
    Actor_Place26(22, 0xa60000, 0x1770000);
    Actor_Place27(22, 0x2000, 20);
    rec7[90] = (rec7[90] ^ 1);
    Actor_Place28(22, 0x28000, 0x28000);
    Actor_Run93(22, 4, 0);
    Actor_Place29(22, 202, 0x18b);
    Actor_Run94(0, 1);
    Actor_Run95(10);
    Actor_Place30(0, 0xb000, 0);
    Actor_Place31(22, 0x3000, 24);
    Actor_Place32(0, 0x100, 0);
    Actor_Run96(0, 2);
    Actor_Run97(20);
    Actor_Place33(22, 0x18000, 0x10000);
    Actor_Run(0, 0x200f59c);
    Actor_Run98(10);
    Actor_Place34(22, 0x103, 0);
    Actor_Run2(22, 0x200f5ec);
    Actor_Run99(0);
    Actor_Place35(0, 0x100, 0x1da);
    Actor_Place36(0, 0xc000, 0);
    Actor_Run100(22);
    Actor_Place37(22, 0x100, 0x1c8);
    Actor_Run101(0, 1);
    Actor_Place38(22, 0x4000, 20);
    Actor_Run102(22, 2);
    Actor_Run103(20);
    Actor_Do8(0xfce);
    Actor_Run104(22, 0);
    record = Actor_Check26(22);
    *(volatile s32 *)(record + 108) = 0x200d72d;
    Actor_Run3(22, 0x200e248);
    Actor_Do9(0x823);
    Actor_Run105();
}

void Scene_RunScene373(s32 a0, s32 a1)
{
    u32 i;
    s32 p8;
    s32 rec8;
    s32 record;

    p8 = a1;
    rec8 = Actor_Check7(22);
    Actor_Run106();
    Actor_Run107(22, 2);
    Actor_Place39(22, 0x100, 20);
    Actor_Run108(0, 2);
    Actor_Place40(0, 0x102, 40);
    Actor_Run109(0, a0);
    Actor_Run110(10);
    Actor_Place41(22, 0x103, 0);
    Actor_Run111(22, p8);
    Actor_Run112(0);
    Actor_Run113(20);
    Actor_Run114(22, 2);
    *(s32 *)(rec8 + 24) = 0x10000;
    *(s32 *)(rec8 + 28) = 0x10000;
    record = Actor_Check8(0);
    *(s32 *)(record + 24) = 0x10000;
    *(s32 *)(record + 28) = 0x10000;
    Actor_Do10(0xfce);
    Actor_Run115(22, 0);
    record = Actor_Check27(22);
    *(s32 *)(record + 108) = 0x200d72d;
    Actor_Run4(22, 0x200e248);
    Actor_Run116();
}

/* Runs a scripted movement/pose sequence for actors 0, 1 and 8, reading two
 * lookup records along the way (one 32-bit-field record, one 16-bit-field
 * record) to copy their stored values onto actor 1. */
void Scene_RunSecondaryActorSequence(void)
{
    u32 i;
    s32 record;
    s32 slot_table;

    Battle_Reset_1();
    ObjectGroup_ConfigureChildValue_1(0, 0);
    Motion_SetAngleToward_1(8, 0, 20);
    slot_table = (s32)gVal2;
    SceneWork_SetStepValue_1(slot_table);
    Motion_SetVarCb_1(8, 2);
    Battle_RunThenWaitIfModeZero_1(8, 0, 20);
    Motion_SetSpeedLim_1(0x10000, 0x2000); /* main:0808a208 */
    Motion_CamBounds_1(0x18e0000, -1, 0x2460000, 1);
    Motion_SetSpeed_1(0, 0xcccc, 0x6666);
    Motion_SetSpeed_2(1, 0xcccc, 0x6666);
    Motion_SetPosReset_1(0, 0x1a4, 0x260);
    Motion_ArmCb_1(0, 0xa000, 0);
    Motion_ArmCb_2(8, 0x3000, 0);
    record = Scene_GetRecord_1(0);
    if (record != 0) {
        /* Copy the record's fields at +8 and +16 onto actor 1. */
        Motion_SetHPosTerrain_1(1, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Motion_SetPosReset_2(1, 0x192, 0x260);
    Motion_ArmCb_3(1, 0xd000, 20);
    BattleEv_RunWait_1(0x1001, 0);
    Motion_ArmCb_4(8, 0x5000, 20);
    Motion_CallWaitAnim_1(8, 3); /* main:0808a110 */
    BattleEv_RunWait_2(0x4008, 0);
    Object_SetModeById_1(0, 3);
    Motion_CallWaitAnim_2(1, 3); /* main:0808a110 */
    Battle_WaitMode0_1(20);
    Motion_SetVarCbObj_1(8, 2); /* main:0808a138 */
    Motion_SetSpeed_3(0x4008, 0); /* main:0808a178 */
    if (UiWork_WaitThenFinalizeCapacity_1(0, 0) == 1) { /* main:0808a070 */
        bump_step(1);
        Motion_SetVarCb_2(8, 1);
    }
    Battle_RunThenWaitIfModeZero_2(0x4008, 0, 40); /* main:0808a188 */
    BattleFx_SpawnLinked_1(8, 0x105, 60);
    /* Pass the slot table's field at +6 for the slot passed above. */
    SceneWork_SetStepValue_2((slot_table + 6)); /* main:0808a170 */
    Battle_RunThenWaitIfModeZero_3(0x4008, 0, 20); /* main:0808a188 */
    Motion_SetVarCbObj_2(1, 1); /* main:0808a138 */
    Battle_WaitMode0_2(40);
    Battle_RunThenWaitIfModeZero_4(0x1001, 0, 40); /* main:0808a188 */
    Motion_SetVarCbObj_3(8, 1); /* main:0808a138 */
    Motion_ArmCb_5(8, 0xd000, 20);
    BattleEv_RunWait_3(0x4008, 0);
    Motion_CallWaitAnim_3(1, 3); /* main:0808a110 */
    Battle_RunThenWaitIfModeZero_5(0x1001, 0, 120); /* main:0808a188 */
    Battle_RunThenWaitIfModeZero_6(0x4008, 0, 20); /* main:0808a188 */
    BattleFx_SpawnLinked_2(1, 0x105, 40);
    Battle_RunThenWaitIfModeZero_7(0x1001, 0, 40); /* main:0808a188 */
    Motion_CallWaitAnim_4(8, 4); /* main:0808a110 */
    Battle_RunThenWaitIfModeZero_8(0x4008, 0, 20);
    Motion_CallWaitAnim_5(1, 3); /* main:0808a110 */
    Battle_WaitMode0_3(40);
    Motion_ArmCb_6(8, 0x5000, 20);
    Battle_RunThenWaitIfModeZero_9(0x4008, 0, 10); /* main:0808a188 */
    Object_SetModeById_2(0, 3);
    Motion_CallWaitAnim_6(1, 3); /* main:0808a110 */
    Battle_WaitMode0_4(20);
    Motion_CallWaitAnim_7(8, 3);
    Object_SetModeById_3(1, 2);
    record = Scene_GetRecord_2(0);
    if (record != 0) {
        /* Copy the record's fields at +10 and +18 onto actor 1. */
        Motion_ResetAndSetPosition_1(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Motion_CommitPos_1(1);
    Motion_SetHPosTerrain_2(1, 0, 0);
    GameFlag_Set_1(0x303);
    Battle_SchedShoulder_1(); /* main:0808a020 */
}

/* Runs the actor 0 / 1 / 9 setup sequence: position, speed and animation
 * calls in a fixed order. Two steps look up a record for one actor and
 * copy a coordinate pair out of it (offsets +8/+16 as s32, then +10/+18
 * as s16) into the call configuring another actor. */
void Scene_RunPrimaryActorSequence(void)
{
    u32 i;
    s32 record;

    BattleRuntime_Reset_1();
    ObjectMotion_PlaceWithinCameraBounds_1(0x1650000, -1, 0x2e20000, 1);
    ObjectMotion_SetPositionAndReset_1(0, 0x16f, 0x2e9);
    ObjectMotion_ArmCallback_1(0, 0xa000, 0);
    record = Scene_GetRecord_1(0);
    if (record != 0) {
        /* Copy the s32 coordinate pair at +8/+16 of the looked-up record. */
        ObjectMotion_SetHorizontalPositionWithTerrain_1(1, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    ObjectMotion_SetPositionAndReset_2(1, 0x15a, 0x2e9);
    ObjectMotion_ArmCallback_2(1, 0xd000, 20);
    SceneWork_SetStepValue_1(0x1c53);
    BattleEvent_RunActionAndWait_1(1, 0);
    ObjectMotion_SetVariantCallbackAndInvokeObject_1(9, 2);
    BattleEffect_SpawnLinkedResourceObject_1(9, 0x100, 0);
    ObjectMotion_ArmCallback_3(9, 0x3000, 10);
    ObjectMotion_ArmCallback_4(9, 0x5000, 10);
    ObjectMotion_ArmCallback_5(9, 0x3000, 40);
    BattleRuntime_RunThenWaitIfModeZero_1(9, 0, 20);
    Object_SetModeById_1(0, 3);
    ObjectMotion_CallThenWaitForAnimationChange_1(1, 3);
    ObjectMotion_SetVariantCallbackAndInvokeObject_2(9, 1);
    ObjectMotion_ArmCallback_6(9, 0x5000, 10);
    BattleRuntime_RunThenWaitIfModeZero_2(9, 0, 20);
    ObjectMotion_SetVariantCallback_1(1, 1);
    BattleEffect_SpawnLinkedResourceObject_2(1, 0x103, 40);
    BattleRuntime_RunThenWaitIfModeZero_3(1, 0, 10);
    ObjectMotion_CallThenWaitForAnimationChange_2(9, 3);
    BattleRuntime_RunThenWaitIfModeZero_4(9, 0, 20);
    Motion_ArmCb_7(0, 0x8000, 0);
    Motion_ArmCb_8(1, 0x1000, 40);
    ObjectMotion_CallThenWaitForAnimationChange_3(9, 4);
    BattleEvent_RunActionAndWait_2(9, 0);
    Motion_ArmCb_9(0, 0xb000, 0);
    Motion_ArmCb_10(1, 0xd000, 10);
    ObjectMotion_SetVariantCallbackAndInvokeObject_3(1, 2);
    BattleRuntime_WaitIfModeZero_1(20);
    BattleRuntime_RunThenWaitIfModeZero_5(1, 0, 10);
    ObjectMotion_CallThenWaitForAnimationChange_4(9, 3);
    BattleRuntime_RunThenWaitIfModeZero_6(9, 0, 20);
    BattleFx_SpawnLinked_3(0, 0x102, 0);
    BattleFx_SpawnLinked_4(1, 0x102, 80);
    BattleRuntime_RunThenWaitIfModeZero_7(1, 0, 20);
    Motion_SetVarCbObj_4(9, 2);
    BattleRuntime_RunThenWaitIfModeZero_8(9, 0, 20);
    Motion_ArmCb_11(1, 0x1000, 20);
    ObjectMotion_SetSpeedParameters_1(1, 0); /* main:0808a178 */
    if (UiWork_WaitThenFinalizeCapacity_1(0, 0) == 0) {
        BattleFx_SpawnLinked_5(1, 0x105, 60);
    } else {
        bump_step(1);
    }
    BattleRuntime_RunThenWaitIfModeZero_9(1, 0, 20);
    Motion_ArmCb_12(1, 0xd000, 10);
    SceneWork_SetStepValue_2((s32)gVal3);
    BattleEvent_RunActionAndWait_3(1, 0);
    ObjectMotion_CallThenWaitForAnimationChange_5(9, 3);
    Battle_RunThenWaitIfModeZero_10(9, 0, 20);
    ObjectMotion_CallThenWaitForAnimationChange_6(1, 3);
    BattleRuntime_WaitIfModeZero_2(20);
    Motion_ArmCb_13(1, 0x1000, 20);
    ObjectMotion_CallThenWaitForAnimationChange_7(1, 3);
    Motion_CallWaitAnim_8(0, 3);
    Object_SetModeById_2(1, 2);
    record = Scene_GetRecord_2(0);
    if (record != 0) {
        /* Copy the s16 coordinate pair at +10/+18 of the looked-up record. */
        ObjectMotion_ResetAndSetPosition_1(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    ObjectMotion_CommitCurrentPositionAndActivate_1(1);
    ObjectMotion_SetHorizontalPositionWithTerrain_2(1, 0, 0);
    GameFlag_Set_1(0x304);
    BattleRuntime_ScheduleShoulderButtonModeUpdate_1();
}

/* Sets up actors 12, 13, 14 and 20 with shared data and movement/speed
 * parameters, then drives actor 11 through a further sequence of moves. */
void Scene_RunCompanionActorSequence(void)
{
    u32 i;
    s32 actor_data;
    s32 shared_data;

    BattleRuntime_Reset_1();
    actor_data = Scene_GetRecord_1(ACTOR_A);
    Actor_Run117(actor_data, 0);
    actor_data = Scene_GetRecord_2(ACTOR_B);
    Actor_Run118(actor_data, 0);
    actor_data = Scene_GetRecord_3(ACTOR_C);
    Actor_Run119(actor_data, 0);
    Object_SetModeById_1(ACTOR_A, 0);
    Object_SetModeById_2(ACTOR_B, 0);
    Object_SetModeById_3(ACTOR_C, 0);
    Actor_Run120(20);
    Actor_Place42(0x20000, 0x20000, 0x10000);
    shared_data = SHARED_DATA;
    Motion_EnableActCb_1(ACTOR_A, shared_data);
    Actor_Run121(10);
    Motion_EnableActCb_2(ACTOR_B, shared_data);
    Actor_Place43(-1, -1, 0xe666);
    Actor_Run122(20);
    Motion_MarkActiveAndSetActionCallback_1(ACTOR_C, shared_data);
    BattleEffect_SpawnLinkedResourceObject_1(ACTOR_D, 0x100, 40);
    ObjectMotion_SetVariantCallbackAndInvokeObject_1(ACTOR_D, 2);
    ObjectMotion_ArmCallback_1(ACTOR_D, 0xd000, 10);
    SceneWork_SetStepValue_1(0x1c90);
    BattleRuntime_RunThenWaitIfModeZero_1(ACTOR_D, 0, 40);
    ObjectMotion_SetAngleToward_1(ACTOR_D, 0, 20);
    BattleEvent_RunActionAndWait_1(ACTOR_D, 0);
    ObjectMotion_ArmCallback_2(ACTOR_D, 0x8000, 10);
    GameFlag_Set_1(0x305);
    BattleRuntime_ScheduleShoulderButtonModeUpdate_1();
}

void Scene_RunLargeStagingSequence(void)
{
    u32 i;
    s32 rec;
    s32 rec3;
    u8 *rec8;
    u8 *record;
    s32 none;
    s32 none2;
    s32 base7_200e590;
    s32 base5_ee8;
    s32 p1;
    s32 k;
    s32 p2;

    rec3 = Actor_Check9(0);
    rec8 = Actor_Check10(14);
    Actor_Run123();
    Actor_Run5(-1, -1, -1, 0);
    Actor_Run124(1);
    rec = 0;
    {
        u8 *slot = (u8 *)(Actor_Check28() + 85);

        none = 0;
        *slot = rec;
    }
    Actor_Run125(1);
    Actor_SetRect(49, 53, 8, 4, 20, 50);
    Actor_SetRect2(2, 102, 84, 41, 2, 1);
    Actor_SetRect3(1, 102, 83, 41, 1, 1);
    Actor_SetRect4(0, 103, 82, 42, 1, 1);
    rec = Actor_Check29(11);
    *(u8 *)(rec + 85) = none;
    k = 0x1840000;
    *(s32 *)(rec + 12) = 0xa00000;
    *(s32 *)(rec + 16) = 0x3480000;
    *(s32 *)(rec + 8) = k;
    Actor_Run126(rec, 0);
    rec = Actor_Check30(12);
    *(u8 *)(rec + 85) = none;
    *(s32 *)(rec + 12) = 0xa00000;
    *(s32 *)(rec + 16) = 0x34c0000;
    *(s32 *)(rec + 8) = k;
    Actor_Run127(rec, 0);
    rec = Actor_Check31(13);
    *(u8 *)(rec + 85) = none;
    *(s32 *)(rec + 16) = 0x3500000;
    *(s32 *)(rec + 12) = 0xa00000;
    *(s32 *)(rec + 8) = k;
    Actor_Run128(rec, 0);
    record = Actor_Run48(11);
    Actor_Run129((s32)record, 0);
    record = Actor_Run49(12);
    Actor_Run130((s32)record, 0);
    Actor_Run131(0, 11);
    base7_200e590 = (s32)gOv2;
    Actor_Run132(0, base7_200e590);
    Actor_Run133();
    base5_ee8 = (s32)gVal4;
    Actor_Run134(base5_ee8, 0, 0);
    Actor_Run135();
    Actor_Run6(0x1530000, 0xa00000, 0x4950000, 0);
    Actor_Run136();
    Actor_Run137(1);
    Actor_Run7(0x547a, 0xa8f);
    Actor_Run8(0x1280000, 0xa00000, 0x3990000, 1);
    Actor_Place44(5, 0x1990000, 0x46e0000);
    Actor_Run138(1);
    Actor_Place45(5, 0xb333, 0x5999);
    Actor_Place46(5, 0x1a4, 0x42c);
    Actor_Run139();
    *(s32 *)(*(u8 *volatile *)gWork + 0x1c8) = 60;
    Actor_Run140();
    Actor_Run141(5);
    Actor_Place47(5, 0x10000, 0x8000);
    Actor_Place48(5, 0x155, 0x428);
    Actor_Place49(5, 0xcccc, 0x6666);
    Actor_Place50(5, 0x167, 0x409);
    Actor_Place51(8, 0x8000, 0x4000);
    Actor_Place52(8, 0x13e, 0x3b3);
    Actor_Run142(8, 2);
    Actor_Place53(5, 0x19c, 0x409);
    Actor_Place54(5, 0x19c, 0x3fb);
    Actor_Place55(5, 0x176, 0x3f0);
    Actor_Place56(5, 0x15b, 0x3bb);
    Actor_Place57(8, 0x13e, 0x3b3);
    Actor_Run143(5, 8, 40);
    Actor_Run144(8, 2);
    Actor_Run145(5, 3);
    Actor_Run146(10);
    Actor_Place58(8, 0x17b, 0x3f9);
    Actor_Run9(0x8000, 0x1000);
    Actor_Place59(5, 0x14d, 0x398);
    Actor_Place60(5, 0x12b, 0x39c);
    Actor_Run147();
    Actor_Run148(10);
    Actor_Place61(5, 0xf000, 30);
    Actor_Run149(5, 2);
    Actor_Run150(20);
    Actor_Run10(0x20000, 0x4000);
    Actor_Run11(0x1830000, 0xa00000, 0x3620000, 1);
    Actor_Run151();
    Actor_Run152(10, 2, 20);
    Actor_Run153((base5_ee8 + 1));
    Actor_Place62(0x100a, 0, 10);
    *(s32 *)(rec3 + 24) = 0x10000;
    *(s32 *)(rec3 + 28) = 0x10000;
    Actor_Run154(0, 1);
    Actor_Run155(10, 0, 40);
    Actor_Run12(0, 0x102);
    Actor_Run156(0, 2);
    Actor_Run157(40);
    Actor_Run158(10, 2);
    Actor_Run159(40);
    Actor_Place63(0x100a, 0, 40);
    Actor_Run160(0, 11);
    Actor_Run161(0, base7_200e590);
    Actor_Run162(5, 1);
    Actor_Run163();
    Actor_Run164(40);
    Actor_Run165(5, 3);
    Actor_Place64(5, 0xd000, 10);
    Actor_Place65(5, 0x138, 0x2f7);
    Actor_Place66(5, 0x169, 0x2f8);
    Actor_Run166(20);
    Actor_Place67(5, 0x8000, 40);
    Actor_Run167(5, 0, 40);
    Actor_Place68(0x6001, 0, 10);
    Actor_Place69(5, 0x100, 0);
    Actor_Run168(5, 4, 40);
    Actor_Place70(5, 0xc000, 30);
    Actor_Run169(5, 2);
    Actor_Run170(40);
    Actor_Run171(5, 0, 30);
    Actor_Run172(5, 3);
    Actor_Run173(10);
    Actor_Run13(0x40000, 0x8000);
    Actor_Run14(0x18c0000, -1, 0x24c0000, 1);
    Actor_Place71(5, 0x1c8, 0x2e3);
    Actor_Run174();
    Actor_Run175(10);
    Actor_Place72(1, 0x105, 40);
    Actor_Run176(1, 2);
    Actor_Run177(20);
    Actor_Run178();
    Actor_Run179(1, 17);
    Actor_Place73(0x2001, 0, 20);
    Actor_Run180(131);
    for (i = 0; i < 60; i++) {
        Actor_Run181(Actor_Check32(1));
        Actor_Run182(1);
    }
    Actor_Run183(1, 1);
    p1 = 0x200d5b1;
    Actor_Run15(p1, 0xc80);
    p2 = 0x200d5d1;
    Actor_Check11(p2, 0xc80);
    record = Actor_Run50(14);
    Actor_Run184((s32)record, 0);
    none2 = 0;
    rec8[85] = none2;
    *(s32 *)((s32)rec8 + 8) = 0x1ac0000;
    *(s32 *)((s32)rec8 + 12) = 0xd00000;
    *(s32 *)((s32)rec8 + 16) = 0x2480000;
    {
        s32 shown = 0x8000;

        *(s32 *)((s32)rec8 + 108) = 0x200d75d;
        *(u16 *)((s32)rec8 + 6) = shown;
    }
    Actor_Run185(4);
    Actor_Place74(14, 0x20000, 0x20000);
    Actor_Run16((s32)rec8, 0x1980000, 0xd00000, 0x2480000);
    Actor_Run186(40);
    Actor_Place75(9, 0x2666, 0x1333);
    Actor_Place76(14, 0x2666, 0x1333);
    Actor_Run187(9);
    Actor_Run17((s32)rec8, 0x1880000, 0xd00000, 0x2480000);
    Actor_Place77(9, 0x17a, 0x248);
    Actor_Run188(20);
    Actor_Place78(0x2005, 0, 10);
    *(volatile s32 *)((s32)rec8 + 108) = none2;
    Actor_Run189(1, 2);
    {
        u8 *record = Actor_Run51(1);
        u8 value = *(volatile u8 *)&record[35];

        record[35] = (u8)(value | 1);
    }
    Actor_Run190(p1);
    Actor_Run191(p2);
    Actor_Run192(1);
    Actor_Run193(1, 0);
    Actor_Run194(9, 0);
    Actor_Place79(1, 0x3000, 0);
    Actor_Run195(1, 1);
    Actor_Run196((s32)rec8);
    Actor_Run197();
    Actor_Run198(10);
    Actor_Place80(5, 0x1a8, 0x270);
    Actor_Run199(1, 5, 60);
    Actor_Run200(1, 2);
    Actor_Run201(10);
    Actor_Place81(0x6001, 0, 20);
    Actor_Run202(5, 3);
    Actor_Run203(10);
    Actor_Run204(5, 0, 20);
    Actor_Place82(1, 0x102, 40);
    Actor_Run205(5, 4);
    Actor_Run206(10);
    Actor_Run207(5, 0, 20);
    Actor_Place83(1, 0x101, 80);
    Actor_Run208(5, 1);
    Actor_Run209(10);
    Actor_Run210(5, 0, 20);
    Actor_Place84(1, 0x102, 80);
    Actor_Run211(5, 4);
    Actor_Run212(10);
    Actor_Run213(5, 0, 20);
    Actor_Run214(1, 3);
    Actor_Run215(10);
    Actor_Place85(5, 0x184, 0x25c);
    Actor_Place86(5, 0xa000, 20);
    Actor_Run216(5, 2);
    Actor_Run217(10);
    Actor_Place87(0x1005, 0, 20);
    Actor_Run218(5, 4);
    Actor_Run219(10);
    Actor_Place88(1, 0x105, 40);
    Actor_Run220(1, 2);
    Actor_Run221(10);
    Actor_Run222(5, 0, 0);
    Actor_Run223(1, 0x8000, 40);
    Actor_SetRect5(1, 1, 2, 25, 2, 5, 10, 14, 4, 14, none2);
    Actor_Run224(40);
    Actor_Run18(5, 0x102);
    Actor_Run19(1, 0x102);
    Actor_Run225(80);
    Actor_Place89(5, 0x101, 40);
    Actor_Place90(0x1005, 0, 20);
    Actor_Run20(1, 0x102);
    Actor_Run226(80);
    Actor_Run227(1, 4);
    Actor_Run228(10);
    Actor_Place91(5, 0x101, 40);
    Actor_Place92(0x1005, 0, 20);
    Actor_Run229(1, 2);
    Actor_Run230(40);
    Actor_Run231(1, 4);
    Actor_Run232(20);
    Actor_Place93(5, 0x101, 80);
    Actor_Run233(5, 4);
    Actor_Run234(10);
    Actor_Place94(0x1005, 0, 10);
    Actor_Place95(5, 0x1000, 40);
    Actor_Run235(5, 1, 40);
    Actor_Run236(1, 2);
    Actor_Run237(10);
    Actor_Place96(5, 0x17c, 0x26c);
    Actor_Run238(10);
    Actor_Place97(0x1005, 0, 10);
    Actor_Place98(1, 0x5000, 30);
    Actor_Run21(1, 0x102);
    Actor_Run239(80);
    Actor_Run240(1, 2);
    Actor_Run241(10);
    Actor_Place99(0x6001, 0, 20);
    Actor_Run242(5, 4);
    Actor_Run243(10);
    Actor_Place100(1, 0x101, 80);
    Actor_Run244(5, 4);
    Actor_Run245(10);
    Actor_Place101(1, 0xcccc, 0x6666);
    Actor_Place102(1, 0x19c, 0x25c);
    Actor_Place103(1, 0x5000, 20);
    Actor_Run246(5, 1);
    Actor_Run247(10);
    Actor_Place104(0x1005, 0, 20);
    Actor_Place105(1, 0x101, 80);
    Actor_Run248(5, 4, 30);
    Actor_Place106(0x1005, 0, 20);
    Actor_Run249(1, 3);
    Actor_Run250(10);
    Actor_Place107(0x6001, 0, 20);
    Actor_Run251(30);
    Actor_Place108(5, 0xe000, 40);
    Actor_Run252(5, 3);
    Actor_Place109(0x1005, 0, 20);
    Actor_Place110(1, 0x101, 80);
    Actor_Place111(5, 0x103, 40);
    Actor_Place112(5, 0xcccc, 0x6666);
    Actor_Place113(5, 0x1ac, 0x274);
    Actor_Run253(20);
    Actor_Place114(1, 0x3000, 0);
    Actor_Run254(5);
    Actor_Run22(0x5001, 0);
    Actor_Run255(5, 1);
    Actor_Run256(60);
    Actor_Place115(5, 0xb000, 30);
    Actor_Run257(1, 2);
    Actor_Place116(5, 0x1ac, 0x274);
    Actor_Place117(5, 0xb000, 20);
    Actor_Run258(1, 3);
    Actor_Run259(10);
    Actor_Run260(5, 4);
    Actor_Run261(10);
    Actor_Place118(0x2005, 0, 20);
    Actor_Run23(1, 0x102);
    Actor_Run262(40);
    Actor_Place119(0x5001, 0, 20);
    Actor_Run263(1, 3);
    Actor_Run264(5, 3);
    Actor_Run265(10);
    Actor_Place120(5, 0x8000, 0x4000);
    Actor_Place121(1, 0x8000, 0x4000);
    Actor_Place122(5, 0x1c2, 0x2ee);
    Actor_Place123(1, 0x1c2, 0x2ee);
    Actor_Run266(60);
    *(s32 *)((*(u8 *volatile *)gWork + 0x1c8)) = 60;
    Actor_Run267();
    Actor_Run268();
    Actor_Run269(12);
    Actor_Run270();
}

void Scene_RunStep8C(void)
{
    Actor_Apply8(0x8c, 0);
}

void Scene_RunStep9(void)
{
    Actor_Do24(9);
    Actor_Run271();
}

void Scene_RunStep17(void)
{
    Actor_Do25(0x17);
    Actor_Run272();
}

void Scene_RunScriptedStepEE4(void)
{
    Actor_Run273();
    Actor_Apply9((s32)&Value_00000ee4, 1);
    Actor_Run274();
}

void Scene_RunScene373SequenceB(void)
{
    u32 i;
    s32 rec7;
    s32 record;

    if (Actor_Check12(0x241) != 0) {
        rec7 = Actor_Check13(0x106);
        if (rec7 != 0) {
            goto L_02005a8a;
        }
        *(u8 *)(Actor_Check33(22) + 91) = rec7;
        Actor_Do11(0x241);
    } else {
        if (Actor_Check14(0x106) != 0) {
            *(u8 *)(Actor_Check34(22) + 91) = 1;
            Actor_Do12(0x241);
        }
    }
    L_02005a8a:;
}

/* overlays/scene/actor/staged_motion/geometry.c */
typedef s32(*FixedPointSqrt)(s32);

s32 Runtime_ComputeFixedPointDistance(s32 *first_position, s32 *second_position)
{
    s32 delta_x = (*first_position++ - *second_position++) >> 16;
    s32 delta_y = (*first_position++ - *second_position++) >> 16;
    s32 delta_z = (*first_position - *second_position) >> 16;
    s32 delta_x_squared = delta_x *delta_x;
    s32 delta_y_squared = delta_y *delta_y;
    s32 delta_z_squared = delta_z *delta_z;

    return ((FixedPointSqrt) 0x030001D8)(delta_x_squared + delta_y_squared + delta_z_squared);
}

/* overlays/scene/actor/staged_motion/move_and_redraw.c */

void Actor_Run275(StagedActorMovementRequest request)
{
}

/* overlays/scene/actor/staged_motion/orbiting_effect.c */
#define UpdateOrbitingSceneObject Value5

typedef struct OrbitingSceneObjectSprite {
    u8 padding_00[5];
    u8 flags_05_low : 5;
    u8 flags_05_bit_5 : 1;
    u8 flags_05_high : 2;
    u8 padding_06[3];
    u8 flags_09_low : 2;
    u8 flags_09_mode : 2;
    u8 flags_09_high : 4;
    u8 padding_0a[18];
    u8 palette;
    u8 padding_1d[10];
    u8 state;
} OrbitingSceneObjectSprite;

typedef struct OrbitingSceneObject {
    u8 padding_00[8];
    s32 x;
    s32 y;
    u8 padding_10[19];
    u8 flags_23;
    u8 padding_24[12];
    s32 orbit_angle;
    u8 padding_34[4];
    s32 orbit_center_x;
    s32 orbit_center_y;
    u8 padding_40[16];
    OrbitingSceneObjectSprite *sprite;
    u8 padding_54;
    u8 mode;
    u8 state;
    u8 padding_57[5];
    u8 active;
    u8 padding_5d[4];
    u8 visible;
    u8 padding_62[10];
    u32 callback;
} OrbitingSceneObject;

extern u8 Value5;

OrbitingSceneObject *GetOrbitingSceneObject();

u8 *AllocateEffectTransfer(s32, s32);

s32 Effect_UpdateOrbitPosition(s32 *p)
{
    s16 *q = (s16 *)p[20];
    s32 a, b;
    s32 d = Actor_Check35(p[12]) * 2;
    if (d > 0)
        d = -d;
    p[2] = p[14] + Actor_Check36(p[12]) * 2;
    p[3] = p[15] + d;
    q[15] = Actor_Check37(p[12] + 0x8000) / 8;
    a = Actor_Check38();
    b = Actor_Check39();
    p[12] = p[12] + ((((u32)a << 9) >> 16) + (((u32)b << 9) >> 16)) + 0x400;
    return 0;
}

void InitializeStagedActorSceneOrbitingEffect(void)
{
    OrbitingSceneObject *actor;
    OrbitingSceneObjectSprite *sprite;
    u8 *transfer;
    s32 zero;

    actor = GetOrbitingSceneObject();
    sprite = actor->sprite;
    sprite->flags_09_mode = 1;
    sprite->flags_05_bit_5 = 0;
    sprite->flags_09_high = 0;

    zero = 0;
    sprite->state = zero;
    NormalizeOrbitingSceneObject(actor, zero);
    actor->active = zero;
    actor->mode = zero;

    if (IsGameFlagSet(0x109) == 0)
        actor->y += 0x200000;

    actor->flags_23 &= 0xfe;
    actor->visible = 1;

    transfer = AllocateEffectTransfer(17, 0x608);
    LoadEffectResource(181);
    transfer += 0x400;
    ConfigurePaletteTransfer(sprite->palette, 128, transfer);
    ReleaseEffectTransfer(17);

    actor->orbit_center_x = actor->x;
    actor->orbit_angle = zero;
    actor->orbit_center_y = actor->y;
    actor->active = 1;
    actor->callback = (u32)&UpdateOrbitingSceneObject;
    actor->state = zero;
}

/* overlays/scene/actor/staged_motion/overlay_object.c */
extern s32 gIw;

void OvObj_UpdateOnFrameBit1(s32 p)
{
    if ((gIw & 2) != 0)
        Actor_Apply10(p, 7);
    else
        Actor_Apply11(p, 0);
    if ((gIw & 0xf) == 0)
        Actor_Do26(p);
}

/* overlays/scene/actor/staged_motion/scene_dialogue.c */

void Dialogue_RunActor181Scene(void)
{
    Actor_Run276();
    Actor_Place262(26, 0, 0);
    Actor_Do27(0xfd0);
    Actor_Apply12(0xb5, 3);
    Actor_Apply13(0xb5, 0);
    Actor_Run277();
}

void Dialogue_RunActorTenFlaggedDialogue(void)
{
    Actor_Run278();
    if (Actor_Check40(0x815) != 0) {
        Actor_Do28(0x11cc);
        Actor_Apply14(10, 0);
    } else {
        Actor_Do29(0xf81);
        Actor_Place263(10, 0, 4);
        Actor_Apply15(10, 0);
    }
    Actor_Run279();
}

void Dialogue_RunActor21FlaggedLine(void)
{
    Actor_Run280();
    if (Actor_Check41(0x202) != 0) {
        Actor_Do30(0xf68);
    } else {
        Actor_Do31(0xf69);
    }
    Actor_Apply16(21, 0);
    Actor_Run281();
}

void Dialogue_RunActor10LineAndFlag81f(void)
{
    Actor_Run282();
    Actor_Place264(0, 10, 20);
    Actor_Do32(0x1c8d);
    Actor_Apply17(10, 0);
    Actor_Do33(0x81f);
    Actor_Run283();
}

void Dialogue_RunActorNineteenDialogue(void)
{
    Actor_Run284();
    Actor_Apply18(19, 2);
    Actor_Do34(20);
    Actor_Place265(19, 0, 20);
    Actor_Do35(0x1c9d);
    Actor_Apply19(19, 0);
    Actor_Do36(0x307);
    Actor_Run285();
}

/* overlays/scene/actor/staged_motion/scene_primary_script.c */
extern s32 gOv3[];

void Dialogue_RunFlag815GatedStep(void)
{
    if (Actor_Check42(0x815) != 0 && Actor_Check43(0x87a) == 0) {
        Actor_Run286();
        Actor_Do37(0x11b6);
        Actor_Apply20(21, 0);
        if (Actor_Apply21(0, 0) == 0) {
            Actor_Place266(21, 0, 60);
            Actor_Apply22(21, 0);
        } else {
            u8 *b = *(u8 **)0x03001ebc;
            u16 *h = (u16 *)(b + 0x1d8);
            *h = *h + 2;
            Actor_Do38(40);
            Actor_Apply23(21, 0);
        }
        Actor_Run287();
    } else {
        Actor_Do39(0x9e);
        Actor_Place267((s32)gOv3, 50, 44);
        Actor_Place268(0, 0x154, 0x378);
        Actor_Do40(7);
    }
}

/* overlays/scene/actor/staged_motion/scene_setup.c */

void Scene_RunActor181Scene(void)
{
    Actor_Run288();
    Actor_Place269(20, 0, 0);
    Actor_Do41(0xfd0);
    Actor_Apply24(0xb5, 3);
    Actor_Apply25(0xb5, 0);
    Actor_Run289();
}

/* overlays/scene/actor/staged_motion/scene_state_interaction.c */
extern s32 gOv4[];
extern s32 gOv5[];
extern s32 gOv6[];

u8 *Actor_Run290(s32);

void State_SetFlag204AndConfigureRegion49_46(void)
{
    s32 p5, p6;
    Actor_Run291();
    Actor_SetMode2(0, 13, 10, 1);
    Actor_Do42(0x204);
    p5 = 20;
    p6 = 50;
    Actor_SetRect14(49, 46, 8, 4, p5, p6);
    Actor_Run292();
}

void State_RunTablePairByActor22State(void)
{
    u8 *rec = Actor_Run290(22);
    if (Actor_Check44(0x823) != 0) {
        u8 *q = rec;
        s32 v;
        q += 100;
        v = *(s16 *)q;
        if (v == 1) {
            Actor_Apply26((s32)gOv4, (s32)gOv5);
        } else if (v == 2) {
            Actor_Apply27((s32)gOv4, (s32)gOv6);
        }
    }
}

/* overlays/scene/actor/staged_motion/shared.c */

void Scene_RunSingleStep(void)
{
    Actor_Run293();
}

/* overlays/scene/actor/staged_motion/staged_actor.c */
/*
 * Reset or tear down a consecutive run of scene actors -- resource_373.
 */
struct Resource373Actor {
    u8 unknown_00[8];
    s32 field08;
    s32 field0c;
    s32 field10;
    u8 unknown_14[0x41];
    u8 flag55;
};

struct Resource373Actor *Actor_Run294(s32 selector);

/*
 * The owner extends through its one literal pool word. The run length is
 * unsigned and the selector advances once per iteration. The Func_ aliases
 * name the call words encoded in the overlay image, not verified runtime
 * addresses.
 */
void Actor_ResetActorRun(s32 first, u32 count, s32 mode)
{
    s32 selector = first;
    u32 i;

    if (mode == 0) {
        for (i = 0; i < count; i++) {
            struct Resource373Actor *actor = Actor_Run294(selector);

            actor->flag55 = 0;
            Actor_Apply28(actor, 0);
            actor->field08 = 0x01860000;   /* 0xc3 << 17. */
            actor->field0c = 0x00a00000;   /* 0xa0 << 16. */
            actor->field10 = 0x034a0000;   /* The literal pool word. */
            selector++;
        }
        return;
    }

    for (i = 0; i < count; i++) {
        Actor_Place270(selector, 0, 0);
        selector++;
    }
}

/* overlays/scene/actor/staged_motion/staged_actor_scene_tail.c */

struct SourceEntity {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct ParticleRecord {
    u8 f00[9];
    u8 f09;
};

struct StagedParticle {
    u8 f00[0x48];
    s32 f48;
    s32 f4c;
    struct ParticleRecord *f50;
    u8 f54;
    u8 f55;
    u8 f56[8];
    u16 f5e;
};

/* Emitter fields are named by offset; the layout is not verified. */
struct Resource373Emitter {
    u8 unknown_00[6];
    u16 field06;
    s32 x;                          /* 0x08 */
    s32 y;                          /* 0x0c */
    s32 z;                          /* 0x10 */
    u8 unknown_14[4];
    s32 field18;
    s32 field1c;
    u8 unknown_20[4];
    s32 field24;
    s32 field28;
    s32 field2c;
    u8 unknown_30[8];
    s32 field38;
    s32 field3c;
    s32 field40;
};

struct Resource373Particle {
    u8 unknown_00[0x28];
    s32 lifetime;                   /* 0x28 */
    u8 unknown_2c[4];
    s32 field30;
    s32 field34;
    u8 unknown_38[0x10];
    s32 field48;
    u8 unknown_4c[9];
    u8 field55;
};

struct StagedVerticalEffect {
    s32 pad0[2];
    s32 f8;
    s32 fc;
    s32 f10;
    s32 pad14;
    s32 f18;
    s32 f1c;
    s32 pad20[17];
    s16 f64;
    s16 pad66;
    s32 *f68;
};

extern s32 gOv7[];

u8 *Actor_Run295();

u8 *Actor_Run296();

u8 *Actor_Run297();

u8 *Actor_Run298();

u8 *Actor_Run299();

u8 *Actor_Run300();

u8 *Actor_Run301();

u8 *Actor_Run302();

u8 *Actor_Run303();

u8 *Actor_Run304();

u8 *Actor_Run305();

u8 *Actor_Run306();
u8 *Actor_Run307();

u8 *Actor_Run308();

u8 *Actor_Run309();
u8 *Actor_Run310();

u8 *Actor_Run311();

u8 *Actor_Run312();
u8 *Actor_Run313();

u8 *Actor_Run314();

u8 *Actor_Run315();

u8 *Actor_Run316();

struct StagedParticle *Actor_Run317(s32, s32, s32, s32);

struct Resource373Particle *Actor_Run318();

/*
 * Staged actor cutscene tail -- resource_373. The long scene script plus the
 * particle, sound, and vertical-motion effects it drives.
 */

/*
 * Each alias names the loader-relocated call word the image holds, not a
 * runtime address. The declarations are old-style because the call sites vary
 * in arity.
 */

/*
 * Calls spelled through these wrappers pass their constants straight into the
 * argument registers. A direct call instead precomputes a costly constant into
 * a pseudo shared with later uses in the block, and a value-returning call
 * sets r0 last of its arguments.
 */

void Scene_RunStagedActorCutsceneSequence(void)
{
    u8 *scene;
    u8 *rec;
    u32 i;
    s32 turn_back;
    s32 turn_side;
    u8 *turned;
    s32 none;
    s32 flag;
    s32 callback_a;
    s32 callback_b;
    s32 callback_c;
    s32 callback_d;
    s32 callback_e;
    s32 callback_f;

    scene = Actor_Run295(0);
    Actor_Run319();
    Actor_Run24(-1, -1, -1, 0);
    Actor_Run320(1);
    Actor_SetRect6(49, 53, 8, 4, 20, 50);
    Actor_Run321(2, 102, 84, 41, 2, 1);
    Actor_Run322(1, 102, 83, 41, 1, 1);
    Actor_Run323(0, 103, 82, 42, 1, 1);
    Actor_Place124(21, 0x1880000, 0x3800000);
    turned = Actor_Run296(21);
    /*
     * Overwritten at once, but the store must stay: its zero halfword
     * temporary is what the record byte stores below reuse out of a high
     * register.
     */
    *(u16 *)(turned + 6) = 0;
    turn_back = 0xc000;
    *(u16 *)(turned + 6) = turn_back;
    Actor_Place125(1, 0x12a0000, 0x2e00000);
    turned = Actor_Run297(1);
    turn_side = 0x4000;
    *(u16 *)(turned + 6) = turn_side;
    Actor_Place126(5, 0x12a0000, 0x2f80000);
    turned = Actor_Run298(5);
    *(u16 *)(turned + 6) = turn_side;
    Actor_Run324(0, 11);
    Actor_Run325(0, 0x200e590);
    rec = Actor_Run299(23);
    rec[85] = 0;
    *(s32 *)(rec + 8) = 0x1840000;
    *(s32 *)(rec + 12) = 0xa00000;
    *(s32 *)(rec + 16) = 0x3480000;
    Actor_Run326(rec, 0);
    rec = Actor_Run300(24);
    rec[85] = 0;
    *(s32 *)(rec + 8) = 0x1840000;
    *(s32 *)(rec + 12) = 0xa00000;
    *(s32 *)(rec + 16) = 0x34c0000;
    Actor_Run327(rec, 0);
    rec = Actor_Run301(25);
    rec[85] = 0;
    *(s32 *)(rec + 8) = 0x1840000;
    *(s32 *)(rec + 12) = 0xa00000;
    *(s32 *)(rec + 16) = 0x3500000;
    Actor_Run328(rec, 0);
    Actor_Run302()[85] = 0;
    Actor_Run329(1);
    Actor_Run25(0x17f0000, 0xa00000, 0x36d0000, 0);
    Actor_Run330();
    Actor_Run331(1);
    *(s32 *)(*(u8 **)0x03001ebc + 0x1c8) = 32;
    Actor_Run332();
    Actor_Place127(5, 0x8000, turn_side);
    Actor_Place128(1, 0x8000, turn_side);
    Actor_Run333(5, 0x200e614);
    Actor_Run334(1, 0x200e5cc);
    Actor_Run335(40);
    Actor_Run336(0, 1);
    *(s32 *)(scene + 24) = 0x10000;
    *(s32 *)(scene + 28) = 0x10000;
    Actor_Run26(0, 0xb000, 40, 0x10000);
    Actor_Run337(0, 3);
    Actor_Run338(10);
    Actor_Place129(0, 0x4ccc, 0x2666);
    Actor_Place130(0, 400, 840);
    Actor_Run339(10);
    Actor_Run340(0, turn_back, 30);
    Actor_Run341(0, 1);
    Actor_Run342(20);
    Actor_Place131(0, 0x8000, 40);
    Actor_Run343();
    Actor_Run344(0, 17);
    Actor_Run345((s32)Actor_Run346, 3200);
    for (i = 0; i < 40; i++) {
        Actor_Run347(scene);
        Actor_Run348(1);
    }
    Actor_Run349(0, 1);
    /* Callback symbols are pooled loads that stay after the preceding call. */
    callback_a = (s32)Actor_Run350;
    Actor_Check15(callback_a, 3200);
    callback_b = (s32)Actor_Run351;
    Actor_Run27(callback_b, 3200);
    Actor_Place132(23, 0x3333, 0x1999);
    Actor_Place133(23, 390, 832);
    Actor_Place134(0, 0xc000, 0);
    Actor_Place135(23, 400, 826);
    Actor_Run352(20);
    {
        u8 *record = Actor_Run303(0);
        u8 value = *(volatile u8 *)&record[35]; /* Keeps the byte in its own register. */

        record[35] = (u8)(value | 1);
    }
    Actor_Run353(0, 1);
    Actor_Run354((s32)Actor_Run346);
    Actor_Run355(callback_a);
    Actor_Run356(callback_b);
    Actor_Run357(1);
    Actor_Run358(0, 0);
    Actor_Run359(23, 0);
    Actor_Run360(23, 0, 0);
    Actor_Run361(20);
    Actor_Run362(0, 11);
    Actor_Run363(0, 0x200e590);
    Actor_Run364(120);
    Actor_SetRect7(7, 102, 84, 41, 2, 1);
    Actor_Run365(0, 1);
    *(s32 *)(scene + 24) = 0x10000;
    *(s32 *)(scene + 28) = 0x10000;
    Actor_Run366(0, 1);
    Actor_Run367(40);
    Actor_Run368(0, 3);
    Actor_Place136(0, 377, 843);
    Actor_Place137(0, 0xc000, 40);
    Actor_Run369(0, 0, 20);
    Actor_Run370(0, 17);
    Actor_Run28((s32)Actor_Run346, 3200);
    for (i = 0; i < 40; i++) {
        Actor_Run371(scene);
        Actor_Run372(1);
    }
    Actor_Run373(0, 1);
    callback_c = (s32)Actor_Run350;
    Actor_Check16(callback_c, 3200);
    callback_d = (s32)Actor_Run374;
    Actor_Run29(callback_d, 3200);
    Actor_Place138(24, 0x3333, 0x1999);
    Actor_Place139(24, 390, 832);
    Actor_Place140(0, 0xc000, 0);
    Actor_Place141(24, 377, 828);
    Actor_Run375(20);
    {
        u8 *record = Actor_Run304(0);
        u8 value = *(volatile u8 *)&record[35];

        record[35] = (u8)(value | 1);
    }
    Actor_Run376(0, 1);
    Actor_Run377((s32)Actor_Run346);
    Actor_Run378(callback_c);
    Actor_Run379(callback_d);
    Actor_Run380(1);
    Actor_Run381(0, 0);
    Actor_Run382(24, 0);
    Actor_Run383(24, 0, 0);
    Actor_Run384(20);
    Actor_Run385(0, 11);
    Actor_Run386(0, 0x200e590);
    Actor_Run387(120);
    Actor_SetRect8(6, 102, 83, 41, 1, 1);
    Actor_Run388(0, 1);
    *(s32 *)(scene + 24) = 0x10000;
    *(s32 *)(scene + 28) = 0x10000;
    Actor_Run389(0, 1);
    Actor_Run390(40);
    Actor_Run391(0, 3);
    Actor_Place142(0, 360, 855);
    Actor_Place143(21, 0xb000, 10);
    Actor_Place144(0, 0xc000, 30);
    Actor_Place145(0, 0xd000, 20);
    Actor_Run392(0, 17);
    Actor_Run30((s32)Actor_Run346, 3200);
    for (i = 0; i < 40; i++) {
        Actor_Run393(scene);
        Actor_Run394(1);
    }
    Actor_Run395(0, 1);
    callback_e = (s32)Actor_Run350;
    Actor_Check17(callback_e, 3200);
    callback_f = (s32)Actor_Run396;
    Actor_Check18(callback_f, 3200);
    Actor_Place146(25, 0x3333, 0x1999);
    Actor_Place147(25, 390, 832);
    Actor_Place148(0, 0xc000, 0);
    Actor_Place149(25, 360, 837);
    Actor_Run397(20);
    {
        u8 *record = Actor_Run305(0);
        u8 value = *(volatile u8 *)&record[35];

        record[35] = (u8)(value | 1);
    }
    Actor_Run398(0, 1);
    Actor_Run399((s32)Actor_Run346);
    Actor_Run400(callback_e);
    Actor_Run401(callback_f);
    Actor_Run402(1);
    Actor_Run403(0, 0);
    Actor_Run404(25, 0);
    Actor_Run405(25, 0, 0);
    Actor_Run406(20);
    Actor_Run407(0, 11);
    Actor_Run408(0, 0x200e590);
    Actor_Run409(120);
    Actor_Run410();
    Actor_SetRect9(5, 103, 82, 42, 1, 1);
    Actor_Run411(0, 1);
    *(s32 *)(scene + 24) = 0x10000;
    *(s32 *)(scene + 28) = 0x10000;
    Actor_Run412(21, 2, 20);
    Actor_Do13(0xf03);
    Actor_Run413(21, 0, 10);
    Actor_Place150(0, 0x1000, 10);
    Actor_Run414(21, 5, 6, 0);
    Actor_Place151(21, 0x4ccc, 0x2666);
    Actor_Place152(21, 397, 832);
    Actor_Run415(20);
    Actor_Place153(21, 0x4000, 60);
    Actor_Place154(21, 0xc000, 60);
    Actor_Run416(21, 3);
    Actor_Run417(21, 3);
    Actor_Run418(21, 0, 10);
    Actor_Place155(21, 372, 832);
    Actor_Run419(20);
    Actor_Place156(21, 0x4000, 40);
    Actor_Place157(21, 0x8000, 40);
    Actor_Run420(21, 3);
    Actor_Run421(21, 3);
    Actor_Run422(21, 0, 20);
    Actor_Place158(21, 0x5000, 20);
    Actor_Run423(21, 3);
    Actor_Run424(21, 0, 10);
    Actor_Run425(0, 2);
    Actor_Run426(20);
    Actor_Run427(21, 4);
    Actor_Check19(21, 0);
    if (Actor_Check45(0, 0) == 0) {
        Actor_Run428(21, 3);
        bump_step(1);
    } else {
        Actor_Run429(21, 4);
    }
    Actor_Run430(21, 0, 20);
    Actor_Do14(0xf0a);
    Actor_Place159(21, 386, 841);
    Actor_Run431(10);
    Actor_Place160(21, 0xd000, 60);
    Actor_Run432(21, 2);
    Actor_Run433(21, 0, 20);
    Actor_Place161(21, 0x5000, 30);
    Actor_Check20(21, 0);
    if (Actor_Check46(0, 0) == 1) {
        bump_step(1);
    }
    Actor_Run434(21, 0, 20);
    Actor_Place162(21, 0xd000, 60);
    Actor_Run435(21, 2);
    Actor_Do15(0xf0e);
    Actor_Run436(21, 0, 20);
    Actor_Place163(21, 386, 825);
    Actor_Run437(10);
    Actor_Run438(21, 4);
    Actor_Run439(60);
    Actor_Run440(21, 0, 60);
    Actor_Place164(21, 0x5000, 10);
    Actor_Run441(21, 0, 10);
    Actor_Place165(21, 372, 832);
    Actor_Place166(21, 0x5000, 10);
    Actor_Run442(0, 2);
    Actor_Run443(20);
    Actor_Run444(21, 3);
    Actor_Run445(20);
    Actor_Run446(21, 0, 20);
    Actor_Place167(0, 258, 60);
    Actor_Run447(21, 2);
    Actor_Run448(21, 0, 20);
    Actor_Run449(0, 3);
    Actor_Run450(21, 3);
    Actor_Run451(21, 0, 10);
    Actor_Run31(0x6666, 0xccc);
    Actor_Run32(0x1790000, 0xa00000, 0x35c0000, 1);
    Actor_Place168(5, 0x10000, 0x8000);
    Actor_Place169(1, 0x10000, 0x8000);
    Actor_Place170(1, 369, 904);
    Actor_Place171(5, 392, 904);
    Actor_Run452(1, 1);
    Actor_Run453(5, 10, 11, 0);
    Actor_Place172(5, 0xa000, 0);
    Actor_Run454(5, 0, 10);
    Actor_Run455(21, 2);
    Actor_Run456(10);
    Actor_Place173(21, 0x3000, 0);
    Actor_Place174(0, 0x1000, 20);
    Actor_Run457(5, 4, 0);
    Actor_Place175(5, 392, 843);
    Actor_Place176(5, 0x9000, 0);
    Actor_Place177(21, 0x3000, 0);
    Actor_Place178(0, 0xd000, 20);
    Actor_Run33(21, 3);
    Actor_Run458(21, 0, 10);
    Actor_Run459(5, 3);
    Actor_Run460(5, 0, 10);
    Actor_Run461(21, 3);
    Actor_Run462(0, 3);
    Actor_Run463(21, 0, 20);
    Actor_Run464(1, 10, 11, 0);
    Actor_Place179(5, 0x4ccc, 0x2666);
    Actor_Place180(1, 0x4ccc, 0x2666);
    Actor_Place181(1, 392, 843);
    Actor_Run307(5)[90] &= 0xfe;
    Actor_Place182(5, 408, 843);
    Actor_Run465(1);
    {
        u8 *record = Actor_Run308(5);
        u8 value = *(volatile u8 *)&record[90];

        record[90] = (u8)(value | 1);
    }
    Actor_Place183(5, 0x8000, 0);
    Actor_Run466(1);
    Actor_Run467(1, 1);
    Actor_Place184(1, 0x8000, 30);
    Actor_Run468(21, 4, 30);
    Actor_Run469(21, 0, 20);
    Actor_Run470(1, 3);
    Actor_Run471(20);
    Actor_Place185(21, 0x5000, 20);
    Actor_Run472(21, 0, 10);
    Actor_Place186(0, 0xd000, 0);
    Actor_Run473(0, 2, 30);
    Actor_Run34(0, 258);
    Actor_Run474(60);
    Actor_Place187(21, 0x3000, 40);
    Actor_Run475(1, 2);
    Actor_Run476(1, 0, 20);
    Actor_Place188(21, 257, 80);
    Actor_Place189(21, 0x5000, 30);
    Actor_Place190(0, 258, 60);
    Actor_Place191(21, 0x3000, 0);
    Actor_Place192(0, 0xd000, 20);
    Actor_Run477(21, 0, 10);
    Actor_Run478(1, 3);
    Actor_Run479(80);
    Actor_Run480(5, 1, 30);
    Actor_Run481(1, 2);
    Actor_Run482(5, 2);
    Actor_Run483(10);
    Actor_Place193(21, 261, 60);
    Actor_Run484(21, 0, 10);
    Actor_Place194(5, 0x8000, 0);
    Actor_Place195(1, 0x8000, 10);
    Actor_Run485(5, 2);
    Actor_Run486(20);
    Actor_Run487(5, 0, 10);
    Actor_Run488(21, 2);
    Actor_Run489(40);
    Actor_Run490(21, 0, 10);
    Actor_Run491(1, 3);
    Actor_Run492(5, 3);
    Actor_Run493(20);
    Actor_Run494(21, 4);
    Actor_Run495(21, 0, 10);
    Actor_Run496(1, 3);
    Actor_Run497(5, 3);
    Actor_Run498(21, 3);
    Actor_Run499(21, 0, 10);
    Actor_Run500(1, 3);
    Actor_Run501(5, 3);
    Actor_Run502(20);
    Actor_Run503(21, 2);
    Actor_Run504(21, 0, 20);
    Actor_Run35(0x9999, 0x1333);
    Actor_Run36(0x1750000, 0xa00000, 0x3450000, 1);
    Actor_Place196(21, 364, 816);
    Actor_Place197(0, 0xd000, 0);
    Actor_Place198(21, 0x3000, 10);
    Actor_Run505(21, 0, 40);
    Actor_Run506(5, 1, 30);
    Actor_Run507(1, 2);
    Actor_Run508(5, 2);
    Actor_Run509(20);
    Actor_Place199(1, 0x8000, 0);
    Actor_Place200(5, 0x8000, 20);
    Actor_Place201(21, 0x5000, 20);
    Actor_Run510(21, 0, 10);
    Actor_Place202(0, 261, 60);
    Actor_Run511(21, 4);
    Actor_Check21(21, 0);
    none = 0; /* One zero shared by the placement call and the byte store. */
    if (Actor_Check47(0, 0) == 1) {
        bump_step(1);
    }
    Actor_Run512(21, 0, 20);
    Actor_Do16(0xf27);
    Actor_Place203(21, 259, 0);
    Actor_Run513(21, 3);
    Actor_Run514(21, 0, 10);
    Actor_Run515(21, 4, 0);
    Actor_Run516(21, 3);
    Actor_Run517(21, 7);
    Actor_Run518(5);
    Actor_SetRect10(21, 14, 2, 24, 2, 1, 10, 14, 4, 14, none);
    Actor_Run519(161);
    rec = Actor_Run310(21);
    {
        u8 value = *(volatile u8 *)&rec[90];

        *(u8 *)(*(s32 *)(rec + 80) + 38) = none;
        rec[90] = (u8)(value & 0xfe);
    }
    Actor_Place204(21, 0x30000, 0x18000);
    Actor_Place205(21, 364, 815);
    Actor_Run520(4);
    for (i = 0; i != 4; i++) {
        *(s32 *)(rec + 16) += 0x18000;
        *(s32 *)(rec + 28) += -0x1999;
        Actor_Run521(1);
    }
    Actor_Run522(21, 0, 0);
    Actor_Place206(1, 0x30000, 0x18000);
    Actor_Run523(1, 6, 0);
    Actor_Place207(1, 374, 827);
    Actor_Run524(5, 0);
    Actor_Place208(1, 0xb000, 0);
    Actor_Place209(5, 256, 0);
    Actor_Run525(5, 2);
    Actor_Place210(1, 256, 10);
    Actor_Run526(1, 13);
    Actor_Run527(1, 2, 5);
    Actor_Run528(143);
    Actor_Place211(0, 0x40000, 0x10000);
    Actor_SetRect11(1, 102, 83, 41, 1, 1);
    Actor_Run529(Actor_Check48(1), 0);
    Actor_Place212(0, 0xd000, 10);
    Actor_Run530(1, 3);
    Actor_Place213(-1, -1, 0xe666);
    Actor_Run531();
    Actor_Place214(1, 258, 80);
    Actor_Run532(21, 8);
    *(s32 *)(rec + 28) = 0x8000;
    Actor_Place215(21, 0x16c0000, 0x32b0000);
    for (i = 0; i != 5; i++) {
        *(s32 *)(rec + 28) += 0x1999;
        Actor_Run533(1);
    }
    none = 0; /* Refreshed after the loops for the closing scene store. */
    Actor_Run534(60);
    Actor_Run535(1, 2);
    Actor_Place216(1, 0x5000, 30);
    Actor_Run536(1, 2);
    Actor_Run537(5, 2);
    Actor_Run538(60);
    Actor_Run539(21, 2);
    Actor_Run540(20);
    Actor_Run541(21, 0, 20);
    Actor_Run37(0x4ccc, 0x999);
    Actor_Run38(0x1740000, 0xa00000, 0x35b0000, 1);
    Actor_Place217(21, 0x30000, 0x18000);
    Actor_Run542(21, 6, 0);
    Actor_Place218(21, 359, 835);
    Actor_Run543(20);
    Actor_Place219(21, 0x4000, 20);
    Actor_Run544(21, 2);
    rec[35] &= 0xfe;
    Actor_Run545(21, 0, 80);
    Actor_Place220(21, 257, 80);
    Actor_Run546(21, 0, 60);
    Actor_Run547(21, 3);
    Actor_Run548(21, 0, 10);
    Actor_Run39(21, 258);
    Actor_Run549(80);
    Actor_Place221(1, 0x5000, 40);
    Actor_Place222(1, 258, 80);
    Actor_Run550(1, 2);
    Actor_Run551(1, 0, 20);
    Actor_Place223(1, 0x8000, 20);
    Actor_Run552(1, 3);
    Actor_Run553(10);
    Actor_Run554(1, 3);
    Actor_Run555(Actor_Check49(1), 1);
    Actor_Run556(1, 6, 0);
    Actor_Run557(1, 1);
    Actor_Place224(1, 0x40000, 0x20000);
    rec = Actor_Run311(1);
    rec[90] &= 0xfe;
    Actor_Place225(1, 403, 827);
    Actor_Run40(5, 258);
    Actor_Place226(5, 0xc000, 20);
    Actor_Run558(5, 0, 1);
    Actor_Run559(1);
    Actor_Place227(1, 0x5000, 20);
    Actor_Run560(1, 0, 20);
    Actor_Place228(1, 256, 0);
    Actor_Run561(1, 13);
    Actor_Run562(1, 2, 5);
    Actor_Run563(Actor_Check50(1), 0);
    Actor_SetRect12(2, 102, 84, 41, 2, 1);
    Actor_Run564(143);
    Actor_Place229(0, 0x40000, 0x10000);
    Actor_Run565(1, 3);
    Actor_Place230(-1, -1, 0xe666);
    Actor_Run566();
    Actor_Place231(1, 258, 30);
    Actor_Place232(5, 0x4ccc, 0x2666);
    Actor_Place233(5, 408, 855);
    Actor_Run567(60);
    Actor_Run568(1, 2);
    Actor_Place234(21, 261, 60);
    Actor_Run569(5, 3);
    Actor_Run570(0, 3);
    Actor_Run571(80);
    Actor_Place235(1, 0x4000, 30);
    Actor_Run572(1, 3);
    Actor_Run573(10);
    Actor_Run574(5, 4);
    Actor_Run575(80);
    Actor_Run576(21, 3);
    Actor_Run577(10);
    Actor_Run578(21, 0, 10);
    Actor_Place236(5, 0xb000, 0);
    Actor_Place237(0, 0xc000, 0);
    Actor_Run579(21, 0, 60);
    Actor_Place238(21, 0x4000, 60);
    Actor_Run580(21, 4);
    Actor_Run581(60);
    Actor_Run582(21, 0, 80);
    Actor_Place239(21, 261, 80);
    Actor_Run583(21, 0, 60);
    Actor_Place240(0, 0xc000, 0);
    Actor_Place241(0, 257, 0);
    Actor_Place242(5, 257, 0);
    Actor_Place243(1, 257, 60);
    Actor_Run584(21, 4);
    Actor_Run585(30);
    Actor_Run586(21, 0, 60);
    Actor_Run587(1, 2);
    Actor_Run588(5, 2);
    Actor_Run589(20);
    Actor_Place244(5, 0x8000, 60);
    Actor_Place245(21, 0x4000, 30);
    Actor_Run590(21, 0, 30);
    Actor_Run591(5, 3);
    Actor_Run592(10);
    Actor_Run593(5, 0, 20);
    Actor_Run594(21, 0, 30);
    Actor_Run595(21, 4);
    Actor_Run596(20);
    Actor_Run597(21, 0, 20);
    Actor_Run598(5, 3);
    Actor_Run599(20);
    Actor_Run600(1, 3);
    Actor_Run601(10);
    Actor_Run602(1, 0, 20);
    Actor_Place246(0, 0xd000, 0);
    Actor_Place247(21, 256, 0);
    Actor_Run603(21, 3);
    Actor_Run604(30);
    Actor_Run605(21, 0, 60);
    Actor_Run606(1, 3);
    Actor_Place248(1, 0x10000, 0x8000);
    Actor_Run607(Actor_Check51(1), 0);
    Actor_Run608(1, 4, 0);
    Actor_Place249(1, 398, 828);
    Actor_Run609(60);
    Actor_Place250(21, 0x4000, 0);
    Actor_Place251(0, 0xc000, 60);
    Actor_Run610(0, 3);
    Actor_Run611(60);
    Actor_Run612(21, 3);
    Actor_Run613(60);
    rec = Actor_Run313(1);
    flag = 1; /* One shared mark bit for the three record flags. */
    rec[90] |= flag;
    rec = Actor_Run314(5);
    rec[90] |= flag;
    rec = Actor_Run315(0);
    Actor_Place252(1, 0x10000, 0x8000);
    Actor_Place253(5, 0x10000, 0x8000);
    Actor_Run614(0, 0, 0);
    Actor_Run615(5, *(s16 *)(rec + 10) + 16, *(s16 *)(rec + 18));
    Actor_Run616(1, *(s16 *)(rec + 10) + 16, *(s16 *)(rec + 18) - 16);
    Actor_Run617(1);
    Actor_Place254(1, 0x5000, 30);
    Actor_Run618(1, 3);
    Actor_Run619(5, 3);
    Actor_Run620(0, 3);
    Actor_Run621(40);
    Actor_Run622(5, *(s16 *)(rec + 10), *(s16 *)(rec + 18));
    Actor_Run623(5, 0, 0);
    Actor_Run624(1, *(s16 *)(rec + 10), *(s16 *)(rec + 18));
    Actor_Run625(1, 0, 0);
    Actor_Run626(1, 5);
    Actor_Run41(0x1790000, 0xa00000, 0x3770000, 1);
    Actor_Run627(0, 13, 10, 0);
    Actor_Place255(0, 376, 912);
    Actor_Place256(0, 0xc000, 0);
    {
        u8 *record = Actor_Run316(21);
        u8 value = *(volatile u8 *)&record[90];

        record[90] = (u8)(value | flag);
    }
    Actor_Run42(21, 6, 5, 0);
    Actor_Place257(21, 373, 887);
    Actor_Place258(21, 0x4000, 0);
    Actor_Place259(0, 0xc000, 40);
    Actor_Run628(21, 3);
    Actor_Run629(0, 3);
    Actor_Run630(20);
    Actor_Run631(0, 1);
    Actor_Run632();
    Actor_Run633(100);
    Actor_SetRect13(49, 46, 8, 4, 20, 50);
    Actor_Do17(514);
    Actor_Do18(303);
    scene[85] = 3;
    *(s32 *)(scene + 12) = 0xa00000;
    *(s32 *)(scene + 60) = 0x80000000;
    *(s32 *)(scene + 40) = none;
    Actor_Run634();
}

void Effect_ConfigureSpawnedParticle(struct SourceEntity *source)
{
    s32 spawn_position[3];
    s32 particle_index;
    struct StagedParticle *particle;
    spawn_position[0] = source->f08;
    spawn_position[1] = source->f0c - (Actor_EntOp(source) << 4) + (s32)0xfff80000;
    spawn_position[2] = source->f10;
    particle_index = Actor_Check52();
    Actor_Place271(((particle_index << 1) + particle_index) << 4, Actor_Check53(), spawn_position);
    particle = Actor_Run317(0x11d, spawn_position[0], spawn_position[1], spawn_position[2]);
    if (particle != 0) {
        particle->f55 = 2;
        particle->f48 = 0x1999;
        particle->f5e = 12;
        Actor_Apply29(particle, 0);
        Actor_Apply30(particle, 0);
        Actor_Apply31(particle, (s32)gOv7);
        {
            struct ParticleRecord *record = particle->f50;
            s32 record_flags = ~12;
            record_flags &= record->f09;
            record_flags |= 4;
            record->f09 = record_flags;
        }
    }
    Actor_Do43(0x8a);
}

/*
 * Play a rising-dust burst from a source entity. The owner extends through its
 * four literal pool words. Both loops count down inclusively, so they run 31
 * lift steps and 8 particles. The address handed to Actor_Run635 is passed
 * through as a plain pointer; whether it is animation data or a callback is
 * not established.
 */
void Effect_SpawnRisingDustBurst(struct Resource373Emitter *emitter)
{
    s32 frame_countdown;

    Actor_Run636(154);

    for (frame_countdown = 30; frame_countdown >= 0; frame_countdown--) {
        emitter->y += 0x10000;              /* 0x80 << 9. */
        emitter->field06 = (u16)(emitter->field06 + 0x2000);  /* 0x80 << 6. */
        emitter->field18 += -2048;          /* The pool word 0xfffff800. */
        emitter->field1c += -2048;
        Actor_Run637(1);
    }

    for (frame_countdown = 7; frame_countdown >= 0; frame_countdown--) {
        struct Resource373Particle *particle =
            Actor_Run318(0x11d, emitter->x, emitter->y, emitter->z);

        if (particle != 0) {
            s32 vertical_speed;

            Actor_Run638(particle, 0);
            Actor_Run635(particle, (const void *)0x0200e6e4);

            vertical_speed = Actor_Check54() + 0x10000;
            particle->field34 = 0x10000;
            particle->field30 = vertical_speed;
            particle->field55 = 2;
            particle->field48 = 0x0a3d;

            particle->lifetime = Actor_Check55() - Actor_Check56();

            Actor_Run639(
                particle,
                ((Actor_Check57() * 3) << 3) + 0x80000,
                Actor_Check58());
        }
    }

    Actor_Run640(131);

    emitter->x = 0;
    emitter->y = 0;
    emitter->z = 0;
    emitter->field38 = (s32)0x80000000;
    emitter->field3c = (s32)0x80000000;
    emitter->field40 = (s32)0x80000000;
    emitter->field24 = 0;
    emitter->field28 = 0;
    emitter->field2c = 0;
}

void Effect_UpdateParticlePosition(s32 *particle, s32 delta_x, s32 delta_z)
{
    s32 position[3];
    if (particle != 0) {
        position[0] = particle[2];
        position[1] = particle[3];
        position[2] = particle[4];
        Actor_Place272(delta_x, delta_z, position);
        Actor_SetMode3((s32)particle, position[0], position[1], position[2]);
    }
}

void Effect_PlayStepSound(void)
{
    if ((*(u32 *)0x03001e40 & 15) == 0)
        Actor_Do44(0x83);
}

void Effect_AnimateVerticalPositive(struct StagedVerticalEffect *effect)
{
    s32 *anchor = effect->f68;
    s32 frame = ++effect->f64;
    if (frame > 31) {
        Actor_Do45((s32)effect);
    } else {
        s32 amplitude = Actor_Check59(frame << 10);
        s32 offset;
        effect->f18 = amplitude;
        effect->f1c = amplitude;
        effect->f8 = anchor[2];
        effect->fc += 0x10000;
        offset = 0x10000 - amplitude;
        effect->f10 = anchor[4] + offset * 5 + 0x80000;
    }
}

void Effect_AnimateVerticalNegative(struct StagedVerticalEffect *effect)
{
    s32 *anchor = effect->f68;
    s32 frame = ++effect->f64;
    if (frame > 31) {
        Actor_Do46((s32)effect);
    } else {
        s32 amplitude = Actor_Check60(frame << 10);
        s32 offset;
        effect->f18 = amplitude;
        effect->f1c = -amplitude;
        effect->f8 = anchor[2];
        effect->fc += 0x10000;
        offset = 0x10000 - amplitude;
        effect->f10 = anchor[4] - offset * 5 + 0x100000;
    }
}

/* overlays/scene/actor/staged_motion/state_update.c */
extern s32 gOv8[];
extern s32 gOv5[];
extern s32 gOv4[];
extern s32 gOv6[];

u8 *Actor_Run641(s32);

u8 *Actor_Run642(s32);

void State_Send210AndApplyRectAt40x84(void)
{
    s32 m, n;
    Actor_Do47(0x210);
    m = 10;
    n = 84;
    Actor_SetRect15(40, 84, 7, 4, m, n);
}

void State_Send210AndApplyRect(void)
{
    s32 m, n;
    Actor_Do48(0x210);
    m = 10;
    n = 84;
    Actor_SetRect16(40, 89, 7, 4, m, n);
}

void State_ApplyFlag801Branch(void)
{
    if (Actor_Check61(0x801) == 0) {
        Actor_Run643();
    } else {
        Actor_Do49(123);
        Actor_Do50(1);
    }
}

void State_SetValue123Mode3(void)
{
    Actor_Do51(123);
    Actor_Do52(3);
}

void State_SetValue123Mode4(void)
{
    Actor_Do53(123);
    Actor_Do54(4);
}

void State_ApplyValues123And2(void)
{
    Actor_Do55(123);
    Actor_Do56(2);
}

void State_RunFlag204Step(void)
{
    s32 m, n;
    Actor_Run644();
    m = 20;
    n = 50;
    Actor_SetRect17(49, 53, 8, 4, m, n);
    Actor_SetMode4(0, 10, 11, 1);
    Actor_Do57(0x204);
    Actor_Run645();
}

void State_RunTablePairWhenActor22State1(void)
{
    u8 *p = Actor_Run641(22);
    if (Actor_Check62(0x823) != 0) {
        u8 *q = p;
        q += 100;
        if (*(s16 *)q == 1) {
            Actor_Apply32((s32)gOv8, (s32)gOv5);
        }
    }
}

void State_RunTablePairWhenActor22State2(void)
{
    u8 *p = Actor_Run642(22);
    if (Actor_Check63(0x823) != 0) {
        u8 *q = p;
        q += 100;
        if (*(s16 *)q == 2) {
            Actor_Apply33((s32)gOv4, (s32)gOv6);
        }
    }
}

void State_SetValue1ThenCall(void)
{
    Actor_Do58(1);
    Actor_Run646();
}

void State_SetValue0ThenCall(void)
{
    Actor_Do59(0);
    Actor_Run647();
}

void State_SetValue24ThenCall(void)
{
    Actor_Do60(0x18);
    Actor_Run648();
}

void State_SetValue25ThenCall(void)
{
    Actor_Do61(0x19);
    Actor_Run649();
}

void State_ApplyRectAndRunTwo(void)
{
    s32 e = 22;
    s32 f = 36;
    Actor_SetRect18(17, 0, 3, 1, e, f);
    Actor_Run650();
    Actor_Run651();
}
