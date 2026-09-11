#include "types.h"
#include "scene.h"
#include "abi/overlays/scene/story/scenario_dispatch/scenario_dispatch.h"

/* overlays/scene/story/scenario_dispatch/run_scene_early_sequence.c */
/* Runs a linear setup sequence once two prior gate checks both pass;
 * either gate failing skips the whole sequence. Every step below is an
 * opaque call into other overlay code, so only the gate values and the
 * shared byte pair are named from what this function itself shows. */

/* Gate id checked before the sequence, and passed again to the call that
 * closes it out. */
#define GATE_ID 2081

extern u8 Value_00001032;

void Scene_RunSupplementalSequenceTwo(void)
{
    s32 byte_pair_addr;

    if (GameFlag_IsSet_1(3842) == 0) {
    } else {
        if (GameFlag_IsSet_2(GATE_ID)!= 0) {
        } else {
            Battle_Reset_1();
            Story_Run();
            Audio_PlayCue_1(182);
            Story_SetRect(0, 71, 100, 71, 1, 1);
            Story_Run2();
            Battle_WaitMode0_1(40);
            /* Address of the byte pair starting at Value_00001032. */
            byte_pair_addr = (s32)&Value_00001032;
            UiText_DrawMessage_1(byte_pair_addr, 1);
            Battle_WaitMode0_2(20);
            Audio_PlayCue_2(183);
            Story_Run6(122, 20, 120, 30, 1, 2);
            Story_SetRect2(122, 20, 1, 2, 120, 30);
            Story_Run3();
            Story_Place(65536, 65536, 65536);
            Battle_WaitMode0_3(20);
            BattleFx_SpawnLinked_1(0, 256, 0);
            Story_Place2(131072, 131072, 65536);
            Battle_WaitMode0_4(20);
            Motion_ArmCb_1(0, 16384, 40);
            Motion_ArmCb_2(0, 32768, 20);
            Motion_ArmCb_3(0, 0, 20);
            Motion_ArmCb_4(0, 16384, 10);
            Motion_Launch_1(0, 4, 20);
            Motion_Launch_2(0, 6, 40);
            Story_Place3(-1, -1, 58982);
            Battle_WaitMode0_5(40);
            UiText_DrawMessage_2(byte_pair_addr + 1, 1);
            GameFlag_Set_1(0x143);
            GameFlag_Set_2(GATE_ID);
            Battle_SchedShoulder_1();
        }
    }
}

/* overlays/scene/story/scenario_dispatch/run_scene_late_sequence.c */
extern u8 Value_00000fe0;

u8 *Story_Run7();

/*
 * resource_37f owner at 0x02000f8c, 1,680 bytes: full scene sequence
 * for entities 0, 1, 5 and 8.
 *
 * Complete owner: saved-link prologue at 0x02000f8c through the sole
 * interworking return at 0x02001600, alignment and six-word trailing pool
 * through 0x0200161b.  A branch at 0x020013d0 skips an eleven-word interior
 * constant pool and resumes at 0x02001400.  All 174 BL sites resolve to 30
 * import veneers and are represented below in machine order.
 */
/* Loader-relocated ROM calls: each site names the pre-relocation call word the image holds. */

/* Additional resolved engine calls: named directly from each target's own
 * reconstructed C source (see the trailing "provisional" comment on each). */

/* Two-field record read used when copying a found record's s32 pair into a
 * setter call alongside an entity id. */
#define RECORD_A32(rec) (*(s32 *)((rec) + 8))
#define RECORD_B32(rec) (*(s32 *)((rec) + 16))

/* Two-field record read used when copying a found record's s16 pair into a
 * setter call alongside an entity id. */
#define RECORD_A16(rec) (*(s16 *)((rec) + 10))
#define RECORD_B16(rec) (*(s16 *)((rec) + 18))

/* Runs the full scene sequence for entities 0, 1, 5 and 8: per-entity setup,
 * a shared movement/pose pass, a branch on one entity's saved state, and a
 * closing pass over the same four entities. */
void Scene_RunFourEntitySequence(void)
{
    u8 *record;

    Battle_Reset_1();
    Battle_WaitMode0_1(); /* main:0808a360 */
    Motion_SetSpeed_1(); /* main:0808a370 */

    record = Scene_GetRecord_1(0);
    if (record != 0)
        Motion_SetHPosTerrain_1(8, RECORD_A32(record), RECORD_B32(record));
    record = Scene_GetRecord_2(0);
    if (record != 0)
        Motion_SetHPosTerrain_2(5, RECORD_A32(record), RECORD_B32(record));
    record = Scene_GetRecord_3(0);
    if (record != 0)
        Motion_SetHPosTerrain_3(1, RECORD_A32(record), RECORD_B32(record));

    Motion_SetSpeed_2(8, 0x9999, 0x4ccc);
    Motion_SetSpeed_3(5, 0x9999, 0x4ccc);
    Motion_SetSpeed_4(1, 0x9999, 0x4ccc);
    Object_SetModeById_1(1, 2);
    Object_SetModeById_2(5, 2);
    Object_SetModeById_3(8, 2);
    Motion_OffsetPositionAndResetMotion_1(1, -16, 0);
    Motion_OffsetPositionAndResetMotion_2(5, 16, 0);
    Motion_OffsetPositionAndResetMotion_3(8, 0, -16);
    Motion_CommitPos_1(8);
    Object_SetModeById_4(8, 1);
    Object_SetModeById_5(0, 0);
    Object_SetModeById_6(1, 0);
    Object_SetModeById_7(5, 0);
    Motion_ArmCb_1(1, 0xe000, 0);
    Motion_ArmCb_2(5, 0xa000, 0);
    Motion_ArmCb_3(8, 0xc000, 30);
    Motion_ArmCb_4(1, 0x8000, 0);
    Motion_ArmCb_5(5, 0, 0);
    Motion_ArmCb_6(0, 0x4000, 0);
    Motion_ArmCb_7(8, 0x8000, 30);
    Motion_ArmCb_8(1, 0x4000, 0);
    Motion_ArmCb_9(5, 0x8000, 0);
    Motion_ArmCb_10(0, 0, 0);
    Motion_ArmCb_11(8, 0x4000, 30);
    Motion_ArmCb_12(1, 0xe000, 0);
    Motion_ArmCb_13(5, 0xa000, 0);
    Motion_ArmCb_14(0, 0xc000, 0);
    Motion_ArmCb_15(8, 0xc000, 40);
    Motion_SetVarCbObj_1(8, 2);
    Battle_WaitMode0_2(10);
    Object_SetModeById_8(8, 2);
    Motion_OffsetPositionAndResetMotion_4(8, 0, -16);
    Motion_CommitPos_2(8);
    Object_SetModeById_9(8, 1);
    Battle_WaitMode0_3(6);
    Motion_ArmCb_16(8, 0x8000, 20);
    Motion_ArmCb_17(8, 0, 20);
    Motion_ArmCb_18(8, 0xc000, 40);
    Motion_SetVarCbObj_2(8, 2);
    Battle_WaitMode0_4(20);
    Object_SetModeById_10(8, 2);
    Motion_OffsetPositionAndResetMotion_5(8, 0, -32);
    Motion_CommitPos_3(8);
    Object_SetModeById_11(8, 1);

    Motion_SetSpeedLim_1(0x20000, 0x4000);
    Motion_CamBounds_1(0x06310000, -1, 0x00960000, 1);
    Object_CommitPositionThenWaitIfModeZero_1();
    Battle_WaitMode0_5(10);
    Motion_SetSpeedLim_2(0x13333, 0x2666);
    Motion_CamBounds_2(0x06550000, -1, 0x00640000, 1);
    Object_CommitPositionThenWaitIfModeZero_2();
    Motion_CamBounds_3(0x06b60000, -1, 0x00640000, 1);
    Object_CommitPositionThenWaitIfModeZero_3();
    Object_SetModeById_12(8, 1);
    Motion_CamBounds_4(0x06d80000, -1, 0x00960000, 1);
    Object_CommitPositionThenWaitIfModeZero_4();
    Battle_WaitMode0_6(40);
    Motion_SetSpeedLim_3(0x26666, 0x4ccc);
    Motion_CamBounds_5(0x06840000, -1, 0x01000000, 1);
    Object_CommitPositionThenWaitIfModeZero_5();
    Motion_CallWaitAnim_1(8, 3);
    Battle_WaitMode0_7(10);

    Motion_ArmCb_19(1, 0xe000, 0);
    Motion_ArmCb_20(5, 0xa000, 10);
    BattleFx_SpawnLinked_1(1, 0x101, 20);
    SceneWork_SetStepValue_1(0xfd6);
    Battle_RunThenWaitIfModeZero_1(1, 0, 10);
    BattleFx_SpawnLinked_2(8, 0x102, 60);
    Motion_SetVarCb_1(8, 2);
    Battle_RunThenWaitIfModeZero_2(8, 0, 10);
    Motion_SetVarCb_2(0, 2);
    Motion_SetVarCb_3(1, 2);
    Motion_SetVarCb_4(5, 2);
    Battle_WaitMode0_8(0, 0x102);
    Battle_WaitMode0_9(1, 0x102); /* main:0808a1f0 */
    Battle_WaitMode0_10(5, 0x102); /* main:0808a1f0 */
    Battle_WaitMode0_11(40);
    Motion_SetVarCbObj_3(8, 2);
    Battle_WaitMode0_12(20);
    BattleEv_RunWait_1(8, 0);
    Motion_CallWaitAnim_2(8, 4);
    Battle_RunThenWaitIfModeZero_3(8, 0, 10);
    Object_LinkPair_1(0, 5, 0);
    Battle_WaitMode0_13(40);
    Motion_SetVarCb_5(0, 1);
    Motion_SetVarCbObj_4(5, 1);
    Battle_WaitMode0_14(10);
    Motion_ArmCb_21(0, 0xc000, 0);
    Motion_ArmCb_22(5, 0xa000, 20);
    Motion_SetVarCbObj_5(5, 2);
    Battle_RunThenWaitIfModeZero_4(5, 0, 10);
    Motion_CallWaitAnim_3(8, 4);
    Battle_RunThenWaitIfModeZero_5(8, 0, 10);
    Motion_ArmCb_23(8, 0x3000, 10);
    Battle_RunThenWaitIfModeZero_6(8, 0, 40);
    Motion_ArmCb_24(8, 0xc000, 20);
    Motion_SetVarCbObj_6(8, 1);
    Battle_WaitMode0_15(10);
    Battle_WaitMode0_16(8, 0x102); /* main:0808a1f0 */
    Motion_ArmCb_25(8, 0x8000, 20);
    Motion_ArmCb_26(8, 0, 20);
    Motion_ArmCb_27(8, 0xc000, 60);
    Motion_SetVarCbObj_7(8, 2);
    Battle_WaitMode0_17(10);
    Battle_RunThenWaitIfModeZero_7(8, 0, 10);
    Motion_ArmCb_28(8, 0x4000, 20);
    Motion_Launch_1(8, 2, 20);

    Battle_RunThenWaitIfModeZero_8(8, 0, 40);
    Motion_ArmCb_29(1, 0, 20);
    Motion_SetSpeed_5(1, 0); /* main:0808a178 */
    if (UiWork_WaitThenFinalizeCapacity_1(0, 0) == 0) {
        /* Passes the address of Value_00000fe0 in place of a record pointer. */
        SceneWork_SetStepValue_2((s32)&Value_00000fe0);
        Motion_SetVarCbObj_8(1, 1);
        Battle_RunThenWaitIfModeZero_9(1, 0, 10);
    } else {
        SceneWork_SetStepValue_3(0xfe1);
        Motion_ArmCb_30(5, 0x8000, 20);
        Battle_RunThenWaitIfModeZero_10(5, 0, 10);
        Motion_ArmCb_31(0, 0xc000, 0);
        Motion_ArmCb_32(1, 0xe000, 0);
        Motion_ArmCb_33(5, 0xa000, 60);
        BattleFx_SpawnLinked_3(0, 0x102, 40);
        Motion_SetVarCbObj_9(1, 1);
        Battle_WaitMode0_18(10);
        Motion_CallWaitAnim_4(1, 3);
        Battle_WaitMode0_19(10);
        Motion_ArmCb_34(1, 0, 0);
        Motion_ArmCb_35(5, 0x8000, 30);
        Motion_SetVarCbObj_10(1, 1);
        Battle_WaitMode0_20(10);
        Battle_RunThenWaitIfModeZero_11(1, 0, 10);
    }

    Motion_SetSpeed_6(8, 0x9999, 0x4ccc);
    Object_SetModeById_13(8, 2);
    Motion_OffsetPositionAndResetMotion_6(8, 0, 48);
    Motion_CommitPos_4(8);
    Object_SetModeById_14(8, 1);
    Battle_WaitMode0_21(6);
    Motion_ArmCb_36(1, 0xe000, 0);
    Motion_ArmCb_37(5, 0xa000, 0);
    Motion_CommitPos_5(8);
    Object_SetModeById_15(8, 1);
    Battle_WaitMode0_22(20);
    Object_SetModeById_16(1, 3);
    Object_SetModeById_17(5, 3);
    Motion_CallWaitAnim_5(0, 3);
    Battle_WaitMode0_23(6);
    Object_SetModeById_18(1, 2);

    record = Scene_GetRecord_4(0);
    if (record != 0)
        Motion_ResetAndSetPosition_1(1, RECORD_A16(record), RECORD_B16(record));
    Object_SetModeById_19(5, 2);
    record = Scene_GetRecord_5(0);
    if (record != 0)
        Motion_ResetAndSetPosition_2(5, RECORD_A16(record), RECORD_B16(record));
    Object_SetModeById_20(8, 2);
    record = Scene_GetRecord_6(0);
    if (record != 0)
        Motion_ResetAndSetPosition_3(8, RECORD_A16(record), RECORD_B16(record));

    Motion_CommitPos_6(8);
    Motion_SetHPosTerrain_4(1, 0, 0);
    Motion_SetHPosTerrain_5(5, 0, 0);
    Motion_SetHPosTerrain_6(8, 0, 0);
    Object_SetModeById_21(8, 1);
    Object_SetModeById_22(1, 1);
    Object_SetModeById_23(5, 1);
    GameFlag_Set_1(0x804);
    GameFlag_Clear_1(0x12f);
    Battle_SchedShoulder_1();
}

/* overlays/scene/story/scenario_dispatch/scenario_dispatch_scene_head.c */

extern u8 Value_00000013;
extern u8 Value_00000010;
extern u8 gOv[];
extern u8 gOv2[];
extern u8 gOv3[];
extern u8 gVal[];
extern u8 gVal2[];
extern u8 gOv4[];
extern u8 gOv5[];
extern u8 gOv6[];
extern u8 gOv7[];
extern u8 gOv8[];
extern s16 gCell[][1];
extern void Story_Run8(void);  /* Story_Run9 veneer. */
extern s32 Story_Run10();       /* GameFlag_IsSet veneer, first site. */
extern void Story_Run11();      /* UiText_DrawMessage veneer, first site. */
extern void Story_Run12();      /* UiText_DrawMessage veneer, second site. */
extern s32 Story_Run13();       /* GameFlag_IsSet veneer, second site. */
extern void Story_Run14(void);  /* Story_Run15 veneer. */

/*
 * Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds, not a runtime address.
 */

/*
 * Resource 37f owner at 0x020003bc: 76 bytes of code plus a five-word literal
 * pool at 0x0200040c-0x0200041f.  The interworking return pops the saved
 * return address into r0, so the owner returns void.  All seven calls go
 * through import veneers, and GameFlag_IsSet is used only as a predicate.  r5
 * holds the workspace pointer, loaded before the message call and read after
 * it; that is why the owner saves r5.
 */

/*
 * These wrappers pass their constants straight into the argument registers.
 * A direct call precomputes a costly constant into a pseudo that is then
 * shared with later uses in the block.  A value-returning call also sets r0
 * last of its arguments.
 */

/*
 * These wrappers pass their constants straight into the argument registers.
 * A direct call precomputes a costly constant into a pseudo that is then
 * shared with later uses in the block.  A value-returning call also sets r0
 * last of its arguments.
 */

s32 SceneData_SelectOverlayDataBySelector(void)
{
    extern s16 gCell2[];

    s16 v = gCell2[224];

    if (v == (s32)&Value_00000013) {
        return (s32)gOv;
    }
    if (v == (s32)&Value_00000010) {
        return (s32)gOv2;
    }
    return (s32)gOv3;
}

/*
 * Return the table at 0x02009f14.  The eight-byte owner includes its one
 * pool word.
 */
u8 *SceneData_GetTable9F14(void)
{
    return (u8 *)0x02009f14;
}

s32 Story_Run16(void)
{
    extern u8 gCell2[];
    extern u8 gWork[];

    s32 base5_2009fd8;
    s32 lo = 11;

    if (gCell[224][0] == (s32)gVal) {
        if (gCell[225][0] >= lo) {
            if (gCell[225][0] > 13) {
                if (gCell[225][0] > 16) {
                    goto L_020000ae;
                }
                return (s32)gOv7;
            }
            return (s32)gOv6;
        }
        L_020000ae:;
        base5_2009fd8 = (s32)gOv5;
        Story_Run17(base5_2009fd8);
        return base5_2009fd8;
    } else {
        if (gCell[224][0] == (s32)gVal2) {
            return (s32)gOv8;
        }
    }
    return (s32)gOv4;
}

void Scene_RunGateSequenceA(void)
{
    extern u8 gWork[];

    u32 i;
    s32 record;
    s32 v5;
    s32 v6;

    Story_Run18();
    v5 = 3;
    v6 = 2;
    Story_Run19(181);
    Story_SetRect3(16, 28, 21, 3, v5, v6);
    Story_Run20(10);
    Story_SetRect4(16, 30, 21, 3, v5, v6);
    Story_Run21(10);
    Story_SetRect5(16, 32, 21, 3, v5, v6);
    Story_Run22(10);
    Story_Run23(0, 2);
    Story_Place4(0, 0x9999, 0x4ccc);
    Story_Run24(0, 120, 98);
    Story_Run25(0, 2);
    Story_Place5(0, 0, -8);
    Story_Run26(10);
    Story_Run27();
    Story_Run28();
    Story_Run29(2);
    Story_Run30();
}

void Dialogue_RunFlag81aMessageBranch(void)
{
    extern u8 *gWork;

    Story_Run8();

    if (Story_Run10(0x81a) != 0) {
        Story_Run11(0x1034, 1);
    } else {
        Story_Run12(0x1031, 1);
        if (Story_Run13(0xf01) != 0) {
            u16 *p = (u16 *)(gWork + 370);
            u16 val = 1;
            *p = val;
        }
    }

    Story_Run14();
}

void Scene_RunOpeningAuxiliarySequence(void)
{
    extern u8 gWork[];

    s32 id;
    s32 v5;
    s32 v6;

    if (Story_Check(0xf01) == 0) {
    } else {
        if (Story_Check2(0x81a) != 0) {
        } else {
            Story_Run31();
            Story_Run32();
            v5 = 1;
            Story_Run33(182);
            Story_SetRect6(0, 70, 30, 42, v5, v5);
            Story_Run34();
            Story_Run35(40);
            id = 0x1032;
            Story_Run4(id, 1);
            Story_Run36(20);
            v6 = 3;
            Story_Run37(183);
            Story_SetRect7(0, 29, 3, 1, v6, 2);
            Story_SetRect8(0, 29, 3, 2, v6, v5);
            Story_SetRect9(1, 109, 4, 81, v5, v5);
            Story_Run38();
            Story_Place6(0x10000, 0x10000, 0x10000);
            Story_Run39(20);
            Story_Place7(0, 0x100, 0);
            Story_Place8(0x20000, 0x20000, 0x10000);
            Story_Run40(20);
            Story_Place9(0, 0x4000, 40);
            Story_Place10(0, 0x8000, 20);
            Story_Run41(0, 0, 20);
            Story_Place11(0, 0x4000, 10);
            Story_Run42(0, 4, 20);
            Story_Run43(0, 6, 40);
            Story_Place12(-1, -1, 0xe666);
            Story_Run44(40);
            Story_Run5(id + 1, 1);
            Story_Do(0x143);
            Story_Do2(0x81a);
            Story_Run45();
        }
    }
}

void Scene_RunFlag821Dialogue(void)
{
    extern u8 *gWork;

    u8 *work;

    Story_Run46();

    if (Story_Check3(0x821) != 0) {
        Story_Run47(0x1034, 1);
    } else if (Story_Check4(0xf02) != 0) {
        work = gWork;
        Story_Run48(0x1031, 1);
        {
            /*
             * The halfword store goes through a pointer local and then an
             * s32 value local, in that order.  Storing the literal straight
             * into the halfword builds the constant in HImode and fetches it
             * from the literal pool, which costs a pool word; splitting the
             * address out first also fixes which register holds it.
             */
            u16 *frame = (u16 *)(work + 370);
            s32 one = 1;
            *frame = (u16)one;
        }
    } else {
        Story_Run49(0x1031, 1);
    }

    Story_Run50();
}
