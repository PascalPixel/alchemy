#include "types.h"
#include "scene.h"

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

struct Rec_3ad *Story_unk3_4();

u8 *Story_unk4_4();

u8 *Story_unk5_4();

u8 *Story_unk6_4();

u8 *Story_unk7_4();

u8 *Story_unk8_4();

u8 *Story_unk9_4();

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

s32 get_default_result(void)
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
    s32 handle = Story_unk24(8);

    if (handle != 0) {
        Story_Apply(handle, 0);
    }
}

void State_MarkActor9AndConfigureRegion7_32(void)
{
    struct Rec_3ad *rec = Story_unk3_4(9);
    s32 p5, p6;

    if (rec != 0) {
        rec->f35 = 1;
        rec->f85 = 0;
    }

    p5 = 8;
    p6 = 32;
    Story_unk11_4(7, 32, 1, 1, p5, p6);
    Story_unk8_2(516);
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

    obj = Story_unk4_4(10);
    Story_unk12_4(10, 5);

    if (obj != 0) {
        Story_unk13_4(obj, 0);
        obj[0x23] = 1;
    }

    row = 21;
    Story_unk14_4(41, 87, 2, 5, row, 59);
    Story_unk15_4(4);

    { s32 f1 = 24; s32 g1 = 62; Story_unk16_4(3, 93, 1, 1,  f1, g1); }
    Story_unk17_4(1, 94, 1, 1, row, 55);
    Story_unk18_4(43, 87, 2, 5, row, 58);
    Story_unk19_4(4);

    Story_unk20_4(41, 87, 2, 5, row, 58);
    Story_unk21_4(4);
    Story_unk22_4(4);

    Story_unk23_4(21, 11, 2, 2, row, 13);
    { s32 f2 = 22; s32 g2 = 15; Story_unk24_4(21, 11, 1, 1,  f2, g2); }
    Story_unk25_4(19, 17, 1, 1, row, 14);
}

void Actor_MarkSlot10AndRunFlag202Step(void)
{
    u32 i;
    u8 *rec7;
    s32 record;

    rec7 = Story_Check(10);
    Story_Do(0x200);
    if ((s32)rec7 != 0) {
        Story_unk26_4((s32)rec7, 0);
        rec7[35] = 1;
    }
    if (Story_unk2(0x202) == 0) {
        Story_unk27_4(157);
        Story_unk28_4();
        Story_unk29_4(80);
        Story_unk2_2(0x202);
    }
}

void State_ApplyFlag203(void)
{
    Story_unk9_2(0x203);
}

void Scene_RunOpeningAuxiliarySequence(void)
{
    /* Gate check on GameFlag_IsSet_1; only proceed with the optional block
     * below when it reports success (0x202). */
    if (GameFlag_IsSet_1(0x202) != 0) {
        /* Two paired setup/confirm calls, run twice with the last
         * argument stepped from 57 to 58. */
        Story_SetRect(41, 86, 2, 6, 21, 57); /* main:080091b8 */
        Story_unk3(4); /* main:080000c0 */
        Story_unk2_5(43, 86, 2, 6, 21, 57); /* main:080091b8 */
        Story_unk4(4); /* main:080000c0 */
        Story_unk3_5(41, 86, 2, 6, 21, 58); /* main:080091b8 */
        Story_unk5(4); /* main:080000c0 */
        Story_unk4_5(43, 86, 2, 6, 21, 58); /* main:080091b8 */
        Story_unk6(4); /* main:080000c0 */
    }
    /* Unconditional call block: two setup calls, a paired setup/confirm
     * pair, then a repeat of the same shape with different constants. */
    Story_unk5_5(2, 93, 1, 1, 24, 62); /* main:080091b8 */
    Story_unk6_5(2, 94, 1, 1, 21, 55); /* main:080091b8 */
    Story_unk7_5(41, 86, 2, 6, 21, 59); /* main:080091b8 */
    Story_unk7(4); /* main:080000c0 */
    Story_unk8_5(1, 93, 1, 1, 24, 62); /* main:080091b8 */
    Story_unk9_5(3, 94, 1, 1, 21, 55); /* main:080091b8 */
    Story_unk10_5(43, 86, 2, 6, 21, 59); /* main:080091b8 */
    Story_unk8(4); /* main:080000c0 */
    /* Final pair closing out the sequence. */
    Motion_SetActionVariant_1(10, 3);
    Story_unk11_5(19, 17, 1, 1, 22, 15); /* main:080091c0 */
}

void Scene_RunScene3adSequenceA(void)
{
    u32 i;
    s32 record;

    if (Story_unk9(0x201) == 0) {
        if (Story_unk10(0x200) == 0) {
            Story_Run(0x1528, 1);
            Story_unk30_4(157);
            Story_unk31_4();
            Story_unk3_2(0x201);
            Story_unk4_2(0x202);
        }
    }
}

void Scene_RunScene3ad(void)
{
    u32 i;
    s32 record;

    Story_unk12_5(41, 87, 2, 5, 21, 59);
    Story_unk32_4(4);
    Story_unk13_5(2, 93, 1, 1, 24, 62);
    Story_unk14_5(2, 94, 1, 1, 21, 55);
    Story_unk15_5(43, 87, 2, 5, 21, 58);
    Story_unk33_4(4);
    Story_unk16_5(3, 93, 1, 1, 24, 62);
    Story_unk17_5(1, 94, 1, 1, 21, 55);
    Story_unk18_5(41, 87, 2, 5, 21, 58);
    Story_unk19_5(21, 11, 2, 2, 21, 13);
    Story_unk20_5(19, 17, 1, 1, 21, 14);
}

void Scene_RunGuardedLine1528Scene(void)
{
    u32 i;
    s32 record;

    if (Story_unk11(0x200) == 0) {
        if (Story_unk12(0x202) == 0) {
            Story_unk2_4(0x1528, 1);
            Story_unk34_4(157);
            Story_unk35_4();
            Story_unk5_2(0x202);
            Story_unk6_2(0x201);
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
        Story_unk36_4();
        Motion_SetSpeed_8();
        Battle_WaitMode0_8(20);
        Battle_SchedShoulder_1();
        Story_unk37_4();
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
    Story_unk38_4(1); /* main:080000c0 */
    Story_unk39_4(); /* main:08009128 */
    Story_unk40_4(1); /* main:080000c0 */
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
    if (Story_unk13(0, 0) == 0) {
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
    Story_unk41_4();
}

void Scene_RunScene3ad(void)
{
    s32 record;
    s32 base5_2547;
    s32 v5;
    s32 base5_254b;

    base5_2547 = (s32)gVal5;
    Story_unk42_4(base5_2547);
    Story_unk43_4(12, 0);
    Story_unk44_4(1, 0, 0);
    Story_unk45_4((base5_2547 + 1));
    Story_unk14(1, 0);
    Story_unk46_4(2, 0, 0);
    Story_unk47_4(3, 0, 0);
    Story_unk48_4(13, 0, 0);
    Story_unk49_4(12, 0, 0);
    L_0200120c:;
    if ((Story_unk25() << 24) != 0) {
        L_02001216:;
        if ((Story_unk26() << 24) == 0) {
            goto L_02001290;
        }
        v5 = 0;
        if ((Story_unk27() << 24) == 0) {
            L_0200122c:;
            v5 = 1;
            L_0200122e:;
            Story_unk50_4();
            record = Story_unk15(0, 0);
            if (record == 0) {
                goto L_02001290;
            }
        }
        if ((Story_unk28() << 24) != 0) {
            goto L_02001296;
        }
        if (v5 == 0) {
            goto L_02001296;
        }
        goto L_0200122e;
    } else {
        if ((Story_unk29() << 24) != 0) {
            if ((Story_unk30() << 24) != 0) {
                goto L_02001296;
            }
            goto L_0200122c;
        } else {
            if ((Story_unk31() << 24) != 0) {
                goto L_02001216;
            }
            base5_254b = (s32)gVal6;
            Story_unk51_4(base5_254b);
            Story_unk52_3(2, 0);
            Story_unk53_3((base5_254b + 1));
            Story_unk54_3(1, 0);
            goto L_0200120c;
            L_02001290:;
            Story_unk55_3();
            goto L_0200129e;
        }
    }
    L_02001296:;
    Story_unk56_3();
    Story_unk57_3();
    L_0200129e:;
}

s32 Dialogue_IsPromptResultZero(void)
{
    return Story_Apply2(0, 0) == 0;
}

s32 Dialogue_AskMessage2549(void)
{
    Story_unk10_2(0x2549);
    Story_Apply3(1, 0);
    return Story_Apply4(0, 0) == 0;
}

s32 Dialogue_AskMessage254d(void)
{
    Story_unk11_2(0x254D);
    Story_Apply5(1, 0);
    return Story_Apply6(0, 0) == 0;
}

s32 SceneData_ReturnOne(void)
{
    return 1;
}

s32 Dialogue_AskMessage2557(void)
{
    Story_unk12_2(0x2557);
    Story_Apply7(12, 0);
    return Story_Apply8(0, 0) == 0;
}

s32 Dialogue_ShowLine255DAndReturnOne(void)
{
    Story_unk13_2(0x255D);
    Story_Apply9(1, 0);
    return 1;
}

s32 Dialogue_AskMessage254a(void)
{
    Story_unk14_2(0x254A);
    Story_Apply10(3, 0);
    return Story_Apply11(0, 0) == 0;
}

s32 Scene_RunScene3ad(void)
{
    s32 record;
    s32 base5_255e;

    Story_unk58_3(2, 0, 0);
    Story_Place(2, 0x102, 60);
    base5_255e = (s32)gVal7;
    Story_unk59_3(base5_255e);
    Story_unk60_3(2, 0);
    Story_unk2_3(12, 0x3000, 0);
    Story_unk61_3(30);
    Story_unk62_3(12, 4);
    Story_unk63_3((base5_255e + 1));
    Story_unk64_3(12, 0);
    Story_unk3_3(3, 0x102, 60);
    Story_unk65_3((base5_255e + 2));
    Story_unk16(3, 0);
    record = Story_unk17(0, 0);
    return !record;
}

s32 Dialogue_AskMessage255c(void)
{
    Story_unk15_2(0x255C);
    Story_Apply12(1, 0);
    return Story_Apply13(0, 0) == 0;
}

void Scene_RunLateScript(void)
{
    u32 i;
    s32 record;
    s32 base5_254e;

    Story_unk4_3(1, 0x105, 60);
    Story_unk66_3(1, 0, 0);
    base5_254e = (s32)gVal8;
    Story_unk67_3(base5_254e);
    Story_unk68_3(1, 0);
    Story_unk69_3(3, 1);
    Story_unk70_3((base5_254e + 1));
    Story_unk71_3(3, 0);
    Story_unk72_3(2, 13, 0);
    Story_unk73_3(60);
    Story_unk74_3((base5_254e + 2));
    Story_unk75_3(2, 0);
    Story_unk76_3(13, 2, 0);
    Story_unk5_3(13, 0x105, 70);
    Story_unk77_3((base5_254e + 3));
    Story_unk78_3(13, 0);
    Story_unk79_3(12, 4);
    Story_unk80_3((base5_254e + 4));
    Story_unk81_3(12, 0);
    Story_unk6_3(12, 0x3000, 0);
    Story_unk82_3(20);
    Story_unk83_3(12, 3);
    Story_unk84_3(0, 3);
    Story_unk85_3(1, 3);
    Story_unk86_3(2, 3);
    Story_unk87_3(3, 3);
    Story_unk88_3(60);
    Story_unk7_3(13, -16, 0);
    Story_unk89_3(13);
    Story_unk90_3(13, 1);
    Story_unk91_3(40);
    Story_unk92_3(13, 3);
    Story_unk8_3(13, 0x5000, 0);
    Story_unk93_3(30);
    Story_unk94_3(0, 3);
    Story_unk95_3(1, 3);
    Story_unk96_3(2, 3);
    Story_unk97_3(3, 3);
    Story_unk9_3(12, 156, 0x210);
    Story_unk98_3(20);
    Story_unk10_3(13, 164, 0x210);
    Story_unk99_3(12);
    Story_unk11_3(12, 168, 0x280);
    Story_unk100_3(13);
    Story_unk12_3(13, 168, 0x280);
    Story_unk13_3(0, 0x5000, 0);
    Story_unk14_3(2, 0x5000, 0);
    Story_unk15_3(3, 0x5000, 0);
    Story_unk16_3(1, 0x5000, 0);
    Story_unk101_3(20);
    Story_unk17_3(0, 0x4000, 0);
    Story_unk18_3(2, 0x4000, 0);
    Story_unk19_3(3, 0x4000, 0);
    Story_unk20_3(1, 0x4000, 0);
    Story_unk102_3(60);
    Story_unk103_3(13, 0, 0);
    Story_unk104_3(12, 0, 0);
    Story_unk105_3(110);
    ((void (*)())Story_unk32)((base5_254e + 5));
    Story_unk106_3(1, 0);
    Story_unk107_3(3, 3);
    Story_unk108_3(30);
    Story_unk109_3((base5_254e + 6));
    Story_unk110_3(3, 0);
    Story_unk111_3(2, 3);
    Story_unk112_3((base5_254e + 7));
    Story_unk113_3(2, 0);
    Story_unk114_3(140);
    Story_unk115_3(1, 0, 0);
    Story_unk116_3((base5_254e + 8));
    Story_unk117_3(1, 0);
    Story_unk118_3(1, 2);
    record = Story_unk18(0);
    if (record != 0) {
        Story_unk119_3(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Story_unk120_3(1);
    Story_unk121_3(1, 0, 0);
    Story_unk122_3(2, 2);
    record = Story_unk19(0);
    if (record != 0) {
        Story_unk123_3(2, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Story_unk124_3(2);
    Story_unk125_3(2, 0, 0);
    Story_unk126_3(3, 2);
    record = Story_unk20(0);
    if (record != 0) {
        Story_unk127_3(3, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Story_unk128_3(3);
    Story_unk129_3(3, 0, 0);
    Story_unk130_3(30);
    Story_unk131_3(0, 1);
    Story_unk132_3();
    Story_unk133_3(0, 0);
    Story_unk7_2(0x94f);
}

void SceneEvent_RunPartyDeparture(void)
{
    s32 record;
    s32 msg;

    Story_unk21_3(1, 0x5000, 0);
    msg = (s32)gVal9;
    Story_unk134_3(msg);
    Story_unk135_3(1, 0);
    Story_unk136_3(2, 3);
    Story_unk137_3((msg + 1));
    Story_unk138_3(2, 0);
    Story_unk139_3(13, 2, 0);
    Story_unk140_3(13, 3);
    Story_unk141_3(20);
    Story_unk142_3((msg + 2));
    Story_unk143_3(13, 0);
    Story_unk22_3(12, 0x3000, 0);
    Story_unk144_3(12, 3);
    Story_unk145_3(30);
    Story_unk146_3((msg + 3));
    Story_unk147_3(12, 0);
    Story_unk148_3(13, 3);
    Story_unk149_3(20);
    Story_unk150_3(0, 3);
    Story_unk151_3(1, 3);
    Story_unk152_3(2, 3);
    Story_unk153_3(3, 3);
    Story_unk154_3(80);
    Story_unk23_3(13, -16, 0);
    Story_unk155_3(13);
    Story_unk156_3(13, 1);
    Story_unk157_3(40);
    Story_unk158_3(13, 3);
    Story_unk24_3(13, 0x5000, 0);
    Story_unk159_3(30);
    Story_unk160_3(0, 3);
    Story_unk161_3(1, 3);
    Story_unk162_3(2, 3);
    Story_unk163_3(3, 3);
    Story_unk25_3(12, 152, 0x210);
    Story_unk164_3(20);
    Story_unk26_3(13, 160, 0x210);
    Story_unk165_3(12);
    Story_unk27_3(12, 168, 0x280);
    Story_unk166_3(13);
    Story_unk28_3(13, 168, 0x280);
    Story_unk29_3(0, 0x5000, 0);
    Story_unk30_3(2, 0x5000, 0);
    Story_unk31_3(3, 0x5000, 0);
    Story_unk32_3(1, 0x5000, 0);
    Story_unk167_3(20);
    Story_unk33_3(0, 0x4000, 0);
    Story_unk34_3(2, 0x4000, 0);
    Story_unk35_3(3, 0x4000, 0);
    Story_unk36_3(1, 0x4000, 0);
    Story_unk168_3(200);
    Story_unk169_3(13, 0, 0);
    Story_unk170_3(12, 0, 0);
    Story_unk171_3(1, 2);
    record = Story_unk21(0);
    if (record != 0) {
        Story_unk172_3(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Story_unk173_3(1);
    Story_unk174_3(1, 0, 0);
    Story_unk175_3(2, 2);
    record = Story_unk22(0);
    if (record != 0) {
        Story_unk176_3(2, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Story_unk177_3(2);
    Story_unk178_3(2, 0, 0);
    Story_unk179_3(3, 2);
    record = Story_unk23(0);
    if (record != 0) {
        Story_unk180_3(3, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Story_unk181_3(3);
    Story_unk182_3(3, 0, 0);
    Story_unk183_3(30);
    Story_unk37_3(0, -16, 0);
    Story_unk184_3(0);
    Story_unk185_3(0, 1);
    Story_unk38_3(0, 168, 0x280);
    Story_unk186_3(60);
    Story_unk187_3();
    Story_unk188_3(3);
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
    Story_unk189_3(Story_unk5_4(8), 0);
    Story_unk190_3(Story_unk6_4(9), 0);
    Story_unk191_2(Story_unk7_4(10), 0);
    Story_unk192_2(Story_unk8_4(11), 0);

    /* Word field at +28 of record 11.  0xf333 comes from the pool. */
    *(s32 *)(Story_unk9_4(11) + 28) = 0xf333;

    if (Story_unk33(0x201) != 0) {
        Story_unk193_2();
    }
    if (Story_unk34(0x202) != 0) {
        Story_unk194_2();
    }
    /* 0x200 is built here rather than taken from the pool. */
    if (Story_unk35(0x200) != 0) {
        Story_unk195_2();
    }
    if (Story_unk36(0x203) != 0) {
        Story_unk196_2(11, 5);
    }
    if (Story_unk37(0x204) != 0) {
        Story_unk197_2(9, 5);
    }
    }

    return 0;
}
