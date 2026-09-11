#include "types.h"
#include "scene.h"
#include "abi/overlays/scene/story/party_departure.h"

#define ACTOR_FLAG(id) (0x4000 | (id))

/*
 * resource_3ad owner at 0x02000094, 64 bytes.
 *
 * Marks record 9 as active, then runs the six-argument entry and one more call.
 *
 * The two stack arguments are locals ASSIGNED AFTER the guard, not initialised
 * at their declaration. Initialising them up front makes the compiler
 * materialise and store them one at a time (`movs r3,#8 / str / movs r3,#32 /
 * str`); assigning them after the record writes keeps both live at once, which
 * is the `movs r3,#8 / movs r2,#32 / str / str` the reference has.
 */
struct Rec_3ad {
    u8 pad00[35];
    u8 f35;                     /* +35 */
    u8 pad24[49];
    u8 f85;                     /* +85 */
};

extern s16 gCell[];
extern u8 Value_0000006a;
extern u8 gOv[];
extern u8 gOv2[];
extern u8 gWork[];
extern u8 gVal[];
extern u8 gVal2[];
extern u8 gVal3[];
extern u8 gVal4[];
extern u8 gVal5[];
extern u8 gVal6[];
extern u8 gVal7[];
extern u8 gVal8[];
extern u8 gVal9[];

struct Rec_3ad *Story_Run3();

u8 *Story_Run4();

u8 *Story_Run5();

u8 *Story_Run6();

u8 *Story_Run7();

u8 *Story_Run8();

u8 *Story_Run9();

/*
 * Overlay resource_3ad. Map repaint sequence: mark the record for slot ten,
 * then run the tile and collision repaints with frame waits between them.
 */

/* Old-style declarations: overlay import arities vary per call site. */

/* Runs a short sequence of external calls with fixed numeric arguments.
 * If a gate check on GameFlag_IsSet_1 succeeds, four extra paired calls run
 * first; the routine then always runs eight more paired calls before a
 * final pair closes it out. */

/* Newly named engine calls: same convention as above, added for raw call
 * sites the mechanical pass left unresolved. Each name states only what its
 * usource shows the function doing. */

/* Actor id encoded with the 0x4000 flag bit set; passed as the lone selector
 * argument to a wait/event call following a step that just acted on that
 * same actor id. */

/* Loader-relocated ROM calls: each site names the pre-relocation call word the image holds. */

/* Contiguous unnamed leaf-owner run for resource_3ad. */

/*
 * Overlay resource_3ad. Exported getter for an in-image table, published
 * from the overlay header.
 */

/*
 * The eight-byte owner at 0x02000030 includes its one pool word, which holds
 * the returned address; the table is returned without being dereferenced.
 * Another overlay exports the same address, but only one overlay is resident
 * at 0x02000000 at a time, so that is a different table -- do not merge the
 * two getters.
 */
u8 *SceneData_GetTable9c34(void)
{
    return (u8 *)0x02009c34;
}

s32 Story_Run10(void)
{
    return 0;
}

/*
 * Overlay resource_3ad. Exported getter for an in-image table, published
 * from the overlay header.
 */

/*
 * The eight-byte owner at 0x0200003c includes its one pool word, which holds
 * the returned address; the table is returned without being dereferenced.
 */
u8 *SceneData_GetTable9CAC(void)
{
    return (u8 *)0x02009cac;
}

s32 SceneData_SelectTable9cc0ByState(void)
{
    if (gCell[224] == (s32)&Value_0000006a) {
        return (s32)gOv;
    }
    return (s32)gOv2;
}

u8 *SceneData_GetTable9dd4(void)
{
    return (u8 *)0x02009dd4;
}

void Actor_SetSlotEightMode0WhenPresent(void)
{
    s32 handle = Story_Check24(8);

    if (handle != 0) {
        Story_Apply(handle, 0);
    }
}

void State_MarkActor9AndConfigureRegion7_32(void)
{
    struct Rec_3ad *rec = Story_Run3(9);
    s32 p5, p6;

    if (rec != 0) {
        rec->f35 = 1;
        rec->f85 = 0;
    }

    p5 = 8;
    p6 = 32;
    Story_Run11(7, 32, 1, 1, p5, p6);
    Story_Do8(516);
}

/*
 * The owner runs from 0x020000d4 for 220 bytes and holds no literal pool.
 * Two of the calls spell their trailing argument pair as locals; that is how
 * the reference puts those two values on the stack, so keep the spelling.
 * Every call here goes through an import veneer in this overlay.
 */
void State_RunRepaintSequenceWithWaits(void)
{
    u8 *obj;
    int row;           /* Assigned at its first use, as the reference does. */

    obj = Story_Run4(10);
    Story_Run12(10, 5);

    if (obj != 0) {
        Story_Run13(obj, 0);
        obj[0x23] = 1;
    }

    row = 21;
    Story_Run14(41, 87, 2, 5, row, 59);
    Story_Run15(4);

    { s32 f1 = 24; s32 g1 = 62; Story_Run16(3, 93, 1, 1,  f1, g1); }
    Story_Run17(1, 94, 1, 1, row, 55);
    Story_Run18(43, 87, 2, 5, row, 58);
    Story_Run19(4);

    Story_Run20(41, 87, 2, 5, row, 58);
    Story_Run21(4);
    Story_Run22(4);

    Story_Run23(21, 11, 2, 2, row, 13);
    { s32 f2 = 22; s32 g2 = 15; Story_Run24(21, 11, 1, 1,  f2, g2); }
    Story_Run25(19, 17, 1, 1, row, 14);
}

void Actor_MarkSlot10AndRunFlag202Step(void)
{
    u32 i;
    u8 *rec7;
    s32 record;

    rec7 = Story_Check(10);
    Story_Do(0x200);
    if ((s32)rec7 != 0) {
        Story_Run26((s32)rec7, 0);
        rec7[35] = 1;
    }
    if (Story_Check2(0x202) == 0) {
        Story_Run27(157);
        Story_Run28();
        Story_Run29(80);
        Story_Do2(0x202);
    }
}

void State_ApplyFlag203(void)
{
    Story_Do9(0x203);
}

void Scene_RunOpeningAuxiliarySequence(void)
{
    /* Gate check on GameFlag_IsSet_1; only proceed with the optional block
     * below when it reports success (0x202). */
    if (GameFlag_IsSet_1(0x202) != 0) {
        /* Two paired setup/confirm calls, run twice with the last
         * argument stepped from 57 to 58. */
        Story_SetRect(41, 86, 2, 6, 21, 57); /* main:080091b8 */
        Story_Check3(4); /* main:080000c0 */
        Story_SetRect2(43, 86, 2, 6, 21, 57); /* main:080091b8 */
        Story_Check4(4); /* main:080000c0 */
        Story_SetRect3(41, 86, 2, 6, 21, 58); /* main:080091b8 */
        Story_Check5(4); /* main:080000c0 */
        Story_SetRect4(43, 86, 2, 6, 21, 58); /* main:080091b8 */
        Story_Check6(4); /* main:080000c0 */
    }
    /* Unconditional call block: two setup calls, a paired setup/confirm
     * pair, then a repeat of the same shape with different constants. */
    Story_SetRect5(2, 93, 1, 1, 24, 62); /* main:080091b8 */
    Story_SetRect6(2, 94, 1, 1, 21, 55); /* main:080091b8 */
    Story_SetRect7(41, 86, 2, 6, 21, 59); /* main:080091b8 */
    Story_Check7(4); /* main:080000c0 */
    Story_SetRect8(1, 93, 1, 1, 24, 62); /* main:080091b8 */
    Story_SetRect9(3, 94, 1, 1, 21, 55); /* main:080091b8 */
    Story_SetRect10(43, 86, 2, 6, 21, 59); /* main:080091b8 */
    Story_Check8(4); /* main:080000c0 */
    /* Final pair closing out the sequence. */
    Motion_SetActionVariant_1(10, 3);
    Story_SetRect11(19, 17, 1, 1, 22, 15); /* main:080091c0 */
}

void Scene_RunScene3adSequenceA(void)
{
    u32 i;
    s32 record;

    if (Story_Check9(0x201) == 0) {
        if (Story_Check10(0x200) == 0) {
            Story_Run(0x1528, 1);
            Story_Run30(157);
            Story_Run31();
            Story_Do3(0x201);
            Story_Do4(0x202);
        }
    }
}

void Scene_RunScene3ad(void)
{
    u32 i;
    s32 record;

    Story_SetRect12(41, 87, 2, 5, 21, 59);
    Story_Run32(4);
    Story_SetRect13(2, 93, 1, 1, 24, 62);
    Story_SetRect14(2, 94, 1, 1, 21, 55);
    Story_SetRect15(43, 87, 2, 5, 21, 58);
    Story_Run33(4);
    Story_SetRect16(3, 93, 1, 1, 24, 62);
    Story_SetRect17(1, 94, 1, 1, 21, 55);
    Story_SetRect18(41, 87, 2, 5, 21, 58);
    Story_SetRect19(21, 11, 2, 2, 21, 13);
    Story_SetRect20(19, 17, 1, 1, 21, 14);
}

void Scene_RunGuardedLine1528Scene(void)
{
    u32 i;
    s32 record;

    if (Story_Check11(0x200) == 0) {
        if (Story_Check12(0x202) == 0) {
            Story_Run2(0x1528, 1);
            Story_Run34(157);
            Story_Run35();
            Story_Do5(0x202);
            Story_Do6(0x201);
        }
    }
}

/* Configures actors 12, 13, 2, 3 and 1 (position, scale, and movement) and
 * runs a scripted multi-actor sequence keyed off two text/index tables
 * (gVal, gVal2), skipping entirely if the guard call at
 * 0x02001fb2 reports the scene is not ready. */
void Scene_RunMultiActorPresentation(void)
{
    s32 prior_actor;
    s32 script_a;
    s32 script_b;

    if (GameFlag_IsSet_1(0x941) == 0) {
    } else {
        GameFlag_Set_1(0x94d);
        Battle_Reset_1();
        Motion_SetHPosTerrain_1(12, 0x900000, 0x1900000);
        Motion_SetSpeed_1(12, 0x18000, 0xc000);
        Motion_ResetPosMode2_1(12, 184, 0x190);
        Motion_CommitPos_1(12);
        Object_SetModeById_1(12, 1);
        Motion_ArmCb_1(12, 0x3000, 0);
        Motion_SetVarCbObj_1(0, 1);
        Battle_WaitMode0_1(30);
        Motion_SetSpeedLim_1(0x8000, 0x1000);
        Motion_CamBounds_1(0xc00000, -1, 0x1b00000, 1);
        prior_actor = Scene_GetRecord_1(0);
        if (prior_actor != 0) {
            Motion_SetHPosTerrain_2(13, *(volatile s32 *)(prior_actor + 8), *(volatile s32 *)(prior_actor + 16));
        }
        Motion_SetSpeed_2(13, 0x14ccc, 0xa666);
        Motion_ResetPosMode2_2(13, 168, 0x1d0);
        Motion_CommitPos_2(13);
        Motion_ArmCb_2(13, 0xc000, 0);
        prior_actor = Scene_GetRecord_2(0);
        if (prior_actor != 0) {
            Motion_SetHPosTerrain_3(2, *(volatile s32 *)(prior_actor + 8), *(volatile s32 *)(prior_actor + 16));
        }
        Motion_SetSpeed_3(2, 0x14ccc, 0xa666);
        Motion_ResetPosMode2_3(2, 152, 0x1e8);
        Motion_CommitPos_3(2);
        Motion_ArmCb_3(2, 0xc000, 0);
        prior_actor = Scene_GetRecord_3(0);
        if (prior_actor != 0) {
            Motion_SetHPosTerrain_4(3, *(volatile s32 *)(prior_actor + 8), *(volatile s32 *)(prior_actor + 16));
        }
        Motion_SetSpeed_4(3, 0x14ccc, 0xa666);
        Motion_ResetPosMode2_4(3, 168, 0x1e8);
        Motion_CommitPos_4(3);
        Motion_ArmCb_4(3, 0xc000, 0);
        prior_actor = Scene_GetRecord_4(0);
        if (prior_actor != 0) {
            Motion_SetHPosTerrain_5(1, *(volatile s32 *)(prior_actor + 8), *(volatile s32 *)(prior_actor + 16));
        }
        Motion_SetSpeed_5(1, 0x14ccc, 0xa666);
        Motion_ResetPosMode2_5(1, 184, 0x1e8);
        Motion_CommitPos_5(1);
        Motion_ArmCb_5(1, 0xc000, 0);
        Battle_WaitMode0_2(30);
        Motion_SetVarCbObj_2(1, 1);
        script_a = (s32)gVal;
        SceneWork_SetStepValue_1(script_a);
        BattleEv_RunWait_1(1, 0);
        Battle_WaitMode0_3(30);
        Motion_CallWaitAnim_1(3, 3);
        Battle_WaitMode0_4(10);
        SceneWork_SetStepValue_2((script_a + 1));
        BattleEv_RunWait_2(3, 0);
        BattleFx_SpawnLinked_1(2, 0x102, 70);
        Motion_SetAngleToward_1(2, 0, 0);
        Motion_SetAngleToward_2(0, 2, 0);
        SceneWork_SetStepValue_3((script_a + 2));
        Motion_SetSpeed_6(2, 0);
        Motion_SetAngleToward_3(3, 0, 0);
        Motion_SetAngleToward_4(1, 0, 0);
        Battle_WaitMode0_5(30);
        if (UiWork_WaitThenFinalizeCapacity_1(0, 0) == 0) {
            SceneWork_SetStepValue_4((script_a + 3));
            BattleEv_RunWait_3(1, 0);
        } else {
            SceneWork_SetStepValue_5((script_a + 4));
            BattleEv_RunWait_4(1, 0);
        }
        BattleFx_SpawnLinked_2(13, 0x100, 70);
        script_b = (s32)gVal2;
        SceneWork_SetStepValue_6(script_b);
        BattleEv_RunWait_5(13, 0);
        Motion_SetVarCb_1(0, 2);
        Motion_SetVarCb_2(1, 2);
        Motion_SetVarCb_3(2, 2);
        Motion_SetVarCb_4(3, 2);
        Motion_ArmCb_6(0, 0xc000, 0);
        Motion_ArmCb_7(1, 0xc000, 0);
        Motion_ArmCb_8(2, 0xc000, 0);
        Motion_ArmCb_9(3, 0xc000, 0);
        Object_SetModeById_2(13, 2);
        Motion_OffsetPositionAndReset_1(13, 0, -16);
        Motion_CommitPos_6(13);
        Object_SetModeById_3(13, 1);
        SceneWork_SetStepValue_7((script_b + 1));
        BattleEv_RunWait_6(13, 0);
        BattleFx_SpawnLinked_3(12, 0x100, 65);
        SceneWork_SetStepValue_8((script_b + 2));
        BattleEv_RunWait_7(12, 0);
        Motion_CallWaitAnim_2(13, 3);
        Battle_WaitMode0_6(80);
        Motion_SetSpeed_7(12, 0x6666, 0x3333);
        Motion_OffsetPositionAndReset_2(12, -13, 0);
        Motion_CommitPos_7(12);
        Motion_ArmCb_10(12, 0x4000, 0);
        BattleFx_SpawnLinked_4(12, 0x102, 70);
        SceneWork_SetStepValue_9((script_b + 3));
        BattleEv_RunWait_8(12, 0);
        Motion_ResetPosMode2_6(12, 168, 0x1b0);
        Battle_WaitMode0_7(40);
        Story_Run36();
        Motion_SetSpeed_8();
        Battle_WaitMode0_8(20);
        Battle_SchedShoulder_1();
        Story_Run37();
    }
}

/* Runs a fixed sequence of camera moves, sprite pose/flag changes, and waits
 * keyed to a byte-per-step table (gVal3, then gVal4), each
 * step address being the table base plus a step index. */
void Scene_RunDialogueCameraChoreography(void)
{
    u32 unused_0; /* Never read or written. */
    s32 unused_1; /* Never read or written. */
    s32 step_table_a; /* Address of gVal3, offset by step index below. */
    s32 step_table_b; /* Address of gVal4, offset by step index below. */

    BattleRuntime_Reset_1();
    ObjectMotion_SetHorizontalPositionWithTerrain_1(1, 0xc80000, 0x2200000);
    ObjectMotion_SetHorizontalPositionWithTerrain_2(0, 0xb80000, 0x2200000);
    ObjectMotion_SetHorizontalPositionWithTerrain_3(3, 0xa80000, 0x2200000);
    ObjectMotion_SetHorizontalPositionWithTerrain_4(2, 0xd40000, 0x2100000);
    ObjectMotion_SetHorizontalPositionWithTerrain_5(13, 0xc80000, 0x2000000);
    Motion_SetHPosTerrain_6(12, 0xa80000, 0x2000000);
    ObjectMotion_ArmCallback_1(1, 0xc000, 0);
    ObjectMotion_ArmCallback_2(0, 0xc000, 0);
    ObjectMotion_ArmCallback_3(3, 0xc000, 0);
    ObjectMotion_ArmCallback_4(2, 0x8000, 0);
    ObjectMotion_ArmCallback_5(13, 0x8000, 0);
    ObjectMotion_ArmCallback_6(12, 0, 0);
    Object_SetModeById_1(12, 1);
    Motion_SetPosReset_1(0, 0);
    Story_Run38(1); /* main:080000c0 */
    Story_Run39(); /* main:08009128 */
    Story_Run40(1); /* main:080000c0 */
    BattleRuntime_WaitIfModeZero_1(); /* main:0808a360 */
    ObjectMotion_SetSpeedParameters_1(); /* main:0808a370 */
    BattleRuntime_WaitIfModeZero_2(20);
    ObjectMotion_CallThenWaitForAnimationChange_1(2, 3);
    BattleRuntime_WaitIfModeZero_3(30);
    BattleEffect_SpawnLinkedResourceObject_1(12, 0x102, 70);
    step_table_a = (s32)gVal3;
    SceneWork_SetStepValue_1(step_table_a);
    BattleEvent_RunActionAndWait_1(12, 0);
    ObjectMotion_CallThenWaitForAnimationChange_2(13, 3);
    SceneWork_SetStepValue_2((step_table_a + 1));
    BattleEvent_RunActionAndWait_2(13, 0);
    ObjectMotion_SetAngleToward_1(2, 13, 0);
    BattleRuntime_WaitIfModeZero_4(30);
    ObjectMotion_SetVariantCallbackAndInvokeObject_1(2, 1);
    BattleRuntime_WaitIfModeZero_5(40);
    SceneWork_SetStepValue_3((step_table_a + 2));
    BattleEvent_RunActionAndWait_3(ACTOR_FLAG(2), 0);
    ObjectMotion_SetAngleToward_2(1, 2, 0);
    BattleRuntime_WaitIfModeZero_6(10);
    BattleEffect_SpawnLinkedResourceObject_2(1, 0x107, 80);
    SceneWork_SetStepValue_4((step_table_a + 3));
    BattleEvent_RunActionAndWait_4(ACTOR_FLAG(1), 0);
    BattleEffect_SpawnLinkedResourceObject_3(2, 0x100, 70);
    ObjectMotion_SetAngleToward_3(2, 1, 0);
    SceneWork_SetStepValue_5((step_table_a + 4));
    BattleEvent_RunActionAndWait_5(ACTOR_FLAG(2), 0);
    ObjectMotion_ArmCallback_7(13, 0x3000, 0);
    BattleRuntime_WaitIfModeZero_7(60);
    ObjectMotion_ArmCallback_8(13, 0x5000, 0);
    BattleRuntime_WaitIfModeZero_8(60);
    SceneWork_SetStepValue_6((step_table_a + 5));
    BattleEvent_RunActionAndWait_6(ACTOR_FLAG(13), 0);
    Battle_WaitMode0_9(30);
    Object_SetModeById_2(0, 3);
    Object_SetModeById_3(3, 3);
    Object_SetModeById_4(1, 3);
    Battle_WaitMode0_10(120);
    ObjectMotion_ArmCallback_9(13, 0x3000, 0);
    Battle_WaitMode0_11(30);
    BattleEffect_SpawnLinkedResourceObject_4(13, 0x108, 80);
    SceneWork_SetStepValue_7((step_table_a + 6));
    BattleEvent_RunActionAndWait_7(ACTOR_FLAG(13), 0);
    ObjectMotion_SetAngleToward_4(2, 13, 0);
    BattleFx_SpawnLinked_5(2, 0x108, 60);
    Motion_CallWaitAnim_3(2, 3);
    Battle_WaitMode0_12(30);
    BattleFx_SpawnLinked_6(3, 0x101, 40);
    Motion_SetAngleToward_5(3, 12, 0);
    SceneWork_SetStepValue_8((step_table_a + 7));
    BattleEvent_RunActionAndWait_8(ACTOR_FLAG(3), 0);
    BattleFx_SpawnLinked_7(13, 0x100, 60);
    Motion_SetAngleToward_6(13, 12, 0);
    SceneWork_SetStepValue_9((step_table_a + 8));
    BattleEv_RunWait_9(ACTOR_FLAG(13), 0);
    BattleFx_SpawnLinked_8(12, 0x102, 70);
    SceneWork_SetStepValue_10((step_table_a + 9));
    BattleEv_RunWait_10(12, 0);
    ObjectMotion_SetVariantCallbackAndInvokeObject_2(12, 1);
    Battle_WaitMode0_13(20);
    SceneWork_SetStepValue_11((step_table_a + 10));
    BattleEv_RunWait_11(ACTOR_FLAG(12), 0);
    Object_LinkPair_1(13, 2, 0);
    Battle_WaitMode0_14(60);
    Motion_SetAngleToward_7(13, 12, 0);
    Motion_SetAngleToward_8(2, 12, 0);
    Motion_CallWaitAnim_4(12, 4);
    Battle_WaitMode0_15(30);
    SceneWork_SetStepValue_12((step_table_a + 11));
    BattleEv_RunWait_12(ACTOR_FLAG(12), 0);
    Motion_SetVarCbObj_3(1, 1);
    Battle_WaitMode0_16(20);
    Motion_SetAngleToward_9(1, 12, 0);
    SceneWork_SetStepValue_13((step_table_a + 12));
    BattleEv_RunWait_13(ACTOR_FLAG(1), 0);
    Motion_SetAngleToward_10(12, 1, 0);
    Battle_WaitMode0_17(20);
    SceneWork_SetStepValue_14((step_table_a + 13));
    BattleEv_RunWait_14(ACTOR_FLAG(12), 0);
    Motion_SetAngleToward_11(3, 12, 0);
    BattleFx_SpawnLinked_9(3, 0x101, 70);
    SceneWork_SetStepValue_15((step_table_a + 14));
    BattleEv_RunWait_15(ACTOR_FLAG(3), 0);
    Motion_CallWaitAnim_5(12, 3);
    Battle_WaitMode0_18(40);
    SceneWork_SetStepValue_16((step_table_a + 15));
    BattleEv_RunWait_16(ACTOR_FLAG(12), 0);
    BattleFx_SpawnLinked_10(13, 0x100, 70);
    SceneWork_SetStepValue_17((step_table_a + 16));
    BattleEv_RunWait_17(ACTOR_FLAG(13), 0);
    Motion_SetAngleToward_12(12, 13, 0);
    Motion_CallWaitAnim_6(12, 3);
    Battle_WaitMode0_19(30);
    SceneWork_SetStepValue_18((step_table_a + 17));
    BattleEv_RunWait_18(ACTOR_FLAG(12), 0);
    Motion_CallWaitAnim_7(13, 3);
    Battle_WaitMode0_20(30);
    SceneWork_SetStepValue_19((step_table_a + 18));
    BattleEv_RunWait_19(ACTOR_FLAG(13), 0);
    BattleFx_SpawnLinked_11(12, 0x108, 70);
    ObjectMotion_SetVariantCallback_1(2, 1);
    Battle_WaitMode0_21(30);
    SceneWork_SetStepValue_20((step_table_a + 19));
    BattleEv_RunWait_20(ACTOR_FLAG(2), 0);
    Motion_SetAngleToward_13(12, 2, 0);
    Motion_CallWaitAnim_8(12, 3);
    Battle_WaitMode0_22(30);
    SceneWork_SetStepValue_21((step_table_a + 20));
    BattleEv_RunWait_21(ACTOR_FLAG(2), 0);
    Motion_SetVarCbObj_4(1, 1);
    SceneWork_SetStepValue_22((step_table_a + 21));
    BattleEv_RunWait_22(ACTOR_FLAG(1), 0);
    ObjectMotion_ArmCallback_10(12, 0x3000, 0);
    Motion_CallWaitAnim_9(12, 4);
    SceneWork_SetStepValue_23((step_table_a + 22));
    BattleEv_RunWait_23(ACTOR_FLAG(12), 0);
    Motion_SetVarCbObj_5(3, 1);
    SceneWork_SetStepValue_24((step_table_a + 23));
    BattleEv_RunWait_24(ACTOR_FLAG(3), 0);
    Motion_SetAngleToward_14(12, 3, 0);
    BattleFx_SpawnLinked_12(12, 0x100, 60);
    SceneWork_SetStepValue_25((step_table_a + 24));
    BattleEv_RunWait_25(ACTOR_FLAG(12), 0);
    Object_LinkPair_2(2, 13, 0);
    Battle_WaitMode0_23(20);
    Motion_CallWaitAnim_10(2, 3);
    Battle_WaitMode0_24(30);
    SceneWork_SetStepValue_26((step_table_a + 25));
    BattleEv_RunWait_26(ACTOR_FLAG(2), 0);
    Motion_CallWaitAnim_11(13, 3);
    Motion_SetAngleToward_15(13, 12, 0);
    Motion_SetAngleToward_16(2, 12, 0);
    Battle_WaitMode0_25(20);
    SceneWork_SetStepValue_27((step_table_a + 26));
    BattleEv_RunWait_27(ACTOR_FLAG(12), 0);
    Battle_WaitMode0_26(12, 0x102);
    SceneWork_SetStepValue_28((step_table_a + 27));
    BattleEv_RunWait_28(ACTOR_FLAG(12), 0);
    Motion_SetAngleToward_17(1, 12, 0);
    SceneWork_SetStepValue_29((step_table_a + 28));
    BattleEv_RunWait_29(ACTOR_FLAG(1), 0);
    BattleFx_SpawnLinked_13(12, 0x105, 70);
    Motion_CallWaitAnim_12(3, 4);
    SceneWork_SetStepValue_30((step_table_a + 29));
    BattleEv_RunWait_30(ACTOR_FLAG(3), 0);
    Motion_SetVarCbObj_6(12, 1);
    SceneWork_SetStepValue_31((step_table_a + 30));
    BattleEv_RunWait_31(ACTOR_FLAG(12), 0);
    Motion_SetVarCbObj_7(12, 1);
    SceneWork_SetStepValue_32((step_table_a + 31));
    BattleEv_RunWait_32(ACTOR_FLAG(13), 0);
    Battle_WaitMode0_27(30);
    Motion_SetAngleToward_18(12, 13, 0);
    Battle_WaitMode0_28(60);
    Motion_CallWaitAnim_13(12, 4);
    SceneWork_SetStepValue_33((step_table_a + 32));
    BattleEv_RunWait_33(ACTOR_FLAG(12), 0);
    Motion_CallWaitAnim_14(13, 3);
    Battle_WaitMode0_29(30);
    Motion_ArmCb_11(12, 0x4000, 0);
    BattleFx_SpawnLinked_14(12, 0x100, 60);
    ObjectMotion_SetSpeedParameters_2(12, 0x18000, 0xc000);
    ObjectMotion_ResetAndSetPositionInMode2_1(12, 144, 0x210);
    ObjectMotion_CommitCurrentPositionAndActivate_1(12);
    ObjectMotion_ResetAndSetPositionInMode2_2(12, 168, 0x230);
    ObjectMotion_CommitCurrentPositionAndActivate_2(12);
    Battle_WaitMode0_30(60);
    Motion_ArmCb_12(12, 0x3000, 0);
    Battle_WaitMode0_31(40);
    Motion_ArmCb_13(12, 0x5000, 0);
    Battle_WaitMode0_32(40);
    Motion_ArmCb_14(12, 0x3000, 0);
    Battle_WaitMode0_33(40);
    ObjectMotion_ResetAndSetPositionInMode2_3(12, 144, 0x210);
    ObjectMotion_CommitCurrentPositionAndActivate_3(12);
    ObjectMotion_ResetAndSetPositionInMode2_4(12, 168, 0x1e8);
    SceneWork_SetStepValue_34((step_table_a + 33));
    BattleEv_RunWait_34(ACTOR_FLAG(2), 0);
    ObjectMotion_CommitCurrentPositionAndActivate_4(12);
    Motion_ArmCb_15(12, 0x3000, 0);
    Object_SetModeById_5(12, 1);
    Motion_CallWaitAnim_15(12, 4);
    Battle_WaitMode0_34(20);
    SceneWork_SetStepValue_35((step_table_a + 34));
    BattleEv_RunWait_35(12, 0);
    Battle_WaitMode0_35(20);
    Object_LinkPair_3(1, 0, 0);
    Battle_WaitMode0_36(60);
    Motion_ArmCb_16(0, 0xc000, 0);
    Motion_ArmCb_17(1, 0xc000, 0);
    BattleFx_SpawnLinked_15(12, 0x102, 60);
    SceneWork_SetStepValue_36((step_table_a + 35));
    BattleEv_RunWait_36(12, 0);
    Motion_SetVarCbObj_8(3, 1);
    SceneWork_SetStepValue_37((step_table_a + 36));
    BattleEv_RunWait_37(ACTOR_FLAG(3), 0);
    BattleFx_SpawnLinked_16(2, 0x102, 60);
    SceneWork_SetStepValue_38((step_table_a + 37));
    BattleEv_RunWait_38(ACTOR_FLAG(2), 0);
    Motion_SetAngleToward_19(1, 0, 0);
    BattleFx_SpawnLinked_17(2, 0x102, 40);
    SceneWork_SetStepValue_39((step_table_a + 38));
    ObjectMotion_SetSpeedParameters_3(ACTOR_FLAG(1), 0); /* main:0808a178 */
    if (Story_Check13(0, 0) == 0) {
        SceneWork_SetStepValue_40((step_table_a + 39));
        BattleEv_RunWait_39(ACTOR_FLAG(12), 0);
    } else {
        SceneWork_SetStepValue_41((step_table_a + 40));
        BattleEv_RunWait_40(ACTOR_FLAG(12), 0);
    }
    Motion_ArmCb_18(12, 0x3000, 0);
    Motion_SetAngleToward_20(0, 12, 0);
    Motion_SetAngleToward_21(1, 12, 0);
    Motion_SetAngleToward_22(3, 12, 0);
    Motion_SetAngleToward_23(2, 12, 0);
    Motion_SetAngleToward_24(13, 12, 0);
    BattleFx_SpawnLinked_18(13, 0x101, 60);
    step_table_b = (s32)gVal4;
    SceneWork_SetStepValue_42(step_table_b);
    BattleEv_RunWait_41(ACTOR_FLAG(13), 0);
    Battle_WaitMode0_37(20);
    BattleFx_SpawnLinked_19(12, 0x108, 60);
    SceneWork_SetStepValue_43((step_table_b + 1));
    BattleEv_RunWait_42(ACTOR_FLAG(12), 0);
    BattleFx_SpawnLinked_20(3, 0x101, 60);
    SceneWork_SetStepValue_44((step_table_b + 2));
    BattleEv_RunWait_43(ACTOR_FLAG(3), 0);
    Motion_SetAngleToward_25(12, 3, 0);
    Battle_WaitMode0_38(10);
    Motion_CallWaitAnim_16(12, 3);
    Motion_ArmCb_19(12, 0x3000, 0);
    SceneWork_SetStepValue_45((step_table_b + 3));
    BattleEv_RunWait_44(ACTOR_FLAG(12), 0);
    Motion_SetAngleToward_26(2, 12, 0);
    SceneWork_SetStepValue_46((step_table_b + 4));
    BattleEv_RunWait_45(ACTOR_FLAG(2), 0);
    Battle_WaitMode0_39(10);
    Motion_CallWaitAnim_17(12, 4);
    SceneWork_SetStepValue_47((step_table_b + 5));
    BattleEv_RunWait_46(ACTOR_FLAG(12), 0);
    Object_LinkPair_4(0, 1, 0);
    Battle_WaitMode0_40(60);
    Motion_SetAngleToward_27(0, 12, 0);
    Motion_SetAngleToward_28(1, 12, 0);
    SceneWork_SetStepValue_48((step_table_b + 6));
    BattleEv_RunWait_47(ACTOR_FLAG(1), 0);
    Motion_SetAngleToward_29(12, 1, 0);
    Motion_CallWaitAnim_18(12, 3);
    Battle_WaitMode0_41(20);
    SceneWork_SetStepValue_49((step_table_b + 7));
    BattleEv_RunWait_48(ACTOR_FLAG(12), 0);
    Object_LinkPair_5(13, 2, 0);
    Battle_WaitMode0_42(60);
    Object_SetModeById_6(13, 3);
    Object_SetModeById_7(2, 3);
    Battle_WaitMode0_43(60);
    Object_SetModeById_8(0, 3);
    Object_SetModeById_9(1, 3);
    Object_SetModeById_10(3, 3);
    Motion_SetAngleToward_30(12, 0, 0);
    Object_SetModeById_11(12, 3);
    Battle_WaitMode0_44(60);
    Story_Run41();
}

void Scene_RunScene3ad(void)
{
    s32 record;
    s32 base5_2547;
    s32 v5;
    s32 base5_254b;

    base5_2547 = (s32)gVal5;
    Story_Run42(base5_2547);
    Story_Run43(12, 0);
    Story_Run44(1, 0, 0);
    Story_Run45((base5_2547 + 1));
    Story_Check14(1, 0);
    Story_Run46(2, 0, 0);
    Story_Run47(3, 0, 0);
    Story_Run48(13, 0, 0);
    Story_Run49(12, 0, 0);
    L_0200120c:;
    if ((Story_Check25() << 24) != 0) {
        L_02001216:;
        if ((Story_Check26() << 24) == 0) {
            goto L_02001290;
        }
        v5 = 0;
        if ((Story_Check27() << 24) == 0) {
            L_0200122c:;
            v5 = 1;
            L_0200122e:;
            Story_Run50();
            record = Story_Check15(0, 0);
            if (record == 0) {
                goto L_02001290;
            }
        }
        if ((Story_Check28() << 24) != 0) {
            goto L_02001296;
        }
        if (v5 == 0) {
            goto L_02001296;
        }
        goto L_0200122e;
    } else {
        if ((Story_Check29() << 24) != 0) {
            if ((Story_Check30() << 24) != 0) {
                goto L_02001296;
            }
            goto L_0200122c;
        } else {
            if ((Story_Check31() << 24) != 0) {
                goto L_02001216;
            }
            base5_254b = (s32)gVal6;
            Story_Run51(base5_254b);
            Story_Run52(2, 0);
            Story_Run53((base5_254b + 1));
            Story_Run54(1, 0);
            goto L_0200120c;
            L_02001290:;
            Story_Run55();
            goto L_0200129e;
        }
    }
    L_02001296:;
    Story_Run56();
    Story_Run57();
    L_0200129e:;
}

s32 Dialogue_IsPromptResultZero(void)
{
    return Story_Apply2(0, 0) == 0;
}

s32 Dialogue_AskMessage2549(void)
{
    Story_Do10(0x2549);
    Story_Apply3(1, 0);
    return Story_Apply4(0, 0) == 0;
}

s32 Dialogue_AskMessage254d(void)
{
    Story_Do11(0x254D);
    Story_Apply5(1, 0);
    return Story_Apply6(0, 0) == 0;
}

s32 SceneData_ReturnOne(void)
{
    return 1;
}

s32 Dialogue_AskMessage2557(void)
{
    Story_Do12(0x2557);
    Story_Apply7(12, 0);
    return Story_Apply8(0, 0) == 0;
}

s32 Dialogue_ShowLine255DAndReturnOne(void)
{
    Story_Do13(0x255D);
    Story_Apply9(1, 0);
    return 1;
}

s32 Dialogue_AskMessage254a(void)
{
    Story_Do14(0x254A);
    Story_Apply10(3, 0);
    return Story_Apply11(0, 0) == 0;
}

s32 Scene_RunScene3ad(void)
{
    s32 record;
    s32 base5_255e;

    Story_Run58(2, 0, 0);
    Story_Place(2, 0x102, 60);
    base5_255e = (s32)gVal7;
    Story_Run59(base5_255e);
    Story_Run60(2, 0);
    Story_Place2(12, 0x3000, 0);
    Story_Run61(30);
    Story_Run62(12, 4);
    Story_Run63((base5_255e + 1));
    Story_Run64(12, 0);
    Story_Place3(3, 0x102, 60);
    Story_Run65((base5_255e + 2));
    Story_Check16(3, 0);
    record = Story_Check17(0, 0);
    return !record;
}

s32 Dialogue_AskMessage255c(void)
{
    Story_Do15(0x255C);
    Story_Apply12(1, 0);
    return Story_Apply13(0, 0) == 0;
}

void Scene_RunLateScript(void)
{
    u32 i;
    s32 record;
    s32 base5_254e;

    Story_Place4(1, 0x105, 60);
    Story_Run66(1, 0, 0);
    base5_254e = (s32)gVal8;
    Story_Run67(base5_254e);
    Story_Run68(1, 0);
    Story_Run69(3, 1);
    Story_Run70((base5_254e + 1));
    Story_Run71(3, 0);
    Story_Run72(2, 13, 0);
    Story_Run73(60);
    Story_Run74((base5_254e + 2));
    Story_Run75(2, 0);
    Story_Run76(13, 2, 0);
    Story_Place5(13, 0x105, 70);
    Story_Run77((base5_254e + 3));
    Story_Run78(13, 0);
    Story_Run79(12, 4);
    Story_Run80((base5_254e + 4));
    Story_Run81(12, 0);
    Story_Place6(12, 0x3000, 0);
    Story_Run82(20);
    Story_Run83(12, 3);
    Story_Run84(0, 3);
    Story_Run85(1, 3);
    Story_Run86(2, 3);
    Story_Run87(3, 3);
    Story_Run88(60);
    Story_Place7(13, -16, 0);
    Story_Run89(13);
    Story_Run90(13, 1);
    Story_Run91(40);
    Story_Run92(13, 3);
    Story_Place8(13, 0x5000, 0);
    Story_Run93(30);
    Story_Run94(0, 3);
    Story_Run95(1, 3);
    Story_Run96(2, 3);
    Story_Run97(3, 3);
    Story_Place9(12, 156, 0x210);
    Story_Run98(20);
    Story_Place10(13, 164, 0x210);
    Story_Run99(12);
    Story_Place11(12, 168, 0x280);
    Story_Run100(13);
    Story_Place12(13, 168, 0x280);
    Story_Place13(0, 0x5000, 0);
    Story_Place14(2, 0x5000, 0);
    Story_Place15(3, 0x5000, 0);
    Story_Place16(1, 0x5000, 0);
    Story_Run101(20);
    Story_Place17(0, 0x4000, 0);
    Story_Place18(2, 0x4000, 0);
    Story_Place19(3, 0x4000, 0);
    Story_Place20(1, 0x4000, 0);
    Story_Run102(60);
    Story_Run103(13, 0, 0);
    Story_Run104(12, 0, 0);
    Story_Run105(110);
    ((void (*)())Story_Check32)((base5_254e + 5));
    Story_Run106(1, 0);
    Story_Run107(3, 3);
    Story_Run108(30);
    Story_Run109((base5_254e + 6));
    Story_Run110(3, 0);
    Story_Run111(2, 3);
    Story_Run112((base5_254e + 7));
    Story_Run113(2, 0);
    Story_Run114(140);
    Story_Run115(1, 0, 0);
    Story_Run116((base5_254e + 8));
    Story_Run117(1, 0);
    Story_Run118(1, 2);
    record = Story_Check18(0);
    if (record != 0) {
        Story_Run119(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Story_Run120(1);
    Story_Run121(1, 0, 0);
    Story_Run122(2, 2);
    record = Story_Check19(0);
    if (record != 0) {
        Story_Run123(2, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Story_Run124(2);
    Story_Run125(2, 0, 0);
    Story_Run126(3, 2);
    record = Story_Check20(0);
    if (record != 0) {
        Story_Run127(3, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Story_Run128(3);
    Story_Run129(3, 0, 0);
    Story_Run130(30);
    Story_Run131(0, 1);
    Story_Run132();
    Story_Run133(0, 0);
    Story_Do7(0x94f);
}

void SceneEvent_RunPartyDeparture(void)
{
    s32 record;
    s32 msg;

    Story_Place21(1, 0x5000, 0);
    msg = (s32)gVal9;
    Story_Run134(msg);
    Story_Run135(1, 0);
    Story_Run136(2, 3);
    Story_Run137((msg + 1));
    Story_Run138(2, 0);
    Story_Run139(13, 2, 0);
    Story_Run140(13, 3);
    Story_Run141(20);
    Story_Run142((msg + 2));
    Story_Run143(13, 0);
    Story_Place22(12, 0x3000, 0);
    Story_Run144(12, 3);
    Story_Run145(30);
    Story_Run146((msg + 3));
    Story_Run147(12, 0);
    Story_Run148(13, 3);
    Story_Run149(20);
    Story_Run150(0, 3);
    Story_Run151(1, 3);
    Story_Run152(2, 3);
    Story_Run153(3, 3);
    Story_Run154(80);
    Story_Place23(13, -16, 0);
    Story_Run155(13);
    Story_Run156(13, 1);
    Story_Run157(40);
    Story_Run158(13, 3);
    Story_Place24(13, 0x5000, 0);
    Story_Run159(30);
    Story_Run160(0, 3);
    Story_Run161(1, 3);
    Story_Run162(2, 3);
    Story_Run163(3, 3);
    Story_Place25(12, 152, 0x210);
    Story_Run164(20);
    Story_Place26(13, 160, 0x210);
    Story_Run165(12);
    Story_Place27(12, 168, 0x280);
    Story_Run166(13);
    Story_Place28(13, 168, 0x280);
    Story_Place29(0, 0x5000, 0);
    Story_Place30(2, 0x5000, 0);
    Story_Place31(3, 0x5000, 0);
    Story_Place32(1, 0x5000, 0);
    Story_Run167(20);
    Story_Place33(0, 0x4000, 0);
    Story_Place34(2, 0x4000, 0);
    Story_Place35(3, 0x4000, 0);
    Story_Place36(1, 0x4000, 0);
    Story_Run168(200);
    Story_Run169(13, 0, 0);
    Story_Run170(12, 0, 0);
    Story_Run171(1, 2);
    record = Story_Check21(0);
    if (record != 0) {
        Story_Run172(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Story_Run173(1);
    Story_Run174(1, 0, 0);
    Story_Run175(2, 2);
    record = Story_Check22(0);
    if (record != 0) {
        Story_Run176(2, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Story_Run177(2);
    Story_Run178(2, 0, 0);
    Story_Run179(3, 2);
    record = Story_Check23(0);
    if (record != 0) {
        Story_Run180(3, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Story_Run181(3);
    Story_Run182(3, 0, 0);
    Story_Run183(30);
    Story_Place37(0, -16, 0);
    Story_Run184(0);
    Story_Run185(0, 1);
    Story_Place38(0, 168, 0x280);
    Story_Run186(60);
    Story_Run187();
    Story_Run188(3);
}

/*
 * Scene-entry hook for resource_3ad: seed the workspace scene id, then run
 * the per-actor setup and the flag-gated steps for that scene.
 */

/* Old-style declarations: overlay import arities vary per call site. */

/*
 * The owner returns 0 on every path, and its 200 bytes include the seven
 * pool words past the return.  0x204 is stored into the workspace at +448
 * and read again later as a story-flag id, so both uses share one value.
 * The scene id is compared against 0x6a spelled as the address of
 * Value_0000006a; that spelling is what reproduces the reference.
 */
s32 Scene_RunSceneEntryHook(void)
{
    u8 *workspace;
    workspace = *(u8 **)0x03001ebc;
    *(s32 *)(workspace + 448) = 0x204;

    if (gCell[224] == (s32)&Value_0000006a) {
    Story_Run189(Story_Run5(8), 0);
    Story_Run190(Story_Run6(9), 0);
    Story_Run191(Story_Run7(10), 0);
    Story_Run192(Story_Run8(11), 0);

    /* Word field at +28 of record 11.  0xf333 comes from the pool. */
    *(s32 *)(Story_Run9(11) + 28) = 0xf333;

    if (Story_Check33(0x201) != 0) {
        Story_Run193();
    }
    if (Story_Check34(0x202) != 0) {
        Story_Run194();
    }
    /* 0x200 is built here rather than taken from the pool. */
    if (Story_Check35(0x200) != 0) {
        Story_Run195();
    }
    if (Story_Check36(0x203) != 0) {
        Story_Run196(11, 5);
    }
    if (Story_Check37(0x204) != 0) {
        Story_Run197(9, 5);
    }
    }

    return 0;
}
