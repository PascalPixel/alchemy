#include "types.h"
#include "scene.h"

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

u8 *Story_unk2_4();
s32 *Story_unk3_4();

u8 *Story_unk4_4();

u8 *Story_unk5_4();

u8 *Story_unk6_4();

u8 *Story_unk7_4();

u8 *Story_unk8_4();

u8 *Story_unk9_4();
u8 *Story_unk10_4();
u8 *Story_unk11_4();
u8 *Story_unk12_4();
u8 *Story_unk13_4();

u8 *Story_unk14_4();
u8 *Story_unk15_4();

u8 *Story_unk16_4();

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
    s32 Story_unk17_4(s32, s32);
    s32 Story_unk18_4(s32, s32, s32, s32, s32, s32);
    s32 *Story_unk19_4();

    s32 Story_unk20_4(s32, s32);

    Story_unk20_4(no, 0);
    return 0;
}

s32 SceneData_SelectByRuntimeSelector(void)
{
    extern u8 *gWork;
    s32 Story_unk17_4(s32, s32);
    s32 Story_unk18_4(s32, s32, s32, s32, s32, s32);
    s32 *Story_unk19_4();

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

s32 Story_unk21_4(void)
{
    extern u8 *gWork;
    s32 Story_unk17_4(s32, s32);
    s32 Story_unk18_4(s32, s32, s32, s32, s32, s32);
    s32 *Story_unk19_4();

    return 0;
}

u8 *SceneData_GetTable8bcc(void)
{
    extern u8 *gWork;
    s32 Story_unk17_4(s32, s32);
    s32 Story_unk18_4(s32, s32, s32, s32, s32, s32);
    s32 *Story_unk19_4();

    return (u8 *)0x02008bcc;
}

s32 SceneData_SelectSecondaryDataByRuntimeSelector(void)
{
    extern u8 *gWork;
    s32 Story_unk17_4(s32, s32);
    s32 Story_unk18_4(s32, s32, s32, s32, s32, s32);
    s32 *Story_unk19_4();

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
    s32 Story_unk17_4(s32, s32);
    s32 Story_unk18_4(s32, s32, s32, s32, s32, s32);
    s32 *Story_unk19_4();

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
    s32 Story_unk17_4(s32, s32);
    s32 Story_unk18_4(s32, s32, s32, s32, s32, s32);
    s32 *Story_unk19_4();

    s32 w = 21;
    s32 h = 14;

    Story_unk11_5(1, 0, 1, 1, w, h);
}

void State_ConfigureRegion0_0_21x14(void)
{
    extern u8 *gWork;
    s32 Story_unk17_4(s32, s32);
    s32 Story_unk18_4(s32, s32, s32, s32, s32, s32);
    s32 *Story_unk19_4();

    s32 w = 21;
    s32 h = 14;

    Story_unk12_5(0, 0, 1, 1, w, h);
}

void State_ApplyTwoRects(void)
{
    extern u8 *gWork;
    s32 Story_unk17_4(s32, s32);
    s32 Story_unk18_4(s32, s32, s32, s32, s32, s32);
    s32 *Story_unk19_4();

    {
        s32 a5 = 1;
        s32 a6 = 3;

        Story_unk13_5(111, 37, 97, 21, a5, a6);
    }
    {
        s32 a5 = 32;
        s32 a6 = 24;

        Story_unk14_5(46, 38, 3, 2, a5, a6);
    }
}

void Scene_RunTwoLayoutSteps(void)
{
    extern u8 *gWork;
    s32 Story_unk17_4(s32, s32);
    s32 Story_unk18_4(s32, s32, s32, s32, s32, s32);
    s32 *Story_unk19_4();

    {
        s32 fifth = 1;
        s32 sixth = 3;

        Story_unk15_5(95, 21, 97, 21, fifth, sixth);
    }
    {
        s32 fifth = 32;
        s32 sixth = 25;

        Story_unk16_5(46, 38, 3, 1, fifth, sixth);
    }
}

void Scene_RunActor9Flag882Scene(void)
{
    extern u8 *gWork;
    s32 Story_unk17_4(s32, s32);
    s32 Story_unk18_4(s32, s32, s32, s32, s32, s32);
    s32 *Story_unk19_4();

    Story_unk22_4();
    Story_unk3_3(9, 0, 0);
    Story_unk8_2(0x882);
    Story_unk23_4();
}

void Scene_RunScene398SequenceA(void)
{
    extern u8 gWork[];

    u32 i;
    u8 *record;

    Story_unk24_4();
    Story_unk25_4(8, 0, 0);
    Story_Do(0x883);
    Story_unk26_4(40);
    Story_unk27_4(15, 2);
    *(u8 *)(Story_unk10(15) + 85) = 0;
    {
        u8 *record = Story_unk4_4(15);
        u8 value = *(volatile u8 *)&record[35];

        record[35] = (u8)(value | 2);
    }
    Story_unk28_4(15, 2);
    Story_SetRect(0, 0, 1, 1, 18, 14);
    Story_unk29_4();
}

void Scene_RunActorFifteenScene(void)
{
    extern u8 *gWork;
    s32 Story_unk17_4(s32, s32);
    s32 Story_unk18_4(s32, s32, s32, s32, s32, s32);
    s32 *Story_unk19_4();

    void Story_unk30_4(s32);

    Story_unk31_4();
    Story_Apply(0xF, 0);
    Story_unk9_2(0x28);
    Story_unk30_4(0xD2);
    Story_Apply2(0xF, 6);
    Story_unk32_4();
}

void Scene_RunActorSixteenScene(void)
{
    extern u8 *gWork;
    s32 Story_unk17_4(s32, s32);
    s32 Story_unk18_4(s32, s32, s32, s32, s32, s32);
    s32 *Story_unk19_4();

    void Story_unk33_4(void);
    void Story_unk34_4(s32);

    Story_unk35_4();
    Story_Apply3(0x10, 0);
    Story_unk10_2(0x28);
    Story_unk34_4(0xD2);
    Story_Apply4(0x10, 6);
    Story_unk33_4();
}

void Scene_RunActor17Steps28AndD2(void)
{
    extern u8 *gWork;
    s32 Story_unk17_4(s32, s32);
    s32 Story_unk18_4(s32, s32, s32, s32, s32, s32);
    s32 *Story_unk19_4();

    void Story_unk36_4(void);

    Story_unk37_4();
    Story_unk17_4(0x11, 0);
    Story_unk11_2(0x28);
    Story_unk12_2(0xD2);
    Story_Apply5(0x11, 6);
    Story_unk36_4();
}

void Scene_RunScene398SequenceB(void)
{
    extern u8 gWork[];
    void Story_unk17_4();
    void Story_unk18_4();

    u32 i;
    s32 rec7;
    s32 rec8;
    s32 record;
    s32 v5;

    rec7 = Story_Check(11);
    rec8 = Story_unk11(12);
    if ((*(volatile s32 *)(rec7 + 8) >> 20) == 35) {
        if ((*(volatile s32 *)(rec7 + 16) >> 20) != 23) {
            goto L_02000330;
        }
        Story_unk2_2(0x303);
    } else {
        L_02000330:;
        Story_unk3_2(0x303);
    }
    if ((*(volatile s32 *)(rec8 + 8) >> 20) == 35) {
        if ((*(volatile s32 *)(rec8 + 16) >> 20) != 23) {
            goto L_02000350;
        }
        Story_unk4_2(0x304);
    } else {
        L_02000350:;
        Story_unk5_2(0x304);
    }
    if (Story_unk2(0x303) == 0) {
        record = Story_unk3(0x304);
        if (record == 0) {
            goto L_020003c2;
        }
    }
    if (Story_unk4(0x302) == 0) {
        Story_unk38_4();
        Story_unk39_4(40);
        Story_unk40_4(210);
        v5 = 36;
        Story_unk41_4(17, 6);
        Story_unk2_5(0, 1, 1, 1, v5, 22);
        Story_unk3_5(0, 2, 1, 1, v5, 24);
        Story_unk42_4();
    }
    Story_unk6_2(0x302);
    goto L_02000414;
    L_020003c2:;
    if (Story_unk5(0x302) != 0) {
        Story_unk43_4();
        Story_unk44_4(40);
        Story_unk45_4(220);
        v5 = 36;
        Story_unk46_4(17, 2);
        Story_unk4_5(1, 1, 1, 1, v5, 22);
        Story_unk5_5(1, 2, 1, 1, v5, 24);
        Story_unk47_4();
    }
    Story_unk7_2(0x302);
    L_02000414:;
}

void ActorDraw_SetSceneCell31AndFlag305(void)
{
    extern u8 *gWork;
    s32 Story_unk17_4(s32, s32);
    s32 Story_unk18_4(s32, s32, s32, s32, s32, s32);
    s32 *Story_unk19_4();

    s32 width = 8;
    s32 height = 13;

    Story_unk18_4(31, 0, 1, 1, width, height);
    Story_unk13_2(0x305);
}

void State_SetGlobalByte17(void)
{
    extern u8 *gWork;
    s32 Story_unk17_4(s32, s32);
    s32 Story_unk18_4(s32, s32, s32, s32, s32, s32);
    s32 *Story_unk19_4();

    FIELD_AT_OFFSET(*(void **)0x03001E70, s8 *, 0x17) = 1;
}

void State_ClearRuntimeByte17(void)
{
    extern u8 *gWork;
    s32 Story_unk17_4(s32, s32);
    s32 Story_unk18_4(s32, s32, s32, s32, s32, s32);
    s32 *Story_unk19_4();

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
    s32 Story_unk17_4(s32, s32);
    s32 Story_unk18_4(s32, s32, s32, s32, s32, s32);
    s32 *Story_unk19_4();

    void Story_unk48_4(void);

    s16 variant = RuntimeSelectorTable[224];

    if (variant == (s32)&Value_00000031) {
        Story_unk49_4();
    } else if (variant == (s32)&Value_00000030) {
        Story_unk48_4();
    } else if (variant == (s32)&Value_0000002f) {
        Story_unk50_4();
    }
    return 0;
}

void RunGuardedSceneSetup(void)
{
    extern u8 *gWork;
    s32 Story_unk17_4(s32, s32);
    s32 Story_unk18_4(s32, s32, s32, s32, s32, s32);
    s32 *Story_unk19_4();

    if (RunSceneCommand(0x305) != 0) {
        s32 width = 8;
        s32 height = 13;

        Story_unk17_5(31, 0, 1, 1, width, height);
        FinishSceneSetup(8, 0);
    }
}

void State_SetRuntimeWord448To516(void)
{
    extern u8 *gWork;
    s32 Story_unk17_4(s32, s32);
    s32 Story_unk18_4(s32, s32, s32, s32, s32, s32);
    s32 *Story_unk19_4();

    /* 448 is built as 224 << 1 and the stored 516 as that same register plus
     * 68; the two are not one running offset. */
    *(s32 *)(gWork + 448) = 516;

    Story_Apply6(8, 1);
    Story_Apply7(10, 2);

    if (Story_unk12(0x882) != 0) {
        Story_unk4_3(9, 0, 0);
    } else {
        Story_Apply8(Story_Run(9), 0);
    }
}

void Scene_RunScene398SequenceC(void)
{
    extern u8 gWork[];
    void Story_unk19_4();

    u32 i;
    u8 *record;
    s32 v5;

    *(s32 *)((*(u8 *volatile *)gWork + 0x1c0)) = 0x204;
    record = Story_unk5_4(18);
    Story_unk51_4((s32)record, 0);
    record = Story_unk6_4(19);
    Story_unk52_3((s32)record, 0);
    record = Story_unk7_4(20);
    Story_unk53_3((s32)record, 0);
    record = Story_unk8_4(21);
    Story_unk54_3((s32)record, 0);
    record = Story_unk9_4(22);
    Story_unk55_3((s32)record, 0);
    record = Story_unk10_4(23);
    Story_unk56_3((s32)record, 0);
    record = Story_unk11_4(24);
    Story_unk57_3((s32)record, 0);
    record = Story_unk12_4(25);
    Story_unk58_3((s32)record, 0);
    record = Story_unk13_4(26);
    Story_unk59_3((s32)record, 0);
    Story_unk60_3(18, 5);
    Story_unk61_3(19, 5);
    Story_unk62_3(20, 5);
    Story_unk63_3(21, 5);
    Story_unk64_3(22, 5);
    Story_unk65_3(23, 3);
    Story_unk66_3(24, 3);
    Story_unk67_3(25, 3);
    Story_unk68_3(26, 3);
    Story_unk69_3(9, 2);
    Story_unk70_3(10, 2);
    Story_unk71_3(11, 2);
    Story_unk72_3(12, 2);
    Story_unk73_3(13, 2);
    Story_unk74_3(14, 2);
    Story_unk75_3(18);
    Story_unk76_3(19);
    Story_unk77_3(20);
    Story_unk78_3(21);
    Story_unk79_3(22);
    Story_unk14_2(23);
    Story_unk80_3(24);
    Story_unk81_3(25);
    Story_unk82_3(26);
    Story_unk15_2(9);
    Story_unk83_3(10);
    Story_unk84_3(11);
    Story_unk85_3(12);
    Story_unk16_2(13);
    Story_unk86_3(14);
    if (Story_unk6(0x883) != 0) {
        Story_unk87_3(8, 0, 0);
        Story_unk88_3(15, 5);
        *(u8 *)(Story_unk13(15) + 85) = 0;
        record = Story_unk14_4(15);
        *(volatile s32 *)((s32)record + 12) = -0x40000;
        {
            u8 *record = Story_unk15_4(15);
            u8 value = *(volatile u8 *)&record[35];

            record[35] = (u8)(value | 2);
        }
        Story_unk89_3(15, 2);
        Story_unk6_5(0, 0, 1, 1, 18, 14);
    } else {
        Story_unk90_3(8, 2);
        record = Story_unk16_4(8);
        Story_unk91_3((s32)record, 0);
        Story_unk92_3(15, 1);
    }
    Story_unk93_3(16, 1);
    if (Story_unk7(0x302) != 0) {
        v5 = 36;
        Story_unk94_3(17, 1);
        Story_unk7_5(0, 1, 1, 1, v5, 22);
        Story_unk8_5(0, 2, 1, 1, v5, 24);
    } else {
        v5 = 36;
        Story_unk95_3(17, 5);
        Story_unk9_5(1, 1, 1, 1, v5, 22);
        Story_unk10_5(1, 2, 1, 1, v5, 24);
    }
    if (Story_unk8(0x303) != 0) {
        Story_Place(11, 0x23a0000, 0x1780000);
    }
    if (Story_unk9(0x304) != 0) {
        Story_unk2_3(12, 0x23a0000, 0x1780000);
    }
}

s32 *Actor_FindSlotAtTile(s32 x, s32 z)
{
    extern u8 *gWork;
    s32 Story_unk17_4(s32, s32);
    s32 Story_unk18_4(s32, s32, s32, s32, s32, s32);
    s32 *Story_unk19_4();

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
    s32 Story_unk17_4(s32, s32);
    s32 Story_unk18_4(s32, s32, s32, s32, s32, s32);
    s32 *Story_unk19_4();

    u8 *player;
    u8 *target;
    u8 *blocker;
    s32 heading;
    s32 tx;
    s32 tz;
    s32 pos[3];

    player = Story_unk2_4(0);
    heading = *(u16 *)(player + 6) >> 12;

    tx = (*(s16 *)(player + 10)
        + (gOv[heading] >> 16)) >> 4;
    tz = (*(s16 *)(player + 18)
        + ((gOv[heading] << 16) >> 16)) >> 4;
    target = (u8 *)Story_unk3_4(tx, tz);
    if (target == 0) return;

    tx = (*(s16 *)(target + 10)
        + (gOv[heading] >> 16)) >> 4;
    tz = (*(s16 *)(target + 18)
        + ((gOv[heading] << 16) >> 16)) >> 4;
    blocker = (u8 *)Story_unk19_4(tx, tz);
    if (blocker != 0) return;

    target[0x22] = 2;

    pos[0] = *(s32 *)(target + 8)
        + (gOv[heading] & (s32)0xffff0000);
    pos[1] = *(s32 *)(target + 12);
    pos[2] = *(s32 *)(target + 16) + (gOv[heading] << 16);

    if (Story_unk14(target, pos) > 0) return;

    Story_unk96_3(player, 8);
    Story_unk17_2(15);
    Story_unk97_3(185);

    *(s32 *)(target + 48) = 0x3333;
    *(s32 *)(target + 52) = 0x3333;
    Story_unk98_3(target, pos[0], pos[1], pos[2]);

    *(s32 *)(player + 48) = 0x3333;
    *(s32 *)(player + 52) = 0x3333;
    Story_unk99_3(player, pos[0], pos[1], pos[2]);

    Story_unk100_3(target);

    *(s32 *)(target + 8) = pos[0];
    *(s32 *)(target + 16) = pos[2];
    *(s32 *)(target + 36) = (s32)blocker;
    *(s32 *)(target + 44) = (s32)blocker;

    Story_unk101_3(player, 1);
    Story_unk102_3();
}

void Resource398_ImportBankNoOp(void)
{
    extern u8 *gWork;
    s32 Story_unk17_4(s32, s32);
    s32 Story_unk18_4(s32, s32, s32, s32, s32, s32);
    s32 *Story_unk19_4();
}
