#include "types.h"
#include "scene.h"
#include "abi/overlays/scene/story/region.h"

#define PrimaryRuntimeSelector Value_00000031
#define SecondaryRuntimeSelector Value_00000030
#define TertiaryRuntimeSelector Value_0000002f

#include "select_overlay_data_by_runtime_selector.h"

extern s16 RuntimeSelectorTable[];
extern unsigned char Value_00000031;
extern unsigned char Value_00000030;
extern unsigned char Value_0000002f;
extern s32 gOv[]; /* Packed direction steps, 16 entries. */
extern u8 SecondaryOverlayData[];
extern u8 TertiaryOverlayData[];
extern u8 DefaultOverlayData[];
extern u8 SecondaryOverlayData[];
extern u8 TertiaryOverlayData[];
extern u8 DefaultOverlayData[];

u8 *Story_Run(s32);

u8 *Story_Run2();
s32 *Story_Run3();

u8 *Story_Run4();

u8 *Story_Run5();

u8 *Story_Run6();

u8 *Story_Run7();

u8 *Story_Run8();

u8 *Story_Run9();
u8 *Story_Run10();
u8 *Story_Run11();
u8 *Story_Run12();
u8 *Story_Run13();

u8 *Story_Run14();
u8 *Story_Run15();

u8 *Story_Run16();

/*
 * Table getter at 0x02000098. The eight-byte owner includes its one pool word
 * at 0x0200009c, which holds 0x02008bcc; the pc-relative load reads it. The
 * word is an address, returned without being dereferenced.
 */

/* 0x02000e9a serves two imports in sibling arms: the three-argument setter in
 * the first and the one-argument record accessor in the second. */

/*
 * gOv is indexed by heading >> 12, X step in the high halfword and Z
 * step in the low, promoted back to 16.16 by masking and shifting rather than
 * by multiply. The s16 at +0x0a and +0x12 are the integer parts of the 16.16
 * words at +0x08 and +0x10, so the tile lookup takes tile coordinates.
 */

/*
 * Push the actor one step ahead of the player. The 268-byte owner at
 * 0x020007f8 includes its alignment halfword and its three pool words.
 * `blocker` is zero wherever it is stored at +0x24 and +0x2c, but it is the
 * register the reference stores, so it stays spelled as itself.
 */

/* Old-style declarations: interfaces vary by call site across this overlay. */

/* Deliberate no-op callback. */

extern u8 PrimaryOverlayData[];

extern u8 PrimaryOverlayData[];

s32 State_ApplyArgMode0AndReturnZero(s32 no)
{
    extern u8 *gWork;
    s32 Story_Run17(s32, s32);
    s32 Story_Run18(s32, s32, s32, s32, s32, s32);
    s32 *Story_Run19();

    s32 Story_Run20(s32, s32);

    Story_Run20(no, 0);
    return 0;
}

s32 SceneData_SelectByRuntimeSelector(void)
{
    extern u8 *gWork;
    s32 Story_Run17(s32, s32);
    s32 Story_Run18(s32, s32, s32, s32, s32, s32);
    s32 *Story_Run19();

    s16 selector = RuntimeSelectorTable[224];

    if (selector == (s32)&PrimaryRuntimeSelector) {
        return (s32)PrimaryOverlayData;
    }
    if (selector == (s32)&SecondaryRuntimeSelector) {
        return (s32)SecondaryOverlayData;
    }
    if (selector == (s32)&TertiaryRuntimeSelector) {
        return (s32)TertiaryOverlayData;
    }
    return (s32)DefaultOverlayData;
}

s32 Story_Run21(void)
{
    extern u8 *gWork;
    s32 Story_Run17(s32, s32);
    s32 Story_Run18(s32, s32, s32, s32, s32, s32);
    s32 *Story_Run19();

    return 0;
}

u8 *SceneData_GetTable8bcc(void)
{
    extern u8 *gWork;
    s32 Story_Run17(s32, s32);
    s32 Story_Run18(s32, s32, s32, s32, s32, s32);
    s32 *Story_Run19();

    return (u8 *)0x02008bcc;
}

s32 SceneData_SelectSecondaryDataByRuntimeSelector(void)
{
    extern u8 *gWork;
    s32 Story_Run17(s32, s32);
    s32 Story_Run18(s32, s32, s32, s32, s32, s32);
    s32 *Story_Run19();

    s16 selector = RuntimeSelectorTable[224];

    if (selector == (s32)&PrimaryRuntimeSelector) {
        return (s32)PrimaryOverlayData;
    }
    if (selector == (s32)&SecondaryRuntimeSelector) {
        return (s32)SecondaryOverlayData;
    }
    if (selector == (s32)&TertiaryRuntimeSelector) {
        return (s32)TertiaryOverlayData;
    }
    return (s32)DefaultOverlayData;
}

s32 SceneData_SelectDataByRuntimeSelector(void)
{
    extern u8 *gWork;
    s32 Story_Run17(s32, s32);
    s32 Story_Run18(s32, s32, s32, s32, s32, s32);
    s32 *Story_Run19();

    s16 selector = RuntimeSelectorTable[224];

    if (selector == (s32)&PrimaryRuntimeSelector) {
        return (s32)PrimaryOverlayData;
    }
    if (selector == (s32)&SecondaryRuntimeSelector) {
        return (s32)SecondaryOverlayData;
    }
    if (selector == (s32)&TertiaryRuntimeSelector) {
        return (s32)TertiaryOverlayData;
    }
    return (s32)DefaultOverlayData;
}

void State_ConfigureRegion1_0_21x14(void)
{
    extern u8 *gWork;
    s32 Story_Run17(s32, s32);
    s32 Story_Run18(s32, s32, s32, s32, s32, s32);
    s32 *Story_Run19();

    s32 w = 21;
    s32 h = 14;

    Story_SetRect11(1, 0, 1, 1, w, h);
}

void State_ConfigureRegion0_0_21x14(void)
{
    extern u8 *gWork;
    s32 Story_Run17(s32, s32);
    s32 Story_Run18(s32, s32, s32, s32, s32, s32);
    s32 *Story_Run19();

    s32 w = 21;
    s32 h = 14;

    Story_SetRect12(0, 0, 1, 1, w, h);
}

void State_ApplyTwoRects(void)
{
    extern u8 *gWork;
    s32 Story_Run17(s32, s32);
    s32 Story_Run18(s32, s32, s32, s32, s32, s32);
    s32 *Story_Run19();

    {
        s32 a5 = 1;
        s32 a6 = 3;

        Story_SetRect13(111, 37, 97, 21, a5, a6);
    }
    {
        s32 a5 = 32;
        s32 a6 = 24;

        Story_SetRect14(46, 38, 3, 2, a5, a6);
    }
}

void Scene_RunTwoLayoutSteps(void)
{
    extern u8 *gWork;
    s32 Story_Run17(s32, s32);
    s32 Story_Run18(s32, s32, s32, s32, s32, s32);
    s32 *Story_Run19();

    {
        s32 fifth = 1;
        s32 sixth = 3;

        Story_SetRect15(95, 21, 97, 21, fifth, sixth);
    }
    {
        s32 fifth = 32;
        s32 sixth = 25;

        Story_SetRect16(46, 38, 3, 1, fifth, sixth);
    }
}

void Scene_RunActor9Flag882Scene(void)
{
    extern u8 *gWork;
    s32 Story_Run17(s32, s32);
    s32 Story_Run18(s32, s32, s32, s32, s32, s32);
    s32 *Story_Run19();

    Story_Run22();
    Story_Place3(9, 0, 0);
    Story_Do8(0x882);
    Story_Run23();
}

void Scene_RunScene398SequenceA(void)
{
    extern u8 gWork[];

    u32 i;
    u8 *record;

    Story_Run24();
    Story_Run25(8, 0, 0);
    Story_Do(0x883);
    Story_Run26(40);
    Story_Run27(15, 2);
    *(u8 *)(Story_Check10(15) + 85) = 0;
    {
        u8 *record = Story_Run4(15);
        u8 value = *(volatile u8 *)&record[35];

        record[35] = (u8)(value | 2);
    }
    Story_Run28(15, 2);
    Story_SetRect(0, 0, 1, 1, 18, 14);
    Story_Run29();
}

void Scene_RunActorFifteenScene(void)
{
    extern u8 *gWork;
    s32 Story_Run17(s32, s32);
    s32 Story_Run18(s32, s32, s32, s32, s32, s32);
    s32 *Story_Run19();

    void Story_Run30(s32);

    Story_Run31();
    Story_Apply(0xF, 0);
    Story_Do9(0x28);
    Story_Run30(0xD2);
    Story_Apply2(0xF, 6);
    Story_Run32();
}

void Scene_RunActorSixteenScene(void)
{
    extern u8 *gWork;
    s32 Story_Run17(s32, s32);
    s32 Story_Run18(s32, s32, s32, s32, s32, s32);
    s32 *Story_Run19();

    void Story_Run33(void);
    void Story_Run34(s32);

    Story_Run35();
    Story_Apply3(0x10, 0);
    Story_Do10(0x28);
    Story_Run34(0xD2);
    Story_Apply4(0x10, 6);
    Story_Run33();
}

void Scene_RunActor17Steps28AndD2(void)
{
    extern u8 *gWork;
    s32 Story_Run17(s32, s32);
    s32 Story_Run18(s32, s32, s32, s32, s32, s32);
    s32 *Story_Run19();

    void Story_Run36(void);

    Story_Run37();
    Story_Run17(0x11, 0);
    Story_Do11(0x28);
    Story_Do12(0xD2);
    Story_Apply5(0x11, 6);
    Story_Run36();
}

void Scene_RunScene398SequenceB(void)
{
    extern u8 gWork[];
    void Story_Run17();
    void Story_Run18();

    u32 i;
    s32 rec7;
    s32 rec8;
    s32 record;
    s32 v5;

    rec7 = Story_Check(11);
    rec8 = Story_Check11(12);
    if ((*(volatile s32 *)(rec7 + 8) >> 20) == 35) {
        if ((*(volatile s32 *)(rec7 + 16) >> 20) != 23) {
            goto L_02000330;
        }
        Story_Do2(0x303);
    } else {
        L_02000330:;
        Story_Do3(0x303);
    }
    if ((*(volatile s32 *)(rec8 + 8) >> 20) == 35) {
        if ((*(volatile s32 *)(rec8 + 16) >> 20) != 23) {
            goto L_02000350;
        }
        Story_Do4(0x304);
    } else {
        L_02000350:;
        Story_Do5(0x304);
    }
    if (Story_Check2(0x303) == 0) {
        record = Story_Check3(0x304);
        if (record == 0) {
            goto L_020003c2;
        }
    }
    if (Story_Check4(0x302) == 0) {
        Story_Run38();
        Story_Run39(40);
        Story_Run40(210);
        v5 = 36;
        Story_Run41(17, 6);
        Story_SetRect2(0, 1, 1, 1, v5, 22);
        Story_SetRect3(0, 2, 1, 1, v5, 24);
        Story_Run42();
    }
    Story_Do6(0x302);
    goto L_02000414;
    L_020003c2:;
    if (Story_Check5(0x302) != 0) {
        Story_Run43();
        Story_Run44(40);
        Story_Run45(220);
        v5 = 36;
        Story_Run46(17, 2);
        Story_SetRect4(1, 1, 1, 1, v5, 22);
        Story_SetRect5(1, 2, 1, 1, v5, 24);
        Story_Run47();
    }
    Story_Do7(0x302);
    L_02000414:;
}

void ActorDraw_SetSceneCell31AndFlag305(void)
{
    extern u8 *gWork;
    s32 Story_Run17(s32, s32);
    s32 Story_Run18(s32, s32, s32, s32, s32, s32);
    s32 *Story_Run19();

    s32 width = 8;
    s32 height = 13;

    Story_Run18(31, 0, 1, 1, width, height);
    Story_Do13(0x305);
}

void State_SetGlobalByte17(void)
{
    extern u8 *gWork;
    s32 Story_Run17(s32, s32);
    s32 Story_Run18(s32, s32, s32, s32, s32, s32);
    s32 *Story_Run19();

    FIELD_AT_OFFSET(*(void **)0x03001E70, s8 *, 0x17) = 1;
}

void State_ClearRuntimeByte17(void)
{
    extern u8 *gWork;
    s32 Story_Run17(s32, s32);
    s32 Story_Run18(s32, s32, s32, s32, s32, s32);
    s32 *Story_Run19();

    FIELD_AT_OFFSET(*(void **)0x03001E70, s8 *, 0x17) = 0;
}

/*
 * Map-variant selector, and the overlay's exported entry. The 72-byte owner at
 * 0x0200046c includes its alignment halfword and four pool words, and returns
 * 0. The selector is the signed halfword at byte offset 448 of RuntimeSelectorTable;
 * the three compared constants are spelled as addresses of Value_ symbols,
 * which is what puts them in the literal pool.
 */
s32 Scene_DispatchByScenarioId(void)
{
    extern u8 *gWork;
    s32 Story_Run17(s32, s32);
    s32 Story_Run18(s32, s32, s32, s32, s32, s32);
    s32 *Story_Run19();

    void Story_Run48(void);

    s16 variant = RuntimeSelectorTable[224];

    if (variant == (s32)&Value_00000031) {
        Story_Run49();
    } else if (variant == (s32)&Value_00000030) {
        Story_Run48();
    } else if (variant == (s32)&Value_0000002f) {
        Story_Run50();
    }
    return 0;
}

void RunGuardedSceneSetup(void)
{
    extern u8 *gWork;
    s32 Story_Run17(s32, s32);
    s32 Story_Run18(s32, s32, s32, s32, s32, s32);
    s32 *Story_Run19();

    if (RunSceneCommand(0x305) != 0) {
        s32 width = 8;
        s32 height = 13;

        Story_SetRect17(31, 0, 1, 1, width, height);
        FinishSceneSetup(8, 0);
    }
}

void State_SetRuntimeWord448To516(void)
{
    extern u8 *gWork;
    s32 Story_Run17(s32, s32);
    s32 Story_Run18(s32, s32, s32, s32, s32, s32);
    s32 *Story_Run19();

    /* 448 is built as 224 << 1 and the stored 516 as that same register plus
     * 68; the two are not one running offset. */
    *(s32 *)(gWork + 448) = 516;

    Story_Apply6(8, 1);
    Story_Apply7(10, 2);

    if (Story_Check12(0x882) != 0) {
        Story_Place4(9, 0, 0);
    } else {
        Story_Apply8(Story_Run(9), 0);
    }
}

void Scene_RunScene398SequenceC(void)
{
    extern u8 gWork[];
    void Story_Run19();

    u32 i;
    u8 *record;
    s32 v5;

    *(s32 *)((*(u8 *volatile *)gWork + 0x1c0)) = 0x204;
    record = Story_Run5(18);
    Story_Run51((s32)record, 0);
    record = Story_Run6(19);
    Story_Run52((s32)record, 0);
    record = Story_Run7(20);
    Story_Run53((s32)record, 0);
    record = Story_Run8(21);
    Story_Run54((s32)record, 0);
    record = Story_Run9(22);
    Story_Run55((s32)record, 0);
    record = Story_Run10(23);
    Story_Run56((s32)record, 0);
    record = Story_Run11(24);
    Story_Run57((s32)record, 0);
    record = Story_Run12(25);
    Story_Run58((s32)record, 0);
    record = Story_Run13(26);
    Story_Run59((s32)record, 0);
    Story_Run60(18, 5);
    Story_Run61(19, 5);
    Story_Run62(20, 5);
    Story_Run63(21, 5);
    Story_Run64(22, 5);
    Story_Run65(23, 3);
    Story_Run66(24, 3);
    Story_Run67(25, 3);
    Story_Run68(26, 3);
    Story_Run69(9, 2);
    Story_Run70(10, 2);
    Story_Run71(11, 2);
    Story_Run72(12, 2);
    Story_Run73(13, 2);
    Story_Run74(14, 2);
    Story_Run75(18);
    Story_Run76(19);
    Story_Run77(20);
    Story_Run78(21);
    Story_Run79(22);
    Story_Do14(23);
    Story_Run80(24);
    Story_Run81(25);
    Story_Run82(26);
    Story_Do15(9);
    Story_Run83(10);
    Story_Run84(11);
    Story_Run85(12);
    Story_Do16(13);
    Story_Run86(14);
    if (Story_Check6(0x883) != 0) {
        Story_Run87(8, 0, 0);
        Story_Run88(15, 5);
        *(u8 *)(Story_Check13(15) + 85) = 0;
        record = Story_Run14(15);
        *(volatile s32 *)((s32)record + 12) = -0x40000;
        {
            u8 *record = Story_Run15(15);
            u8 value = *(volatile u8 *)&record[35];

            record[35] = (u8)(value | 2);
        }
        Story_Run89(15, 2);
        Story_SetRect6(0, 0, 1, 1, 18, 14);
    } else {
        Story_Run90(8, 2);
        record = Story_Run16(8);
        Story_Run91((s32)record, 0);
        Story_Run92(15, 1);
    }
    Story_Run93(16, 1);
    if (Story_Check7(0x302) != 0) {
        v5 = 36;
        Story_Run94(17, 1);
        Story_SetRect7(0, 1, 1, 1, v5, 22);
        Story_SetRect8(0, 2, 1, 1, v5, 24);
    } else {
        v5 = 36;
        Story_Run95(17, 5);
        Story_SetRect9(1, 1, 1, 1, v5, 22);
        Story_SetRect10(1, 2, 1, 1, v5, 24);
    }
    if (Story_Check8(0x303) != 0) {
        Story_Place(11, 0x23a0000, 0x1780000);
    }
    if (Story_Check9(0x304) != 0) {
        Story_Place2(12, 0x23a0000, 0x1780000);
    }
}

s32 *Actor_FindSlotAtTile(s32 x, s32 z)
{
    extern u8 *gWork;
    s32 Story_Run17(s32, s32);
    s32 Story_Run18(s32, s32, s32, s32, s32, s32);
    s32 *Story_Run19();

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

void StagedActor_PushActorAhead(void)
{
    extern u8 *gWork;
    s32 Story_Run17(s32, s32);
    s32 Story_Run18(s32, s32, s32, s32, s32, s32);
    s32 *Story_Run19();

    u8 *player;
    u8 *target;
    u8 *blocker;
    s32 heading;
    s32 tx;
    s32 tz;
    s32 pos[3];

    player = Story_Run2(0);
    heading = *(u16 *)(player + 6) >> 12;

    tx = (*(s16 *)(player + 10)
        + (gOv[heading] >> 16)) >> 4;
    tz = (*(s16 *)(player + 18)
        + ((gOv[heading] << 16) >> 16)) >> 4;
    target = (u8 *)Story_Run3(tx, tz);
    if (target == 0) return;

    tx = (*(s16 *)(target + 10)
        + (gOv[heading] >> 16)) >> 4;
    tz = (*(s16 *)(target + 18)
        + ((gOv[heading] << 16) >> 16)) >> 4;
    blocker = (u8 *)Story_Run19(tx, tz);
    if (blocker != 0) return;

    target[0x22] = 2;

    pos[0] = *(s32 *)(target + 8)
        + (gOv[heading] & (s32)0xffff0000);
    pos[1] = *(s32 *)(target + 12);
    pos[2] = *(s32 *)(target + 16) + (gOv[heading] << 16);

    if (Story_Check14(target, pos) > 0) return;

    Story_Run96(player, 8);
    Story_Do17(15);
    Story_Run97(185);

    *(s32 *)(target + 48) = 0x3333;
    *(s32 *)(target + 52) = 0x3333;
    Story_Run98(target, pos[0], pos[1], pos[2]);

    *(s32 *)(player + 48) = 0x3333;
    *(s32 *)(player + 52) = 0x3333;
    Story_Run99(player, pos[0], pos[1], pos[2]);

    Story_Run100(target);

    *(s32 *)(target + 8) = pos[0];
    *(s32 *)(target + 16) = pos[2];
    *(s32 *)(target + 36) = (s32)blocker;
    *(s32 *)(target + 44) = (s32)blocker;

    Story_Run101(player, 1);
    Story_Run102();
}

void Resource398_ImportBankNoOp(void)
{
    extern u8 *gWork;
    s32 Story_Run17(s32, s32);
    s32 Story_Run18(s32, s32, s32, s32, s32, s32);
    s32 *Story_Run19();
}
