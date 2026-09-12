#include "types.h"
#include "scene.h"

#define SCENE_WORD_1C8 (*(u32 *)(*(u8 **)0x03001ebc + 456))
#define SCENE_WORK_FIELD_456 (*(u32 *)(*(u8 **)0x03001ebc + 456))
#define RATIO_HI 52428
#define RATIO_LO 26214
#define ACTOR_SHOWN_OFFSET 100

struct Object {
    u8 filler00[8];
    s32 x;
};

struct SceneRecord {
    u8 pad[100];
    u16 value100;
};

typedef struct {
    u8 filler0[6];
    u16 unk6;
} T;

/*
 * resource_383 owner at 0x02002ba0, 80 bytes.
 * Points two records at a third: each gets the angle from its own offset to the
 * reference record, stored as a halfword at +6.
 */
struct Rec_383 {
    u8 pad00[6];
    u16 f6;                     /* +6  */
    s32 f8;                     /* +8  */
    u8 pad0c[4];
    s32 f16;                    /* +16 */
};

extern s16 gCell[];
extern u8 gOv[];
extern u8 gOv2[];
extern u8 Value_00001280;
extern u8 gOv3[];
extern u8 gOv4[];
extern u8 gOv5[];
extern u8 gOv6[];
extern u8 gVal[];
extern u8 Value_00000854;
extern u8 Value_000012c3;
extern s32 gOv7[];
extern s32 gOv8[];
extern u8 gVal2[];
extern u8 gVal3[];
extern u8 gOv9[];
extern u8 gVal4[];
extern u8 gVal5[];
extern u8 gVal6[];
extern u8 gOv10[];
extern u8 gOv11[];
extern u8 gOv12[];

u8 *Talk_unk16_4(s32);
u8 *Talk_unk17_4(s32);
u8 *Talk_unk18_4(s32);
u8 *Talk_unk19_4(s32);

struct Object *Talk_unk20_4(s32);

u8 *Talk_unk21_4(s32);

u8 *Talk_unk22_4(s32);

u8 *Talk_unk23_4(s32);

u8 *Talk_unk24_4(s32 actor);

u8 *Talk_unk25_4(s32 actor);
u8 *Talk_unk26_4(s32 actor);

u8 *Talk_unk27_4();
u8 *Scene_GetRecord_2();

void *Scene_GetRecord_1();
u8 *Talk_unk28_4(s32);

T *Talk_unk29_4(s32);

struct Rec_383 *Talk_unk30_4();
struct Rec_383 *Talk_unk31_4();
struct Rec_383 *Talk_unk32_4();


u8 *Talk_unk33_4();

u8 *Talk_unk34_4();

u8 *Talk_unk35_4();

u8 *Talk_unk36_4();

u8 *Talk_unk37_4(s32);

u8 *Talk_unk38_4(s32);

u8 *Talk_unk39_4(s32);
u8 *Talk_unk40_4(s32, s32);

/*
 * Update one actor's animation descriptor when its current state matches the
 * expected state. The helper is called by the 17-entry scene transition table
 * at 0x02002564.
 *
 * The owner starts with push {r5,r6,r7,lr} at 0x020026e4, returns through
 * pop {r5,r6,r7}/pop {r0}/bx r0 at 0x02002716-0x0200271a, and is immediately
 * followed by the callback owner at 0x0200271c. It has no trailing pool, so
 * the complete span is 56 bytes.
 */

/* Word at +456 of the shared scene work record. */

/* Field at +456 of the shared scene work record, addressed through the
 * loader-fixed pointer at 0x03001ebc. */

/* Pair of ratio-like arguments shared by three setup calls below (each
 * applied to a different index: 0, 1, 2). */

/* The "shown" half word at +100 of an actor record. */

/* Phase/status word at 0x1c0 of the shared scene work record. */

static __inline__ void bump_step(s32 off, s32 amount)
{
    extern u8 gWork[];

    u8 *work = *(u8 **)gWork;
    u16 *slot = (u16 *)((s32)work + off);
    s32 next = *slot + amount;

    *slot = next;
}

/*
 * Head export stub at 0x02000030. The eight-byte owner includes its one pool
 * word at 0x02000034, which the load reads and which sits past the return.
 * The word is an address returned without being dereferenced, so this is a
 * getter for an in-image table; it names bytes only while this overlay is
 * resident at 0x02000000.
 */
u8 *SceneData_GetTableD8f8(void)
{
    extern u8 *gWork;

    return (u8 *)0x0200d8f8;
}

/*
 * Head export stub at 0x02000038. The eight-byte owner includes its one pool
 * word at 0x0200003c, which the load reads and which sits past the return.
 * The word is an address returned without being dereferenced, so this is a
 * getter for an in-image table; it names bytes only while this overlay is
 * resident at 0x02000000.
 */
u8 *SceneData_GetTableDa60(void)
{
    extern u8 *gWork;

    return (u8 *)0x0200da60;
}

/*
 * Head export stub at 0x02000040. The eight-byte owner includes its one pool
 * word at 0x02000044, which the load reads and which sits past the return.
 * The word is an address returned without being dereferenced, so this is a
 * getter for an in-image table; it names bytes only while this overlay is
 * resident at 0x02000000.
 */
u8 *SceneData_GetTableDa80(void)
{
    extern u8 *gWork;

    return (u8 *)0x0200da80;
}

u8 *SceneData_SelectTableBySceneRange(void)
{
    extern u8 *gWork;

    s16 *tbl = gCell;
    u8 *ret;
    s32 v = tbl[225];
    s32 low = 15;

    if (v <= 17) {
        if (v >= low) {
            ret = gOv;
        } else {
            ret = gOv2;
        }
    } else {
        ret = gOv2;
    }
    Talk_unk14_2(ret);
    return ret;
}

void ActorDraw_SetSceneCellByAngle(void)
{
    extern u8 *gWork;

    s32 x;
    s32 z;

    if (*(u16 *)(Talk_unk16_4(0) + 6) >= 0xa000
        && *(u16 *)(Talk_unk17_4(0) + 6) <= 0xe000) {
        Talk_unk41_4();
        x = 42;
        z = 85;
        Talk_unk4_5(41, 85, 1, 1, x, z);
    } else if (*(u16 *)(Talk_unk18_4(0) + 6) >= 0x2000
               && *(u16 *)(Talk_unk19_4(0) + 6) <= 0x6000) {
        Talk_unk42_4();
        x = 42;
        z = 85;
        Talk_unk5_5(43, 85, 1, 1, x, z);
    }
}

void Scene_RunObjectTwentySixPositionCheck(void)
{
    extern u8 *gWork;

    struct Object *obj;
    s32 x;
    s32 z;

    Talk_unk43_4();
    obj = Talk_unk20_4(26);
    if ((obj->x >> 20) == 42) {
        x = 41;
        z = 24;
        Talk_unk6_5(101, 24, 3, 4, x, z);
        Talk_unk15_2(0x859);
    }
    Talk_unk44_4();
}

void Scene_RunActorNineteenAngleDialogue(void)
{
    extern u8 *gWork;

    s32 v = *(u16 *)(Talk_unk21_4(0) + 6);

    Talk_unk45_4();
    if (v >= 0xa001 && v <= 0xdfff) {
        Talk_Apply(4, 19);
    } else {
        if (Talk_unk10(0x855) == 0) {
            Talk_unk16_2((s32)&Value_00001280);
        } else {
            Talk_unk17_2(0x1370);
        }
        Talk_Apply2(19, 0);
    }
    Talk_unk46_4();
}

void Scene_RunActorTwentyAngleDialogue(void)
{
    extern u8 *gWork;

    s32 v = *(u16 *)(Talk_unk22_4(0) + 6);

    Talk_unk47_4();
    if (v >= 0xa001 && v <= 0xdfff) {
        Talk_Apply3(5, 20);
    } else {
        if (Talk_unk11(0x855) == 0) {
            Talk_unk18_2(0x1282);
        } else {
            Talk_unk19_2(0x1372);
        }
        Talk_Apply4(20, 0);
    }
    Talk_unk48_4();
}

void Scene_RunActorTwentyThreeAngleDialogue(void)
{
    extern u8 *gWork;

    s32 v = *(u16 *)(Talk_unk23_4(0) + 6);

    Talk_unk49_4();
    if (v >= 0xa001 && v <= 0xdfff) {
        Talk_Apply5(1, 23);
    } else {
        if (Talk_unk12(0x855) == 0) {
            Talk_unk20_2(0x128d);
        } else {
            Talk_unk21_2(0x137b);
        }
        Talk_Apply6(23, 0);
    }
    Talk_unk50_4();
}

void Scene_RunActorEighteenConditionalScene(void)
{
    Talk_unk51_4();
    if (Talk_unk13() == 0) {
        Talk_Apply7(18, 4);
        Talk_unk22_2(20);
        Talk_unk23_2(0x1384);
        Talk_Apply8(18, 0);
    } else {
        Talk_Apply9(231, 3);
        Talk_Apply10(231, 0);
    }
    Talk_unk52_4();
}

void Dialogue_ShowLine12BB(void)
{
    Talk_unk53_4();
    Talk_unk24_2(0x12bb);
    Talk_Apply11(1, 0);
}

void State_SetFlags92bAnd94b(void)
{
    Talk_unk54_4();
    Talk_Apply12(0x92b, 1);
    Talk_Apply13(0x94b, 1);
    Talk_unk55_4();
}

void State_SetFlags929And949(void)
{
    Talk_unk56_4();
    Talk_Apply14(0x929, 1);
    Talk_Apply15(0x949, 1);
    Talk_unk57_4();
}

u8 *SceneData_SelectTableE1fcBySceneRangeOrFlag855(void)
{
    s16 *tbl = gCell;
    s32 v = tbl[225];
    s32 lo = 15;

    if (v <= 17) {
        if (v >= lo) {
            return gOv3;
        }
    }
    if (Talk_unk14(0x855) != 0) {
        return gOv4;
    }
    return gOv5;
}

void ActorDraw_RunActorModeOneThenZeroWithStep(s32 x)
{
    Talk_Apply16(x, 1);
    Talk_unk112_2(x, 0, 2);
    Talk_Apply17(x, 0);
}

void State_RunGuardedActorStep(s32 x)
{
    extern u8 *gWork;

    u8 *flag = (u8 *)Talk_unk15() + 91;
    s32 zero = 0;

    *flag = 1;
    Talk_unk58_4();
    Talk_Apply18(x, 1);
    Talk_unk25_2(2);
    Talk_Apply19(x, 0);
    Talk_unk59_4();
    *flag = zero;
}

void Dialogue_PromptAndCountSkip(s32 x)
{
    extern u8 *gWork;

    Talk_unk113_2(x, 0, 2);
    Talk_Apply20(x, 0);
    if (Talk_Apply21(0, 0) != 0) {
        *(u16 *)(gWork + 472) += 1;
    }
    Talk_Apply22(x, 0);
}

void Dialogue_RunActorElevenDialogue(void)
{
    Talk_unk60_4();
    Talk_unk26_2(0x1247);
    Talk_Apply23(11, 1);
    Talk_unk27_2(11);
    Talk_unk61_4();
}

void Scene_RunScene383(void)
{
    extern u8 gWork[];

    u32 i;
    s32 record;

    Talk_unk62_4();
    Talk_Do(0x1253);
    Talk_unk63_4(15);
    Talk_Place(15, 0x8000, 0);
    Talk_unk64_4();
}

void State_BranchOnSlotZeroFacingAndFlag855(void)
{
    s32 value = *(u16 *)(Talk_unk28_4(0) + 6);

    Talk_unk65_4();
    if (value >= 0xa001 && value <= 0xdfff) {
        Talk_Apply24(6, 21);
    } else if (Talk_unk16(0x855) == 0) {
        Talk_unk28_2(0x1284);
        Talk_unk29_2(21);
    } else {
        Talk_unk30_2(0x1374);
        Talk_Apply25(21, 0);
    }
    Talk_unk66_4();
}

void Dialogue_RunActor9FlaggedLine(void)
{
    Talk_unk67_4();
    if (Talk_unk17(0x855) == 0) {
        Talk_unk31_2(0x1243);
    } else {
        Talk_unk32_2(0x1353);
    }
    Talk_unk33_2(9);
    Talk_unk68_4();
}

void Dialogue_RunActorTwelveFlaggedDialogue(void)
{
    Talk_unk69_4();
    if (Talk_unk18(0x855) != 0) {
        Talk_unk34_2(0x135c);
    } else {
        Talk_unk35_2(0x124c);
    }
    Talk_unk36_2(12);
    Talk_unk70_4();
}

void Scene_RunFlag856DialogueBranch(void)
{
    extern u8 gWork[];

    s32 g;
    g = 0x851;
    Talk_unk71_4();
    if (Talk_unk19(0x856) != 0) {
        if (Talk_unk20(g) == 0) {
            Talk_unk37_2(0x1276);
            Talk_unk38_2(16);
            Talk_unk39_2(10);
            Talk_unk114_2(16, 3, 20);
            Talk_unk40_2(g);
        } else {
            Talk_unk41_2(0x1278);
        }
    } else {
        Talk_unk42_2(0x1250);
    }
    Talk_unk43_2(16);
    Talk_unk72_4();
}

void Dialogue_ShowLine128E(void)
{
    Talk_unk73_4();
    Talk_unk44_2(0x128e);
    Talk_unk45_2(18);
    Talk_unk74_4();
}

void Actor_StepActor24AnimationByFacing(void)
{
    T *p;
    s16 *q;
    s32 v;
    s32 n;

    p = Talk_unk29_4(24);
    Talk_unk75_4();
    Talk_Apply26(24, 2);
    Talk_unk46_2(0x12ac);
    Talk_Apply27(24, 0);
    Talk_unk115_2(24, 0x40000, 0x20000);
    if ((u32)((p->unk6 & 0xf000) - 0x5000) <= 0x6000) {
        q = (s16 *)((u8 *)p + 100);
        v = *q;
        if (v <= 2) {
            Talk_Apply28(24, gOv7[v]);
            *(u16 *)q = *(u16 *)q + 1;
            goto clamp;
        }
    } else {
        q = (s16 *)((u8 *)p + 100);
        v = *q;
        if (v > 2) {
            Talk_Apply29(24, gOv7[v]);
            *(u16 *)q = *(u16 *)q + 1;
            goto clamp;
        }
    }
    Talk_Apply30(24, gOv8[v]);
    n = *(u16 *)q - 1;
    *(u16 *)q = n;
clamp:
    if (*q > 5) {
        n = 0;
        *(u16 *)q = n;
    }
    if (*q < 0) {
        n = 5;
        *(u16 *)q = n;
    }
    Talk_unk47_2(24);
    Talk_unk76_4();
}

void Dialogue_RunActor10Line(void)
{
    Talk_unk77_3();
    Talk_unk48_2(0x1356);
    Talk_unk49_2(10);
    Talk_unk78_3();
}

void Dialogue_RunActor11Line(void)
{
    Talk_unk79_3();
    Talk_unk50_2(0x1359);
    Talk_unk51_2(11);
    Talk_unk80_3();
}

void Dialogue_RunActor14Line(void)
{
    Talk_unk81_3();
    Talk_unk52_2(0x1368);
    Talk_unk53_2(14);
    Talk_unk82_3();
}

void Scene_RunScene383SequenceC(void)
{
    extern u8 gWork[];

    u32 i;
    s32 record;

    Talk_unk83_3();
    if (Talk_Check(0x857) == 0) {
        Talk_unk84_3((s32)gVal);
        Talk_unk85_3(16, 20);
        Talk_unk86_3(16, 3, 20);
        Talk_unk87_3(16, 30);
        Talk_unk88_3(16, 0, 0);
        Talk_unk89_3(30);
        Talk_unk90_3(16, 2);
        Talk_unk91_3(30);
        Talk_unk92_3(0, 16, 20);
        Talk_unk93_3(16, 3, 20);
        bump_step(1);
        if (Talk_Run() == 0) {
            Talk_unk94_3(((s32)gVal + 3));
            Talk_unk95_3(16, 20);
            Talk_unk96_3();
            goto L_02000906;
        }
        Talk_unk2_2(0x857);
        Talk_unk97_3(189, 0);
    }
    Talk_unk3_2(0x1364);
    Talk_unk98_3(16, 0);
    Talk_unk99_3(20);
    if (Talk_unk2(0, 0) != 0) {
        bump_step(1);
    }
    Talk_unk100_3(16, 0);
    Talk_unk101_3();
    L_02000906:;
}

void Scene_RunScene383(void)
{
    extern u8 gWork[];

    u32 i;
    s32 record;

    Talk_unk102_3();
    Talk_unk103_3(18, 0, 2);
    if (Talk_unk3(0x85b) == 0) {
        Talk_unk4_2(0x137c);
        Talk_unk104_3(18, 0);
    } else {
        Talk_unk5_2(0x1385);
        Talk_unk105_3(18, 0);
    }
    if (Talk_unk4(0, 0) == 0) {
        Talk_unk106_3(20);
        Talk_unk107_3(18, 0);
        Talk_unk108_3(20);
        Talk_unk109_3(18, 2);
        Talk_unk110_3(20);
        if (Talk_unk2_4() == 0) {
            Talk_unk111_3(18, 4);
            Talk_unk112_3(20);
            Talk_unk6_2(0x1384);
            Talk_unk113_3(18, 0);
            goto L_020009ec;
        }
        Talk_unk114_3(231, 3);
        Talk_unk115_3(231, 0);
        Talk_unk7_2(0x85b);
    } else {
        bump_step(1);
        Talk_unk116_3(20);
        Talk_unk117_3(18, 3);
        Talk_unk118_3(20);
        Talk_unk119_3(18, 0);
    }
    L_020009ec:;
    Talk_unk2_3(18, 0x4000, 0);
    Talk_unk120_3();
}

void Dialogue_RunActorNineFlaggedDialogue(void)
{
    Talk_unk121_3();
    if (Talk_unk21(0x855) == 0) {
        Talk_unk54_2(0x1245);
    } else {
        Talk_unk55_2(0x1355);
    }
    Talk_unk56_2(9);
    Talk_unk122_3();
}

void Dialogue_RunActorElevenFlaggedDialogue(void)
{
    Talk_unk123_3();
    if (Talk_unk22(0x855) == 0) {
        Talk_unk57_2(0x124b);
    } else {
        Talk_unk58_2(0x135b);
    }
    Talk_unk59_2(11);
    Talk_unk124_3();
}

void Dialogue_ShowLine124EOr135E(void)
{
    Talk_unk125_3();
    if (Talk_unk23(0x855) == 0) {
        Talk_unk60_2(0x124e);
    } else {
        Talk_unk61_2(0x135e);
    }
    Talk_unk62_2(12);
    Talk_unk126_3();
}

void Dialogue_RunActor16FlaggedLine(void)
{
    Talk_unk127_3();
    if (Talk_unk24(0x855) == 0) {
        Talk_unk63_2(0x127c);
    } else {
        Talk_unk64_2(0x136c);
    }
    Talk_unk65_2(16);
    Talk_unk128_3();
}

void Dialogue_RunActorEighteenBranchedDialogue(void)
{
    Talk_unk129_3();
    if (Talk_unk25(0x855) == 0) {
        Talk_unk66_2(0x1294);
    } else if (Talk_unk26(0x85b) == 0) {
        Talk_unk67_2(0x1382);
    } else {
        Talk_unk68_2(0x1cf4);
    }
    Talk_unk69_2(18);
    Talk_unk130_3();
}

/* Configures actors 0, 1, 2 (position, movement, and animation timing), then
 * branches on whether actor 0 is already set up: one path sets up actors 0-2
 * with poses and movement, the other only advances actor 2's animation. Both
 * paths converge to check actor 0's record and, depending on that check,
 * configure either actor 2 or actor 1 from it before the scene finishes. */
void Scene_RunSetupSequence(void)
{
    extern u8 gWork[];

    u8 *Scene_GetRecord_1();
    u8 *Scene_GetRecord_2();

    u8 *record;

    Battle_Reset_1();
    Motion_SetSpeed_1(0, 0xcccc, 0x6666);
    Motion_SetSpeed_2(1, 0xcccc, 0x6666);
    Motion_SetSpeed_3(2, 0xcccc, 0x6666);
    Audio_PlayCue_1(19);
    Motion_SetPosReset_1(0, 0x180, 0x198);
    Motion_ArmCb_1(0, 0xc000, 0);
    Motion_SetHPosTerrain_1(1, 0x1800000, 0x1980000);
    Motion_SetPosReset_2(1, 0x170, 0x198);
    Motion_ArmCb_2(1, 0xd000, 20);
    if (GameFlag_IsSet_1(0x850) != 0) {
    } else {
        GameFlag_Set_1(0x850);
        Talk_unk131_3(2, 0);
        Battle_WaitMode0_1(40);
        Talk_unk132_3();
        SceneWork_SetStepValue_1(0x1256);
        Audio_PlayCue_2(60);
        Battle_WaitMode0_2(30);
        Talk_unk133_3(2, 3, 30);
        Talk_unk134_3(2, 30);
        Motion_SetVarCb_1(0, 1);
        Motion_SetVarCbObj_1(1, 1);
        Battle_WaitMode0_3(20);
        Talk_unk135_3(2, 0);
        Battle_WaitMode0_4(40);
        Talk_unk136_3();
        Talk_unk137_3(2, 30);
        Talk_unk138_3(0, 1, 50);
        Motion_ArmCb_3(0, 0xc000, 0);
        Motion_ArmCb_4(1, 0xd000, 0);
        Battle_WaitMode0_5(20);
        Talk_unk139_3(2, 0);
        Battle_WaitMode0_6(40);
        Talk_unk140_3();
        Motion_SetVarCbObj_2(2, 1);
        Talk_unk141_3(2, 50);
        Object_SetModeById_1(0, 3);
        Talk_unk142_3(1, 3, 20);
        Talk_unk143_3(2, 3, 20);
        Talk_unk144_3(2, 40);
        Motion_SetVarCbObj_3(2, 1);
        Battle_WaitMode0_7(30);
        Talk_unk3_3(2, 0xc000, 0);
        Battle_WaitMode0_8(30);
        Motion_SetPosReset_3(2, 0x178, 0x178);
        Battle_WaitMode0_9(40);
        Talk_unk145_3(0, 1, 50);
        Motion_ArmCb_5(0, 0xc000, 0);
        Motion_ArmCb_6(1, 0xd000, 0);
        Motion_SetVarCbObj_4(2, 1);
        Battle_WaitMode0_10(50);
        Talk_unk146_3(2, 3, 30);
        Motion_ArmCb_7(2, 0x4000, 0);
        Battle_WaitMode0_11(10);
        BattleEv_RunWait_1(2, 0);
        Motion_SetSpeed_4(2, 0);
        goto L_join_setup_paths;
    }
    Audio_PlayCue_3(60);
    SceneWork_SetStepValue_2(0x125d);
    Motion_SetSpeed_5(2, 0);
    L_join_setup_paths:;
    if (UiWork_WaitThenFinalizeCapacity_1(0, 0) == 0) {
        Talk_unk147_3();
        GameFlag_Set_2(0x856);
        Object_SetModeById_2(2, 2);
        record = Scene_GetRecord_1(0);
        if (record != 0) {
            /* Read the two s16 fields at +10 and +18 of the record. */
            Motion_ResetAndSetPosition_1(2, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Motion_CommitPos_1(2);
        Motion_SetHPosTerrain_2(2, 0, 0);
    } else {
        BattleEv_RunWait_2(2, 0);
    }
    Object_SetModeById_3(1, 2);
    record = Scene_GetRecord_2(0);
    if (record != 0) {
        /* Read the two s16 fields at +10 and +18 of the record. */
        Motion_ResetAndSetPosition_2(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Motion_CommitPos_2(1);
    Motion_SetHPosTerrain_3(1, 0, 0);
    Audio_PlayCueForPartyMember_1();
    Battle_SchedShoulder_1();
}

void State_SetValue123Mode11(void)
{
    Talk_unk70_2(123);
    Talk_unk71_2(11);
}

/* Sets up the opening sequence: two calls with fixed argument pairs, a
 * write to the scene work record, and two more calls with fixed args. */
void Scene_RunOpeningSequenceHead(void)
{
    extern u8 gWork[];

    ObjectMotion_SetSpeedParameters_1(0, 32768, 16384); /* object_id 0, speed_limit 32768, acceleration 16384 */
    Motion_ResetPosMode2_1(0, 728, 408); /* object_id 0, x 728, z 408 */
    SCENE_WORD_1C8 = 16;
    Audio_PlayCue_1(123);
    Talk_unk8_2(15); /* main:0808a248 */
}

/* Runs the second part of an opening sequence: two parameterised calls, then
 * (only if a flag read via Value_00000854 is clear) a short block of setup
 * calls, then unconditionally sets a scene work field and two more calls. */
void Scene_RunOpeningSequenceSecond(void)
{
    extern u8 gWork[];

    ObjectMotion_SetSpeedParameters_1(0, 32768, 16384); /* object 0, speed_limit 32768, acceleration 16384 */
    ObjectMotion_SetPositionAndReset_1(0, 744, 408); /* object 0, x 744, z 408 */
    if (GameFlag_IsSet_1((s32)&Value_00000854) == 0) {
        BattleRuntime_Reset_1();
        SceneWork_SetStepValue_1((s32)&Value_000012c3);
        BattleEvent_RunActionAndWait_1(8, 0); /* action 8 */
        BattleRuntime_ScheduleShoulderButtonModeUpdate_1();
    }
    SCENE_WORK_FIELD_456 = 16;
    Audio_PlayCue_1(123);
    Talk_unk9_2(14); /* main:0808a248 */
}

/* Third scene step: sets up actors 24 and 25 (fetching each one's record),
 * runs a shared series of configuration calls touching actors 0-2, 10, 14,
 * 20, 24 and 25, then marks the two fetched records with a byte flag. */
void Scene_RunOpeningSequenceThird(void)
{
    extern u8 gWork[];

    void *Scene_GetRecord_1();
    void *Scene_GetRecord_2();

    void *actor24;
    void *actor25;

    actor24 = Scene_GetRecord_1(24);
    actor25 = Scene_GetRecord_2(25);
    BattleRuntime_Reset_1();
    ObjectMotion_SetSpeedParameters_1(0, RATIO_HI, RATIO_LO);
    ObjectMotion_SetSpeedParameters_2(1, RATIO_HI, RATIO_LO);
    ObjectMotion_SetSpeedParameters_3(2, RATIO_HI, RATIO_LO);
    ObjectMotion_SetPositionAndReset_1(0, 232, 696);
    ObjectMotion_SetPositionAndReset_2(0, 200, 696);
    BattleRuntime_WaitIfModeZero_1(10);
    BattleFx_SpawnLinked_1(25, 256, 0);
    BattleFx_SpawnLinked_2(24, 256, 0);
    BattleRuntime_WaitIfModeZero_2(60);
    Talk_unk4_3(25, 0, 10);
    ObjectMotion_SetVariantCallbackAndInvokeObject_1(24, 2);
    BattleRuntime_WaitIfModeZero_3(20);
    SceneWork_SetStepValue_1(4758);
    Talk_unk3_4(24, 20);
    BattleRuntime_WaitIfModeZero_4(25, 258); /* main:0808a1f0 */
    BattleRuntime_WaitIfModeZero_5(60);
    Talk_unk4_4(25, 20);
    ObjectMotion_SetVariantCallbackAndInvokeObject_2(24, 1);
    Talk_unk5_4(24, 30);
    ObjectMotion_SetSpeedParameters_4(24, 262144, 131072);
    ObjectMotion_SetSpeedParameters_5(25, 229376, 114688);
    Motion_EnableActCb_1(25, 33609776);
    Motion_EnableActCb_2(24, 33609056);
    Object_LookupAndStep_1(24);
    Talk_SetRect(14, 45, 3, 1, 14, 44); /* main:080091c0 */
    GameFlag_Set_1(2130);
    GameFlag_Set_2(768);
    Talk_unk6_4(33598369, 3200); /* main:080000d0 */
    /* Byte flag at +100 of each record: 1 for actor 24's, 3 for actor 25's. */
    ((struct SceneRecord *)actor24)->value100 = 1;
    ((struct SceneRecord *)actor25)->value100 = 3;
    BattleRuntime_ScheduleShoulderButtonModeUpdate_1();
}

/* Sets up actors 24 and 25 (pose picked from a per-actor threshold field at
 * +100), positions and moves actors 0-2 and 24-25, and advances the shared
 * scene step counter partway through. */
void Scene_RunScene383SequenceB(void)
{
    extern u8 gWork[];

    s32 actor24;
    s32 actor25;
    s32 record_00001299;

    actor24 = Scene_GetRecord_1(24);
    actor25 = Scene_GetRecord_2(25);
    BattleRuntime_Reset_1();
    Talk_unk10_2(0x200aba1);
    GameFlag_Clear_1(0x300);
    if (*(s16 *)(actor24 + 100) <= 3) {
        ObjectMotion_EnableActionAndSetCallback_1(24, 0x200d678);
    } else {
        ObjectMotion_EnableActionAndSetCallback_2(24, 0x200d650);
    }
    if (*(s16 *)(actor25 + 100) <= 2) {
        Motion_EnableActCb_3(25, 0x200d768);
    } else {
        Motion_EnableActCb_4(25, 0x200d650);
    }
    ObjectMotion_SetSpeedParameters_1(0, 0xcccc, 0x6666);
    ObjectMotion_SetSpeedParameters_2(1, 0xcccc, 0x6666);
    ObjectMotion_SetSpeedParameters_3(2, 0xcccc, 0x6666);
    ObjectMotion_SetPositionAndReset_1(0, 248, 0x2d8);
    ObjectMotion_SetHorizontalPositionWithTerrain_1(2, 0xf80000, 0x2d80000);
    ObjectMotion_SetHorizontalPositionWithTerrain_2(1, 0xf80000, 0x2d80000);
    ObjectMotion_ResetAndSetPositionInMode2_1(2, 0x108, 0x2e8);
    ObjectMotion_SetPositionAndReset_2(1, 232, 0x2e8);
    ObjectMotion_CommitCurrentPositionAndActivate_1(2);
    ObjectMotion_ArmCallback_1(0, 0x4000, 0);
    Motion_SetAngleToward_1(1, 0, 0);
    Talk_unk148_3(2, 0, 30);
    ObjectMotion_SetVariantCallbackAndInvokeObject_1(2, 1);
    record_00001299 = (s32)gVal2;
    SceneWork_SetStepValue_1(record_00001299);
    BattleEvent_RunActionAndWait_1(2, 0);
    Motion_SetAngleToward_2(0, 2, 0);
    Talk_unk149_3(1, 2, 20);
    Object_SetModeById_1(0, 3);
    Talk_unk150_3(1, 3, 20);
    Talk_unk151_2(0, 1, 10);
    ObjectMotion_SetSpeedParameters_4(1, 0);
    if (UiWork_WaitThenFinalizeCapacity_1(0, 0) != 0) {
        bump_step(1);
    }
    Talk_unk152_2(1, 30);
    SceneWork_SetStepValue_2((record_00001299 + 4));
    Motion_SetAngleToward_3(0, 2, 0);
    Talk_unk153_2(1, 2, 50);
    BattleEffect_SpawnLinkedResourceObject_1(2, 0x100, 0);
    BattleRuntime_WaitIfModeZero_1(60);
    Talk_unk154_2(0, 1, 50);
    Motion_SetAngleToward_4(0, 2, 0);
    Talk_unk155_2(1, 2, 30);
    Talk_unk156_2(2, 3, 10);
    Talk_unk157_2(2, 20);
    BattleEffect_SpawnLinkedResourceObject_2(0, 0x102, 0);
    BattleFx_SpawnLinked_3(1, 0x102, 0);
    BattleRuntime_WaitIfModeZero_2(60);
    Talk_unk158_2(2, 3, 20);
    Talk_unk159_2(2, 30);
    SceneWork_SetStepValue_3(0x129f);
    BattleEvent_RunActionAndWait_2(1, 0);
    ObjectMotion_ArmCallback_2(0, 0x4000, 0);
    BattleRuntime_WaitIfModeZero_3(40);
    Object_SetModeById_2(0, 3);
    Object_SetModeById_3(1, 3);
    Talk_unk160_2(2, 3, 50);
    Motion_ResetPosMode2_2(2, 248, 0x2d8);
    ObjectMotion_SetPositionAndReset_3(1, 248, 0x2d8);
    ObjectMotion_SetHorizontalPositionWithTerrain_3(1, 0, 0);
    Motion_SetHPosTerrain_4(2, 0, 0);
    Motion_SetHPosTerrain_5(24, 0x680000, 0x2b80000);
    Motion_SetHPosTerrain_6(25, 0x780000, 0x2b80000);
    ObjectMotion_ArmCallback_3(24, 0, 0);
    ObjectMotion_ArmCallback_4(25, 0x8000, 0);
    Talk_unk2_5(14, 50, 3, 1, 14, 44);
    BattleRuntime_ScheduleShoulderButtonModeUpdate_1();
}

void Scene_RunSteps107And250(void)
{
    extern u8 gWork[];

    Talk_unk72_2(0x107);
    Talk_unk73_2(0x250);
    Talk_unk161_2();
}

/* Configures actor records 24 and 25 (their +100 "shown" half words end up
 * set to 1 and 3 respectively) and finishes with a six-argument call that
 * repeats actor id 14. */
void Scene_ConfigurePairedActors(void)
{
    extern u8 gWork[];

    u32 i;
    u8 *record;

    BattleRuntime_WaitIfModeZero_1(30);
    ObjectMotion_SetVariantCallbackAndInvokeObject_1(24, 1);
    BattleRuntime_WaitIfModeZero_2(20);
    SceneWork_SetStepValue_1((s32)gVal3);
    Talk_unk162_2(24, 20);
    ObjectMotion_ArmCallback_1(25, 0, 20);
    BattleRuntime_WaitIfModeZero_3(25, 0x102);
    ObjectMotion_SetVariantCallbackAndInvokeObject_2(25, 2);
    Talk_unk163_2(25, 20);
    Motion_CallWaitAnim_1(24, 4);
    BattleRuntime_WaitIfModeZero_4(20);
    Talk_unk164_2(24, 20);
    ObjectMotion_SetSpeedParameters_1(24, 0x40000, 0x20000);
    ObjectMotion_SetSpeedParameters_2(25, 0x38000, 0x1c000);
    ObjectMotion_EnableActionAndSetCallback_1(25, 0x200d830);
    ObjectMotion_EnableActionAndSetCallback_2(24, 0x200d560);
    Object_LookupAndStep_1(24);
    {
        u8 *record = Scene_GetRecord_1(24);
        s32 shown = 1;

        *(u16 *)(record + ACTOR_SHOWN_OFFSET) = shown;
    }
    {
        u8 *record = Scene_GetRecord_2(25);
        s32 shown = 3;

        *(u16 *)(record + ACTOR_SHOWN_OFFSET) = shown;
    }
    Talk_unk3_5(14, 48, 4, 1, 14, 44);
}

void Actor_UpdateAnimationOnStateMatch(s32 actor, s32 expected, s32 next, const u8 *desc)
{
    u8 *rec = Talk_unk24_4(actor);

    if (*(s16 *)(rec + 100) == expected) {
        Talk_Apply31(actor, desc);
        *(u16 *)(rec + 100) = (u16)next;
    }
}

void State_SetFlagByActorPosition(void)
{
    u8 *p0 = Talk_unk25_4(0);
    s32 rx = *(s32 *)(p0 + 8);
    s32 x;
    s32 z;

    u8 *p1 = Talk_unk26_4(0);
    x = rx >> 20;
    z = *(s32 *)(p1 + 16);
    x = x - 34;
    z = z >> 20;

    if ((u32)x <= 1 && z > 40 && z <= 42) {
        Talk_unk74_2(148 << 2);
    } else {
        Talk_unk75_2(148 << 2);
    }
}

void Actor_FaceActors24And25TowardActorZero(void)
{
    struct Rec_383 *origin = Talk_unk30_4(0);
    struct Rec_383 *first = Talk_unk31_4(24);
    struct Rec_383 *second = Talk_unk32_4(25);

    first->f6 = Talk_unk27(origin->f16 - first->f16, origin->f8 - first->f8);
    second->f6 = Talk_unk28(origin->f16 - second->f16, origin->f8 - second->f8);
}

void Scene_RunLateSequence(void)
{
    extern u8 gWork[];

    u32 i;
    s32 record;
    s32 v5;

    Talk_unk165_2();
    Talk_unk5_3(10, 0x3180000, 0x1a00000);
    Talk_unk6_3(11, 0x3200000, 0x1900000);
    Talk_unk7_3(12, 0x3080000, 0x1a00000);
    Talk_unk8_3(10, 0x3000, 0);
    Talk_unk9_3(11, 0x3000, 0);
    Talk_unk10_3(12, 0x3000, 0);
    Talk_unk11_3(11, 0xcccc, 0x6666);
    Talk_unk12_3(12, 0xcccc, 0x6666);
    Talk_unk13_3(0, 0x3100000, 0x1c00000);
    Talk_unk14_3(1, 0x3280000, 0x1b00000);
    Talk_unk15_3(2, 0x3080000, 0x1b00000);
    Talk_unk166_2(0, 19);
    Talk_unk167_2(1, 19);
    Talk_unk168_2(2, 19);
    v5 = 2;
    *(u8 *)(Talk_unk29(0) + 35) = v5;
    *(u8 *)(Talk_unk30(1) + 35) = v5;
    *(u8 *)(Talk_unk31(2) + 35) = v5;
    record = Talk_unk32(0);
    Talk_unk169_2(record, 0);
    record = Talk_unk33(2);
    Talk_unk170_2(record, 0);
    record = Talk_unk34(1);
    Talk_unk171_2(record, 0);
    Talk_unk16_3(8, 0xb000, 0);
    *(s32 *)((*(u8 *volatile *)gWork + 0x1c0)) = 0x209;
    Talk_unk172_2(0, 0);
    Talk_unk173_2();
    Talk_unk174_2();
    Talk_unk175_2();
    Talk_unk176_2(60);
    Talk_unk177_2(10, 3, 20);
    Talk_unk11_2(0x12dd);
    Talk_unk178_2(10, 30);
    Talk_unk179_2(8, 30);
    Talk_unk17_3(11, 0x328, 0x1c8);
    Talk_unk18_3(12, 0x318, 0x1c8);
    Talk_unk180_2(12);
    Talk_unk181_2(12, 0, 0);
    Talk_unk182_2(11);
    Talk_unk183_2(11, 0, 0);
    Talk_unk184_2(30);
    Talk_unk185_2(11, 3, 20);
    Talk_unk186_2(11, 20);
    Talk_unk187_2(12, 0, 30);
    Talk_unk188_2(12, 60);
    Talk_unk189_2();
}

s32 OvObj_GetObjectTwoByte118(void)
{
    return Talk_unk37_4(2)[0x118];
}

s32 OvObj_RunObjectTwoWhenFlagged(void)
{
    u8 *o;

    Talk_unk190_2();
    o = Talk_unk38_4(2);
    o += 0xf8;
    if (*(s32 *)o & 1) {
        Talk_SetMode(2, 0, 0, 0);
        Talk_unk76_2(126);
        Talk_unk77(0);
        return Talk_unk35(2);
    }
}

/* Sets up actors 10, 11, 12 (position, pose, flags) and actors 0-2 and 8-9 and
 * 13-14 in several later waves, moving and animating them through a long
 * scripted sequence, branches once on an actor-12 state check, then advances
 * the shared scene phase twice before returning. */
void RunEventScript01(void)
{
    extern u8 gWork[];

    u32 i;
    s32 record;
    u8 *work;
    s32 base5_200d17c;
    u8 *actor12_record;

    record = Scene_GetRecord_1(12);
    actor12_record = *(volatile s32 *)(record + 80);
    BattleRuntime_Reset_1();
    ObjectMotion_SetHorizontalPositionWithTerrain_1(10, 0x3180000, 0x1a00000);
    ObjectMotion_SetHorizontalPositionWithTerrain_2(11, 0x3200000, 0x1900000);
    ObjectMotion_SetHorizontalPositionWithTerrain_3(12, 0x3080000, 0x1880000);
    record = Scene_GetRecord_2(10);
    Talk_unk191_2(record, 0);
    record = Scene_GetRecord_3(11);
    Talk_unk192_2(record, 0);
    record = Scene_GetRecord_4(12);
    Talk_unk193_2(record, 0);
    Object_SetModeById_1(10, 9);
    Object_SetModeById_2(11, 9);
    Object_SetModeById_3(12, 9);
    /* Clear the low bit of the flag byte at +35. */
    *(u8 *)(Scene_GetRecord_5(12) + 35) &= 254;
    /* Set flag bits 0x0c of the byte at +9. */
    actor12_record[9] |= 12;
    base5_200d17c = (s32)gOv9;
    ObjectMotion_EnableActionAndSetCallback_1(10, base5_200d17c);
    ObjectMotion_SetHorizontalPositionWithTerrain_4(0, 0x3180000, 0x1b80000);
    ObjectMotion_SetHorizontalPositionWithTerrain_5(1, 0x3280000, 0x1b00000);
    ObjectMotion_SetHorizontalPositionWithTerrain_6(2, 0x3080000, 0x1b80000);
    ObjectMotion_ArmCallback_1(0, 0xc000, 0);
    ObjectMotion_ArmCallback_2(1, 0xb000, 0);
    ObjectMotion_ArmCallback_3(2, 0xb000, 0);
    ObjectMotion_SetSpeedParameters_1(8, 0xcccc, 0x6666);
    ObjectMotion_ArmCallback_4(8, 0xb000, 0);
    SCENE_PHASE = 0x209;
    ObjectMotion_SetPositionAndReset_1(0, 0);
    Object_CommitPositionThenWaitIfModeZero_1();
    Talk_unk194_2();
    Talk_unk195_2();
    ObjectMotion_EnableActionAndSetCallback_2(11, base5_200d17c);
    BattleRuntime_WaitIfModeZero_1(30);
    ObjectMotion_EnableActionAndSetCallback_3(12, base5_200d17c);
    BattleRuntime_WaitIfModeZero_2(30);
    SceneWork_SetStepValue_1(0x12e4);
    Talk_unk196_2(10, 20);
    BattleEffect_SpawnLinkedResourceObject_1(8, 0x102, 0);
    BattleRuntime_WaitIfModeZero_3(60);
    Talk_unk197_2(8, 20);
    ObjectMotion_SetPositionAndReset_2(8, 0x328, 0x1c8);
    ObjectMotion_SetSpeedParameters_2(1, 0xcccc, 0x6666);
    ObjectMotion_SetPositionAndReset_3(1, 0x318, 0x1b0);
    ObjectMotion_ArmCallback_5(1, 0, 0);
    Motion_SetPosReset_4(8, 0x328, 0x198);
    ObjectMotion_ArmCallback_6(8, 0x8000, 0);
    Motion_SetPosReset_5(1, 0x328, 0x1b0);
    ObjectMotion_ArmCallback_7(1, 0xc000, 0);
    BattleRuntime_WaitIfModeZero_4(20);
    Talk_unk198_2(8, 3, 20);
    Talk_unk199_2(8, 20);
    Motion_SetPosReset_6(8, 0x300, 0x198);
    BattleRuntime_WaitIfModeZero_5(20);
    ObjectMotion_SetAngleToward_1(8, 0, 0);
    ObjectMotion_SetAngleToward_2(0, 8, 0);
    ObjectMotion_SetAngleToward_3(1, 8, 0);
    Talk_unk200_2(2, 8, 40);
    Talk_unk201_2(8, 30);
    Object_SetModeById_4(0, 3);
    Object_SetModeById_5(1, 3);
    Talk_unk202_2(2, 3, 20);
    Motion_SetPosReset_7(8, 0x2e8, 0x198);
    BattleRuntime_WaitIfModeZero_6(50);
    ObjectMotion_SetVariantCallbackAndInvokeObject_1(11, 2);
    Talk_unk203_2(11, 20);
    ObjectMotion_EnableActionAndSetCallback_4(11, base5_200d17c);
    ObjectMotion_SetVariantCallbackAndInvokeObject_2(1, 1);
    BattleRuntime_WaitIfModeZero_7(20);
    ObjectMotion_SetAngleToward_4(0, 11, 0);
    Motion_SetAngleToward_5(1, 11, 0);
    Talk_unk204_2(2, 11, 20);
    Talk_unk205_2(1, 20);
    ObjectMotion_SetVariantCallbackAndInvokeObject_3(12, 2);
    Talk_unk206_2(12, 30);
    Motion_EnableActCb_5(12, base5_200d17c);
    BattleEffect_SpawnLinkedResourceObject_2(1, 0x103, 0);
    BattleRuntime_WaitIfModeZero_8(60);
    Talk_unk207_2(1, 30);
    Talk_unk208_2(2, 0, 30);
    Talk_unk209_2(0, 2, 30);
    Talk_unk210_2(2, 3, 20);
    Talk_unk211_2(2, 20);
    Talk_unk212_2(1, 2, 30);
    Talk_unk213_2(0, 3, 30);
    Talk_unk214_2(2, 20);
    BattleEffect_SpawnLinkedResourceObject_3(1, 0x100, 0);
    BattleRuntime_WaitIfModeZero_9(60);
    Talk_unk215_2(1, 30);
    BattleFx_SpawnLinked_4(2, 0x101, 0);
    BattleRuntime_WaitIfModeZero_10(60);
    Talk_unk216_2(0, 1, 20);
    Object_SetModeById_6(0, 3);
    Talk_unk217_2(1, 3, 20);
    Talk_unk218_2(2, 0);
    Talk_unk219_2();
    Motion_SetAngleToward_6(0, 2, 0);
    Talk_unk220_2(1, 2, 10);
    ObjectMotion_SetVariantCallback_1(0, 1);
    ObjectMotion_SetVariantCallbackAndInvokeObject_4(1, 1);
    BattleRuntime_WaitIfModeZero_11(20);
    Battle_WaitMode0_12(1, 0x102);
    Battle_WaitMode0_13(60);
    Talk_unk221_2(0, 1, 10);
    ObjectMotion_SetSpeedParameters_3(1, 0);
    if (UiWork_WaitThenFinalizeCapacity_1(0, 0) == 0) {
        Battle_WaitMode0_14(20);
        Motion_SetAngleToward_7(0, 2, 0);
        Talk_unk222_2(1, 2, 20);
        Talk_unk223_2();
        Motion_SetVarCb_2(0, 1);
        Motion_SetVarCbObj_5(1, 1);
        ((void (*)())Talk_unk36)(20);
        Talk_unk224_2(1, 20);
    } else {
        bump_step(1);
        BattleFx_SpawnLinked_5(1, 0x102, 0);
        Battle_WaitMode0_15(60);
        Talk_unk225_2(1, 20);
        Motion_SetAngleToward_8(0, 2, 0);
        Talk_unk226_2(1, 2, 20);
        Talk_unk227_2();
        Motion_SetVarCb_3(0, 1);
        Motion_SetVarCbObj_6(1, 1);
        Battle_WaitMode0_16(20);
    }
    BattleFx_SpawnLinked_6(2, 0x102, 0);
    Battle_WaitMode0_17(60);
    Talk_unk228_2(2, 4, 20);
    SceneWork_SetStepValue_2(0x12f2);
    Talk_unk229_2(2, 20);
    Object_SetModeById_7(0, 3);
    Talk_unk230_2(1, 3, 40);
    Motion_SetPosReset_8(8, 0x328, 0x198);
    Motion_ArmCb_8(8, 0x8000, 0);
    Battle_WaitMode0_18(30);
    Motion_SetAngleToward_9(0, 8, 0);
    Motion_SetAngleToward_10(1, 8, 0);
    Talk_unk231_2(2, 8, 20);
    Motion_SetVarCbObj_7(8, 1);
    Battle_WaitMode0_19(20);
    Talk_unk232_2(8, 20);
    ObjectMotion_SetSpeedParameters_4(9, 0xcccc, 0x6666);
    ObjectMotion_SetSpeedParameters_5(13, 0xcccc, 0x6666);
    Motion_SetSpeed_6(14, 0xcccc, 0x6666);
    Motion_SetHPosTerrain_7(9, 0x2e80000, 0x1980000);
    Motion_SetPosReset_9(9, 0x300, 0x198);
    Talk_unk233_2(9, 10, 30);
    Motion_SetHPosTerrain_8(13, 0x2e80000, 0x1980000);
    Motion_SetPosReset_10(13, 0x300, 0x198);
    Motion_SetHPosTerrain_9(14, 0x2e80000, 0x1980000);
    ObjectMotion_ResetAndSetPositionInMode2_1(14, 0x310, 0x190);
    Motion_SetPosReset_11(13, 0x308, 0x1a8);
    ObjectMotion_CommitCurrentPositionAndActivate_1(14);
    Motion_SetAngleToward_11(13, 10, 0);
    Talk_unk234_2(14, 10, 20);
    Motion_SetAngleToward_12(0, 9, 0);
    Motion_SetAngleToward_13(1, 9, 0);
    Talk_unk235_2(2, 9, 20);
    Talk_unk236_2(9, 4, 20);
    Talk_unk237_2(9, 20);
    Talk_unk238_2(11, 20);
    Talk_unk239_2(12, 30);
    Talk_unk240_2(9, 13, 20);
    Motion_SetVarCbObj_8(13, 1);
    Battle_WaitMode0_20(20);
    Talk_unk241_2(13, 20);
    Talk_unk242_2(9, 3, 30);
    Talk_unk243_2(9, 14, 20);
    Talk_unk244_2(9, 3, 30);
    Talk_unk245_2(9, 10, 20);
    Talk_unk246_2(9, 20);
    Object_SetModeById_8(13, 3);
    Talk_unk247_2(14, 3, 20);
    Talk_unk248_2(13, 14, 20);
    Object_SetModeById_9(13, 3);
    Talk_unk249_2(14, 3, 20);
    ObjectMotion_ResetAndSetPositionInMode2_2(14, 0x318, 0x188);
    Motion_SetPosReset_12(13, 0x310, 0x190);
    Motion_SetAngleToward_14(13, 12, 0);
    ObjectMotion_CommitCurrentPositionAndActivate_2(14);
    Talk_unk250_2(14, 11, 20);
    Motion_SetVarCbObj_9(13, 1);
    Battle_WaitMode0_21(20);
    Talk_unk251_2(13, 20);
    Talk_unk252_2(14, 4, 20);
    Talk_unk253_2(14, 30);
    Talk_unk254_2(13, 0, 20);
    Talk_unk255_2(13, 20);
    Object_SetModeById_10(0, 3);
    Object_SetModeById_11(1, 3);
    Talk_unk256_2(2, 3, 50);
    /* Write the field at +0x1c8, then the phase/status word at +0x1c0, of
     * the shared scene work record. */
    work = *(u8 *volatile *)gWork;
    *(volatile s32 *)((work + 0x1c8)) = 30;
    *(volatile s32 *)((work + 0x1c0)) = 0x201;
    Talk_unk257_2();
    Motion_SetSpeed_7();
    Battle_WaitMode0_22(60);
    BattleRuntime_ScheduleShoulderButtonModeUpdate_1();
}

void RunDialoguePromptScene(void)
{
    extern u8 gWork[];

    u32 i;
    s32 off1c8;
    s32 off1d8;
    u8 *rec8;
    u8 *record;
    u8 *work;
    s32 base6_12fc;
    s32 base5_200d354;
    s32 base5_200d4c8;
    s32 base5_1324;
    u8 *p7;

    p7 = *(volatile s32 *)gWork;
    Talk_unk12_2(0x855);
    Talk_unk258_2();
    {
        u8 *record = Talk_unk33_4(12);
        u8 value = *(volatile u8 *)&record[35];

        record[35] = (u8)(value | 1);
    }
    Talk_unk19_3(15, 0x368, 0x1a9);
    Talk_unk20_3(16, 0x368, 0x199);
    Talk_unk21_3(17, 0x368, 0x179);
    Talk_unk22_3(11, 0x3080000, 0x1880000);
    Talk_unk23_3(10, 0x3180000, 0x1880000);
    Talk_unk24_3(12, 0x3280000, 0x1880000);
    Talk_unk259_2(10, 5);
    Talk_unk260_2(11, 5);
    Talk_unk261_2(12, 5);
    Talk_unk262_2(11, 0, 0);
    Talk_unk263_2(10, 0, 0);
    Talk_unk264_2(12, 0, 0);
    record = Talk_unk34_4(10);
    Talk_unk265_2((s32)record, 1);
    record = Talk_unk35_4(11);
    Talk_unk266_2((s32)record, 1);
    record = Talk_unk36_4(12);
    Talk_unk267_2((s32)record, 1);
    Talk_unk25_3(13, 0x3000000, 0x1980000);
    Talk_unk26_3(14, 0x3000000, 0x1a80000);
    Talk_unk27_3(9, 0x310, 0x1a8);
    Talk_unk28_3(8, 0x3280000, 0x1980000);
    Talk_unk268_2(13, 9, 0);
    Talk_unk269_2(8, 9, 0);
    Talk_unk270_2(14, 10, 0);
    Talk_unk271_2(9, 10, 0);
    Talk_unk29_3(0, 0x3180000, 0x1b80000);
    Talk_unk30_3(1, 0x3280000, 0x1b80000);
    Talk_unk31_3(2, 0x3080000, 0x1b80000);
    Talk_unk272_2(0, 10, 0);
    Talk_unk273_2(1, 10, 0);
    Talk_unk274_2(2, 10, 0);
    work = *(u8 *volatile *)gWork;
    off1c8 = 0x1c8;
    *(volatile s32 *)((s32)work + off1c8) = 30;
    *(volatile s32 *)(((s32)work + 0x1c0)) = 0x201;
    Talk_unk275_2();
    Talk_unk276_2();
    Talk_unk277_2(40);
    Talk_unk278_2(10, 2);
    base6_12fc = (s32)gVal5;
    Talk_unk279_2(base6_12fc);
    Talk_unk280(10, 20);
    Talk_unk281(9, 1);
    Talk_unk282(20);
    Talk_unk283(9, 4, 20);
    Talk_unk284(9, 20);
    Talk_unk285(13, 3);
    Talk_unk286(8, 3, 20);
    Talk_unk287(11, 2);
    Talk_unk288(12, 2);
    Talk_unk289(60);
    Talk_unk32_3(13, 0xcccc, 0x6666);
    Talk_unk33_3(13, 0x2ea, 0x198);
    Talk_unk34_3(9, 0xb000, 0);
    Talk_unk35_3(14, 0xb000, 0);
    base5_200d354 = (s32)gOv10;
    Talk_unk290(11, base5_200d354);
    Talk_unk291(20);
    Talk_unk292(10, base5_200d354);
    Talk_unk293(15);
    Talk_unk294(12, base5_200d354);
    Talk_unk295(35);
    Talk_unk7_4(8, 0x200d2fc);
    Talk_unk296(20);
    Talk_unk297(13);
    Talk_unk298(13, 0, 0);
    Talk_unk299(40);
    Talk_unk36_3(9, 0xcccc, 0x6666);
    Talk_unk37_3(14, 0xcccc, 0x6666);
    Talk_unk38_3(9, 0x310, 0x198);
    Talk_unk300(9, 0, 0);
    Talk_unk39_3(14, 0x300, 0x198);
    Talk_unk301(14, 0, 20);
    Talk_unk302(9, 3, 20);
    Talk_unk303(9, 20);
    Talk_unk304(14, 3, 20);
    Talk_unk40_3(14, 0x2000, 10);
    Talk_unk305(14, 20);
    Talk_unk306(0, 1, 50);
    Talk_unk307(0, 2, 50);
    Talk_unk308(0, 9, 0);
    Talk_unk309(1, 9, 0);
    Talk_unk310(2, 9, 20);
    Talk_unk311(0, 3);
    Talk_unk312(1, 3);
    Talk_unk313(2, 3, 50);
    Talk_unk314(9, 14, 0);
    Talk_unk315(9, 3, 20);
    Talk_unk316(9, 20);
    Talk_unk8_4(14, 0x200d3ac);
    Talk_unk317(50);
    Talk_unk5(9, 0x200d444);
    Talk_unk41_3(1, 0xcccc, 0x6666);
    Talk_unk42_3(2, 0xcccc, 0x6666);
    Talk_unk43_3(1, 0x318, off1c8);
    Talk_unk44_3(1, 0xd000, 0);
    Talk_unk318(30);
    Talk_unk45_3(2, 0x318, 0x198);
    Talk_unk319(2, 0, 0);
    Talk_unk46_3(1, 0x328, off1c8);
    Talk_unk47_3(1, 0xd000, 0);
    Talk_unk320(100);
    Talk_unk321(14, 9, 60);
    Talk_unk322(9, 14, 40);
    Talk_unk323(9, 3, 40);
    Talk_unk324(9, 0, 0);
    Talk_unk325(20);
    Talk_unk326(9, 2);
    Talk_unk327(124);
    Talk_unk328(15, 4);
    Talk_unk48_3(18, 0x3680000, 0x1a80000);
    Talk_unk329(18, 1);
    Talk_unk49_3(18, 0xcccc, 0x6666);
    Talk_unk50_3(18, 0, -8);
    Talk_unk330(18);
    Talk_unk331(18, 2);
    Talk_unk332(60);
    Talk_unk333((base6_12fc + 5), 1);
    Talk_unk334(15, 2);
    Talk_unk335(18, 0, 0);
    {
        u8 *work0 = *(u8 **)gWork;
        u16 *slot0;
        s32 next0;

        off1d8 = 0x1d8;
        slot0 = (u16 *)((s32)work0 + off1d8);
        next0 = *slot0 + 1;
        *slot0 = next0;
    }
    Talk_unk336(14, 1);
    Talk_unk337(14, 20);
    Talk_unk338(0, 1, 40);
    Talk_unk339(9, 14, 20);
    Talk_unk340(9, 3, 20);
    Talk_unk341(9, 30);
    Talk_unk342(0, 14, 0);
    Talk_unk343(1, 14, 40);
    Talk_unk344(14, 0, 0);
    Talk_unk345(40);
    Talk_unk346(14, 2);
    Talk_unk347(124);
    Talk_unk348(16, 4);
    {
        u8 *rec;
        s32 t = 0;
        u16 zero_sym = (u16)(t + t);

        rec = (u8 *)Talk_unk37(19);
        rec[85] = zero_sym;
    }
    Talk_unk349(19, 1);
    Talk_unk51_3(19, 0x3680000, 0x1980000);
    Talk_unk52_3(19, 0xcccc, 0x6666);
    Talk_unk53_3(19, 0, -8);
    Talk_unk350(19);
    Talk_unk351(19, 2);
    Talk_unk352(60);
    Talk_unk353((base6_12fc + 8), 1);
    Talk_unk354(16, 2);
    Talk_unk355(19, 0, 0);
    bump_step(off1d8, 1);
    Talk_unk54_3(9, 0x102, 0);
    Talk_unk356(60);
    Talk_unk357(9, 20);
    Talk_unk358(0, 2);
    Talk_unk359(1, 2);
    Talk_unk360(2, 2);
    Talk_unk361(20);
    Talk_unk362(14, 3, 50);
    Talk_unk363(9, 0, 20);
    Talk_unk364(9, 30);
    Talk_unk55_3(14, 0xd000, 0);
    Talk_unk365(30);
    Talk_unk56_3(14, 0x100, 0);
    Talk_unk366(60);
    Talk_unk57_3(14, 0x358, 0x178);
    Talk_unk367(20);
    Talk_unk368(14, 9, 20);
    Talk_unk369(14, 20);
    Talk_unk370(9, 14, 0);
    Talk_unk58_3(2, 0x100, 0);
    Talk_unk371(60);
    Talk_unk372(2, 14, 30);
    Talk_unk373(9, 2, 20);
    Talk_unk374(9, 3, 20);
    Talk_unk375(9, 0);
    Talk_unk59_3(14, 0x5000, 0);
    Talk_unk376(30);
    Talk_unk377(2, 9, 20);
    Talk_unk378(2, 3, 20);
    Talk_unk379(9, 20);
    Talk_unk380(0, 2, 0);
    Talk_unk381(1, 2, 20);
    Talk_unk382(0, 3);
    Talk_unk383(1, 3, 40);
    Talk_unk384(2, 3, 30);
    Talk_unk385(9, 1);
    Talk_unk386(20);
    Talk_unk387(9, 4, 20);
    Talk_unk388(9, 20);
    Talk_unk389(0, 9, 0);
    Talk_unk390(1, 9, 0);
    Talk_unk391(2, 9, 20);
    Talk_unk392(0, 1);
    Talk_unk393(1, 1);
    Talk_unk60_3(2, 0x102, 0);
    Talk_unk394(60);
    Talk_unk395(2, 40);
    Talk_unk396(9, 3, 20);
    Talk_unk397(0, 4);
    Talk_unk398(1, 4);
    Talk_unk399(2, 3);
    Talk_unk400(30);
    Talk_unk401(9, 4, 20);
    Talk_unk402(9, 20);
    Talk_unk403(0, 1, 20);
    Talk_unk404(2, 2);
    Talk_unk405(2, 4, 30);
    Talk_unk61_3(2, 0x18000, 0xc000);
    Talk_unk62_3(2, 0x320, 0x198);
    Talk_unk406(2, 2);
    Talk_unk407(2, 0);
    Talk_unk408(0, 9, 0);
    Talk_unk409(1, 9, 30);
    Talk_unk410(9, 4, 20);
    Talk_unk411(9, 20);
    Talk_unk9_4(0, 0x102);
    Talk_unk10_4(1, 0x102);
    Talk_unk11_4(2, 0x102);
    Talk_unk412(60);
    Talk_unk413(2, 20);
    Talk_unk414(9, 1);
    Talk_unk415(20);
    Talk_unk416(9, 20);
    Talk_unk63_3(0, 0x101, 0);
    Talk_unk64_3(1, 0x101, 0);
    Talk_unk65_3(2, 0x101, 0);
    Talk_unk417(60);
    Talk_unk418(9, 4, 20);
    Talk_unk419(9, 20);
    Talk_unk420(0, 1);
    Talk_unk421(1, 1);
    Talk_unk422(2, 1);
    Talk_unk423(20);
    Talk_unk424(9, 3, 20);
    Talk_unk425(9, 40);
    Talk_unk66_3(2, 0x105, 0);
    Talk_unk426(60);
    Talk_unk67_3(9, 0x348, 0x1a8);
    Talk_unk427(9, 0, 20);
    Talk_unk428(9, 20);
    Talk_unk429(0, 3, 20);
    Talk_unk68_3(9, 0x5000, 0);
    Talk_unk430(20);
    Talk_unk431(9, 20);
    Talk_unk432(1, 1);
    Talk_unk433(20);
    Talk_unk434(1, 20);
    Talk_unk435(9, 3, 20);
    Talk_unk436(9, 30);
    Talk_unk437(9, 14, 20);
    Talk_unk438(9, 20);
    Talk_unk69_3(14, 0x358, 0x198);
    base5_200d4c8 = (s32)gOv11;
    Talk_unk439(9, base5_200d4c8);
    Talk_unk440(14, base5_200d4c8);
    Talk_unk70_3(1, 0x318, off1c8);
    Talk_unk71_3(2, 0xcccc, 0x6666);
    Talk_unk72_3(2, 0x308, 0x1b0);
    Talk_unk441(1);
    Talk_unk73_3(1, 0xd000, 0);
    Talk_unk442(2);
    Talk_unk74_3(2, 0xd000, 0);
    Talk_unk443(9);
    Talk_unk444(0, 9, 0);
    rec8 = Talk_unk6(14);
    {
        u8 *target = rec8 + 91;
        s32 shown = 1;

        *target = shown;
    }
    *(s32 *)((s32)rec8 + 56) = -0x80000000;
    *(s32 *)((s32)rec8 + 60) = -0x80000000;
    *(s32 *)((s32)rec8 + 64) = -0x80000000;
    Talk_unk75_3(9, 0x100, 0);
    Talk_unk445(14, 1);
    Talk_unk446(50);
    Talk_unk447(9, 0, 0);
    Talk_unk76_3(1, 0x328, 0x1b8);
    Talk_unk77_2(1, 0xb000, 0);
    Talk_unk448(20);
    Talk_unk449(9, 20);
    Talk_unk450(0, 1, 50);
    Talk_unk451(0, 9, 0);
    Talk_unk452(1, 9, 20);
    Talk_unk453(9, 3, 20);
    Talk_unk78_2(9, 0x2e8, 0x198);
    Talk_unk79_2(14, 0x2e8, 0x198);
    Talk_unk454(9);
    Talk_unk455(14);
    Talk_unk456(30);
    Talk_unk80_2(0, 0x318, 0x198);
    Talk_unk457(0, 1, 30);
    Talk_unk81_2(2, 0x105, 0);
    Talk_unk458(50);
    Talk_unk459(0, 2, 0);
    Talk_unk460(1, 2, 20);
    Talk_unk461(1, 1);
    Talk_unk462(20);
    Talk_unk463(1, 40);
    Talk_unk464(2, 3, 30);
    Talk_unk465(0, 1, 40);
    Talk_unk466(0, 4);
    Talk_unk467(1, 4, 30);
    Talk_unk468(0, 2, 0);
    Talk_unk469(1, 2, 20);
    Talk_unk470(1, 1);
    Talk_unk471(20);
    Talk_unk472(1, 40);
    Talk_unk473(2, 2);
    Talk_unk474(20);
    Talk_unk475(2, 20);
    Talk_unk476(0, 3);
    Talk_unk477(1, 3, 20);
    Talk_unk478(2, 4, 20);
    Talk_unk479(2, 20);
    Talk_unk82_2(0, 0x101, 0);
    Talk_unk83_2(1, 0x101, 0);
    Talk_unk480(60);
    Talk_unk481(2, 3, 20);
    Talk_unk482(2, 20);
    Talk_unk84_2(0, 0x100, 0);
    Talk_unk85_2(1, 0x100, 0);
    Talk_unk483(60);
    Talk_unk484(2, 3, 20);
    Talk_unk485(2, 20);
    Talk_unk86_2(0, 0x102, 0);
    Talk_unk87_2(1, 0x102, 0);
    Talk_unk486(60);
    Talk_unk487(2, 4, 20);
    Talk_unk488(2, 20);
    Talk_unk489(1, 3, 20);
    Talk_unk490(1, 30);
    Talk_unk7(2, 0);
    if (Talk_unk8(0, 0) != 0) {
        Talk_unk491(20);
        Talk_unk492(1, 1);
        Talk_unk493(20);
        Talk_unk494(1, 20);
        Talk_unk495(2, 4, 20);
        Talk_unk496(2, 20);
    }
    Talk_unk497(2, 3, 30);
    Talk_unk88_2(2, 0x320, 0x1c8);
    Talk_unk498(0, 2, 0);
    Talk_unk499(1, 2, 0);
    Talk_unk89_2(2, 0x358, 0x1c8);
    Talk_unk500(0, 2, 0);
    Talk_unk501(1, 2, 0);
    Talk_unk90_2(2, 0x358, 0x178);
    Talk_unk502(0, 2, 0);
    Talk_unk503(1, 2, 0);
    Talk_unk504(2, 0, 0);
    Talk_unk505(20);
    Talk_unk506(2, 2);
    rec8 = Talk_unk91_2(2, 17, 65);
    Talk_unk507(60);
    base5_1324 = (s32)gVal6;
    Talk_unk508(base5_1324, 1);
    Talk_unk509((s32)rec8);
    Talk_unk510(17, 2);
    Talk_unk511(20);
    Talk_unk512(2, 3, 20);
    Talk_unk92_2(2, 0x358, 0x1c8);
    Talk_unk513(0, 2, 0);
    Talk_unk514(1, 2, 0);
    Talk_unk93_2(2, 0x320, 0x1c8);
    Talk_unk515(0, 2, 0);
    Talk_unk516(1, 2, 0);
    Talk_unk94_2(2, 0x308, 0x1a8);
    Talk_unk517(0, 2, 0);
    Talk_unk518(1, 2, 30);
    Talk_unk519(2, 3, 20);
    Talk_unk520((base5_1324 + 1));
    Talk_unk521(2, 20);
    Talk_unk522(0, 3);
    {
        u16 *slot = (u16 *)((s32)p7 + 0x1d8);
        s32 saved = *(s16 *)slot;

        if (Talk_unk12_4()!= 0) {
            Talk_unk13_2(0x132a);
            Talk_unk523(2, 0);
            Talk_unk524();
        }
        Talk_unk525(2);
        *slot = saved;
    }
    Talk_unk526(1, 3, 50);
    Talk_unk95_2(2, 0x308, 0x198);
    Talk_unk527(0, 2, 0);
    Talk_unk528(1, 2, 0);
    Talk_unk96_2(2, 0x2e8, 0x198);
    Talk_unk529(40);
    Talk_unk530(0, 1, 20);
    Talk_unk531(1, 1);
    Talk_unk532(20);
    Talk_unk533(1, 20);
    Talk_unk534(0, 3, 20);
    Talk_unk535(1, 2);
    record = Talk_unk9(0);
    if ((s32)record != 0) {
        Talk_unk536(1, *(s16 *)((s32)record + 10), *(s16 *)((s32)record + 18));
    }
    Talk_unk537(1);
    Talk_unk538(1, 0, 0);
    Talk_unk539(30);
    Talk_unk540(8, 0, 0);
    Talk_unk541(9, 0, 0);
    Talk_unk542(13, 0, 0);
    Talk_unk543(14, 0, 0);
    Talk_unk544(10, 0, 0);
    Talk_unk545(11, 0, 0);
    Talk_unk546(12, 0, 0);
    Talk_unk547(2, 0, 0);
    *(s32 *)((*(u8 *volatile *)gWork + 0x1c0)) = 0x209;
    Talk_unk548();
}

void State_SetWord1c0To209AndRun(void)
{
    u8 *state;

    state = *(u8 **)0x03001ebc;
    *(s32 *)(state + 0x1c0) = 0x209;
    Talk_unk549();
    Talk_unk550();
    Talk_unk78(1);
}

void Actor_SetModeZeroAndValue(s32 a, s32 b)
{
    Talk_Apply32(a, 0);
    Talk_unk79(b);
}

void Scene_RunSplitTripleSteps(s32 a, s32 b, s32 c)
{
    Talk_unk116_2(a, b, 0);
    Talk_unk80(c);
}

void Actor_SetPairZeroAndValue(s32 a, s32 b, s32 c)
{
    Talk_unk117_2(a, b, 0);
    Talk_unk81(c);
}

void Effect_ApplyThreeValuesAndFinish(s32 a, s32 b, s32 c)
{
    Talk_unk118_2(a, b, c);
    Talk_unk82(a);
    Talk_unk83(c);
}

void Effect_ApplyPairWithValue141(s32 a, s32 b)
{
    Talk_Apply33(141, 1);
    Talk_Apply34(a, b);
    Talk_unk551();
    Talk_unk84(1);
    Talk_unk85(1);
}

void State_SetValue2ThenFinish(void)
{
    Talk_unk86(2);
    Talk_unk552();
    Talk_unk553();
}

void OvObj_ConfigureObject22WithResource17(s32 a)
{
    u8 *o;
    u8 *q;
    u8 *p;
    u8 *v;
    s32 z;
    s32 m;

    z = 0;
    o = Talk_unk39_4(22);
    if (o != 0) {
        q = *(u8 **)(o + 0x50);
        p = q + 38;
        *p = z;
        p += 1;
        *p = z;
        m = 33;
        m = -m;
        q[5] &= m;
        q[9] &= 15;
        o[0x55] = z;
        o[0x5c] = 1;
        v = Talk_unk40_4(17, 0x608);
        Talk_unk87(a);
        v += 0x400;
        Talk_unk119_2(q[28], 0x80, v);
        Talk_unk88(17);
    }
}

u8 *SceneData_FindEntryAtPosition(s32 *o)
{
    s32 x = (o[0] + (s32)0xFFC00000) >> 19;
    s32 y = (o[2] + (s32)0xFD900000) >> 19;
    u8 *e = gOv6;
    u8 *ret = 0;
    u32 i;

    for (i = 0; i <= 36; i++, e += 16) {
        s32 a = e[0];

        if (a == x || a + 1 == x) {
            s32 b = e[1];

            if (b == y || b + 1 == y) {
                ret = e;
                break;
            }
        }
    }
    return ret;
}

s32 Actor_CheckTileFreeOfKinds(u8 *p)
{
    s32 x;
    s32 y;

    if (p == 0) {
        return 1;
    }
    x = (p[0] << 19) + 0x480000;
    y = (p[1] << 19) + 0x2780000;
    if (Talk_unk120_2(x, y, 0) != 0 || Talk_unk121_2(x, y, 2) != 0
        || Talk_unk122_2(x, y, 24) != 0 || Talk_unk123_2(x, y, 25) != 0) {
        return -1;
    }
    return 0;
}

void Actor_ApplyScaledBytePairPosition(s32 a, u8 *p)
{
    Talk_SetMode2(a, (p[0] << 19) + 0x480000, 0, (p[1] << 19) + 0x2780000);
}

void Scene_RunScene383(void)
{
    extern u8 gWork[];

    Talk_unk554();
    Talk_unk13_4(0xa80000, -1, 0x2900000, 1);
    Talk_unk97_2(0, 0xcccc, 0x6666);
    Talk_unk98_2(1, 0xcccc, 0x6666);
    Talk_unk99_2(2, 0xcccc, 0x6666);
    Talk_unk100_2(0, 248, 0x2b8);
    Talk_unk101_2(1, 0xf80000, 0x2b80000);
    Talk_unk102_2(2, 0xf80000, 0x2b80000);
    Talk_unk103_2(0, 200, 0x2b8);
    Talk_unk104_2(1, 248, 0x2c8);
    Talk_unk105_2(2, 232, 0x2b8);
    Talk_unk555(1);
    Talk_unk106_2(1, 0xc000, 0);
    Talk_unk107_2(2, 0x8000, 0);
    Talk_unk556(0);
    Talk_unk557(1, 12);
    Talk_unk558();
    Talk_unk14_4(0x300000, 0x2400000, 0x1200000, 0x2e00000);
    Talk_unk108_2(1, 0x10000, 0x8000);
    Talk_unk109_2(2, 0xc000, 0x6000);
    Talk_unk110_2(24, 0x10000, 0x13333);
    Talk_unk111_2(25, 0x18000, 0x18000);
    *(volatile u16 *)gOv12 = 0;
    Talk_unk15_4(0x200c8c9, 0xc94);
    Talk_unk559(0x1ff);
    Talk_unk560();
    Talk_unk561(9);
}
