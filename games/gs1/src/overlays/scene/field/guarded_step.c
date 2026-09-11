#include "types.h"
#include "scene.h"

#define TARGET_ID 9
#define GATE_CODE 2059

extern u8 SceneEventRuntime_ScriptData[];
extern u8 SceneEventRuntime_MessageData[];
extern u8 SceneEventRuntime_ActorData[];
extern u8 SceneEventRuntime_EffectData[];
extern struct Cam *gCam;
extern s32 gEntCnt;
extern u8 gCell[];
extern u8 gWork[];
extern u16 gCell2[][1];
extern s32 gEntTbl[];

u16 *Field_unk8_4(s32);

u16 *Field_unk9_4(s32);

u8 *Field_unk10_4();

u8 *Field_unk11_4(s32 index);

s32 *Field_unk12_4(s32);

s32 *Field_unk13_4(s32);
s32 *Field_unk14_4(s32);

s32 *Field_unk15_4(s32);

/* Scene-record accessor. */

static __inline__ void ConfigureScene(s32 actor, s32 x, s32 y, s32 mode)
{
    Field_SetMode(actor, x, y, mode);
}

static __inline__ void ConfigureScene2(s32 actor, s32 x, s32 y, s32 mode)
{
    Field_unk16_4(actor, x, y, mode);
}

u8 *SceneEventRuntime_GetScriptData(void)
{
    return SceneEventRuntime_ScriptData;
}

s32 SceneEventRuntime_ReturnZero(void)
{
    return 0;
}

u8 *SceneEventRuntime_GetMessageData(void)
{
    return SceneEventRuntime_MessageData;
}

u8 *SceneEventRuntime_GetActorData(void)
{
    return SceneEventRuntime_ActorData;
}

u8 *SceneEventRuntime_GetEffectData(void)
{
    return SceneEventRuntime_EffectData;
}

s32 SceneEventRuntime_SelectInitialSceneByFlags(void)
{
    s32 no;

    if (Field_unk28(0x818) != 0) {
        if (Field_unk29(0x813) == 0) {
            no = 3;
            goto apply;
        }
        goto fail;
    }
    if (Field_unk30(0x812) == 0) {
        no = 4;
apply:
        Field_unk6_2(no);
        return 1;
    }
fail:
    return -1;
}

void Scene_RunLoopedLayoutSequence(void)
{
    s32 i;

    { s32 k5 = 2, k6 = 1; Field_unk27_5(0, 28, 17, 8, k5, k6); }
    Field_unk7_2(200);
    for (i = 0; i != 22; i++) {
        Field_unk28_5(10, 61, 17, 40, 2, 1);
        Field_unk8_2(4);
        Field_unk29_5(8, 61, 17, 40, 2, 1);
        Field_unk9_2(4);
    }
    { s32 k5 = 4, k6 = 3;
      Field_unk30_5(0, 59, 15, 38, k5, k6);
      Field_unk31_5(4, 59, 17, 38, k5, k6); }
    Field_unk32_5(8, 60, 17, 39, 2, 2);
    { s32 k5 = 17, k6 = 8; Field_unk33_5(0, 0, 2, 1, k5, k6); }
    Field_unk10_2(0x207);
    Field_unk17_4();
}

void Effect_UpdateScrollingSpriteRows(void)
{
    extern struct Ent gFarStub[];

    s32 *cp = &gCam->x;
    struct Ent *e = gFarStub;
    s32 sx = cp[0] / 65536;
    s32 sy = 80 - cp[1] / 65536;
    s32 v;
    u32 i;

    if ((u32)(sy + 16) <= 175) {
        v = (gEntCnt >> 10) - sx;
        v |= -32;
        {
            for (i = 0; i <= 8; i++) {
                e->f06 = v;
                e->f04 = sy;
                Field_EntOp(e, 0);
                v += 32;
                e++;
            }
        }
        v = (gEntCnt >> 9) - sx;
        v |= -32;
        {
            for (i = 0; i <= 8; i++) {
                e->f06 = v;
                e->f04 = sy;
                Field_EntOp2(e, 0);
                v += 32;
                e++;
            }
        }
        v = (gEntCnt >> 8) - sx;
        v |= -32;
        {
            for (i = 0; i <= 8; i++) {
                e->f06 = v;
                e->f04 = sy + 8;
                Field_EntOp3(e, 0);
                v += 32;
                e++;
            }
        }
    }
    gEntCnt += 0x80;
}

void State_RunWhenSlotZeroFacingC000(void)
{
    u16 *p = Field_unk8_4(0);
    if (p[3] == 0xc000) {
        Field_unk18_4();
    }
}

void State_RunWhenActorZeroFacing4000(void)
{
    u16 *p = Field_unk9_4(0);
    if (p[3] == 0x4000) {
        Field_unk19_4();
    }
}

/* If the code-2059 check passes, runs a short setup/configuration sequence
 * for id 9: two no-argument calls bracket a select call and two calls each
 * taking a pair of numeric arguments. */
void Scene_RunPrimarySequenceHead(void)
{
    extern u8 gFarStub[];

    if (GameFlag_IsSet_1(GATE_CODE) == 0) {
        Battle_Reset_1();
        Scene_GetRecord_1(TARGET_ID);
        Motion_SetSpeed_1(TARGET_ID, 13107, 0x00001999); /* object_id, speed_limit, acceleration */
        Motion_SetPosReset_1(TARGET_ID, 504, 152); /* object_id, x=504, z=152 */
        Battle_SchedShoulder_1();
    }
}

s32 Field_unk20_4(void)
{
    extern u8 gFarStub[];

    u8 *record;

    *(s32 *)((*(u8 *volatile *)gWork + 0x1c0)) = 0x204;
    Field_unk21_4();
    Field_Do(0x144);
    record = (u8 *)Field_Check(18);
    record[89] = 0;
    record += 35;
    {
        u8 value = *(volatile u8 *)record;

        *record = (u8)(value | 2);
    }
    Field_unk22_4((s32)Field_unk10_4(18), 0);
    *(u8 *)(Field_unk31(18) + 35) &= 254;
    Field_unk23_4(18, 1);
    if ((u32)((gCell2[225][0] - 3) << 16) > 0x10000) {
        Field_unk24_4(5, 0, 0);
        Field_unk25_4(1, 0, 0);
    }
    if (Field_unk2(0x818) != 0) {
        Field_Place(18, 0x1200000, 0xb20000);
        Field_unk2_3(17, 0x6480000, 0x6480000);
        Field_unk3_3(10, 0xe80000, 0x780000);
        Field_unk4_3(12, 0x1580000, 0x780000);
        Field_unk5_3(10, 0xe80000, 0x780000);
        Field_SetRect(0, 59, 15, 38, 4, 3);
        Field_unk6_3(12, 0x1580000, 0x780000);
        Field_unk2_6(4, 59, 17, 38, 4, 3);
        Field_unk3_6(8, 60, 17, 39, 2, 2);
        Field_unk4_6(0, 1, 2, 1, 17, 7);
    } else if (Field_unk3(0x816) != 0
                && Field_unk4(0x817) != 0) {
        Field_unk7_3(10, 0xe80000, 0x780000);
        Field_unk8_3(12, 0x1580000, 0x780000);
        Field_unk5_6(0, 28, 17, 8, 2, 1);
        Field_unk9_3(10, 0xe80000, 0x780000);
        Field_unk6_6(0, 59, 15, 38, 4, 3);
        Field_unk10_3(12, 0x1580000, 0x780000);
        Field_unk7_6(4, 59, 17, 38, 4, 3);
        Field_unk8_6(8, 60, 17, 39, 2, 2);
        Field_unk4_6(0, 0, 2, 1, 17, 8);
    } else {
        if (Field_unk5(0x816) != 0) {
            Field_unk11_3(10, 0xe80000, 0x780000);
            Field_unk9_6(0, 59, 15, 38, 4, 3);
        }
        if (Field_unk6(0x817) != 0) {
            Field_unk12_3(12, 0x1580000, 0x780000);
            Field_unk10_5(4, 59, 17, 38, 4, 3);
        }
    }
    if (Field_unk7(0x80b) != 0) {
        Field_unk13_3(9, 0x1f80000, 0x980000);
        Field_unk11_5(2, 28, 34, 10, 2, 1);
        Field_unk12_5(2, 30, 16, 10, 2, 1);
        Field_unk13_5(0, 55, 32, 40, 4, 3);
    }
    if (Field_unk8(0x80c) != 0) {
        Field_unk14_3(11, 0x2880000, 0x980000);
        Field_unk14_5(4, 28, 36, 10, 2, 1);
        Field_unk15_5(4, 30, 18, 10, 2, 1);
        Field_unk16_5(4, 55, 36, 40, 4, 3);
    }
    if (Field_unk9(0x80d) != 0) {
        Field_unk15_3(13, 0x1f80000, 0xc80000);
        Field_unk17_5(2, 29, 34, 11, 2, 1);
        Field_unk18_5(2, 31, 16, 11, 2, 1);
        Field_unk19_5(0, 58, 32, 43, 4, 1);
    }
    if (Field_unk10(0x80e) != 0) {
        Field_unk16_3(15, 0x2880000, 0xc80000);
        Field_unk20_5(4, 29, 36, 11, 2, 1);
        Field_unk21_5(4, 31, 18, 11, 2, 1);
        Field_unk22_5(4, 58, 36, 43, 4, 1);
    }
    {
    s16 *state = (s16 *)gCell2;

    if (state[225] == 3) {
        if (Field_unk11(0x30a) != 0) {
            Field_unk26_4(1, 0, 0);
            Field_unk27_4(5, 0, 0);
        } else if (Field_unk12(0x109) == 0) {
            Field_unk28_4();
            Field_unk2_2(0x30a);
        }
    }
    if (state[225] == 4) {
        if (Field_unk13(0x30b) != 0) {
            Field_unk29_4(1, 0, 0);
            Field_unk30_4(5, 0, 0);
        } else if (Field_unk14(0x109) == 0) {
            Field_unk31_4();
            Field_unk3_2(0x30b);
        }
    }
    }
    if (Field_unk15(0x814) != 0) {
        Field_unk32_4(141);
        Field_unk17_3(0x10000, 0x10000, 0x10000);
        Field_unk33_4();
    }
    return 0;
}

void Field_unk34_4(void)
{
    extern u8 gFarStub[];

    u32 i;
    s32 value;
    volatile s32 *p;
    s32 buf;

    p = (volatile s32 *)gFarStub;
    buf = Field_unk16(14, 0x400);
    Field_Run(0x200a56c, buf);
    value = Field_unk32(Field_unk33(), 128, buf);
    for (i = 0; i < 9; i++) {
        volatile s32 *q = p;

        *q++ = 0;
        *q++ = 0x40004000;
        p += 3;
        *q = value | 0xac00;
    }
    value = Field_unk34(Field_unk35(), 128, buf + 128);
    for (i = 0; i < 9; i++) {
        volatile s32 *q = p;

        *q++ = 0;
        *q++ = 0x40004000;
        p += 3;
        *q = value | 0xdc00;
    }
    value = Field_unk36(Field_unk37(), 128, buf + 0x100);
    for (i = 0; i < 9; i++) {
        volatile s32 *q = p;

        *q++ = 0;
        *q++ = 0x40004000;
        p += 3;
        *q = value | 0xc00;
    }
    Field_unk35_4(14);
    {
        s32 size = 0xc80;

        Field_unk36_4(0x2008eb1, size);
    }
}

void Scene_CallWhenCheck9_31_9(void)
{
    if (Field_unk22_3(9, 31, 9) != 0) {
        Field_unk37_4();
    }
}

void Scene_RunGuardedStep11(void)
{
    if (Field_unk23_3(11, 40, 9) != 0) {
        Field_unk38_4();
    }
}

void Scene_RunGuardedStep13(void)
{
    if (Field_unk24_3(13, 31, 12) != 0) {
        Field_unk39_4();
    }
}

void Scene_RunGuardedStep15(void)
{
    if (Field_unk25_3(15, 40, 12) != 0) {
        Field_unk40_4();
    }
}

void ConfigureSceneAndCheckActors(void)
{
    ConfigureScene(2, 0x00d00000, 0x00700000, 0);
    if (Field_unk38(10, 14, 7) != 0) {
        Field_unk41_4();
    }
}

    if (Field_unk39(12, 21, 7) != 0) {
        Field_unk42_4();
    }
}

void Scene_RunClosingSequence(void)
{
    extern u8 gFarStub[];

    s32 first;
    s32 kind;
    s32 second;

    first = Field_unk17(0);
    kind = *(volatile s32 *)(first + 8) >> 20;
    second = Field_unk18(0);
    if ((*(volatile s32 *)(second + 16) >> 20) == 8) {
        if ((u32)(kind - 17) <= 1) {
            Field_unk2_4(2, 0x1100000, 0x800000, 255);
            Field_unk3_4(2, 0x1200000, 0x800000, 255);
        }
    }
}

void Scene_RunGuardSequenceA(void)
{
    extern u8 gFarStub[];

    u32 i;
    s32 record;

    record = Field_unk19(17);
    if (record != 0) {
        if ((*(volatile s32 *)(record + 16) >> 20) == 8) {
            Field_unk43_4();
            Field_unk44_4(185);
            Field_unk18_3(17, 0x3333, 0x1999);
            Field_unk19_3(0, 0x3333, 0x1999);
            *(u8 *)(Field_unk40(17) + 90) &= 254;
            Field_unk45_4(0, 8);
            record = Field_unk41(0);
            Field_unk46_4(0, *(s16 *)(record + 10), 136);
            Field_unk20_3(17, 0x120, 120);
            Field_unk47_4(17);
            Field_unk48_4(0, 1);
            Field_unk49_4();
        }
    }
}

void Scene_RunFiveValueStep9(void)
{
    extern u8 gFarStub[];

    Field_SetRange(9, 31, 9, 30, 9);
    Field_unk50_4();
}

void Scene_RunFiveValueStep11(void)
{
    extern u8 gFarStub[];

    Field_unk2_5(11, 40, 9, 41, 9);
    Field_unk51_4();
}

void Scene_ApplyRect13_31_12_30_12(void)
{
    Field_unk3_5(13, 31, 12, 30, 12);
    Field_unk52_4();
}

void Scene_RunFiveValueStep15(void)
{
    extern u8 gFarStub[];

    Field_unk4_5(15, 40, 12, 41, 12);
    Field_unk53_4();
}

void Scene_ApplyRect10_14_7_13_7(void)
{
    Field_unk5_5(10, 14, 7, 13, 7);
    Field_unk54_4();
}

/*
 * Fetches scene record 10 and, when it exists, hands a coarse coordinate
 * derived from it to a five-argument routine, which receives both the
 * coordinate and the coordinate plus one; the fifth argument travels on the
 * stack. The `>> 20` reduction to a cell index is by analogy with the rest of
 * the tree and is not verified, and the repeated 13 is as written.
 */
void Actor_UseActorTenCellAndNext(void)
{
    u8 *record = Field_unk11_4(10);
    s32 cell;

    if (record == 0) {
        return;
    }

    cell = *(s32 *)(record + 16) >> 20;
    Field_unk55_4(10, 13, cell + 1, 13, cell);
}

void Actor_MoveActor10ByRow(void)
{
    s32 *p = Field_unk14_4(10);
    if (p != NULL) {
        s32 v = p[4] >> 20;
        Field_unk6_5(10, 13, v - 1, 13, v);
    }
}

void Scene_ApplyRect12_21_7_22_7(void)
{
    Field_unk7_5(12, 21, 7, 22, 7);
    Field_unk56_4();
}

void Actor_ApplyActorTwelveZCellPair(void)
{
    s32 *p = Field_unk12_4(12);
    if (p != NULL) {
        s32 v = p[4] >> 20;
        Field_unk8_5(12, 22, v + 1, 22, v);
    }
}

void Actor_RunSlot12ColumnStep(void)
{
    s32 *p = Field_unk15_4(12);
    if (p != NULL) {
        s32 v = p[4] >> 20;
        Field_unk9_5(12, 22, v - 1, 22, v);
    }
}

s32 Actor_IsActorAtTile(s32 no, s32 x, s32 z)
{
    s32 *p = Field_unk13_4(no);
    if (p == NULL || (p[2] >> 20) != x) {
        return 0;
    }
    if ((p[4] >> 20) != z) {
        return 0;
    }
    return 1;
}

void SceneData_InitEntTbl(void)
{
    s32 *p = gEntTbl;
    p[0] = 0;
    p[1] = 55;
    p[2] = 32;
    p[3] = 40;
    p[4] = 4;
    p[5] = 3;
    p[6] = 2;
    p[7] = 30;
    p[8] = 34;
    p[9] = 10;
    p[10] = 2;
    p[11] = 1;
    p[12] = 2;
    p[13] = 28;
    p[14] = 34;
    p[15] = 10;
    p[16] = 2;
    p[17] = 1;
    p[18] = 2;
    p[19] = 30;
    p[20] = 16;
    p[21] = 10;
    p[22] = 2;
    p[23] = 1;
    p[24] = 0x80b;
    p[25] = 0x4000;
    p[26] = 500;
    p[27] = 132;
    p[28] = 8;
    p[29] = 55;
    p[30] = 32;
    p[31] = 40;
    p[32] = 4;
    p[33] = 3;
    p[34] = 2;
    p[35] = 30;
    p[36] = 34;
    p[37] = 10;
    p[38] = 2;
    p[39] = 1;
    p[40] = 2;
    p[41] = 28;
    p[42] = 16;
    p[43] = 10;
    p[44] = 2;
    p[45] = 1;
    p[46] = 9;
    p[47] = 488;
    p[48] = 152;
    Field_unk57_4();
}

void SceneData_FillEntTbl(void)
{
    s32 *p = gEntTbl;
    p[0] = 4;
    p[1] = 55;
    p[2] = 36;
    p[3] = 40;
    p[4] = 4;
    p[5] = 3;
    p[6] = 4;
    p[7] = 30;
    p[8] = 36;
    p[9] = 10;
    p[10] = 2;
    p[11] = 1;
    p[12] = 4;
    p[13] = 28;
    p[14] = 36;
    p[15] = 10;
    p[16] = 2;
    p[17] = 1;
    p[18] = 4;
    p[19] = 30;
    p[20] = 18;
    p[21] = 10;
    p[22] = 2;
    p[23] = 1;
    p[24] = 0x80c;
    p[25] = 0x4000;
    p[26] = 654;
    p[27] = 132;
    p[28] = 12;
    p[29] = 55;
    p[30] = 36;
    p[31] = 40;
    p[32] = 4;
    p[33] = 3;
    p[34] = 4;
    p[35] = 30;
    p[36] = 36;
    p[37] = 10;
    p[38] = 2;
    p[39] = 1;
    p[40] = 4;
    p[41] = 28;
    p[42] = 18;
    p[43] = 10;
    p[44] = 2;
    p[45] = 1;
    p[46] = 11;
    p[47] = 664;
    p[48] = 152;
    Field_unk58_4();
}

void SceneData_InitEntTblAndRunB(void)
{
    s32 *p = gEntTbl;
    p[0] = 0;
    p[1] = 58;
    p[2] = 32;
    p[3] = 43;
    p[4] = 4;
    p[5] = 1;
    p[6] = 2;
    p[7] = 31;
    p[8] = 34;
    p[9] = 11;
    p[10] = 2;
    p[11] = 1;
    p[12] = 2;
    p[13] = 29;
    p[14] = 34;
    p[15] = 11;
    p[16] = 2;
    p[17] = 1;
    p[18] = 2;
    p[19] = 31;
    p[20] = 16;
    p[21] = 11;
    p[22] = 2;
    p[23] = 1;
    p[24] = 0x80d;
    p[25] = 0xc000;
    p[26] = 500;
    p[27] = 216;
    p[28] = 8;
    p[29] = 58;
    p[30] = 32;
    p[31] = 43;
    p[32] = 4;
    p[33] = 1;
    p[34] = 2;
    p[35] = 31;
    p[36] = 34;
    p[37] = 11;
    p[38] = 2;
    p[39] = 1;
    p[40] = 2;
    p[41] = 29;
    p[42] = 16;
    p[43] = 11;
    p[44] = 2;
    p[45] = 1;
    p[46] = 13;
    p[47] = 488;
    p[48] = 200;
    Field_unk59_4();
}

void SceneData_BuildEntTbl(void)
{
    s32 *p = gEntTbl;
    p[0] = 4;
    p[1] = 58;
    p[2] = 36;
    p[3] = 43;
    p[4] = 4;
    p[5] = 1;
    p[6] = 4;
    p[7] = 31;
    p[8] = 36;
    p[9] = 11;
    p[10] = 2;
    p[11] = 1;
    p[12] = 4;
    p[13] = 29;
    p[14] = 36;
    p[15] = 11;
    p[16] = 2;
    p[17] = 1;
    p[18] = 4;
    p[19] = 31;
    p[20] = 18;
    p[21] = 11;
    p[22] = 2;
    p[23] = 1;
    p[24] = 0x80e;
    p[25] = 0xc000;
    p[26] = 0x28e;
    p[27] = 216;
    p[28] = 12;
    p[29] = 58;
    p[30] = 36;
    p[31] = 43;
    p[32] = 4;
    p[33] = 1;
    p[34] = 4;
    p[35] = 31;
    p[36] = 36;
    p[37] = 11;
    p[38] = 2;
    p[39] = 1;
    p[40] = 4;
    p[41] = 29;
    p[42] = 18;
    p[43] = 11;
    p[44] = 2;
    p[45] = 1;
    p[46] = 15;
    p[47] = 664;
    p[48] = 200;
    Field_unk60_4();
}

void Scene_RunGuard(void)
{
    extern u8 gFarStub[];

    u32 i;
    s32 record;

    Field_unk61_4();
    if (Field_unk20(0x818) == 0) {
        if (Field_unk21(0x816) == 0) {
            Field_unk4_4(0x20000, 0x4000);
            Field_unk5_4(0x11e0000, -1, 0x920000, 1);
            Field_unk62_4();
            Field_unk63_4(186);
            Field_unk23_5(0, 59, 15, 38, 4, 3);
            if (Field_unk22(0x817) != 0) {
                Field_unk24_5(8, 60, 17, 39, 2, 2);
            }
            Field_unk64_4(0, 0, 0);
            Field_unk65_4(30);
            Field_unk4_2(0x816);
            if (Field_unk23(0x817) != 0) {
                Field_unk66_4();
            }
        }
    }
    Field_unk67_4();
}

void Scene_RunGuard(void)
{
    extern u8 gFarStub[];

    u32 i;
    s32 record;

    Field_unk68_4();
    if (Field_unk24(0x818) == 0) {
        if (Field_unk25(0x817) == 0) {
            Field_unk6_4(0x20000, 0x4000);
            Field_unk7_4(0x11e0000, -1, 0x920000, 1);
            Field_unk69_4();
            Field_unk70_4(186);
            Field_unk25_5(4, 59, 17, 38, 4, 3);
            if (Field_unk26(0x816) != 0) {
                Field_unk26_5(8, 60, 17, 39, 2, 2);
            }
            Field_unk21_3(0, 0x8000, 0);
            Field_unk71_4(30);
            Field_unk5_2(0x817);
            if (Field_unk27(0x816) != 0) {
                Field_unk72_4();
            }
        }
    }
    Field_unk73_4();
}

void Scene_RunSplitPairSteps(s32 a, s32 b)
{
    Field_Apply(a, 0);
    Field_unk11_2(b);
}
