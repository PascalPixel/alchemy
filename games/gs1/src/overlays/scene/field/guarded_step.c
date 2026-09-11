#include "types.h"
#include "scene.h"
#include "abi/overlays/scene/field/guarded_step.h"

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

u16 *Field_Run8(s32);

u16 *Field_Run9(s32);

u8 *Field_Run10();

u8 *Field_Run11(s32 index);

s32 *Field_Run12(s32);

s32 *Field_Run13(s32);
s32 *Field_Run14(s32);

s32 *Field_Run15(s32);

/* Scene-record accessor. */

static __inline__ void ConfigureScene(s32 actor, s32 x, s32 y, s32 mode)
{
    Field_SetMode(actor, x, y, mode);
}

static __inline__ void ConfigureScene2(s32 actor, s32 x, s32 y, s32 mode)
{
    Field_Run16(actor, x, y, mode);
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

    if (Field_Check28(0x818) != 0) {
        if (Field_Check29(0x813) == 0) {
            no = 3;
            goto apply;
        }
        goto fail;
    }
    if (Field_Check30(0x812) == 0) {
        no = 4;
apply:
        Field_Do6(no);
        return 1;
    }
fail:
    return -1;
}

void Scene_RunLoopedLayoutSequence(void)
{
    s32 i;

    { s32 k5 = 2, k6 = 1; Field_SetRect27(0, 28, 17, 8, k5, k6); }
    Field_Do7(200);
    for (i = 0; i != 22; i++) {
        Field_SetRect28(10, 61, 17, 40, 2, 1);
        Field_Do8(4);
        Field_SetRect29(8, 61, 17, 40, 2, 1);
        Field_Do9(4);
    }
    { s32 k5 = 4, k6 = 3;
      Field_SetRect30(0, 59, 15, 38, k5, k6);
      Field_SetRect31(4, 59, 17, 38, k5, k6); }
    Field_SetRect32(8, 60, 17, 39, 2, 2);
    { s32 k5 = 17, k6 = 8; Field_SetRect33(0, 0, 2, 1, k5, k6); }
    Field_Do10(0x207);
    Field_Run17();
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
    u16 *p = Field_Run8(0);
    if (p[3] == 0xc000) {
        Field_Run18();
    }
}

void State_RunWhenActorZeroFacing4000(void)
{
    u16 *p = Field_Run9(0);
    if (p[3] == 0x4000) {
        Field_Run19();
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

s32 Field_Run20(void)
{
    extern u8 gFarStub[];

    u8 *record;

    *(s32 *)((*(u8 *volatile *)gWork + 0x1c0)) = 0x204;
    Field_Run21();
    Field_Do(0x144);
    record = (u8 *)Field_Check(18);
    record[89] = 0;
    record += 35;
    {
        u8 value = *(volatile u8 *)record;

        *record = (u8)(value | 2);
    }
    Field_Run22((s32)Field_Run10(18), 0);
    *(u8 *)(Field_Check31(18) + 35) &= 254;
    Field_Run23(18, 1);
    if ((u32)((gCell2[225][0] - 3) << 16) > 0x10000) {
        Field_Run24(5, 0, 0);
        Field_Run25(1, 0, 0);
    }
    if (Field_Check2(0x818) != 0) {
        Field_Place(18, 0x1200000, 0xb20000);
        Field_Place2(17, 0x6480000, 0x6480000);
        Field_Place3(10, 0xe80000, 0x780000);
        Field_Place4(12, 0x1580000, 0x780000);
        Field_Place5(10, 0xe80000, 0x780000);
        Field_SetRect(0, 59, 15, 38, 4, 3);
        Field_Place6(12, 0x1580000, 0x780000);
        Field_SetRect2(4, 59, 17, 38, 4, 3);
        Field_SetRect3(8, 60, 17, 39, 2, 2);
        Field_SetRect4(0, 1, 2, 1, 17, 7);
    } else if (Field_Check3(0x816) != 0
                && Field_Check4(0x817) != 0) {
        Field_Place7(10, 0xe80000, 0x780000);
        Field_Place8(12, 0x1580000, 0x780000);
        Field_SetRect5(0, 28, 17, 8, 2, 1);
        Field_Place9(10, 0xe80000, 0x780000);
        Field_SetRect6(0, 59, 15, 38, 4, 3);
        Field_Place10(12, 0x1580000, 0x780000);
        Field_SetRect7(4, 59, 17, 38, 4, 3);
        Field_SetRect8(8, 60, 17, 39, 2, 2);
        Field_SetRect4(0, 0, 2, 1, 17, 8);
    } else {
        if (Field_Check5(0x816) != 0) {
            Field_Place11(10, 0xe80000, 0x780000);
            Field_SetRect9(0, 59, 15, 38, 4, 3);
        }
        if (Field_Check6(0x817) != 0) {
            Field_Place12(12, 0x1580000, 0x780000);
            Field_SetRect10(4, 59, 17, 38, 4, 3);
        }
    }
    if (Field_Check7(0x80b) != 0) {
        Field_Place13(9, 0x1f80000, 0x980000);
        Field_SetRect11(2, 28, 34, 10, 2, 1);
        Field_SetRect12(2, 30, 16, 10, 2, 1);
        Field_SetRect13(0, 55, 32, 40, 4, 3);
    }
    if (Field_Check8(0x80c) != 0) {
        Field_Place14(11, 0x2880000, 0x980000);
        Field_SetRect14(4, 28, 36, 10, 2, 1);
        Field_SetRect15(4, 30, 18, 10, 2, 1);
        Field_SetRect16(4, 55, 36, 40, 4, 3);
    }
    if (Field_Check9(0x80d) != 0) {
        Field_Place15(13, 0x1f80000, 0xc80000);
        Field_SetRect17(2, 29, 34, 11, 2, 1);
        Field_SetRect18(2, 31, 16, 11, 2, 1);
        Field_SetRect19(0, 58, 32, 43, 4, 1);
    }
    if (Field_Check10(0x80e) != 0) {
        Field_Place16(15, 0x2880000, 0xc80000);
        Field_SetRect20(4, 29, 36, 11, 2, 1);
        Field_SetRect21(4, 31, 18, 11, 2, 1);
        Field_SetRect22(4, 58, 36, 43, 4, 1);
    }
    {
    s16 *state = (s16 *)gCell2;

    if (state[225] == 3) {
        if (Field_Check11(0x30a) != 0) {
            Field_Run26(1, 0, 0);
            Field_Run27(5, 0, 0);
        } else if (Field_Check12(0x109) == 0) {
            Field_Run28();
            Field_Do2(0x30a);
        }
    }
    if (state[225] == 4) {
        if (Field_Check13(0x30b) != 0) {
            Field_Run29(1, 0, 0);
            Field_Run30(5, 0, 0);
        } else if (Field_Check14(0x109) == 0) {
            Field_Run31();
            Field_Do3(0x30b);
        }
    }
    }
    if (Field_Check15(0x814) != 0) {
        Field_Run32(141);
        Field_Place17(0x10000, 0x10000, 0x10000);
        Field_Run33();
    }
    return 0;
}

void Field_Run34(void)
{
    extern u8 gFarStub[];

    u32 i;
    s32 value;
    volatile s32 *p;
    s32 buf;

    p = (volatile s32 *)gFarStub;
    buf = Field_Check16(14, 0x400);
    Field_Run(0x200a56c, buf);
    value = Field_Check32(Field_Check33(), 128, buf);
    for (i = 0; i < 9; i++) {
        volatile s32 *q = p;

        *q++ = 0;
        *q++ = 0x40004000;
        p += 3;
        *q = value | 0xac00;
    }
    value = Field_Check34(Field_Check35(), 128, buf + 128);
    for (i = 0; i < 9; i++) {
        volatile s32 *q = p;

        *q++ = 0;
        *q++ = 0x40004000;
        p += 3;
        *q = value | 0xdc00;
    }
    value = Field_Check36(Field_Check37(), 128, buf + 0x100);
    for (i = 0; i < 9; i++) {
        volatile s32 *q = p;

        *q++ = 0;
        *q++ = 0x40004000;
        p += 3;
        *q = value | 0xc00;
    }
    Field_Run35(14);
    {
        s32 size = 0xc80;

        Field_Run36(0x2008eb1, size);
    }
}

void Scene_CallWhenCheck9_31_9(void)
{
    if (Field_Place22(9, 31, 9) != 0) {
        Field_Run37();
    }
}

void Scene_RunGuardedStep11(void)
{
    if (Field_Place23(11, 40, 9) != 0) {
        Field_Run38();
    }
}

void Scene_RunGuardedStep13(void)
{
    if (Field_Place24(13, 31, 12) != 0) {
        Field_Run39();
    }
}

void Scene_RunGuardedStep15(void)
{
    if (Field_Place25(15, 40, 12) != 0) {
        Field_Run40();
    }
}

void ConfigureSceneAndCheckActors(void)
{
    ConfigureScene(2, 0x00d00000, 0x00700000, 0);
    if (Field_Check38(10, 14, 7) != 0) {
        Field_Run41();
    }
}

    if (Field_Check39(12, 21, 7) != 0) {
        Field_Run42();
    }
}

void Scene_RunClosingSequence(void)
{
    extern u8 gFarStub[];

    s32 first;
    s32 kind;
    s32 second;

    first = Field_Check17(0);
    kind = *(volatile s32 *)(first + 8) >> 20;
    second = Field_Check18(0);
    if ((*(volatile s32 *)(second + 16) >> 20) == 8) {
        if ((u32)(kind - 17) <= 1) {
            Field_Run2(2, 0x1100000, 0x800000, 255);
            Field_Run3(2, 0x1200000, 0x800000, 255);
        }
    }
}

void Scene_RunGuardSequenceA(void)
{
    extern u8 gFarStub[];

    u32 i;
    s32 record;

    record = Field_Check19(17);
    if (record != 0) {
        if ((*(volatile s32 *)(record + 16) >> 20) == 8) {
            Field_Run43();
            Field_Run44(185);
            Field_Place18(17, 0x3333, 0x1999);
            Field_Place19(0, 0x3333, 0x1999);
            *(u8 *)(Field_Check40(17) + 90) &= 254;
            Field_Run45(0, 8);
            record = Field_Check41(0);
            Field_Run46(0, *(s16 *)(record + 10), 136);
            Field_Place20(17, 0x120, 120);
            Field_Run47(17);
            Field_Run48(0, 1);
            Field_Run49();
        }
    }
}

void Scene_RunFiveValueStep9(void)
{
    extern u8 gFarStub[];

    Field_SetRange(9, 31, 9, 30, 9);
    Field_Run50();
}

void Scene_RunFiveValueStep11(void)
{
    extern u8 gFarStub[];

    Field_SetRange2(11, 40, 9, 41, 9);
    Field_Run51();
}

void Scene_ApplyRect13_31_12_30_12(void)
{
    Field_SetRange3(13, 31, 12, 30, 12);
    Field_Run52();
}

void Scene_RunFiveValueStep15(void)
{
    extern u8 gFarStub[];

    Field_SetRange4(15, 40, 12, 41, 12);
    Field_Run53();
}

void Scene_ApplyRect10_14_7_13_7(void)
{
    Field_SetRange5(10, 14, 7, 13, 7);
    Field_Run54();
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
    u8 *record = Field_Run11(10);
    s32 cell;

    if (record == 0) {
        return;
    }

    cell = *(s32 *)(record + 16) >> 20;
    Field_Run55(10, 13, cell + 1, 13, cell);
}

void Actor_MoveActor10ByRow(void)
{
    s32 *p = Field_Run14(10);
    if (p != NULL) {
        s32 v = p[4] >> 20;
        Field_SetRange6(10, 13, v - 1, 13, v);
    }
}

void Scene_ApplyRect12_21_7_22_7(void)
{
    Field_SetRange7(12, 21, 7, 22, 7);
    Field_Run56();
}

void Actor_ApplyActorTwelveZCellPair(void)
{
    s32 *p = Field_Run12(12);
    if (p != NULL) {
        s32 v = p[4] >> 20;
        Field_SetRange8(12, 22, v + 1, 22, v);
    }
}

void Actor_RunSlot12ColumnStep(void)
{
    s32 *p = Field_Run15(12);
    if (p != NULL) {
        s32 v = p[4] >> 20;
        Field_SetRange9(12, 22, v - 1, 22, v);
    }
}

s32 Actor_IsActorAtTile(s32 no, s32 x, s32 z)
{
    s32 *p = Field_Run13(no);
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
    Field_Run57();
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
    Field_Run58();
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
    Field_Run59();
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
    Field_Run60();
}

void Scene_RunGuard(void)
{
    extern u8 gFarStub[];

    u32 i;
    s32 record;

    Field_Run61();
    if (Field_Check20(0x818) == 0) {
        if (Field_Check21(0x816) == 0) {
            Field_Run4(0x20000, 0x4000);
            Field_Run5(0x11e0000, -1, 0x920000, 1);
            Field_Run62();
            Field_Run63(186);
            Field_SetRect23(0, 59, 15, 38, 4, 3);
            if (Field_Check22(0x817) != 0) {
                Field_SetRect24(8, 60, 17, 39, 2, 2);
            }
            Field_Run64(0, 0, 0);
            Field_Run65(30);
            Field_Do4(0x816);
            if (Field_Check23(0x817) != 0) {
                Field_Run66();
            }
        }
    }
    Field_Run67();
}

void Scene_RunGuard(void)
{
    extern u8 gFarStub[];

    u32 i;
    s32 record;

    Field_Run68();
    if (Field_Check24(0x818) == 0) {
        if (Field_Check25(0x817) == 0) {
            Field_Run6(0x20000, 0x4000);
            Field_Run7(0x11e0000, -1, 0x920000, 1);
            Field_Run69();
            Field_Run70(186);
            Field_SetRect25(4, 59, 17, 38, 4, 3);
            if (Field_Check26(0x816) != 0) {
                Field_SetRect26(8, 60, 17, 39, 2, 2);
            }
            Field_Place21(0, 0x8000, 0);
            Field_Run71(30);
            Field_Do5(0x817);
            if (Field_Check27(0x816) != 0) {
                Field_Run72();
            }
        }
    }
    Field_Run73();
}

void Scene_RunSplitPairSteps(s32 a, s32 b)
{
    Field_Apply(a, 0);
    Field_Do11(b);
}
