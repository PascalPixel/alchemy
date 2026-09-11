#include "types.h"
#include "scene.h"
#include "abi/overlays/scene/dialogue/flagged_message/flagged_message.h"
#include "select_overlay_data_by_runtime_selector.h"
#include "select_overlay_data_by_runtime_selector_body.inc"

/* overlays/scene/dialogue/flagged_message/actor_presentation.c */
u8 *Talk_Run6(s32);
u8 *Talk_Run7(s32);

void Actor_SetMode3AndFlagBit1(s32 no)
{
    u8 *p = Talk_Run6(no);
    u8 *flag;

    Talk_Apply(Talk_Run7(no), 0);
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
        if (Talk_Check26(0x93e) != 0) {
            return (s32)gOv5;
        }
        return (s32)gOv4;
    }

    if (room == (s32)&Value_00000070) {
        /* 0x950 is built from an immediate and a shift. */
        if (Talk_Check27(0x950) != 0) {
            return (s32)gOv3;
        }
        return (s32)gOv2;
    }

    if (room == (s32)&Value_0000006c) {
        if (Talk_Check28(0x950) != 0) {
            return (s32)gOv8;
        }
        if (Talk_Check29(0x93e) != 0) {
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
        if (Talk_Check30(0x93e) != 0) {
            return (s32)gOv13;
        }
        return (s32)gOv12;
    }

    if (scene == (s32)&Value_00000070) {
        /* 0x950 is built by shifting a small constant, not loaded. */
        if (Talk_Check31(0x950) != 0) {
            return (s32)gOv11;
        }
        return (s32)gOv10;
    }

    if (scene == (s32)&Value_0000006c) {
        if (Talk_Check32(0x950) != 0) {
            return (s32)gOv16;
        }
        if (Talk_Check33(0x93e) != 0) {
            return (s32)gOv15;
        }
        return (s32)gOv14;
    }

    return (s32)gOv9;
}

/* overlays/scene/dialogue/flagged_message/scene_dialogue.c */

extern u8 gVal[];
extern u8 gWork[];

u8 *Talk_Run8();

u8 *Talk_Run9();

/* Contiguous unnamed leaf-owner run for resource_3ae. */

void Dialogue_ShowLine1CF8(void)
{
    Talk_Run10();
    Talk_Do23(0x1CF8);
    Talk_Apply3(8, 0);
    Talk_Run11();
}

void Dialogue_RunActor12Event(void)
{
    s32 rec7;
    s32 rec8;
    u8 *record;
    s16 angle;

    record = Talk_Run8(0);
    angle = (u16)((*(u16 *)(record + 6) + 0x2000) & ~0x3fff);
    Talk_Run12();
    if (Talk_Check(0x8a7) != 0) {
        if (Talk_Check2(0x8a9) != 0) {
            Talk_Do(0x1d23);
            Talk_Run13(12, 0);
            goto L_02000496;
        }
        Talk_Run14((s32)gVal);
        Talk_Check3(12, 0);
        if (Talk_Check4(0, 0) == 0) {
            Talk_Run15(10);
            Talk_Run16(((s32)gVal + 1));
            Talk_Run17(12, 0);
            Talk_Place(12, 88, 0x508);
            Talk_Place2(12, 0x4000, 0);
            Talk_Run18(20);
            Talk_Do2(0x8a9);
            goto L_02000496;
        }
        Talk_Run19(((s32)gVal + 2));
        Talk_Run20(12, 0);
    } else {
        if ((u16)angle != 0x8000) {
            goto L_0200049a;
        }
        Talk_Do3(0x1d16);
        Talk_Run21(12, 0);
        if (Talk_Check5(0x8a5) != 0) {
            rec7 = Talk_Check34(235);
            rec8 = Talk_Check35(rec7, 235);
            Talk_Run22(12, 3);
            Talk_Place3(12, 88, 0x508);
            Talk_Place4(12, 0x4000, 0);
            bump_step(1);
            Talk_Run23(12, 0);
            Talk_Run24(rec7, rec8);
            Talk_Do4(0x8a7);
            record = Talk_Run9(0);
            Talk_Place5(0, *(s16 *)(record + 10), 0x518);
            Talk_Place6(0, 72, 0x518);
            Talk_Place7(12, 88, 0x518);
            Talk_Run25(12, 0, 0);
        } else {
            Talk_Run26(12, 0);
        }
    }
    L_02000496:;
    Talk_Run27();
    L_0200049a:;
}

void Dialogue_RunActor8Message1f09(void)
{
    Talk_Run28();
    Talk_Do24(0x1F09);
    Talk_Apply4(8, 0);
    Talk_Run29();
}

void Dialogue_RunActor10Message1f15(void)
{
    Talk_Run30();
    Talk_Do25(0x1F15);
    Talk_Apply5(10, 0);
    Talk_Run31();
}

/* overlays/scene/dialogue/flagged_message/scene_primary_script.c */

extern u8 gWork[];
extern u8 RuntimeSelectorTable[];
extern u8 gVal2[];
extern u8 gVal3[];
extern u8 gVal4[];
extern s16 gCell[][1];

u8 *Talk_Run32();

u8 *Talk_Run33();

void Scene_RunScene3ae(void)
{
    u32 i;
    s32 record;

    Talk_Run34();
    Talk_Do5(0x8aa);
    Talk_Place8(0, 0x188, 0x128);
    Talk_Place9(8, 0x13333, 0x9999);
    Talk_Place10(8, 0x198, 0x128);
    Talk_Place11(8, 0x8000, 0);
    Talk_Run35(20);
    Talk_Run36();
}

void Scene_RunScene3ae(void)
{
    u32 i;
    s32 record;

    Talk_Run37();
    if (Talk_Check6(0x8a6) == 0) {
        Talk_Do6(0x1cfd);
        Talk_Check7(11, 0);
        if (Talk_Check8(0, 0) == 0) {
            Talk_Run38(11, 0);
            Talk_Do7(0x8a6);
            goto L_020002c2;
        }
        bump_step(1);
        Talk_Run39(11, 0);
    } else {
        Talk_Do8(0x1cfe);
        Talk_Run40(11, 0);
    }
    L_020002c2:;
    Talk_Run41();
}

void Scene_RunScene3ae(void)
{
    u32 i;
    s32 record;

    Talk_Run42(0);
    Talk_Run43();
    if (Talk_Check9(0x8a7) != 0) {
        if (Talk_Check10(0x8a9) != 0) {
            Talk_Do9(0x1d23);
            Talk_Check11(12, 0);
            Talk_Place12(12, 0x4000, 0);
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
    Talk_Run44();
    if (Talk_Check12(0x8a5) != 0) {
        Talk_Do10(0x1d0b);
        Talk_Run45(8, 0);
    } else {
        Talk_Do11(0x1d04);
        Talk_Check13(8, 0);
        if (Talk_Check14(0, 0) == 1) {
            Talk_Run46(8, 0, 10);
        } else {
            bump_step(1);
            Talk_Run47(six00, 5);
            Talk_Check15(8, 0);
            rec7 = Talk_Check36(19, 8, 11, 4, 2);
            Talk_Run(0xc8a, rec7, 0, 0);
            base6_2000240 = (s32)RuntimeSelectorTable;
            Talk_Run48(*(volatile s32 *)(base6_2000240 + 16), 6, rec7, 24, 8);
            if (Talk_Check16(-1, 0) == 1) {
                Talk_Run49(rec7, 2);
                Talk_Check37(0, 4);
                Talk_Run50(10);
                Talk_Run51(8, 0);
                goto L_02000660;
            } else {
                if ((u32)six00 <= (u32)*(volatile s32 *)(base6_2000240 + 16)) {
                    goto L_0200061e;
                }
                Talk_Run52(rec7, 2);
                Talk_Check38(0, 3);
                Talk_Run53(10);
                bump_step(1);
                Talk_Run54(113);
                Talk_Run51(8, 0);
                goto L_02000660;
            }
            L_0200061e:;
            Talk_Run55(rec7, 2);
            Talk_Run56(0, 3);
            Talk_Run57(10);
            bump_step(3);
            Talk_Run58(8, 0);
            Talk_Check39(235, 0);
            Talk_Do12(0x8a5);
            Talk_Do13(-six00);
        }
        L_02000660:;
        Talk_Run59();
    }
}

void Scene_RunScene3ae(void)
{
    u32 i;
    s32 record;

    Talk_Run60();
    if (Talk_Check17(0x8a8) != 0) {
        Talk_Run61(11, 0, 0);
        Talk_Run62(20);
        Talk_Do14(0x1f1c);
        Talk_Run63(11, 0);
        ((void (*)())Talk_Check40)();
    } else {
        Talk_Run64(20);
        Talk_Place13(11, 0x100, 50);
        Talk_Run65(11, 0, 0);
        Talk_Run66(20);
        Talk_Do15(0x1f18);
        Talk_Run67(11, 0);
        if (Talk_Check18(0x8a6) != 0) {
            Talk_Run68(20);
            Talk_Place14(11, 0x102, 40);
            Talk_Check19(11, 0);
            if (Talk_Check20(0, 0) == 0) {
                Talk_Run69(20);
                Talk_Run70(11, 0);
                Talk_Do16(0x8a8);
                goto L_020007be;
            }
            ((void (*)())Talk_Check41)(10);
            bump_step(1);
            Talk_Run71(11, 0);
            Talk_Run72(10);
            Talk_Run73(11, 0, 0);
            Talk_Run74(30);
        } else {
            Talk_Run75(10);
            Talk_Run76(11, 0, 0);
            Talk_Run77(30);
        }
        L_020007be:;
        Talk_Run78();
    }
}

void Scene_RunScene3ae(void)
{
    u8 *work;

    Talk_Run79();
    Talk_Run80(158);
    Talk_Place15(0, 0x8000, 0x4000);
    Talk_Run81(0, 3);
    work = RuntimeSelectorTable;
    if (*(s16 *)(work + 0x1c0) == (s32)gVal2) {
        Talk_Place16(0, 0x130, 0x570);
        Talk_Place17(0x20096b8, 78, 86);
    } else {
        if (*(s16 *)(work + 0x1c0) == (s32)gVal3) {
            Talk_Run82(0, 248, 192);
            Talk_Place18(0x20096ce, 74, 9);
        }
    }
    Talk_Run83(16);
    Talk_Run84(3);
    Talk_Run85();
}

s32 Scene_RunScene3ae(void)
{
    u32 i;
    s32 record;

    if (gCell[225][0] == 90) {
        Talk_Do17(0x950);
    }
    if (gCell[224][0] == (s32)gVal2) {
        Talk_Run86();
    } else {
        if (gCell[224][0] == (s32)gVal3) {
            Talk_Run87();
        } else {
            if (gCell[224][0] == (s32)gVal4) {
                Talk_Run88();
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
        if (Talk_Check21(0x8ac) == 0) {
            Talk_Do18(0x8ac);
            Talk_Run89();
        }
    }
    if (gCell[225][0] == 2) {
        if (Talk_Check22(0x109) == 0) {
            Talk_Do19(0x8a9);
        }
    }
    if (Talk_Check23(0x911) != 0) {
        if (Talk_Check24(0x8a9) == 0) {
            Talk_Place19(12, 0x580000, 0x5180000);
            Talk_Run90(12, 0, 0);
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
    Talk_Run91();
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

    Talk_Do20(0x8ab);
    Talk_Run92();
    Talk_Run93();
    Talk_Do21(0x23eb);
    record = Talk_Run32(11);
    none = 0;
    record[35] = none;
    *(u8 *)(*(volatile s32 *)((s32)record + 80) + 9) |= 12;
    *(u8 *)(*(volatile s32 *)((s32)record + 80) + 21) |= 12;
    Talk_Run2(0xe80000, -1, 0x1300000, 1);
    Talk_Place20(0, 0x10000, 0x8000);
    Talk_Place21(0, 216, 0x110);
    Talk_Place22(0, 0x2000, 0);
    Talk_Run94();
    Talk_Run95(20);
    Talk_Place23(13, 0x102, 50);
    Talk_Run96(13, 0);
    Talk_Run97(10);
    Talk_Place24(10, 0x107, 50);
    Talk_Run98(10, 0, 0);
    Talk_Run99(10, 4, 13);
    Talk_Run100(10, 4, 30);
    Talk_Run101(10, 0);
    Talk_Run102(10);
    Talk_Run103(11, 2);
    Talk_Run104(20);
    Talk_Run105(11, 0);
    Talk_Run106(10);
    Talk_Run107(13, 4);
    Talk_Run108(20);
    Talk_Run3(13, 0);
    Talk_Run109(10);
    Talk_Place25(10, 0x103, 55);
    Talk_Place26(10, 0x20000, 0x10000);
    Talk_Run110(10, 16, 0);
    Talk_Run111(10, 7, 0);
    v5 = 254;
    Talk_Run112(10, 24, 0);
    *(u8 *)(Talk_Check42(10) + 90) &= v5;
    Talk_Place27(10, -16, 0);
    Talk_Run113(153);
    Talk_Place28(13, 0x26666, 0x13333);
    Talk_Run114(13, 16, 0);
    Talk_Run115(10);
    v6 = 1;
    Talk_Run116(10, 1);
    *(u8 *)(Talk_Check43(10) + 90) |= v6;
    Talk_Run4(13, 0x102);
    Talk_Run117(13, 2);
    Talk_Run118(155);
    Talk_Do22(10);
    Talk_Run119(155);
    Talk_Run120(10);
    Talk_Run121(155);
    Talk_Run122(10);
    Talk_Run123(20);
    Talk_Place29(13, 0x6666, 0x3333);
    Talk_Run124(13, 6, 0);
    Talk_Run125(159);
    Talk_Place30(13, -8, 0);
    Talk_Run126(20);
    Talk_Run127(10, 0);
    Talk_Run128(10);
    Talk_Place31(13, 0x102, 70);
    Talk_Place32(16, 0x10000, 0x8000);
    Talk_Place33(16, -8, 0);
    Talk_Place34(16, 0x5000, 0);
    Talk_Run129(30);
    Talk_Run130(16, 4);
    Talk_Run131(20);
    Talk_Run132(16, 0);
    Talk_Run133(10);
    Talk_Place35(10, 0xe000, 0);
    Talk_Run134(35);
    Talk_Run135(10, 2);
    Talk_Run136(20);
    Talk_Run137(10, 0);
    Talk_Run138(10);
    Talk_Place36(16, 0x2000, 0);
    Talk_Run139(55);
    Talk_Place37(16, 0x5000, 0);
    Talk_Run140(30);
    Talk_Run141(16, 0);
    Talk_Run142(10);
    Talk_Place38(11, 0xe000, 0);
    Talk_Run143(20);
    Talk_Place39(11, 0x102, 50);
    Talk_Run144(11, 0);
    Talk_Run145(10);
    Talk_Run146(13, 2);
    Talk_Run147(20);
    Talk_Run148(13, 0);
    Talk_Run149(10);
    Talk_Place40(13, 0xa000, 0);
    Talk_Run150(60);
    Talk_Place41(13, 0x8000, 0);
    Talk_Run151(30);
    Talk_Run152(13, 0);
    Talk_Run153(10);
    Talk_Run154(10, 0, 0);
    Talk_Run155(11, 0, 0);
    Talk_Run156(20);
    Talk_Run157(10, 2);
    Talk_Run158(20);
    Talk_Run159(10, 0);
    Talk_Place42(10, 0x13333, 0x9999);
    Talk_Run160(10, 8, 0);
    Talk_Place43(16, 0x20000, 0x10000);
    Talk_Place44(16, -8, 16);
    Talk_Place45(16, 0x8000, 0);
    Talk_Run161(16, 0);
    Talk_Run162(10);
    Talk_Place46(10, 0x102, 50);
    *(u8 *)(Talk_Check44(10) + 90) &= v5;
    Talk_Place47(10, 0xcccc, 0x6666);
    Talk_Place48(10, -8, 0);
    *(u8 *)(Talk_Check45(10) + 90) |= v6;
    Talk_Run163(20);
    Talk_Run164(10, 2);
    Talk_Run165(20);
    Talk_Run166(10, 0);
    Talk_Run167(10);
    Talk_Run168(16, 4);
    Talk_Run169(20);
    Talk_Run170(16, 0);
    Talk_Run171(10);
    *(u8 *)(Talk_Check46(10) + 90) &= v5;
    Talk_Place49(10, 0xcccc, 0x6666);
    Talk_Place50(10, -16, 0);
    *(u8 *)(Talk_Check47(10) + 90) |= v6;
    Talk_Run172(10, 0, 0);
    Talk_Run173(20);
    Talk_Run174(10, 4);
    Talk_Run175(20);
    Talk_Place51(10, 0x1cccc, 0xe666);
    Talk_Run176(10, 8, 0);
    Talk_Run177(10, 6, 0);
    Talk_Run178(10, 24, 0);
    Talk_Run179(133);
    Talk_Run180(16, 6, 0);
    Talk_Run5(16, 0x20096e4);
    *(u8 *)(Talk_Check48(10) + 90) &= v5;
    Talk_Run181(10, 6, 0);
    Talk_Place52(10, -12, 4);
    record = Talk_Check25(10);
    record[89] = none;
    record[35] = 2;
    *(u8 *)(*(volatile s32 *)((s32)record + 80) + 9) |= 12;
    *(u8 *)(*(volatile s32 *)((s32)record + 80) + 38) = none;
    {
        s32 target = *(volatile s32 *)((s32)record + 80);
        s32 shown = 0xc000;

        *(volatile u16 *)(target + 30) = shown;
    }
    Talk_Place53(10, -12, 4);
    Talk_Place54(10, 0x4000, 0);
    {
        u8 *record = Talk_Run33(10);
        u8 value = *(volatile u8 *)&record[90];

        record[90] = (u8)(value | v6);
    }
    Talk_Run182(159);
    Talk_Run183(20);
    Talk_Place55(11, 0x102, 50);
    Talk_Place56(11, 0x18000, 0xc000);
    Talk_Run184(11, 24, 0);
    Talk_Run185(11, 0xc000, 0);
    Talk_Run186(10);
    Talk_Run187(11, 0);
    Talk_Run188(10);
    Talk_Run189(16, 4);
    Talk_Run190(20);
    Talk_Run191(16, 0);
    Talk_Run192(20);
    Talk_Place57(16, 0xcccc, 0x6666);
    Talk_Place58(16, -8, 0);
    Talk_Run193(20);
    Talk_Run194(16, 0);
    Talk_Run195(10);
    Talk_Run196(10, 2);
    Talk_Run197(20);
    Talk_Run198(10);
    Talk_Run199(16, 0, 0);
    Talk_Run200(40);
    Talk_Run201(13, 3);
    Talk_Run202(10);
    Talk_Run203(13, 3);
    Talk_Run204(20);
    Talk_Run205(10);
    Talk_Place59(16, 0x10000, 0x8000);
    Talk_Place60(16, 24, -24);
    Talk_Run206(16, 8, 0);
    Talk_Place61(16, 0xe000, 0);
    Talk_Run207(20);
    Talk_Place62(13, 0x10000, 0x8000);
    Talk_Place63(13, 0, -8);
    Talk_Run208(10);
    Talk_Run209();
}

/* overlays/scene/dialogue/flagged_message/scene_setup.c */
s32 *Talk_Run210(s32);
s32 *Talk_Run211(s32);

void Scene_PlaceSlots14And15(void)
{
    s32 pos14;
    s32 pos15;

    pos14 = Talk_Run210(14)[2] >> 20;   /* [r0,#8], asrs #20 */
    pos15 = Talk_Run211(15)[2] >> 20;

    Talk_SetRect(5, 12, 5, 1, 5, 11);
    Talk_SetRect2(1, 0, 1, 1, pos15, 11);
    Talk_SetRect3(1, 0, 1, 1, pos14, 11);

    Talk_Do26(14);
    Talk_Do27(15);
}

/* overlays/scene/dialogue/flagged_message/scene_state_interaction.c */

void Scene_RunActorThirteenFlagDialogue(void)
{
    Talk_Run212();

    if (Talk_Check49(0x8A7) != 0) {
        Talk_Do28(0x1D1F);
        Talk_Apply6(13, 0);
    } else if (Talk_Check50(0x8A5) != 0) {
        Talk_Do29(0x1D1B);
        Talk_Apply7(13, 0);
    } else {
        Talk_Do30(0x1D19);
        Talk_Apply8(13, 0);
    }

    Talk_Run213();
}

/* overlays/scene/dialogue/flagged_message/shared.c */
s32 Talk_Run214(void)
{
    return 0;
}

/* overlays/scene/dialogue/flagged_message/state_update.c */

extern u8 *gWork;

void State_ApplyValues14And0And5(void)
{
    Talk_Place64(0xE, 0, 5);
}

void State_SetRuntimeWord448To521AndSend303(void)
{
    u8 *state = gWork;
    s32 *slot = (s32 *)(state + 0x1C0);

    *slot = 0x209;
    Talk_Do31(0x12F);
}
