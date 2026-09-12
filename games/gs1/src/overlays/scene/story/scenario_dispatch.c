#include "types.h"
#include "scene.h"

/* overlays/scene/story/scenario_dispatch/scenario_dispatch.c */
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
            FunctionHead_02001236();
            Audio_PlayCue_1(182);
            Story_SetRect(0, 71, 100, 71, 1, 1);
            FunctionHead_0200133a();
            Battle_WaitMode0_1(40);
            /* Address of the byte pair starting at Value_00001032. */
            byte_pair_addr = (s32)&Value_00001032;
            UiText_DrawMessage_1(byte_pair_addr, 1);
            Battle_WaitMode0_2(20);
            Audio_PlayCue_2(183);
            Story_unk6_4(122, 20, 120, 30, 1, 2);
            Story_unk2_5(122, 20, 1, 2, 120, 30);
            Story_unk3_4();
            Story_Place(65536, 65536, 65536);
            Battle_WaitMode0_3(20);
            BattleFx_SpawnLinked_1(0, 256, 0);
            Story_unk2_3(131072, 131072, 65536);
            Battle_WaitMode0_4(20);
            Motion_ArmCb_1(0, 16384, 40);
            Motion_ArmCb_2(0, 32768, 20);
            Motion_ArmCb_3(0, 0, 20);
            Motion_ArmCb_4(0, 16384, 10);
            Motion_Launch_1(0, 4, 20);
            Motion_Launch_2(0, 6, 40);
            Story_unk3_3(-1, -1, 58982);
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

u8 *Scene_GetRecord();

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
extern void Story_unk8_4(void);  /* Story_unk9_4 veneer. */
extern s32 Story_unk10_4();       /* GameFlag_IsSet veneer, first site. */
extern void Story_unk11_4();      /* UiText_DrawMessage veneer, first site. */
extern void Story_unk12_4();      /* UiText_DrawMessage veneer, second site. */
extern s32 Story_unk13_4();       /* GameFlag_IsSet veneer, second site. */
extern void Story_unk14_4(void);  /* BattleFx_FinishAction veneer. */

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

s32 Scene_RunSupplementalSequenceOne(void)
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
        Story_unk17_4(base5_2009fd8);
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

    Story_unk18_4();
    v5 = 3;
    v6 = 2;
    Story_unk19_4(181);
    Story_unk3_5(16, 28, 21, 3, v5, v6);
    Story_unk20_4(10);
    Story_unk4_5(16, 30, 21, 3, v5, v6);
    Story_unk21_4(10);
    Story_unk5_5(16, 32, 21, 3, v5, v6);
    Story_unk22_4(10);
    Story_unk23_4(0, 2);
    Story_unk4_3(0, 0x9999, 0x4ccc);
    Story_unk24_4(0, 120, 98);
    Story_unk25_4(0, 2);
    Story_unk5_3(0, 0, -8);
    Story_unk26_4(10);
    Story_unk27_4();
    Story_unk28_4();
    Story_unk29_4(2);
    Story_unk30_4();
}

void Dialogue_RunFlag81aMessageBranch(void)
{
    extern u8 *gWork;

    Story_unk8_4();

    if (Story_unk10_4(0x81a) != 0) {
        Story_unk11_4(0x1034, 1);
    } else {
        Story_unk12_4(0x1031, 1);
        if (Story_unk13_4(0xf01) != 0) {
            u16 *p = (u16 *)(gWork + 370);
            u16 val = 1;
            *p = val;
        }
    }

    Story_unk14_4();
}

void Scene_RunOpeningAuxiliarySequence(void)
{
    extern u8 gWork[];

    s32 id;
    s32 v5;
    s32 v6;

    if (Story_Check(0xf01) == 0) {
    } else {
        if (Story_unk2(0x81a) != 0) {
        } else {
            Story_unk31_4();
            Story_unk32_4();
            v5 = 1;
            Story_unk33_4(182);
            Story_unk6_5(0, 70, 30, 42, v5, v5);
            Story_unk34_4();
            Story_unk35_4(40);
            id = 0x1032;
            Story_unk4_4(id, 1);
            Story_unk36_4(20);
            v6 = 3;
            Story_unk37_4(183);
            Story_unk7_5(0, 29, 3, 1, v6, 2);
            Story_unk8_5(0, 29, 3, 2, v6, v5);
            Story_unk9_5(1, 109, 4, 81, v5, v5);
            Story_unk38_4();
            Story_unk6_3(0x10000, 0x10000, 0x10000);
            Story_unk39_4(20);
            Story_unk7_3(0, 0x100, 0);
            Story_unk8_3(0x20000, 0x20000, 0x10000);
            Story_unk40_4(20);
            Story_unk9_3(0, 0x4000, 40);
            Story_unk10_3(0, 0x8000, 20);
            Story_unk41_4(0, 0, 20);
            Story_unk11_3(0, 0x4000, 10);
            Story_unk42_4(0, 4, 20);
            Story_unk43_4(0, 6, 40);
            Story_unk12_3(-1, -1, 0xe666);
            Story_unk44_4(40);
            Story_unk5_4(id + 1, 1);
            Story_Do(0x143);
            Story_unk2_2(0x81a);
            Story_unk45_4();
        }
    }
}

void Scene_RunFlag821Dialogue(void)
{
    extern u8 *gWork;

    u8 *work;

    Story_unk46_4();

    if (Story_unk3(0x821) != 0) {
        Story_unk47_4(0x1034, 1);
    } else if (Story_unk4(0xf02) != 0) {
        work = gWork;
        Story_unk48_4(0x1031, 1);
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
        Story_unk49_4(0x1031, 1);
    }

    Story_unk50_4();
}

/* overlays/scene/story/scenario_dispatch/actor_find_slot_by_tile_position.c */
/* overlays/scene/story/scenario_dispatch/find_slot_by_tile_position.c */
/* Contiguous unnamed state-owner run for resource_37f. */


extern s32 *FunctionHead_02001236(s32);

    s32 dir;

    if (rec == 0) {
        return;
    }
    dir = rec[2] >> 20;
    Story_Do(0x302);
    Story_unk2_2(0x303);
    if (dir == 93) {
        Story_unk3_2(0x303);
    } else if (dir == 95) {
        Story_unk4_2(0x302);
    }
}


extern s32 *FunctionHead_0200133a(s32);

    s32 dir;

    if (rec == 0) {
        return;
    }
    dir = rec[2] >> 20;
    Story_unk5_2(0x300);
    Story_unk6_2(0x301);
    if (dir == 115) {
        Story_unk7_2(0x300);
    } else if (dir == 113) {
        Story_unk8_2(0x301);
    }
}


extern s32 *Story_unk3_4(s32);

    s32 dir;

    if (rec == 0) {
        return;
    }
    dir = rec[2] >> 20;
    Story_unk9_2(0x310);
    Story_unk10_2(0x311);
    if (dir == 99) {
        Story_unk11_2(0x311);
    } else if (dir == 101) {
        Story_unk12_2(0x310);
    }
    Story_unk13_2(0);
}


extern s32 *Story_unk4_4(s32);

    s32 dir;

    if (rec == 0) {
        return;
    }
    dir = rec[2] >> 20;
    Story_unk14_2(0x312);
    Story_unk15_2(0x313);
    if (dir == 103) {
        Story_unk16_2(0x313);
    } else if (dir == 105) {
        Story_unk17_2(0x312);
    }
    Story_unk18_2(0);
}


extern s32 *Story_unk5_4(s32);

    s32 dir;

    if (rec == 0) {
        return;
    }
    dir = rec[2] >> 20;
    Story_unk19_2(0x314);
    Story_unk20_2(0x315);
    if (dir == 107) {
        Story_unk21_2(0x315);
    } else if (dir == 109) {
        Story_unk22_2(0x314);
    }
    Story_unk23_2(0);
}


extern s32 *Story_unk6_4ByTilePosition(s32);

    s32 dir;

    if (rec == 0) {
        return;
    }
    dir = rec[2] >> 20;
    Story_unk24_2(0x316);
    Story_unk25_2(0x317);
    if (dir == 111) {
        Story_unk26_2(0x317);
    } else if (dir == 113) {
        Story_unk27_2(0x316);
    }
    Story_unk28_2(0);
}


extern s32 *Story_unk7_4ByTilePosition(s32);

    s32 dir;

    if (rec == 0) {
        return;
    }
    dir = rec[2] >> 20;
    Story_unk29_2(0x318);
    Story_unk30_2(0x319);
    if (dir == 115) {
        Story_unk31_2(0x319);
    } else if (dir == 117) {
        Story_unk32_2(0x318);
    }
    Story_unk33_2(0);
}


extern s32 *Story_unk8_4ByTilePosition(s32);

    s32 dir;

    if (rec == 0) return;
    dir = rec[2] >> 20;
    Story_unk34_2(0x31a);
    Story_unk35_2(0x31b);
    if (dir == 119) {
        Story_unk36_2(0x31b);
    } else if (dir == 121) {
        Story_unk37_2(0x31a);
    }
    Story_unk38_2(0);
}


extern u8 gWork[];

s32 *Actor_FindSlotByTilePosition(s32 x, s32 z)
{
    s32 **slots = (s32 **)(gWork + 0x14);
    u32 i;

    for (i = 8; i <= 65; i++) {
        s32 *p = slots[i];

        if (x == (p[2] >> 20) && z == (p[4] >> 20)) {
            return p;
        }
    }
    return 0;
}

/* overlays/scene/story/scenario_dispatch/dispatch_by_scenario_id.c */
extern s16 gCellByTilePosition[];

s32 Scene_DispatchByScenarioId(void)
{
    s32 scenario = gCellByTilePosition[224];

    if (scenario == (s32)&Value_00000013) {
        FunctionHead_02001236();
    } else if (scenario == (s32)&Value_00000010) {
        FunctionHead_0200133a();
    }
    return 0;
}

/* overlays/scene/script/gate_open_step.c */
extern u8 gOvGateOpenStep[];

void Scene_RunGateGateOpenStep(void)
{
    Script_Do(0x144);
    *(s32 *)((*(u8 *volatile *)gWork + 0x1c0)) = 0x100;
    if (Script_Check(0x814) != 0) {
        s32 zero = 0;
        *(volatile s32 *)gOvGateOpenStep = zero;
        Script_Run(0x2009ac9, 0xc80);
    }
    if (Script_unk2(0x879) != 0) {
        Script_SetRect(5, 6, 1, 1, 6, 6);
        Script_unk2_5(5, 6, 1, 1, 7, 6);
        Script_unk3_5(5, 6, 1, 1, 8, 6);
        Script_unk4_5(0, 1, 3, 1, 6, 5);
    }
    if (Script_unk3(0x815) != 0) {
        Script_Place(8, 0x780000, 0xe80000);
        Script_unk5_4(2, 10, 1, 1, 6, 14);
        Script_unk6_4(2, 10, 1, 1, 7, 14);
        Script_unk7_4(2, 10, 1, 1, 8, 14);
    }
}

/* overlays/scene/script/gate_close_step.c */

void Scene_RunGateGateCloseStep(void)
{
    u32 i;
    s32 record;

    Script_unk6_3();
    *(s32 *)((*(u8 *volatile *)gWork + 0x1c0)) = 0x100;
    Script_unk7_3();
    ((void (*)())Script_unk7)(0, 0);
    Script_unk8_2(4);
    Script_Run(-1, -1, -1, 0);
    Script_unk2_4(0x9999, 0x1333);
    Script_unk3_4(0x4c80000, -1, 0x880000, 1);
    record = Script_Check(0);
    if (record != 0) {
        Script_unk9_2(8, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
    }
    record = Script_unk2(0);
    if (record != 0) {
        Script_unk10_2(5, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
    }
    record = Script_unk3(0);
    if (record != 0) {
        Script_unk11_2(1, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
    }
    Script_Place(8, 0x9999, 0x4ccc);
    Script_unk2_3(5, 0x9999, 0x4ccc);
    Script_unk3_3(1, 0x9999, 0x4ccc);
    Script_unk12_2(1, 2);
    Script_unk13_2(5, 2);
    Script_unk14_2(8, 2);
    Script_unk4_3(1, -16, 0);
    Script_unk15(5, 16, 0);
    Script_unk5_2(8, 0, -32);
    Script_unk16(1);
    Script_unk17(1, 0);
    Script_unk18(5, 0);
    Script_unk6_2(1, 0xc000, 0);
    Script_unk7_2(5, 0xc000, 0);
    Script_unk19(8);
    Script_unk20(8, 1);
    Script_unk21(40);
    Script_unk22(8, 2);
    Script_unk23(20);
    Script_unk8(8, 0x3000, 40);
    Script_unk9(8, 0x5000, 40);
    Script_unk10(8, 0x3000, 20);
    Script_unk24(8, 4, 20);
    Script_Do(0xfd3);
    Script_unk4_4(0x4008, 0);
    Script_unk25(20);
    Script_unk5_3(0x4c80000, -1, 0x940000, 1);
    Script_unk26(1, 2);
    record = Script_unk4(0);
    if (record != 0) {
        Script_unk27(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Script_unk28(5, 2);
    record = Script_unk5(0);
    if (record != 0) {
        Script_unk29(5, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Script_unk30(8, 2);
    record = Script_unk6(0);
    if (record != 0) {
        Script_unk31(8, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Script_unk32(1);
    Script_unk33(1, 0, 0);
    Script_unk34(5, 0, 0);
    Script_unk35(8);
    Script_unk36(8, 0, 0);
    Script_unk37(1, 1);
    Script_unk38(5, 1);
    Script_unk39(8, 1);
    Script_unk2_2(0x802);
    *(s32 *)((*(u8 *volatile *)gWork + 0x1c0)) = 0x204;
    Script_unk3_2(0x12f);
    Script_unk40();
}
