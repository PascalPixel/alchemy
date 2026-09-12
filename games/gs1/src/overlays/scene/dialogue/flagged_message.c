#include "types.h"
#include "scene.h"
#include "select_overlay_data_by_runtime_selector.h"
#include "select_overlay_data_by_runtime_selector_body.inc"

/* overlays/scene/dialogue/flagged_message/actor_presentation.c */
u8 *Talk_unk6_4(s32);
u8 *Talk_unk7_4(s32);

void Actor_SetMode3AndFlagBit1(s32 no)
{
    u8 *p = Talk_unk6_4(no);
    u8 *flag;

    Talk_Apply(Talk_unk7_4(no), 0);
    Talk_Apply2(no, 3);
    flag = p + 85;
    *flag = 0;
    p += 35;
    {
        u8 bit = 2;

        *p = bit | *p;
    }
}

/* overlays/scene/dialogue/flagged_message/scene_data.c */
#define PrimaryRuntimeSelector Value_0000006b
#define SecondaryRuntimeSelector Value_00000070
#define TertiaryRuntimeSelector Value_0000006c

extern s16 RuntimeSelectorTable[];
extern u8 Value_0000006b;
extern u8 Value_00000070;
extern u8 Value_0000006c;
extern u8 gOv[];
extern u8 gOv2[];
extern u8 gOv3[];
extern u8 gOv4[];
extern u8 gOv5[];
extern u8 gOv6[];
extern u8 gOv7[];
extern u8 gOv8[];
extern u8 gOv9[];
extern u8 gOv10[];
extern u8 gOv11[];
extern u8 gOv12[];
extern u8 gOv13[];
extern u8 gOv14[];
extern u8 gOv15[];
extern u8 gOv16[];

/*
 * Data selector for resource_3ae.  The 164-byte owner includes its 13-word
 * literal pool.
 */

/* Per-call-site import labels.  All four sites reach the same event-flag test,
 * but each keeps its own name because the relocated call word differs. */

/*
 * All four symbols name the loader-relocated call word each site holds; they
 * reach one event-flag test.
 */

s32 SceneData_SelectByRuntimeSelector(void)
{
}

/*
 * Table getter for resource_3ae. The owner at 0x02000098 is eight bytes and
 * includes its one pool word at 0x0200009c: the pc-relative load reads that
 * word, so the word is part of the owner and not a following gap.
 *
 * The pool word is an address that is returned without being dereferenced.
 * Many getters share this body but each returns a different address, so the
 * constant here is not interchangeable with theirs.
 */
u8 *SceneData_GetTable98a0(void)
{
    return (u8 *)0x020098a0;
}

/*
 * The selector is the signed halfword at byte offset 448 of the resident
 * workspace, RuntimeSelectorTable[224].  It is compared as a signed 32-bit value
 * against full words held in the pool, which is why the three constants are
 * spelled as the addresses of Value_ symbols rather than as immediates.
 * 0x93e and 0x950 are read as event-flag ids only from their use as the sole
 * argument of the flag test.
 */
s32 SceneData_SelectDataBySelectorAndFlags(void)
{
    s16 room = RuntimeSelectorTable[224];

    if (room == (s32)&Value_0000006b) {
        if (Talk_unk26(0x93e) != 0) {
            return (s32)gOv5;
        }
        return (s32)gOv4;
    }

    if (room == (s32)&Value_00000070) {
        /* 0x950 is built from an immediate and a shift. */
        if (Talk_unk27(0x950) != 0) {
            return (s32)gOv3;
        }
        return (s32)gOv2;
    }

    if (room == (s32)&Value_0000006c) {
        if (Talk_unk28(0x950) != 0) {
            return (s32)gOv8;
        }
        if (Talk_unk29(0x93e) != 0) {
            return (s32)gOv7;
        }
        return (s32)gOv6;
    }

    return (s32)gOv;
}

/*
 * Pick one of eight in-image data tables from the scene selector and two
 * event flags. The selector is the signed halfword at offset 448 of the
 * shared workspace, and each arm compares it as a signed 32-bit value
 * against a full word, which is why the selector constants are spelled as
 * addresses. 0x93e and 0x950 are taken to be event-flag ids only from their
 * use here. The 164-byte owner includes its thirteen-word literal pool.
 */
s32 SceneData_SelectSecondaryDataBySelectorAndFlags(void)
{
    s16 scene = RuntimeSelectorTable[224];

    if (scene == (s32)&Value_0000006b) {
        if (Talk_unk30(0x93e) != 0) {
            return (s32)gOv13;
        }
        return (s32)gOv12;
    }

    if (scene == (s32)&Value_00000070) {
        /* 0x950 is built by shifting a small constant, not loaded. */
        if (Talk_unk31(0x950) != 0) {
            return (s32)gOv11;
        }
        return (s32)gOv10;
    }

    if (scene == (s32)&Value_0000006c) {
        if (Talk_unk32(0x950) != 0) {
            return (s32)gOv16;
        }
        if (Talk_unk33(0x93e) != 0) {
            return (s32)gOv15;
        }
        return (s32)gOv14;
    }

    return (s32)gOv9;
}

/* overlays/scene/dialogue/flagged_message/scene_dialogue.c */

extern u8 gVal[];
extern u8 gWork[];

u8 *Talk_unk8_4();

u8 *Talk_unk9_4();

/* Contiguous unnamed leaf-owner run for resource_3ae. */

void Dialogue_ShowLine1CF8(void)
{
    Talk_unk10_4();
    Talk_unk23_2(0x1CF8);
    Talk_Apply3(8, 0);
    Talk_unk11_4();
}

void Dialogue_RunActor12Event(void)
{
    s32 rec7;
    s32 rec8;
    u8 *record;
    s16 angle;

    record = Talk_unk8_4(0);
    angle = (u16)((*(u16 *)(record + 6) + 0x2000) & ~0x3fff);
    Talk_unk12_4();
    if (Talk_Check(0x8a7) != 0) {
        if (Talk_unk2(0x8a9) != 0) {
            Talk_Do(0x1d23);
            Talk_unk13_4(12, 0);
            goto L_02000496;
        }
        Talk_unk14_4((s32)gVal);
        Talk_unk3(12, 0);
        if (Talk_unk4(0, 0) == 0) {
            Talk_unk15_4(10);
            Talk_unk16_4(((s32)gVal + 1));
            Talk_unk17_4(12, 0);
            Talk_Place(12, 88, 0x508);
            Talk_unk2_3(12, 0x4000, 0);
            Talk_unk18_4(20);
            Talk_unk2_2(0x8a9);
            goto L_02000496;
        }
        Talk_unk19_4(((s32)gVal + 2));
        Talk_unk20_4(12, 0);
    } else {
        if ((u16)angle != 0x8000) {
            goto L_0200049a;
        }
        Talk_unk3_2(0x1d16);
        Talk_unk21_4(12, 0);
        if (Talk_unk5(0x8a5) != 0) {
            rec7 = Talk_unk34(235);
            rec8 = Talk_unk35(rec7, 235);
            Talk_unk22_4(12, 3);
            Talk_unk3_3(12, 88, 0x508);
            Talk_unk4_3(12, 0x4000, 0);
            bump_step(1);
            Talk_unk23_4(12, 0);
            Talk_unk24_4(rec7, rec8);
            Talk_unk4_2(0x8a7);
            record = Talk_unk9_4(0);
            Talk_unk5_3(0, *(s16 *)(record + 10), 0x518);
            Talk_unk6_3(0, 72, 0x518);
            Talk_unk7_3(12, 88, 0x518);
            Talk_unk25_4(12, 0, 0);
        } else {
            Talk_unk26_4(12, 0);
        }
    }
    L_02000496:;
    Talk_unk27_4();
    L_0200049a:;
}

void Dialogue_RunActor8Message1f09(void)
{
    Talk_unk28_4();
    Talk_unk24_2(0x1F09);
    Talk_Apply4(8, 0);
    Talk_unk29_4();
}

void Dialogue_RunActor10Message1f15(void)
{
    Talk_unk30_4();
    Talk_unk25_2(0x1F15);
    Talk_Apply5(10, 0);
    Talk_unk31_4();
}

/* overlays/scene/dialogue/flagged_message/scene_primary_script.c */

extern u8 gWork[];
extern u8 RuntimeSelectorTable[];
extern u8 gVal2[];
extern u8 gVal3[];
extern u8 gVal4[];
extern s16 gCell[][1];

u8 *Talk_unk32_4();

u8 *Talk_unk33_4();

void Scene_RunScene3ae(void)
{
    u32 i;
    s32 record;

    Talk_unk34_4();
    Talk_unk5_2(0x8aa);
    Talk_unk8_3(0, 0x188, 0x128);
    Talk_unk9_3(8, 0x13333, 0x9999);
    Talk_unk10_3(8, 0x198, 0x128);
    Talk_unk11_3(8, 0x8000, 0);
    Talk_unk35_4(20);
    Talk_unk36_4();
}

void Scene_RunScene3ae(void)
{
    u32 i;
    s32 record;

    Talk_unk37_4();
    if (Talk_unk6(0x8a6) == 0) {
        Talk_unk6_2(0x1cfd);
        Talk_unk7(11, 0);
        if (Talk_unk8(0, 0) == 0) {
            Talk_unk38_4(11, 0);
            Talk_unk7_2(0x8a6);
            goto L_020002c2;
        }
        bump_step(1);
        Talk_unk39_4(11, 0);
    } else {
        Talk_unk8_2(0x1cfe);
        Talk_unk40_4(11, 0);
    }
    L_020002c2:;
    Talk_unk41_4();
}

void Scene_RunScene3ae(void)
{
    u32 i;
    s32 record;

    Talk_unk42_4(0);
    Talk_unk43_4();
    if (Talk_unk9(0x8a7) != 0) {
        if (Talk_unk10(0x8a9) != 0) {
            Talk_unk9_2(0x1d23);
            Talk_unk11(12, 0);
            Talk_unk12_3(12, 0x4000, 0);
        }
    }
}

void Scene_RunOpeningAuxiliarySequence(void)
{
    u32 i;
    s32 rec7;
    s32 record;
    s32 base6_2000240;
    s32 six00;

    six00 = 0x258;
    Talk_unk44_4();
    if (Talk_unk12(0x8a5) != 0) {
        Talk_unk10_2(0x1d0b);
        Talk_unk45_4(8, 0);
    } else {
        Talk_unk11_2(0x1d04);
        Talk_unk13(8, 0);
        if (Talk_unk14(0, 0) == 1) {
            Talk_unk46_4(8, 0, 10);
        } else {
            bump_step(1);
            Talk_unk47_4(six00, 5);
            Talk_unk15(8, 0);
            rec7 = Talk_unk36(19, 8, 11, 4, 2);
            Talk_Run(0xc8a, rec7, 0, 0);
            base6_2000240 = (s32)RuntimeSelectorTable;
            Talk_unk48_4(*(volatile s32 *)(base6_2000240 + 16), 6, rec7, 24, 8);
            if (Talk_unk16(-1, 0) == 1) {
                Talk_unk49_4(rec7, 2);
                Talk_unk37(0, 4);
                Talk_unk50_4(10);
                Talk_unk51_4(8, 0);
                goto L_02000660;
            } else {
                if ((u32)six00 <= (u32)*(volatile s32 *)(base6_2000240 + 16)) {
                    goto L_0200061e;
                }
                Talk_unk52_4(rec7, 2);
                Talk_unk38(0, 3);
                Talk_unk53_4(10);
                bump_step(1);
                Talk_unk54_4(113);
                Talk_unk51_4(8, 0);
                goto L_02000660;
            }
            L_0200061e:;
            Talk_unk55_4(rec7, 2);
            Talk_unk56_4(0, 3);
            Talk_unk57_4(10);
            bump_step(3);
            Talk_unk58_4(8, 0);
            Talk_unk39(235, 0);
            Talk_unk12_2(0x8a5);
            Talk_unk13_2(-six00);
        }
        L_02000660:;
        Talk_unk59_4();
    }
}

void Scene_RunScene3ae(void)
{
    u32 i;
    s32 record;

    Talk_unk60_4();
    if (Talk_unk17(0x8a8) != 0) {
        Talk_unk61_4(11, 0, 0);
        Talk_unk62_4(20);
        Talk_unk14_2(0x1f1c);
        Talk_unk63_4(11, 0);
        ((void (*)())Talk_unk40)();
    } else {
        Talk_unk64_4(20);
        Talk_unk13_3(11, 0x100, 50);
        Talk_unk65_4(11, 0, 0);
        Talk_unk66_4(20);
        Talk_unk15_2(0x1f18);
        Talk_unk67_4(11, 0);
        if (Talk_unk18(0x8a6) != 0) {
            Talk_unk68_4(20);
            Talk_unk14_3(11, 0x102, 40);
            Talk_unk19(11, 0);
            if (Talk_unk20(0, 0) == 0) {
                Talk_unk69_4(20);
                Talk_unk70_4(11, 0);
                Talk_unk16_2(0x8a8);
                goto L_020007be;
            }
            ((void (*)())Talk_unk41)(10);
            bump_step(1);
            Talk_unk71_4(11, 0);
            Talk_unk72_4(10);
            Talk_unk73_4(11, 0, 0);
            Talk_unk74_4(30);
        } else {
            Talk_unk75_4(10);
            Talk_unk76_4(11, 0, 0);
            Talk_unk77_3(30);
        }
        L_020007be:;
        Talk_unk78_3();
    }
}

void Scene_RunScene3ae(void)
{
    u8 *work;

    Talk_unk79_3();
    Talk_unk80_3(158);
    Talk_unk15_3(0, 0x8000, 0x4000);
    Talk_unk81_3(0, 3);
    work = RuntimeSelectorTable;
    if (*(s16 *)(work + 0x1c0) == (s32)gVal2) {
        Talk_unk16_3(0, 0x130, 0x570);
        Talk_unk17_3(0x20096b8, 78, 86);
    } else {
        if (*(s16 *)(work + 0x1c0) == (s32)gVal3) {
            Talk_unk82_3(0, 248, 192);
            Talk_unk18_3(0x20096ce, 74, 9);
        }
    }
    Talk_unk83_3(16);
    Talk_unk84_3(3);
    Talk_unk85_3();
}

void Scene_RunScene3ae(void)
{
    u32 i;
    s32 record;

    if (gCell[225][0] == 90) {
        Talk_unk17_2(0x950);
    }
    if (gCell[224][0] == (s32)gVal2) {
        Talk_unk86_3();
    } else {
        if (gCell[224][0] == (s32)gVal3) {
            Talk_unk87_3();
        } else {
            if (gCell[224][0] == (s32)gVal4) {
                Talk_unk88_3();
            }
        }
    }
    return 0;
}

void Scene_RunScene3ae(void)
{
    u32 i;
    s32 record;

    if (gCell[225][0] == 1) {
        if (Talk_unk21(0x8ac) == 0) {
            Talk_unk18_2(0x8ac);
            Talk_unk89_3();
        }
    }
    if (gCell[225][0] == 2) {
        if (Talk_unk22(0x109) == 0) {
            Talk_unk19_2(0x8a9);
        }
    }
    if (Talk_unk23(0x911) != 0) {
        if (Talk_unk24(0x8a9) == 0) {
            Talk_unk19_3(12, 0x580000, 0x5180000);
            Talk_unk90_3(12, 0, 0);
        }
    }
}

/* Runs a fixed sequence of scene calls (positioning, timing, and animation
 * triggers) for scene 3ae, sequence A. */
void Scene_RunScene3aeSequenceA(void)
{
    u32 i;
    s32 record;

    Battle_Reset_1();
    Talk_unk91_3();
    Motion_SetHPosTerrain_1(8, 0x1480000, 0x5900000);
    /* Set the flag byte at +91 of record 8. */
    *(u8 *)(Scene_GetRecord_1(8) + 91) = 1;
    Battle_WaitMode0_1();
    Motion_SetSpeed_1();
    Battle_WaitMode0_2(20);
    Motion_RealignToTrackedObjectAndArmCallback_1(1, -16, 0, 0x8000);
    Motion_CommitPos_1(1);
    Motion_ArmCb_1(0, 0xa000, 0);
    Battle_WaitMode0_3(20);
    SceneWork_SetStepValue_1(0x1f89);
    Motion_ArmCb_2(0, 0xa000, 0);
    Motion_SetSpeed_2(1, 0x19999, 0xcccc);
    Motion_SetPosReset_1(1, 232, 0x590);
    Motion_ArmCb_3(1, 0x8000, 0);
    Motion_CamBounds_1(0xb80000, -1, 0x5a00000, 1);
    Object_CommitPositionThenWaitIfModeZero_1();
    Battle_WaitMode0_4(10);
    Motion_Launch_1(1, 6, 15);
    Motion_Launch_2(1, 6, 40);
    BattleEv_RunWait_1(1, 0);
    Battle_WaitMode0_5(20);
    Motion_CamBounds_2(0x1080000, -1, 0x5a80000, 1);
    Object_CommitPositionThenWaitIfModeZero_2();
    Battle_WaitMode0_6(20);
    BattleFx_SpawnLinked_1(8, 0x100, 50);
    Motion_SetSpeed_3(8, 0x13333, 0x9999);
    Motion_SetPosReset_2(8, 0x108, 0x590);
    Motion_ArmCb_4(8, 0x8000, 0);
    Battle_WaitMode0_7(10);
    Motion_ArmCb_5(1, 0, 0);
    Battle_WaitMode0_8(20);
    Battle_WaitMode0_9(10);
    Motion_CallWaitAnim_1(8, 4);
    Battle_WaitMode0_10(10);
    BattleEv_RunWait_2(8, 0);
    Battle_WaitMode0_11(20);
    BattleFx_SpawnLinked_2(1, 0x102, 40);
    Battle_WaitMode0_12(30);
    Motion_ArmCb_6(1, 0x8000, 0);
    Battle_WaitMode0_13(50);
    BattleEv_RunWait_3(1, 0);
    Battle_WaitMode0_14(20);
    Motion_ArmCb_7(1, 0x2000, 0);
    Battle_WaitMode0_15(30);
    Motion_SetSpeed_4(1, 0x10000, 0x8000);
    Motion_SetPosReset_3(1, 0x108, 0x5b8);
    Motion_ArmCb_8(1, 0, 0);
    Motion_ArmCb_9(0, 0x8000, 0);
    Motion_ArmCb_10(8, 0x4000, 0);
    Battle_WaitMode0_16(30);
    BattleEv_RunWait_4(1, 0);
    Battle_WaitMode0_17(10);
    Motion_CallWaitAnim_2(0, 3);
    Battle_WaitMode0_18(30);
    Motion_CallWaitAnim_3(1, 3);
    Battle_WaitMode0_19(30);
    Motion_SetSpeed_5(1, 0x13333, 0x9999);
    Object_SetModeById_1(1, 2);
    /* If a record is returned, pass its s16 fields at +10 and +18 back in as
     * arguments. */
    record = Scene_GetRecord_2(0);
    if (record != 0) {
        Motion_ResetAndSetPosition_1(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Motion_CommitPos_2(1);
    Motion_SetHPosTerrain_2(1, 0, 0);
    /* Same routine as above, called directly instead of through the Value
     * wrapper. */
    Battle_WaitMode0_20(20);
    /* Clear the flag byte at +91 of record 8. */
    *(u8 *)(Scene_GetRecord_3(8) + 91) = 0;
    Motion_EnableActCb_1(8, 2);
    record = Scene_GetRecord_4(8);
    /* Store the integer part of the 16.16 fixed-point fields at +8 and +16
     * into the halfwords at +100 and +102. */
    {
        s32 shown = *(s32 *)(record + 8) / 0x10000;

        *(u16 *)(record + 100) = shown;
    }
    {
        s32 shown = *(s32 *)(record + 16) / 0x10000;

        *(u16 *)(record + 102) = shown;
    }
    Battle_SchedShoulder_1();
}

void Scene_RunScene3aeSequenceB(void)
{
    u32 i;
    u8 *record;
    s32 none;
    s32 v5;
    s32 v6;

    Talk_unk20_2(0x8ab);
    Talk_unk92_3();
    Talk_unk93_3();
    Talk_unk21_2(0x23eb);
    record = Talk_unk32_4(11);
    none = 0;
    record[35] = none;
    *(u8 *)(*(volatile s32 *)((s32)record + 80) + 9) |= 12;
    *(u8 *)(*(volatile s32 *)((s32)record + 80) + 21) |= 12;
    Talk_unk2_4(0xe80000, -1, 0x1300000, 1);
    Talk_unk20_3(0, 0x10000, 0x8000);
    Talk_unk21_3(0, 216, 0x110);
    Talk_unk22_3(0, 0x2000, 0);
    Talk_unk94_3();
    Talk_unk95_3(20);
    Talk_unk23_3(13, 0x102, 50);
    Talk_unk96_3(13, 0);
    Talk_unk97_3(10);
    Talk_unk24_3(10, 0x107, 50);
    Talk_unk98_3(10, 0, 0);
    Talk_unk99_3(10, 4, 13);
    Talk_unk100_3(10, 4, 30);
    Talk_unk101_3(10, 0);
    Talk_unk102_3(10);
    Talk_unk103_3(11, 2);
    Talk_unk104_3(20);
    Talk_unk105_3(11, 0);
    Talk_unk106_3(10);
    Talk_unk107_3(13, 4);
    Talk_unk108_3(20);
    Talk_unk3_4(13, 0);
    Talk_unk109_3(10);
    Talk_unk25_3(10, 0x103, 55);
    Talk_unk26_3(10, 0x20000, 0x10000);
    Talk_unk110_3(10, 16, 0);
    Talk_unk111_3(10, 7, 0);
    v5 = 254;
    Talk_unk112_3(10, 24, 0);
    *(u8 *)(Talk_unk42(10) + 90) &= v5;
    Talk_unk27_3(10, -16, 0);
    Talk_unk113_3(153);
    Talk_unk28_3(13, 0x26666, 0x13333);
    Talk_unk114_3(13, 16, 0);
    Talk_unk115_3(10);
    v6 = 1;
    Talk_unk116_3(10, 1);
    *(u8 *)(Talk_unk43(10) + 90) |= v6;
    Talk_unk4_4(13, 0x102);
    Talk_unk117_3(13, 2);
    Talk_unk118_3(155);
    Talk_unk22_2(10);
    Talk_unk119_3(155);
    Talk_unk120_3(10);
    Talk_unk121_3(155);
    Talk_unk122_3(10);
    Talk_unk123_3(20);
    Talk_unk29_3(13, 0x6666, 0x3333);
    Talk_unk124_3(13, 6, 0);
    Talk_unk125_3(159);
    Talk_unk30_3(13, -8, 0);
    Talk_unk126_3(20);
    Talk_unk127_3(10, 0);
    Talk_unk128_3(10);
    Talk_unk31_3(13, 0x102, 70);
    Talk_unk32_3(16, 0x10000, 0x8000);
    Talk_unk33_3(16, -8, 0);
    Talk_unk34_3(16, 0x5000, 0);
    Talk_unk129_3(30);
    Talk_unk130_3(16, 4);
    Talk_unk131_3(20);
    Talk_unk132_3(16, 0);
    Talk_unk133_3(10);
    Talk_unk35_3(10, 0xe000, 0);
    Talk_unk134_3(35);
    Talk_unk135_3(10, 2);
    Talk_unk136_3(20);
    Talk_unk137_3(10, 0);
    Talk_unk138_3(10);
    Talk_unk36_3(16, 0x2000, 0);
    Talk_unk139_3(55);
    Talk_unk37_3(16, 0x5000, 0);
    Talk_unk140_3(30);
    Talk_unk141_3(16, 0);
    Talk_unk142_3(10);
    Talk_unk38_3(11, 0xe000, 0);
    Talk_unk143_3(20);
    Talk_unk39_3(11, 0x102, 50);
    Talk_unk144_3(11, 0);
    Talk_unk145_3(10);
    Talk_unk146_3(13, 2);
    Talk_unk147_3(20);
    Talk_unk148_3(13, 0);
    Talk_unk149_3(10);
    Talk_unk40_3(13, 0xa000, 0);
    Talk_unk150_3(60);
    Talk_unk41_3(13, 0x8000, 0);
    Talk_unk151_2(30);
    Talk_unk152_2(13, 0);
    Talk_unk153_2(10);
    Talk_unk154_2(10, 0, 0);
    Talk_unk155_2(11, 0, 0);
    Talk_unk156_2(20);
    Talk_unk157_2(10, 2);
    Talk_unk158_2(20);
    Talk_unk159_2(10, 0);
    Talk_unk42_3(10, 0x13333, 0x9999);
    Talk_unk160_2(10, 8, 0);
    Talk_unk43_3(16, 0x20000, 0x10000);
    Talk_unk44_3(16, -8, 16);
    Talk_unk45_3(16, 0x8000, 0);
    Talk_unk161_2(16, 0);
    Talk_unk162_2(10);
    Talk_unk46_3(10, 0x102, 50);
    *(u8 *)(Talk_unk44(10) + 90) &= v5;
    Talk_unk47_3(10, 0xcccc, 0x6666);
    Talk_unk48_3(10, -8, 0);
    *(u8 *)(Talk_unk45(10) + 90) |= v6;
    Talk_unk163_2(20);
    Talk_unk164_2(10, 2);
    Talk_unk165_2(20);
    Talk_unk166_2(10, 0);
    Talk_unk167_2(10);
    Talk_unk168_2(16, 4);
    Talk_unk169_2(20);
    Talk_unk170_2(16, 0);
    Talk_unk171_2(10);
    *(u8 *)(Talk_unk46(10) + 90) &= v5;
    Talk_unk49_3(10, 0xcccc, 0x6666);
    Talk_unk50_3(10, -16, 0);
    *(u8 *)(Talk_unk47(10) + 90) |= v6;
    Talk_unk172_2(10, 0, 0);
    Talk_unk173_2(20);
    Talk_unk174_2(10, 4);
    Talk_unk175_2(20);
    Talk_unk51_3(10, 0x1cccc, 0xe666);
    Talk_unk176_2(10, 8, 0);
    Talk_unk177_2(10, 6, 0);
    Talk_unk178_2(10, 24, 0);
    Talk_unk179_2(133);
    Talk_unk180_2(16, 6, 0);
    Talk_unk5_4(16, 0x20096e4);
    *(u8 *)(Talk_unk48(10) + 90) &= v5;
    Talk_unk181_2(10, 6, 0);
    Talk_unk52_3(10, -12, 4);
    record = Talk_unk25(10);
    record[89] = none;
    record[35] = 2;
    *(u8 *)(*(volatile s32 *)((s32)record + 80) + 9) |= 12;
    *(u8 *)(*(volatile s32 *)((s32)record + 80) + 38) = none;
    {
        s32 target = *(volatile s32 *)((s32)record + 80);
        s32 shown = 0xc000;

        *(volatile u16 *)(target + 30) = shown;
    }
    Talk_unk53_3(10, -12, 4);
    Talk_unk54_3(10, 0x4000, 0);
    {
        u8 *record = Talk_unk33_4(10);
        u8 value = *(volatile u8 *)&record[90];

        record[90] = (u8)(value | v6);
    }
    Talk_unk182_2(159);
    Talk_unk183_2(20);
    Talk_unk55_3(11, 0x102, 50);
    Talk_unk56_3(11, 0x18000, 0xc000);
    Talk_unk184_2(11, 24, 0);
    Talk_unk185_2(11, 0xc000, 0);
    Talk_unk186_2(10);
    Talk_unk187_2(11, 0);
    Talk_unk188_2(10);
    Talk_unk189_2(16, 4);
    Talk_unk190_2(20);
    Talk_unk191_2(16, 0);
    Talk_unk192_2(20);
    Talk_unk57_3(16, 0xcccc, 0x6666);
    Talk_unk58_3(16, -8, 0);
    Talk_unk193_2(20);
    Talk_unk194_2(16, 0);
    Talk_unk195_2(10);
    Talk_unk196_2(10, 2);
    Talk_unk197_2(20);
    Talk_unk198_2(10);
    Talk_unk199_2(16, 0, 0);
    Talk_unk200_2(40);
    Talk_unk201_2(13, 3);
    Talk_unk202_2(10);
    Talk_unk203_2(13, 3);
    Talk_unk204_2(20);
    Talk_unk205_2(10);
    Talk_unk59_3(16, 0x10000, 0x8000);
    Talk_unk60_3(16, 24, -24);
    Talk_unk206_2(16, 8, 0);
    Talk_unk61_3(16, 0xe000, 0);
    Talk_unk207_2(20);
    Talk_unk62_3(13, 0x10000, 0x8000);
    Talk_unk63_3(13, 0, -8);
    Talk_unk208_2(10);
    Talk_unk209_2();
}

/* overlays/scene/dialogue/flagged_message/scene_setup.c */
s32 *Talk_unk210_2(s32);
s32 *Talk_unk211_2(s32);

void Scene_PlaceSlots14And15(void)
{
    s32 pos14;
    s32 pos15;

    pos14 = Talk_unk210_2(14)[2] >> 20;   /* [r0,#8], asrs #20 */
    pos15 = Talk_unk211_2(15)[2] >> 20;

    Talk_SetRect(5, 12, 5, 1, 5, 11);
    Talk_unk2_5(1, 0, 1, 1, pos15, 11);
    Talk_unk3_5(1, 0, 1, 1, pos14, 11);

    Talk_unk26_2(14);
    Talk_unk27_2(15);
}

/* overlays/scene/dialogue/flagged_message/scene_state_interaction.c */

void Scene_RunActorThirteenFlagDialogue(void)
{
    Talk_unk212_2();

    if (Talk_unk49(0x8A7) != 0) {
        Talk_unk28_2(0x1D1F);
        Talk_Apply6(13, 0);
    } else if (Talk_unk50(0x8A5) != 0) {
        Talk_unk29_2(0x1D1B);
        Talk_Apply7(13, 0);
    } else {
        Talk_unk30_2(0x1D19);
        Talk_Apply8(13, 0);
    }

    Talk_unk213_2();
}

/* overlays/scene/dialogue/flagged_message/shared.c */
s32 get_followup_default_result(void)
{
    return 0;
}

/* overlays/scene/dialogue/flagged_message/state_update.c */

extern u8 *gWork;

void State_ApplyValues14And0And5(void)
{
    Talk_unk64_3(0xE, 0, 5);
}

void State_SetRuntimeWord448To521AndSend303(void)
{
    u8 *state = gWork;
    s32 *slot = (s32 *)(state + 0x1C0);

    *slot = 0x209;
    Talk_unk31_2(0x12F);
}
