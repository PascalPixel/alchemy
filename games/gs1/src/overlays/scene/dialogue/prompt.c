#include "types.h"
#include "scene.h"
#include "abi/overlays/scene/dialogue/prompt.h"

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

u8 *Talk_Run16(s32);
u8 *Talk_Run17(s32);
u8 *Talk_Run18(s32);
u8 *Talk_Run19(s32);

struct Object *Talk_Run20(s32);

u8 *Talk_Run21(s32);

u8 *Talk_Run22(s32);

u8 *Talk_Run23(s32);

u8 *Talk_Run24(s32 actor);

u8 *Talk_Run25(s32 actor);
u8 *Talk_Run26(s32 actor);

u8 *Talk_Run27();
u8 *Scene_GetRecord_2();

void *Scene_GetRecord_1();
u8 *Talk_Run28(s32);

T *Talk_Run29(s32);

struct Rec_383 *Talk_Run30();
struct Rec_383 *Talk_Run31();
struct Rec_383 *Talk_Run32();

u8 *Scene_GetRecord_1();

u8 *Talk_Run33();

u8 *Talk_Run34();

u8 *Talk_Run35();

u8 *Talk_Run36();

u8 *Talk_Run37(s32);

u8 *Talk_Run38(s32);

u8 *Talk_Run39(s32);
u8 *Talk_Run40(s32, s32);

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
    Talk_Do14(ret);
    return ret;
}

void ActorDraw_SetSceneCellByAngle(void)
{
    extern u8 *gWork;

    s32 x;
    s32 z;

    if (*(u16 *)(Talk_Run16(0) + 6) >= 0xa000
        && *(u16 *)(Talk_Run17(0) + 6) <= 0xe000) {
        Talk_Run41();
        x = 42;
        z = 85;
        Talk_SetRect4(41, 85, 1, 1, x, z);
    } else if (*(u16 *)(Talk_Run18(0) + 6) >= 0x2000
               && *(u16 *)(Talk_Run19(0) + 6) <= 0x6000) {
        Talk_Run42();
        x = 42;
        z = 85;
        Talk_SetRect5(43, 85, 1, 1, x, z);
    }
}

void Scene_RunObjectTwentySixPositionCheck(void)
{
    extern u8 *gWork;

    struct Object *obj;
    s32 x;
    s32 z;

    Talk_Run43();
    obj = Talk_Run20(26);
    if ((obj->x >> 20) == 42) {
        x = 41;
        z = 24;
        Talk_SetRect6(101, 24, 3, 4, x, z);
        Talk_Do15(0x859);
    }
    Talk_Run44();
}

void Scene_RunActorNineteenAngleDialogue(void)
{
    extern u8 *gWork;

    s32 v = *(u16 *)(Talk_Run21(0) + 6);

    Talk_Run45();
    if (v >= 0xa001 && v <= 0xdfff) {
        Talk_Apply(4, 19);
    } else {
        if (Talk_Check10(0x855) == 0) {
            Talk_Do16((s32)&Value_00001280);
        } else {
            Talk_Do17(0x1370);
        }
        Talk_Apply2(19, 0);
    }
    Talk_Run46();
}

void Scene_RunActorTwentyAngleDialogue(void)
{
    extern u8 *gWork;

    s32 v = *(u16 *)(Talk_Run22(0) + 6);

    Talk_Run47();
    if (v >= 0xa001 && v <= 0xdfff) {
        Talk_Apply3(5, 20);
    } else {
        if (Talk_Check11(0x855) == 0) {
            Talk_Do18(0x1282);
        } else {
            Talk_Do19(0x1372);
        }
        Talk_Apply4(20, 0);
    }
    Talk_Run48();
}

void Scene_RunActorTwentyThreeAngleDialogue(void)
{
    extern u8 *gWork;

    s32 v = *(u16 *)(Talk_Run23(0) + 6);

    Talk_Run49();
    if (v >= 0xa001 && v <= 0xdfff) {
        Talk_Apply5(1, 23);
    } else {
        if (Talk_Check12(0x855) == 0) {
            Talk_Do20(0x128d);
        } else {
            Talk_Do21(0x137b);
        }
        Talk_Apply6(23, 0);
    }
    Talk_Run50();
}

void Scene_RunActorEighteenConditionalScene(void)
{
    Talk_Run51();
    if (Talk_Check13() == 0) {
        Talk_Apply7(18, 4);
        Talk_Do22(20);
        Talk_Do23(0x1384);
        Talk_Apply8(18, 0);
    } else {
        Talk_Apply9(231, 3);
        Talk_Apply10(231, 0);
    }
    Talk_Run52();
}

void Dialogue_ShowLine12BB(void)
{
    Talk_Run53();
    Talk_Do24(0x12bb);
    Talk_Apply11(1, 0);
}

void State_SetFlags92bAnd94b(void)
{
    Talk_Run54();
    Talk_Apply12(0x92b, 1);
    Talk_Apply13(0x94b, 1);
    Talk_Run55();
}

void State_SetFlags929And949(void)
{
    Talk_Run56();
    Talk_Apply14(0x929, 1);
    Talk_Apply15(0x949, 1);
    Talk_Run57();
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
    if (Talk_Check14(0x855) != 0) {
        return gOv4;
    }
    return gOv5;
}

void ActorDraw_RunActorModeOneThenZeroWithStep(s32 x)
{
    Talk_Apply16(x, 1);
    Talk_Place112(x, 0, 2);
    Talk_Apply17(x, 0);
}

void State_RunGuardedActorStep(s32 x)
{
    extern u8 *gWork;

    u8 *flag = (u8 *)Talk_Check15() + 91;
    s32 zero = 0;

    *flag = 1;
    Talk_Run58();
    Talk_Apply18(x, 1);
    Talk_Do25(2);
    Talk_Apply19(x, 0);
    Talk_Run59();
    *flag = zero;
}

void Dialogue_PromptAndCountSkip(s32 x)
{
    extern u8 *gWork;

    Talk_Place113(x, 0, 2);
    Talk_Apply20(x, 0);
    if (Talk_Apply21(0, 0) != 0) {
        *(u16 *)(gWork + 472) += 1;
    }
    Talk_Apply22(x, 0);
}

void Dialogue_RunActorElevenDialogue(void)
{
    Talk_Run60();
    Talk_Do26(0x1247);
    Talk_Apply23(11, 1);
    Talk_Do27(11);
    Talk_Run61();
}

void Scene_RunScene383(void)
{
    extern u8 gWork[];

    u32 i;
    s32 record;

    Talk_Run62();
    Talk_Do(0x1253);
    Talk_Run63(15);
    Talk_Place(15, 0x8000, 0);
    Talk_Run64();
}

void State_BranchOnSlotZeroFacingAndFlag855(void)
{
    s32 value = *(u16 *)(Talk_Run28(0) + 6);

    Talk_Run65();
    if (value >= 0xa001 && value <= 0xdfff) {
        Talk_Apply24(6, 21);
    } else if (Talk_Check16(0x855) == 0) {
        Talk_Do28(0x1284);
        Talk_Do29(21);
    } else {
        Talk_Do30(0x1374);
        Talk_Apply25(21, 0);
    }
    Talk_Run66();
}

void Dialogue_RunActor9FlaggedLine(void)
{
    Talk_Run67();
    if (Talk_Check17(0x855) == 0) {
        Talk_Do31(0x1243);
    } else {
        Talk_Do32(0x1353);
    }
    Talk_Do33(9);
    Talk_Run68();
}

void Dialogue_RunActorTwelveFlaggedDialogue(void)
{
    Talk_Run69();
    if (Talk_Check18(0x855) != 0) {
        Talk_Do34(0x135c);
    } else {
        Talk_Do35(0x124c);
    }
    Talk_Do36(12);
    Talk_Run70();
}

void Scene_RunFlag856DialogueBranch(void)
{
    extern u8 gWork[];

    s32 g;
    g = 0x851;
    Talk_Run71();
    if (Talk_Check19(0x856) != 0) {
        if (Talk_Check20(g) == 0) {
            Talk_Do37(0x1276);
            Talk_Do38(16);
            Talk_Do39(10);
            Talk_Place114(16, 3, 20);
            Talk_Do40(g);
        } else {
            Talk_Do41(0x1278);
        }
    } else {
        Talk_Do42(0x1250);
    }
    Talk_Do43(16);
    Talk_Run72();
}

void Dialogue_ShowLine128E(void)
{
    Talk_Run73();
    Talk_Do44(0x128e);
    Talk_Do45(18);
    Talk_Run74();
}

void Actor_StepActor24AnimationByFacing(void)
{
    T *p;
    s16 *q;
    s32 v;
    s32 n;

    p = Talk_Run29(24);
    Talk_Run75();
    Talk_Apply26(24, 2);
    Talk_Do46(0x12ac);
    Talk_Apply27(24, 0);
    Talk_Place115(24, 0x40000, 0x20000);
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
    Talk_Do47(24);
    Talk_Run76();
}

void Dialogue_RunActor10Line(void)
{
    Talk_Run77();
    Talk_Do48(0x1356);
    Talk_Do49(10);
    Talk_Run78();
}

void Dialogue_RunActor11Line(void)
{
    Talk_Run79();
    Talk_Do50(0x1359);
    Talk_Do51(11);
    Talk_Run80();
}

void Dialogue_RunActor14Line(void)
{
    Talk_Run81();
    Talk_Do52(0x1368);
    Talk_Do53(14);
    Talk_Run82();
}

void Scene_RunScene383SequenceC(void)
{
    extern u8 gWork[];

    u32 i;
    s32 record;

    Talk_Run83();
    if (Talk_Check(0x857) == 0) {
        Talk_Run84((s32)gVal);
        Talk_Run85(16, 20);
        Talk_Run86(16, 3, 20);
        Talk_Run87(16, 30);
        Talk_Run88(16, 0, 0);
        Talk_Run89(30);
        Talk_Run90(16, 2);
        Talk_Run91(30);
        Talk_Run92(0, 16, 20);
        Talk_Run93(16, 3, 20);
        bump_step(1);
        if (Talk_Run() == 0) {
            Talk_Run94(((s32)gVal + 3));
            Talk_Run95(16, 20);
            Talk_Run96();
            goto L_02000906;
        }
        Talk_Do2(0x857);
        Talk_Run97(189, 0);
    }
    Talk_Do3(0x1364);
    Talk_Run98(16, 0);
    Talk_Run99(20);
    if (Talk_Check2(0, 0) != 0) {
        bump_step(1);
    }
    Talk_Run100(16, 0);
    Talk_Run101();
    L_02000906:;
}

void Scene_RunScene383(void)
{
    extern u8 gWork[];

    u32 i;
    s32 record;

    Talk_Run102();
    Talk_Run103(18, 0, 2);
    if (Talk_Check3(0x85b) == 0) {
        Talk_Do4(0x137c);
        Talk_Run104(18, 0);
    } else {
        Talk_Do5(0x1385);
        Talk_Run105(18, 0);
    }
    if (Talk_Check4(0, 0) == 0) {
        Talk_Run106(20);
        Talk_Run107(18, 0);
        Talk_Run108(20);
        Talk_Run109(18, 2);
        Talk_Run110(20);
        if (Talk_Run2() == 0) {
            Talk_Run111(18, 4);
            Talk_Run112(20);
            Talk_Do6(0x1384);
            Talk_Run113(18, 0);
            goto L_020009ec;
        }
        Talk_Run114(231, 3);
        Talk_Run115(231, 0);
        Talk_Do7(0x85b);
    } else {
        bump_step(1);
        Talk_Run116(20);
        Talk_Run117(18, 3);
        Talk_Run118(20);
        Talk_Run119(18, 0);
    }
    L_020009ec:;
    Talk_Place2(18, 0x4000, 0);
    Talk_Run120();
}

void Dialogue_RunActorNineFlaggedDialogue(void)
{
    Talk_Run121();
    if (Talk_Check21(0x855) == 0) {
        Talk_Do54(0x1245);
    } else {
        Talk_Do55(0x1355);
    }
    Talk_Do56(9);
    Talk_Run122();
}

void Dialogue_RunActorElevenFlaggedDialogue(void)
{
    Talk_Run123();
    if (Talk_Check22(0x855) == 0) {
        Talk_Do57(0x124b);
    } else {
        Talk_Do58(0x135b);
    }
    Talk_Do59(11);
    Talk_Run124();
}

void Dialogue_ShowLine124EOr135E(void)
{
    Talk_Run125();
    if (Talk_Check23(0x855) == 0) {
        Talk_Do60(0x124e);
    } else {
        Talk_Do61(0x135e);
    }
    Talk_Do62(12);
    Talk_Run126();
}

void Dialogue_RunActor16FlaggedLine(void)
{
    Talk_Run127();
    if (Talk_Check24(0x855) == 0) {
        Talk_Do63(0x127c);
    } else {
        Talk_Do64(0x136c);
    }
    Talk_Do65(16);
    Talk_Run128();
}

void Dialogue_RunActorEighteenBranchedDialogue(void)
{
    Talk_Run129();
    if (Talk_Check25(0x855) == 0) {
        Talk_Do66(0x1294);
    } else if (Talk_Check26(0x85b) == 0) {
        Talk_Do67(0x1382);
    } else {
        Talk_Do68(0x1cf4);
    }
    Talk_Do69(18);
    Talk_Run130();
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
        Talk_Run131(2, 0);
        Battle_WaitMode0_1(40);
        Talk_Run132();
        SceneWork_SetStepValue_1(0x1256);
        Audio_PlayCue_2(60);
        Battle_WaitMode0_2(30);
        Talk_Run133(2, 3, 30);
        Talk_Run134(2, 30);
        Motion_SetVarCb_1(0, 1);
        Motion_SetVarCbObj_1(1, 1);
        Battle_WaitMode0_3(20);
        Talk_Run135(2, 0);
        Battle_WaitMode0_4(40);
        Talk_Run136();
        Talk_Run137(2, 30);
        Talk_Run138(0, 1, 50);
        Motion_ArmCb_3(0, 0xc000, 0);
        Motion_ArmCb_4(1, 0xd000, 0);
        Battle_WaitMode0_5(20);
        Talk_Run139(2, 0);
        Battle_WaitMode0_6(40);
        Talk_Run140();
        Motion_SetVarCbObj_2(2, 1);
        Talk_Run141(2, 50);
        Object_SetModeById_1(0, 3);
        Talk_Run142(1, 3, 20);
        Talk_Run143(2, 3, 20);
        Talk_Run144(2, 40);
        Motion_SetVarCbObj_3(2, 1);
        Battle_WaitMode0_7(30);
        Talk_Place3(2, 0xc000, 0);
        Battle_WaitMode0_8(30);
        Motion_SetPosReset_3(2, 0x178, 0x178);
        Battle_WaitMode0_9(40);
        Talk_Run145(0, 1, 50);
        Motion_ArmCb_5(0, 0xc000, 0);
        Motion_ArmCb_6(1, 0xd000, 0);
        Motion_SetVarCbObj_4(2, 1);
        Battle_WaitMode0_10(50);
        Talk_Run146(2, 3, 30);
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
        Talk_Run147();
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
    Talk_Do70(123);
    Talk_Do71(11);
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
    Talk_Do8(15); /* main:0808a248 */
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
    Talk_Do9(14); /* main:0808a248 */
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
    Talk_Place4(25, 0, 10);
    ObjectMotion_SetVariantCallbackAndInvokeObject_1(24, 2);
    BattleRuntime_WaitIfModeZero_3(20);
    SceneWork_SetStepValue_1(4758);
    Talk_Run3(24, 20);
    BattleRuntime_WaitIfModeZero_4(25, 258); /* main:0808a1f0 */
    BattleRuntime_WaitIfModeZero_5(60);
    Talk_Run4(25, 20);
    ObjectMotion_SetVariantCallbackAndInvokeObject_2(24, 1);
    Talk_Run5(24, 30);
    ObjectMotion_SetSpeedParameters_4(24, 262144, 131072);
    ObjectMotion_SetSpeedParameters_5(25, 229376, 114688);
    Motion_EnableActCb_1(25, 33609776);
    Motion_EnableActCb_2(24, 33609056);
    Object_LookupAndStep_1(24);
    Talk_SetRect(14, 45, 3, 1, 14, 44); /* main:080091c0 */
    GameFlag_Set_1(2130);
    GameFlag_Set_2(768);
    Talk_Run6(33598369, 3200); /* main:080000d0 */
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
    Talk_Do10(0x200aba1);
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
    Talk_Run148(2, 0, 30);
    ObjectMotion_SetVariantCallbackAndInvokeObject_1(2, 1);
    record_00001299 = (s32)gVal2;
    SceneWork_SetStepValue_1(record_00001299);
    BattleEvent_RunActionAndWait_1(2, 0);
    Motion_SetAngleToward_2(0, 2, 0);
    Talk_Run149(1, 2, 20);
    Object_SetModeById_1(0, 3);
    Talk_Run150(1, 3, 20);
    Talk_Run151(0, 1, 10);
    ObjectMotion_SetSpeedParameters_4(1, 0);
    if (UiWork_WaitThenFinalizeCapacity_1(0, 0) != 0) {
        bump_step(1);
    }
    Talk_Run152(1, 30);
    SceneWork_SetStepValue_2((record_00001299 + 4));
    Motion_SetAngleToward_3(0, 2, 0);
    Talk_Run153(1, 2, 50);
    BattleEffect_SpawnLinkedResourceObject_1(2, 0x100, 0);
    BattleRuntime_WaitIfModeZero_1(60);
    Talk_Run154(0, 1, 50);
    Motion_SetAngleToward_4(0, 2, 0);
    Talk_Run155(1, 2, 30);
    Talk_Run156(2, 3, 10);
    Talk_Run157(2, 20);
    BattleEffect_SpawnLinkedResourceObject_2(0, 0x102, 0);
    BattleFx_SpawnLinked_3(1, 0x102, 0);
    BattleRuntime_WaitIfModeZero_2(60);
    Talk_Run158(2, 3, 20);
    Talk_Run159(2, 30);
    SceneWork_SetStepValue_3(0x129f);
    BattleEvent_RunActionAndWait_2(1, 0);
    ObjectMotion_ArmCallback_2(0, 0x4000, 0);
    BattleRuntime_WaitIfModeZero_3(40);
    Object_SetModeById_2(0, 3);
    Object_SetModeById_3(1, 3);
    Talk_Run160(2, 3, 50);
    Motion_ResetPosMode2_2(2, 248, 0x2d8);
    ObjectMotion_SetPositionAndReset_3(1, 248, 0x2d8);
    ObjectMotion_SetHorizontalPositionWithTerrain_3(1, 0, 0);
    Motion_SetHPosTerrain_4(2, 0, 0);
    Motion_SetHPosTerrain_5(24, 0x680000, 0x2b80000);
    Motion_SetHPosTerrain_6(25, 0x780000, 0x2b80000);
    ObjectMotion_ArmCallback_3(24, 0, 0);
    ObjectMotion_ArmCallback_4(25, 0x8000, 0);
    Talk_SetRect2(14, 50, 3, 1, 14, 44);
    BattleRuntime_ScheduleShoulderButtonModeUpdate_1();
}

void Scene_RunSteps107And250(void)
{
    extern u8 gWork[];

    Talk_Do72(0x107);
    Talk_Do73(0x250);
    Talk_Run161();
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
    Talk_Run162(24, 20);
    ObjectMotion_ArmCallback_1(25, 0, 20);
    BattleRuntime_WaitIfModeZero_3(25, 0x102);
    ObjectMotion_SetVariantCallbackAndInvokeObject_2(25, 2);
    Talk_Run163(25, 20);
    Motion_CallWaitAnim_1(24, 4);
    BattleRuntime_WaitIfModeZero_4(20);
    Talk_Run164(24, 20);
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
    Talk_SetRect3(14, 48, 4, 1, 14, 44);
}

void Actor_UpdateAnimationOnStateMatch(s32 actor, s32 expected, s32 next, const u8 *desc)
{
    u8 *rec = Talk_Run24(actor);

    if (*(s16 *)(rec + 100) == expected) {
        Talk_Apply31(actor, desc);
        *(u16 *)(rec + 100) = (u16)next;
    }
}

void State_SetFlagByActorPosition(void)
{
    u8 *p0 = Talk_Run25(0);
    s32 rx = *(s32 *)(p0 + 8);
    s32 x;
    s32 z;

    u8 *p1 = Talk_Run26(0);
    x = rx >> 20;
    z = *(s32 *)(p1 + 16);
    x = x - 34;
    z = z >> 20;

    if ((u32)x <= 1 && z > 40 && z <= 42) {
        Talk_Do74(148 << 2);
    } else {
        Talk_Do75(148 << 2);
    }
}

void Actor_FaceActors24And25TowardActorZero(void)
{
    struct Rec_383 *origin = Talk_Run30(0);
    struct Rec_383 *first = Talk_Run31(24);
    struct Rec_383 *second = Talk_Run32(25);

    first->f6 = Talk_Check27(origin->f16 - first->f16, origin->f8 - first->f8);
    second->f6 = Talk_Check28(origin->f16 - second->f16, origin->f8 - second->f8);
}

void Scene_RunLateSequence(void)
{
    extern u8 gWork[];

    u32 i;
    s32 record;
    s32 v5;

    Talk_Run165();
    Talk_Place5(10, 0x3180000, 0x1a00000);
    Talk_Place6(11, 0x3200000, 0x1900000);
    Talk_Place7(12, 0x3080000, 0x1a00000);
    Talk_Place8(10, 0x3000, 0);
    Talk_Place9(11, 0x3000, 0);
    Talk_Place10(12, 0x3000, 0);
    Talk_Place11(11, 0xcccc, 0x6666);
    Talk_Place12(12, 0xcccc, 0x6666);
    Talk_Place13(0, 0x3100000, 0x1c00000);
    Talk_Place14(1, 0x3280000, 0x1b00000);
    Talk_Place15(2, 0x3080000, 0x1b00000);
    Talk_Run166(0, 19);
    Talk_Run167(1, 19);
    Talk_Run168(2, 19);
    v5 = 2;
    *(u8 *)(Talk_Check29(0) + 35) = v5;
    *(u8 *)(Talk_Check30(1) + 35) = v5;
    *(u8 *)(Talk_Check31(2) + 35) = v5;
    record = Talk_Check32(0);
    Talk_Run169(record, 0);
    record = Talk_Check33(2);
    Talk_Run170(record, 0);
    record = Talk_Check34(1);
    Talk_Run171(record, 0);
    Talk_Place16(8, 0xb000, 0);
    *(s32 *)((*(u8 *volatile *)gWork + 0x1c0)) = 0x209;
    Talk_Run172(0, 0);
    Talk_Run173();
    Talk_Run174();
    Talk_Run175();
    Talk_Run176(60);
    Talk_Run177(10, 3, 20);
    Talk_Do11(0x12dd);
    Talk_Run178(10, 30);
    Talk_Run179(8, 30);
    Talk_Place17(11, 0x328, 0x1c8);
    Talk_Place18(12, 0x318, 0x1c8);
    Talk_Run180(12);
    Talk_Run181(12, 0, 0);
    Talk_Run182(11);
    Talk_Run183(11, 0, 0);
    Talk_Run184(30);
    Talk_Run185(11, 3, 20);
    Talk_Run186(11, 20);
    Talk_Run187(12, 0, 30);
    Talk_Run188(12, 60);
    Talk_Run189();
}

s32 OvObj_GetObjectTwoByte118(void)
{
    return Talk_Run37(2)[0x118];
}

s32 OvObj_RunObjectTwoWhenFlagged(void)
{
    u8 *o;

    Talk_Run190();
    o = Talk_Run38(2);
    o += 0xf8;
    if (*(s32 *)o & 1) {
        Talk_SetMode(2, 0, 0, 0);
        Talk_Do76(126);
        Talk_Do77(0);
        return Talk_Check35(2);
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
    Talk_Run191(record, 0);
    record = Scene_GetRecord_3(11);
    Talk_Run192(record, 0);
    record = Scene_GetRecord_4(12);
    Talk_Run193(record, 0);
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
    Talk_Run194();
    Talk_Run195();
    ObjectMotion_EnableActionAndSetCallback_2(11, base5_200d17c);
    BattleRuntime_WaitIfModeZero_1(30);
    ObjectMotion_EnableActionAndSetCallback_3(12, base5_200d17c);
    BattleRuntime_WaitIfModeZero_2(30);
    SceneWork_SetStepValue_1(0x12e4);
    Talk_Run196(10, 20);
    BattleEffect_SpawnLinkedResourceObject_1(8, 0x102, 0);
    BattleRuntime_WaitIfModeZero_3(60);
    Talk_Run197(8, 20);
    ObjectMotion_SetPositionAndReset_2(8, 0x328, 0x1c8);
    ObjectMotion_SetSpeedParameters_2(1, 0xcccc, 0x6666);
    ObjectMotion_SetPositionAndReset_3(1, 0x318, 0x1b0);
    ObjectMotion_ArmCallback_5(1, 0, 0);
    Motion_SetPosReset_4(8, 0x328, 0x198);
    ObjectMotion_ArmCallback_6(8, 0x8000, 0);
    Motion_SetPosReset_5(1, 0x328, 0x1b0);
    ObjectMotion_ArmCallback_7(1, 0xc000, 0);
    BattleRuntime_WaitIfModeZero_4(20);
    Talk_Run198(8, 3, 20);
    Talk_Run199(8, 20);
    Motion_SetPosReset_6(8, 0x300, 0x198);
    BattleRuntime_WaitIfModeZero_5(20);
    ObjectMotion_SetAngleToward_1(8, 0, 0);
    ObjectMotion_SetAngleToward_2(0, 8, 0);
    ObjectMotion_SetAngleToward_3(1, 8, 0);
    Talk_Run200(2, 8, 40);
    Talk_Run201(8, 30);
    Object_SetModeById_4(0, 3);
    Object_SetModeById_5(1, 3);
    Talk_Run202(2, 3, 20);
    Motion_SetPosReset_7(8, 0x2e8, 0x198);
    BattleRuntime_WaitIfModeZero_6(50);
    ObjectMotion_SetVariantCallbackAndInvokeObject_1(11, 2);
    Talk_Run203(11, 20);
    ObjectMotion_EnableActionAndSetCallback_4(11, base5_200d17c);
    ObjectMotion_SetVariantCallbackAndInvokeObject_2(1, 1);
    BattleRuntime_WaitIfModeZero_7(20);
    ObjectMotion_SetAngleToward_4(0, 11, 0);
    Motion_SetAngleToward_5(1, 11, 0);
    Talk_Run204(2, 11, 20);
    Talk_Run205(1, 20);
    ObjectMotion_SetVariantCallbackAndInvokeObject_3(12, 2);
    Talk_Run206(12, 30);
    Motion_EnableActCb_5(12, base5_200d17c);
    BattleEffect_SpawnLinkedResourceObject_2(1, 0x103, 0);
    BattleRuntime_WaitIfModeZero_8(60);
    Talk_Run207(1, 30);
    Talk_Run208(2, 0, 30);
    Talk_Run209(0, 2, 30);
    Talk_Run210(2, 3, 20);
    Talk_Run211(2, 20);
    Talk_Run212(1, 2, 30);
    Talk_Run213(0, 3, 30);
    Talk_Run214(2, 20);
    BattleEffect_SpawnLinkedResourceObject_3(1, 0x100, 0);
    BattleRuntime_WaitIfModeZero_9(60);
    Talk_Run215(1, 30);
    BattleFx_SpawnLinked_4(2, 0x101, 0);
    BattleRuntime_WaitIfModeZero_10(60);
    Talk_Run216(0, 1, 20);
    Object_SetModeById_6(0, 3);
    Talk_Run217(1, 3, 20);
    Talk_Run218(2, 0);
    Talk_Run219();
    Motion_SetAngleToward_6(0, 2, 0);
    Talk_Run220(1, 2, 10);
    ObjectMotion_SetVariantCallback_1(0, 1);
    ObjectMotion_SetVariantCallbackAndInvokeObject_4(1, 1);
    BattleRuntime_WaitIfModeZero_11(20);
    Battle_WaitMode0_12(1, 0x102);
    Battle_WaitMode0_13(60);
    Talk_Run221(0, 1, 10);
    ObjectMotion_SetSpeedParameters_3(1, 0);
    if (UiWork_WaitThenFinalizeCapacity_1(0, 0) == 0) {
        Battle_WaitMode0_14(20);
        Motion_SetAngleToward_7(0, 2, 0);
        Talk_Run222(1, 2, 20);
        Talk_Run223();
        Motion_SetVarCb_2(0, 1);
        Motion_SetVarCbObj_5(1, 1);
        ((void (*)())Talk_Check36)(20);
        Talk_Run224(1, 20);
    } else {
        bump_step(1);
        BattleFx_SpawnLinked_5(1, 0x102, 0);
        Battle_WaitMode0_15(60);
        Talk_Run225(1, 20);
        Motion_SetAngleToward_8(0, 2, 0);
        Talk_Run226(1, 2, 20);
        Talk_Run227();
        Motion_SetVarCb_3(0, 1);
        Motion_SetVarCbObj_6(1, 1);
        Battle_WaitMode0_16(20);
    }
    BattleFx_SpawnLinked_6(2, 0x102, 0);
    Battle_WaitMode0_17(60);
    Talk_Run228(2, 4, 20);
    SceneWork_SetStepValue_2(0x12f2);
    Talk_Run229(2, 20);
    Object_SetModeById_7(0, 3);
    Talk_Run230(1, 3, 40);
    Motion_SetPosReset_8(8, 0x328, 0x198);
    Motion_ArmCb_8(8, 0x8000, 0);
    Battle_WaitMode0_18(30);
    Motion_SetAngleToward_9(0, 8, 0);
    Motion_SetAngleToward_10(1, 8, 0);
    Talk_Run231(2, 8, 20);
    Motion_SetVarCbObj_7(8, 1);
    Battle_WaitMode0_19(20);
    Talk_Run232(8, 20);
    ObjectMotion_SetSpeedParameters_4(9, 0xcccc, 0x6666);
    ObjectMotion_SetSpeedParameters_5(13, 0xcccc, 0x6666);
    Motion_SetSpeed_6(14, 0xcccc, 0x6666);
    Motion_SetHPosTerrain_7(9, 0x2e80000, 0x1980000);
    Motion_SetPosReset_9(9, 0x300, 0x198);
    Talk_Run233(9, 10, 30);
    Motion_SetHPosTerrain_8(13, 0x2e80000, 0x1980000);
    Motion_SetPosReset_10(13, 0x300, 0x198);
    Motion_SetHPosTerrain_9(14, 0x2e80000, 0x1980000);
    ObjectMotion_ResetAndSetPositionInMode2_1(14, 0x310, 0x190);
    Motion_SetPosReset_11(13, 0x308, 0x1a8);
    ObjectMotion_CommitCurrentPositionAndActivate_1(14);
    Motion_SetAngleToward_11(13, 10, 0);
    Talk_Run234(14, 10, 20);
    Motion_SetAngleToward_12(0, 9, 0);
    Motion_SetAngleToward_13(1, 9, 0);
    Talk_Run235(2, 9, 20);
    Talk_Run236(9, 4, 20);
    Talk_Run237(9, 20);
    Talk_Run238(11, 20);
    Talk_Run239(12, 30);
    Talk_Run240(9, 13, 20);
    Motion_SetVarCbObj_8(13, 1);
    Battle_WaitMode0_20(20);
    Talk_Run241(13, 20);
    Talk_Run242(9, 3, 30);
    Talk_Run243(9, 14, 20);
    Talk_Run244(9, 3, 30);
    Talk_Run245(9, 10, 20);
    Talk_Run246(9, 20);
    Object_SetModeById_8(13, 3);
    Talk_Run247(14, 3, 20);
    Talk_Run248(13, 14, 20);
    Object_SetModeById_9(13, 3);
    Talk_Run249(14, 3, 20);
    ObjectMotion_ResetAndSetPositionInMode2_2(14, 0x318, 0x188);
    Motion_SetPosReset_12(13, 0x310, 0x190);
    Motion_SetAngleToward_14(13, 12, 0);
    ObjectMotion_CommitCurrentPositionAndActivate_2(14);
    Talk_Run250(14, 11, 20);
    Motion_SetVarCbObj_9(13, 1);
    Battle_WaitMode0_21(20);
    Talk_Run251(13, 20);
    Talk_Run252(14, 4, 20);
    Talk_Run253(14, 30);
    Talk_Run254(13, 0, 20);
    Talk_Run255(13, 20);
    Object_SetModeById_10(0, 3);
    Object_SetModeById_11(1, 3);
    Talk_Run256(2, 3, 50);
    /* Write the field at +0x1c8, then the phase/status word at +0x1c0, of
     * the shared scene work record. */
    work = *(u8 *volatile *)gWork;
    *(volatile s32 *)((work + 0x1c8)) = 30;
    *(volatile s32 *)((work + 0x1c0)) = 0x201;
    Talk_Run257();
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
    Talk_Do12(0x855);
    Talk_Run258();
    {
        u8 *record = Talk_Run33(12);
        u8 value = *(volatile u8 *)&record[35];

        record[35] = (u8)(value | 1);
    }
    Talk_Place19(15, 0x368, 0x1a9);
    Talk_Place20(16, 0x368, 0x199);
    Talk_Place21(17, 0x368, 0x179);
    Talk_Place22(11, 0x3080000, 0x1880000);
    Talk_Place23(10, 0x3180000, 0x1880000);
    Talk_Place24(12, 0x3280000, 0x1880000);
    Talk_Run259(10, 5);
    Talk_Run260(11, 5);
    Talk_Run261(12, 5);
    Talk_Run262(11, 0, 0);
    Talk_Run263(10, 0, 0);
    Talk_Run264(12, 0, 0);
    record = Talk_Run34(10);
    Talk_Run265((s32)record, 1);
    record = Talk_Run35(11);
    Talk_Run266((s32)record, 1);
    record = Talk_Run36(12);
    Talk_Run267((s32)record, 1);
    Talk_Place25(13, 0x3000000, 0x1980000);
    Talk_Place26(14, 0x3000000, 0x1a80000);
    Talk_Place27(9, 0x310, 0x1a8);
    Talk_Place28(8, 0x3280000, 0x1980000);
    Talk_Run268(13, 9, 0);
    Talk_Run269(8, 9, 0);
    Talk_Run270(14, 10, 0);
    Talk_Run271(9, 10, 0);
    Talk_Place29(0, 0x3180000, 0x1b80000);
    Talk_Place30(1, 0x3280000, 0x1b80000);
    Talk_Place31(2, 0x3080000, 0x1b80000);
    Talk_Run272(0, 10, 0);
    Talk_Run273(1, 10, 0);
    Talk_Run274(2, 10, 0);
    work = *(u8 *volatile *)gWork;
    off1c8 = 0x1c8;
    *(volatile s32 *)((s32)work + off1c8) = 30;
    *(volatile s32 *)(((s32)work + 0x1c0)) = 0x201;
    Talk_Run275();
    Talk_Run276();
    Talk_Run277(40);
    Talk_Run278(10, 2);
    base6_12fc = (s32)gVal5;
    Talk_Run279(base6_12fc);
    Talk_Run280(10, 20);
    Talk_Run281(9, 1);
    Talk_Run282(20);
    Talk_Run283(9, 4, 20);
    Talk_Run284(9, 20);
    Talk_Run285(13, 3);
    Talk_Run286(8, 3, 20);
    Talk_Run287(11, 2);
    Talk_Run288(12, 2);
    Talk_Run289(60);
    Talk_Place32(13, 0xcccc, 0x6666);
    Talk_Place33(13, 0x2ea, 0x198);
    Talk_Place34(9, 0xb000, 0);
    Talk_Place35(14, 0xb000, 0);
    base5_200d354 = (s32)gOv10;
    Talk_Run290(11, base5_200d354);
    Talk_Run291(20);
    Talk_Run292(10, base5_200d354);
    Talk_Run293(15);
    Talk_Run294(12, base5_200d354);
    Talk_Run295(35);
    Talk_Run7(8, 0x200d2fc);
    Talk_Run296(20);
    Talk_Run297(13);
    Talk_Run298(13, 0, 0);
    Talk_Run299(40);
    Talk_Place36(9, 0xcccc, 0x6666);
    Talk_Place37(14, 0xcccc, 0x6666);
    Talk_Place38(9, 0x310, 0x198);
    Talk_Run300(9, 0, 0);
    Talk_Place39(14, 0x300, 0x198);
    Talk_Run301(14, 0, 20);
    Talk_Run302(9, 3, 20);
    Talk_Run303(9, 20);
    Talk_Run304(14, 3, 20);
    Talk_Place40(14, 0x2000, 10);
    Talk_Run305(14, 20);
    Talk_Run306(0, 1, 50);
    Talk_Run307(0, 2, 50);
    Talk_Run308(0, 9, 0);
    Talk_Run309(1, 9, 0);
    Talk_Run310(2, 9, 20);
    Talk_Run311(0, 3);
    Talk_Run312(1, 3);
    Talk_Run313(2, 3, 50);
    Talk_Run314(9, 14, 0);
    Talk_Run315(9, 3, 20);
    Talk_Run316(9, 20);
    Talk_Run8(14, 0x200d3ac);
    Talk_Run317(50);
    Talk_Check5(9, 0x200d444);
    Talk_Place41(1, 0xcccc, 0x6666);
    Talk_Place42(2, 0xcccc, 0x6666);
    Talk_Place43(1, 0x318, off1c8);
    Talk_Place44(1, 0xd000, 0);
    Talk_Run318(30);
    Talk_Place45(2, 0x318, 0x198);
    Talk_Run319(2, 0, 0);
    Talk_Place46(1, 0x328, off1c8);
    Talk_Place47(1, 0xd000, 0);
    Talk_Run320(100);
    Talk_Run321(14, 9, 60);
    Talk_Run322(9, 14, 40);
    Talk_Run323(9, 3, 40);
    Talk_Run324(9, 0, 0);
    Talk_Run325(20);
    Talk_Run326(9, 2);
    Talk_Run327(124);
    Talk_Run328(15, 4);
    Talk_Place48(18, 0x3680000, 0x1a80000);
    Talk_Run329(18, 1);
    Talk_Place49(18, 0xcccc, 0x6666);
    Talk_Place50(18, 0, -8);
    Talk_Run330(18);
    Talk_Run331(18, 2);
    Talk_Run332(60);
    Talk_Run333((base6_12fc + 5), 1);
    Talk_Run334(15, 2);
    Talk_Run335(18, 0, 0);
    {
        u8 *work0 = *(u8 **)gWork;
        u16 *slot0;
        s32 next0;

        off1d8 = 0x1d8;
        slot0 = (u16 *)((s32)work0 + off1d8);
        next0 = *slot0 + 1;
        *slot0 = next0;
    }
    Talk_Run336(14, 1);
    Talk_Run337(14, 20);
    Talk_Run338(0, 1, 40);
    Talk_Run339(9, 14, 20);
    Talk_Run340(9, 3, 20);
    Talk_Run341(9, 30);
    Talk_Run342(0, 14, 0);
    Talk_Run343(1, 14, 40);
    Talk_Run344(14, 0, 0);
    Talk_Run345(40);
    Talk_Run346(14, 2);
    Talk_Run347(124);
    Talk_Run348(16, 4);
    {
        u8 *rec;
        s32 t = 0;
        u16 zero_sym = (u16)(t + t);

        rec = (u8 *)Talk_Check37(19);
        rec[85] = zero_sym;
    }
    Talk_Run349(19, 1);
    Talk_Place51(19, 0x3680000, 0x1980000);
    Talk_Place52(19, 0xcccc, 0x6666);
    Talk_Place53(19, 0, -8);
    Talk_Run350(19);
    Talk_Run351(19, 2);
    Talk_Run352(60);
    Talk_Run353((base6_12fc + 8), 1);
    Talk_Run354(16, 2);
    Talk_Run355(19, 0, 0);
    bump_step(off1d8, 1);
    Talk_Place54(9, 0x102, 0);
    Talk_Run356(60);
    Talk_Run357(9, 20);
    Talk_Run358(0, 2);
    Talk_Run359(1, 2);
    Talk_Run360(2, 2);
    Talk_Run361(20);
    Talk_Run362(14, 3, 50);
    Talk_Run363(9, 0, 20);
    Talk_Run364(9, 30);
    Talk_Place55(14, 0xd000, 0);
    Talk_Run365(30);
    Talk_Place56(14, 0x100, 0);
    Talk_Run366(60);
    Talk_Place57(14, 0x358, 0x178);
    Talk_Run367(20);
    Talk_Run368(14, 9, 20);
    Talk_Run369(14, 20);
    Talk_Run370(9, 14, 0);
    Talk_Place58(2, 0x100, 0);
    Talk_Run371(60);
    Talk_Run372(2, 14, 30);
    Talk_Run373(9, 2, 20);
    Talk_Run374(9, 3, 20);
    Talk_Run375(9, 0);
    Talk_Place59(14, 0x5000, 0);
    Talk_Run376(30);
    Talk_Run377(2, 9, 20);
    Talk_Run378(2, 3, 20);
    Talk_Run379(9, 20);
    Talk_Run380(0, 2, 0);
    Talk_Run381(1, 2, 20);
    Talk_Run382(0, 3);
    Talk_Run383(1, 3, 40);
    Talk_Run384(2, 3, 30);
    Talk_Run385(9, 1);
    Talk_Run386(20);
    Talk_Run387(9, 4, 20);
    Talk_Run388(9, 20);
    Talk_Run389(0, 9, 0);
    Talk_Run390(1, 9, 0);
    Talk_Run391(2, 9, 20);
    Talk_Run392(0, 1);
    Talk_Run393(1, 1);
    Talk_Place60(2, 0x102, 0);
    Talk_Run394(60);
    Talk_Run395(2, 40);
    Talk_Run396(9, 3, 20);
    Talk_Run397(0, 4);
    Talk_Run398(1, 4);
    Talk_Run399(2, 3);
    Talk_Run400(30);
    Talk_Run401(9, 4, 20);
    Talk_Run402(9, 20);
    Talk_Run403(0, 1, 20);
    Talk_Run404(2, 2);
    Talk_Run405(2, 4, 30);
    Talk_Place61(2, 0x18000, 0xc000);
    Talk_Place62(2, 0x320, 0x198);
    Talk_Run406(2, 2);
    Talk_Run407(2, 0);
    Talk_Run408(0, 9, 0);
    Talk_Run409(1, 9, 30);
    Talk_Run410(9, 4, 20);
    Talk_Run411(9, 20);
    Talk_Run9(0, 0x102);
    Talk_Run10(1, 0x102);
    Talk_Run11(2, 0x102);
    Talk_Run412(60);
    Talk_Run413(2, 20);
    Talk_Run414(9, 1);
    Talk_Run415(20);
    Talk_Run416(9, 20);
    Talk_Place63(0, 0x101, 0);
    Talk_Place64(1, 0x101, 0);
    Talk_Place65(2, 0x101, 0);
    Talk_Run417(60);
    Talk_Run418(9, 4, 20);
    Talk_Run419(9, 20);
    Talk_Run420(0, 1);
    Talk_Run421(1, 1);
    Talk_Run422(2, 1);
    Talk_Run423(20);
    Talk_Run424(9, 3, 20);
    Talk_Run425(9, 40);
    Talk_Place66(2, 0x105, 0);
    Talk_Run426(60);
    Talk_Place67(9, 0x348, 0x1a8);
    Talk_Run427(9, 0, 20);
    Talk_Run428(9, 20);
    Talk_Run429(0, 3, 20);
    Talk_Place68(9, 0x5000, 0);
    Talk_Run430(20);
    Talk_Run431(9, 20);
    Talk_Run432(1, 1);
    Talk_Run433(20);
    Talk_Run434(1, 20);
    Talk_Run435(9, 3, 20);
    Talk_Run436(9, 30);
    Talk_Run437(9, 14, 20);
    Talk_Run438(9, 20);
    Talk_Place69(14, 0x358, 0x198);
    base5_200d4c8 = (s32)gOv11;
    Talk_Run439(9, base5_200d4c8);
    Talk_Run440(14, base5_200d4c8);
    Talk_Place70(1, 0x318, off1c8);
    Talk_Place71(2, 0xcccc, 0x6666);
    Talk_Place72(2, 0x308, 0x1b0);
    Talk_Run441(1);
    Talk_Place73(1, 0xd000, 0);
    Talk_Run442(2);
    Talk_Place74(2, 0xd000, 0);
    Talk_Run443(9);
    Talk_Run444(0, 9, 0);
    rec8 = Talk_Check6(14);
    {
        u8 *target = rec8 + 91;
        s32 shown = 1;

        *target = shown;
    }
    *(s32 *)((s32)rec8 + 56) = -0x80000000;
    *(s32 *)((s32)rec8 + 60) = -0x80000000;
    *(s32 *)((s32)rec8 + 64) = -0x80000000;
    Talk_Place75(9, 0x100, 0);
    Talk_Run445(14, 1);
    Talk_Run446(50);
    Talk_Run447(9, 0, 0);
    Talk_Place76(1, 0x328, 0x1b8);
    Talk_Place77(1, 0xb000, 0);
    Talk_Run448(20);
    Talk_Run449(9, 20);
    Talk_Run450(0, 1, 50);
    Talk_Run451(0, 9, 0);
    Talk_Run452(1, 9, 20);
    Talk_Run453(9, 3, 20);
    Talk_Place78(9, 0x2e8, 0x198);
    Talk_Place79(14, 0x2e8, 0x198);
    Talk_Run454(9);
    Talk_Run455(14);
    Talk_Run456(30);
    Talk_Place80(0, 0x318, 0x198);
    Talk_Run457(0, 1, 30);
    Talk_Place81(2, 0x105, 0);
    Talk_Run458(50);
    Talk_Run459(0, 2, 0);
    Talk_Run460(1, 2, 20);
    Talk_Run461(1, 1);
    Talk_Run462(20);
    Talk_Run463(1, 40);
    Talk_Run464(2, 3, 30);
    Talk_Run465(0, 1, 40);
    Talk_Run466(0, 4);
    Talk_Run467(1, 4, 30);
    Talk_Run468(0, 2, 0);
    Talk_Run469(1, 2, 20);
    Talk_Run470(1, 1);
    Talk_Run471(20);
    Talk_Run472(1, 40);
    Talk_Run473(2, 2);
    Talk_Run474(20);
    Talk_Run475(2, 20);
    Talk_Run476(0, 3);
    Talk_Run477(1, 3, 20);
    Talk_Run478(2, 4, 20);
    Talk_Run479(2, 20);
    Talk_Place82(0, 0x101, 0);
    Talk_Place83(1, 0x101, 0);
    Talk_Run480(60);
    Talk_Run481(2, 3, 20);
    Talk_Run482(2, 20);
    Talk_Place84(0, 0x100, 0);
    Talk_Place85(1, 0x100, 0);
    Talk_Run483(60);
    Talk_Run484(2, 3, 20);
    Talk_Run485(2, 20);
    Talk_Place86(0, 0x102, 0);
    Talk_Place87(1, 0x102, 0);
    Talk_Run486(60);
    Talk_Run487(2, 4, 20);
    Talk_Run488(2, 20);
    Talk_Run489(1, 3, 20);
    Talk_Run490(1, 30);
    Talk_Check7(2, 0);
    if (Talk_Check8(0, 0) != 0) {
        Talk_Run491(20);
        Talk_Run492(1, 1);
        Talk_Run493(20);
        Talk_Run494(1, 20);
        Talk_Run495(2, 4, 20);
        Talk_Run496(2, 20);
    }
    Talk_Run497(2, 3, 30);
    Talk_Place88(2, 0x320, 0x1c8);
    Talk_Run498(0, 2, 0);
    Talk_Run499(1, 2, 0);
    Talk_Place89(2, 0x358, 0x1c8);
    Talk_Run500(0, 2, 0);
    Talk_Run501(1, 2, 0);
    Talk_Place90(2, 0x358, 0x178);
    Talk_Run502(0, 2, 0);
    Talk_Run503(1, 2, 0);
    Talk_Run504(2, 0, 0);
    Talk_Run505(20);
    Talk_Run506(2, 2);
    rec8 = Talk_Place91(2, 17, 65);
    Talk_Run507(60);
    base5_1324 = (s32)gVal6;
    Talk_Run508(base5_1324, 1);
    Talk_Run509((s32)rec8);
    Talk_Run510(17, 2);
    Talk_Run511(20);
    Talk_Run512(2, 3, 20);
    Talk_Place92(2, 0x358, 0x1c8);
    Talk_Run513(0, 2, 0);
    Talk_Run514(1, 2, 0);
    Talk_Place93(2, 0x320, 0x1c8);
    Talk_Run515(0, 2, 0);
    Talk_Run516(1, 2, 0);
    Talk_Place94(2, 0x308, 0x1a8);
    Talk_Run517(0, 2, 0);
    Talk_Run518(1, 2, 30);
    Talk_Run519(2, 3, 20);
    Talk_Run520((base5_1324 + 1));
    Talk_Run521(2, 20);
    Talk_Run522(0, 3);
    {
        u16 *slot = (u16 *)((s32)p7 + 0x1d8);
        s32 saved = *(s16 *)slot;

        if (Talk_Run12()!= 0) {
            Talk_Do13(0x132a);
            Talk_Run523(2, 0);
            Talk_Run524();
        }
        Talk_Run525(2);
        *slot = saved;
    }
    Talk_Run526(1, 3, 50);
    Talk_Place95(2, 0x308, 0x198);
    Talk_Run527(0, 2, 0);
    Talk_Run528(1, 2, 0);
    Talk_Place96(2, 0x2e8, 0x198);
    Talk_Run529(40);
    Talk_Run530(0, 1, 20);
    Talk_Run531(1, 1);
    Talk_Run532(20);
    Talk_Run533(1, 20);
    Talk_Run534(0, 3, 20);
    Talk_Run535(1, 2);
    record = Talk_Check9(0);
    if ((s32)record != 0) {
        Talk_Run536(1, *(s16 *)((s32)record + 10), *(s16 *)((s32)record + 18));
    }
    Talk_Run537(1);
    Talk_Run538(1, 0, 0);
    Talk_Run539(30);
    Talk_Run540(8, 0, 0);
    Talk_Run541(9, 0, 0);
    Talk_Run542(13, 0, 0);
    Talk_Run543(14, 0, 0);
    Talk_Run544(10, 0, 0);
    Talk_Run545(11, 0, 0);
    Talk_Run546(12, 0, 0);
    Talk_Run547(2, 0, 0);
    *(s32 *)((*(u8 *volatile *)gWork + 0x1c0)) = 0x209;
    Talk_Run548();
}

void State_SetWord1c0To209AndRun(void)
{
    u8 *state;

    state = *(u8 **)0x03001ebc;
    *(s32 *)(state + 0x1c0) = 0x209;
    Talk_Run549();
    Talk_Run550();
    Talk_Do78(1);
}

void Actor_SetModeZeroAndValue(s32 a, s32 b)
{
    Talk_Apply32(a, 0);
    Talk_Do79(b);
}

void Scene_RunSplitTripleSteps(s32 a, s32 b, s32 c)
{
    Talk_Place116(a, b, 0);
    Talk_Do80(c);
}

void Actor_SetPairZeroAndValue(s32 a, s32 b, s32 c)
{
    Talk_Place117(a, b, 0);
    Talk_Do81(c);
}

void Effect_ApplyThreeValuesAndFinish(s32 a, s32 b, s32 c)
{
    Talk_Place118(a, b, c);
    Talk_Do82(a);
    Talk_Do83(c);
}

void Effect_ApplyPairWithValue141(s32 a, s32 b)
{
    Talk_Apply33(141, 1);
    Talk_Apply34(a, b);
    Talk_Run551();
    Talk_Do84(1);
    Talk_Do85(1);
}

void State_SetValue2ThenFinish(void)
{
    Talk_Do86(2);
    Talk_Run552();
    Talk_Run553();
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
    o = Talk_Run39(22);
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
        v = Talk_Run40(17, 0x608);
        Talk_Do87(a);
        v += 0x400;
        Talk_Place119(q[28], 0x80, v);
        Talk_Do88(17);
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
    if (Talk_Place120(x, y, 0) != 0 || Talk_Place121(x, y, 2) != 0
        || Talk_Place122(x, y, 24) != 0 || Talk_Place123(x, y, 25) != 0) {
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

    Talk_Run554();
    Talk_Run13(0xa80000, -1, 0x2900000, 1);
    Talk_Place97(0, 0xcccc, 0x6666);
    Talk_Place98(1, 0xcccc, 0x6666);
    Talk_Place99(2, 0xcccc, 0x6666);
    Talk_Place100(0, 248, 0x2b8);
    Talk_Place101(1, 0xf80000, 0x2b80000);
    Talk_Place102(2, 0xf80000, 0x2b80000);
    Talk_Place103(0, 200, 0x2b8);
    Talk_Place104(1, 248, 0x2c8);
    Talk_Place105(2, 232, 0x2b8);
    Talk_Run555(1);
    Talk_Place106(1, 0xc000, 0);
    Talk_Place107(2, 0x8000, 0);
    Talk_Run556(0);
    Talk_Run557(1, 12);
    Talk_Run558();
    Talk_Run14(0x300000, 0x2400000, 0x1200000, 0x2e00000);
    Talk_Place108(1, 0x10000, 0x8000);
    Talk_Place109(2, 0xc000, 0x6000);
    Talk_Place110(24, 0x10000, 0x13333);
    Talk_Place111(25, 0x18000, 0x18000);
    *(volatile u16 *)gOv12 = 0;
    Talk_Run15(0x200c8c9, 0xc94);
    Talk_Run559(0x1ff);
    Talk_Run560();
    Talk_Run561(9);
}
