#include "types.h"
#include "scene.h"

struct Slot02000338 {
    u8 head[6];
    u16 angle;
};

struct SceneMotion {
    u8 unk_00[8];
    s32 x;
    s32 y;
    s32 z;
    s32 ground;
    s32 scale_x;
    s32 scale_y;
    u8 unk_20[8];
    s32 velocity;
    u8 unk_2c[47];
    u8 state;
    u8 unk_5c[8];
    s16 timer;
    s16 delay;
    s32 active;
    void (*callback)(struct SceneMotion *);
};

struct Actor {
    s32 f00;
    s32 f04;
    s32 f08;
};

struct Actor02000c0c {
    u8 head[18];
    u8 gap18[6];
    s32 span;
    s32 reach;
    u8 gap32[48];
    u8 *attached;
    u8 gap84;
    u8 state;
};

extern u8 Value_0000004b;
extern u8 Value_0000004c;
extern u8 gOv[];
extern u8 gOv2[];
extern u8 gOv3[];
extern u8 gOv4[], gOv5[], gOv6[];
extern u8 gOv7[];
extern u8 gOv8[];
extern u8 gOv9[];
extern u8 *gWork;   /* pointer cell holding the overlay work */
extern u8 gOv10[];
extern u32 gIw;
extern s16 gCell[][1];

u8 *Scene_GetRecord_1();

u8 *Scene_GetRecord_2();

struct Slot02000338 *Actor_unk5_4(s32);

struct SceneMotion *Actor_unk6_4(s32, s32, s32, s32);

struct Actor *Actor_unk7_4(s32);

u8 *Actor_unk8_4();

u8 *Actor_unk9_4();

u8 *Actor_unk10_4();
u8 *Actor_unk11_4();

u8 *Actor_unk12_4();

u8 *Actor_unk13_4();

u8 *Actor_unk14_4();

/* Each Func_ symbol names the call word the overlay image holds before loader
 * relocation, not a runtime address. One word can serve two sites with
 * different targets, so the sites stay separate and must not be merged onto a
 * shared name. Names that describe what a target does are provisional. */

/* Calls spelled through these wrappers pass their constants straight into the
 * argument registers. Spelling them as direct calls instead precomputes a
 * costly constant into a temporary that is then shared with later uses in the
 * same block. A value-returning call also sets r0 last of its arguments. */

static __inline__ struct SceneMotion *CreateActor(
    struct SceneMotion *(*func)(s32, s32, s32, s32),
    s32 kind, s32 x, s32 y, s32 z)
{
    u32 Actor_unk15_4(void);

    return func(kind, x, y, z);
}

void Actor_unk16_4();       /* opens a scripted sequence */

void Actor_unk17_4();       /* queues a cue / script id */

s32 Actor_unk18_4();        /* result is compared against 1 */

void Actor_unk19_4();       /* closes the scripted sequence */

s32 Actor_unk20_4();        /* tests a flag id; nonzero when set */

void Actor_unk21_4();       /* opens a scripted sequence */

void Actor_unk22_4();       /* queues a cue / script id */

void Actor_unk23_4();       /* closes the scripted sequence */

s32 Actor_unk24_4();        /* tests a flag id; nonzero when set */

void Actor_unk25_4();       /* opens a scripted sequence */

void Actor_unk26_4();       /* queues a cue / script id */

void Actor_unk27_4();       /* closes the scripted sequence */

u8 *Actor_unk28_4();        /* scene/actor record accessor */

void Actor_unk29_4();       /* opens a scripted sequence */

s32 Actor_unk30_4();        /* tests a flag id; nonzero when set */

void Actor_unk31_4();       /* queues a cue / script id */

void Actor_unk32_4();       /* closes the scripted sequence */

s32 Actor_unk33_4();           /* tests a progress flag */

void Actor_unk34_4();          /* sets a scene flag */

s32 Actor_unk35_4();           /* tests a progress flag */

s32 Actor_unk36_4();           /* tests a progress flag */

void Actor_unk37_4();          /* sets a scene flag */

s32 Actor_unk38_4();           /* tests a progress flag, second site */

s32 Actor_unk39_4();           /* tests a progress flag, second site */

void Actor_unk40_4();          /* sets a scene flag */

void Actor_unk41_4(void);      /* scene continuation */

void Actor_unk42_4(void);      /* scene continuation */

s32 SceneData_SelectTableByWord224(void)
{
    extern s16 gCell2[];

    s16 v = gCell2[224];

    if (v == (s32)&Value_0000004b) {
        return (s32)gOv;
    }
    if (v == (s32)&Value_0000004c) {
        return (s32)gOv2;
    }
    return (s32)gOv3;
}

s32 Actor_unk43_4(void)
{
    return 0;
}

/*
 * The owner at 0x02000074 is eight bytes and includes its one pool word at
 * 0x02000078: the pc-relative load reads that word, so the word belongs to
 * this owner. The word is an address returned without being dereferenced.
 * Many getters share this body, but each returns a different address.
 */
u8 *SceneData_GetTable9390(void)
{
    return (u8 *)0x02009390;
}

u8 *SceneData_SelectFlaggedTable(void)
{
    extern s16 gCell2[];

    s32 id = gCell2[224];
    if (id == (s32)&Value_0000004b) {
        if (Actor_unk15(0x909)) {
            gOv4[142] = 0;
            gOv4[166] = 0;
        }
        return gOv4;
    }
    if (id == (s32)&Value_0000004c) {
        if (Actor_unk16(0x8fd))
            gOv5[46] = 1;
        if (Actor_unk17(0x8fe) || Actor_unk18(0x907))
            gOv5[94] = 1;
        Actor_unk5_2(gOv5);
        return gOv5;
    }
    return gOv6;
}

s32 SceneData_GetPrimaryTable(void)
{
    extern s16 gCell2[];

    s16 v = gCell2[224];

    if (v == (s32)&Value_0000004b) {
        return (s32)gOv7;
    }
    if (v == (s32)&Value_0000004c) {
        return (s32)gOv8;
    }
    return (s32)gOv9;
}

void Scene_RunActorEightPromptDialogue(void)
{
    extern s16 gCell2[];

    u8 *work;

    Actor_unk16_4();
    Actor_unk17_4(0x18bd);
    /* r1 is set before r0; the argument order is unchanged. */
    Actor_unk19(8, 0);

    if (Actor_unk18_4(0, 0) == 1) {
        Actor_unk44_4(8, 0);
    } else {
        work = gWork;
        *(u16 *)(work + 472) = (u16)(*(u16 *)(work + 472) + 1);
        Actor_unk45_4(8, 0);
    }

    Actor_unk19_4();
}

void Dialogue_ShowLine1918(void)
{
    Actor_unk46_4();
    Actor_unk6_2(0x1918);
    Actor_Apply(9, 0);
    Actor_unk47_4();
}

/*
 * One of two branches of the opening setup: a short branch that only moves
 * actor 14, or a longer branch that positions actor 18 and a second record
 * from their x/y/z fields at +8/+12/+16, clearing a byte at +85 of a
 * separately looked-up record on the way.
 */
void Scene_RunOpeningAuxiliarySequence(void)
{
    void Motion_SetAngleToward_1();
    void Motion_CallWaitAnim_1();
    void Motion_SetSpeedLim_1();

    u8 *rec18;
    u8 *ready_flag;
    u8 *record;

    Battle_Reset_1();
    if (GameFlag_IsSet_1(0x909) != 0) {
        SceneWork_SetStepValue_1(0x191f);
        BattleEventRuntime_ProcessAction_1(14, 0); /* object 14, action 0 */
    } else {
        Object_SetModeById_1(14, 4); /* object 14, action 4 */
        SceneWork_SetStepValue_2(0x18c7);
        Battle_RunThenWaitIfModeZero_1(14, 0, 10);
        ready_flag = GameFlag_IsSet_2(0x8ff);
        if (ready_flag == 0) {
            rec18 = Scene_GetRecord_1(18);
            /* Clear the byte at +85 of the lookup result. */
            *(u8 *)(RuntimeBlock_GetOffset1e0Pointer_1() + 85) = ready_flag;
            Motion_SetSpeedLim_1(0x10000, 0x2000);
            Motion_CamBounds_1(*(s32 *)(rec18 + 8), *(s32 *)(rec18 + 12), *(s32 *)(rec18 + 16), 1); /* use_setter 1 */
            Motion_SetAngleToward_1(0, 0x4000, 0);
            Motion_ArmCb_1(14, 0x3000, 0);
            Object_CommitPositionThenWaitIfModeZero_1();
            Battle_WaitMode0_1(120); /* should_wait 120 */
            record = Scene_GetRecord_2(0);
            Motion_CamBounds_2(*(s32 *)(record + 8), *(s32 *)(record + 12), *(s32 *)(record + 16), 1); /* use_setter 1 */
            Object_CommitPositionThenWaitIfModeZero_2();
        }
        Motion_CallWaitAnim_1(14, 4);
    }
    Battle_SchedShoulder_1();
}

void Dialogue_RunActor17Message1924(void)
{
    void Actor_unk48_4(void);
    void Motion_CallWaitAnim_1(void);
    void Motion_SetSpeedLim_1(void);

    Actor_unk49_4();
    Actor_unk7_2(0x1924);
    Actor_Apply2(17, 0);
    Actor_unk48_4();
}

void Dialogue_RunActor9Message1932(void)
{
    void Motion_SetAngleToward_1(void);
    void Motion_CallWaitAnim_1(void);
    void Motion_SetSpeedLim_1(void);

    Actor_unk50_4();
    Actor_unk8_2(0x1932);
    Actor_Apply3(9, 0);
    Actor_unk51_4();
}

void Dialogue_RunActor10Message18d9(void)
{
    void Motion_SetAngleToward_1(void);
    void Actor_unk52_4(void);
    void Actor_unk53_4(void);

    Actor_unk52_4();
    Actor_unk9_2(0x18D9);
    Actor_Apply4(10, 0);
    Actor_unk53_4();
}

void Dialogue_RunActor14Message18e1(void)
{
    void Motion_SetAngleToward_1(void);
    void Motion_CallWaitAnim_1(void);
    void Motion_SetSpeedLim_1(void);

    Actor_unk54_4();
    Actor_unk10_2(0x18E1);
    Actor_Apply5(14, 0);
    Actor_unk55_4();
}

void Dialogue_RunActor21Message194a(void)
{
    void Motion_SetAngleToward_1(void);
    void Motion_CallWaitAnim_1(void);
    void Motion_SetSpeedLim_1(void);

    Actor_unk56_4();
    Actor_unk11_2(0x194A);
    Actor_Apply6(21, 0);
    Actor_unk57_4();
}

s32 Actor_IsSlotZeroAngleInRange(void)
{
    struct Slot02000338 *slot = Actor_unk5_4(0);

    if ((u32)((slot->angle + 0x5FFF) << 16) <= 0x3FFE0000) {
        return 1;
    }
    return 0;
}

void Scene_RunActorFifteenFlagBranch(void)
{
    void Actor_unk58_4();
    void Actor_unk59_4();

    if (Actor_unk20_4(0x242) == 0) {
        Actor_unk21_4();
        Actor_unk22_4(0x18e7);
        /* r1 is set before r0 here; the argument order is unchanged. */
        Actor_unk20(15, 0);
        Actor_unk23_4();
        return;
    }

    if (Actor_unk21() != 0) {
        Actor_unk59_4(19, 15);
        return;
    }

    Actor_unk60_4();
    Actor_unk61_4(0x18ea);
    if (Actor_unk22(0x909) != 0) {
        Actor_unk58_4(0x1941);
    }
    Actor_unk62_4(15, 0);
    Actor_unk63_4();
}

void Scene_RunActorTwentyFlagBranch(void)
{
    void Actor_unk64_4(void);
    void Actor_unk65_4(void);
    void Actor_unk66_4(s32);

    if (Actor_unk23(0x241) == 0) {
        Actor_unk67_4();
        Actor_unk12_2(0x18ED);
        Actor_Apply7(20, 0);
        Actor_unk64_4();
        return;
    }

    if (Actor_unk24() != 0) {
        Actor_Apply8(20, 17);
        return;
    }

    Actor_unk68_4();
    Actor_unk66_4(0x18EE);
    if (Actor_unk25(0x909) != 0) {
        Actor_unk13_2(0x1943);
    }
    Actor_Apply9(17, 0);
    Actor_unk65_4();
}

void Scene_RunActorTwentyOneFlagBranch(void)
{
    if (Actor_unk24_4(0x240) == 0) {
        Actor_unk25_4();
        Actor_unk26_4(0x18f1);
        Actor_unk69_4(21, 0);
        Actor_unk27_4();
        return;
    }

    if (Actor_unk26() != 0) {
        Actor_unk70_4(21, 16);
        return;
    }

    Actor_unk71_4();
    Actor_unk72_4(0x18f2);
    if (Actor_unk27(0x909) != 0) {
        Actor_unk73_4(0x1945);
    }
    Actor_unk74_4(16, 0);
    Actor_unk75_4();
}

void Scene_RunFacingGatedDialogue18(void)
{
    u8 *rec;

    rec = Actor_unk28_4(0);

    if ((u32)((*(u16 *)(rec + 6) + 0x5fff) << 16) <= 0x3ffe0000) {
        Actor_unk76_4(6, 18);
        return;
    }

    Actor_unk29_4();

    if (Actor_unk30_4(0x909) != 0) {
        Actor_unk31_4(0x1947);
        Actor_unk77_4(18, 0);
    } else {
        Actor_unk78_4(0x18f5);
        Actor_unk79_4(18, 0);
    }

    Actor_unk32_4();
}

void Scene_RunEarlySequence(void)
{
    u32 i;
    u8 *work;
    u8 *record;
    s32 idx;
    u8 *tbl;
    s32 off;
    s32 o4;
    s32 a;
    s32 b;
    s32 c;

    work = *(u8 *volatile *)&gWork;
    Actor_unk80_4();
    for (i = 8; i <= 65; i++) {
        record = (u8 *)Actor_Check(i);
        if (record != 0) {
            record[85] = 0;
        }
    }
    tbl = gOv10;
    idx = ((s32)((s32)(*(u16 *)(work + 0x16c) - 2) << 16) >> 16);
    off = idx << 3;
    o4 = off + 4;
    a = *(s16 *)(tbl + o4);
    b = *(s16 *)(tbl + o4 + 2);
    if (idx == 1) {
        Actor_unk81_4(188);
        Actor_SetRect(42, 33, a, b, 2, 2);
        c = a + 2;
        Actor_unk2_5(42, 35, c, b, 2, 2);
        Actor_unk82_4(4);
        Actor_unk3_5(40, 33, a, b, 2, 2);
        Actor_unk4_5(40, 35, c, b, 2, 2);
        Actor_unk83_4(4);
    } else {
        Actor_unk84_4(158);
        if (idx == 3) {
            Actor_unk5_5(33, 42, 8, 17, 1, 2);
        }
        Actor_unk85_4(*(s32 *)(tbl + off), a, b);
    }
    Actor_Place(0, 0x8000, 0x4000);
    *(s32 *)((*(u8 *volatile *)&gWork + 0x1c0)) = 0x100;
    *(u8 *)(Actor_unk28(0) + 85) = 0;
    Actor_unk86_4(0, 2);
    if (idx == 6) {
        Actor_unk87_4(0, 2, 0);
    } else {
        if (idx != 1) {
            Actor_unk2_3(0, 2, -4);
        } else {
            Actor_unk88_4(0, 2);
            Actor_unk3_3(0, 0, -4);
        }
    }
    Actor_unk89_4(10);
    Actor_unk90_4(*(s16 *)(work + 0x16c));
    Actor_unk91_4();
    Actor_unk92_4();
    Actor_unk93_4();
}

void Scene_RunScene3a3SequenceB(void)
{
    u32 i;
    s32 record;
    u8 *p5;

    p5 = *(volatile s32 *)&gWork;
    Actor_unk94_4();
    *(u8 *)(Actor_unk29(0) + 85) = 0;
    Actor_unk95_4(123);
    Actor_unk4_3(0, 2, -16);
    Actor_unk96_4(*(s16 *)(((s32)p5 + 0x16c)));
    Actor_unk97_4();
    Actor_unk98_4();
    Actor_unk99_4();
}

void SceneMotion_UpdateTimedActor(struct SceneMotion *work)
{
    u32 Actor_unk15_4(void);

    if (work->delay != 0) {
        if (--work->delay == 1)
            Actor_unk5_3(-1, -1, 0xe666);
    }
    if (work->velocity == 0) {
        Actor_Apply10(work, 1);
        work->y += -0x18000;
        if (work->y < work->ground) {
            if (work->active != 0) {
                Actor_unk14_2(229);
                work->active = 0;
                work->delay = 4;
                Actor_unk6_3(0, 0x10000, 0x10000);
            }
            work->y = work->ground;
        }
        work->state = 1;
    } else {
        work->state = 0;
    }
    if (work->timer == 0) {
        Actor_unk15_2(152);
        work->active = 1;
        Actor_Apply11(work, 2);
        work->velocity = 0x30000;
    }
    if (++work->timer == 60)
        work->timer = 0;
}

void Scene_RunScene3a3SequenceC(void)
{
    u32 i;
    s32 record;

    record = Actor_unk2(18);
    {
        s32 shown = 0;

        *(volatile u16 *)(record + 100) = shown;
    }
    {
        s32 shown = 0;

        *(volatile u16 *)((record + 100) + 2) = shown;
    }
    *(volatile s32 *)(record + 72) = 0x6666;
    *(volatile s32 *)(record + 108) = 0x20086f1;
    Actor_unk7_3(18, 0x13333, 0x9999);
    Actor_unk8_3(18, 28, 0x1cc);
    Actor_unk9_3(18, 24, 0x1c0);
    Actor_unk100_4(229);
    Actor_unk101_4(18);
    Actor_unk10_3(0, 0x10000, 0x10000);
    Actor_unk102_4(4);
    Actor_unk11_3(-1, -1, 0xe666);
    Actor_unk103_4(40);
    Actor_unk104_4(18, 1);
}

void State_SetFlag906ByActorNineteenX(void)
{
    extern s16 gCell2[];

    struct Actor *p = Actor_unk7_4(19);

    if ((p->f08 >> 20) == 22) {
        Actor_unk16_2(0x906);
    } else {
        Actor_unk17_2(0x906);
    }
}

/*
 * Mirror three progress flags into three scene flags, then let the scene id
 * at gCell2[224] select one of two continuations. Every call here
 * leaves through its own veneer, so the sites stay separate.
 */
s32 State_SyncProgressFlagsAndDispatch(void)
{
    extern s16 gCell2[];

    s16 scene;

    if (Actor_unk33_4(0x8fd) != 0) {
        Actor_unk34_4(0x240);
    }

    if (Actor_unk35_4(0x8fe) != 0 || Actor_unk36_4(0x907) != 0) {
        Actor_unk37_4(0x241);
    }

    if (Actor_unk38_4(0x8fe) != 0 && Actor_unk39_4(0x907) != 0) {
        Actor_unk40_4(0x242);
    }

    scene = gCell2[224];
    if (scene == (s32)&Value_0000004b) {
        Actor_unk41_4();
    } else if (scene == (s32)&Value_0000004c) {
        Actor_unk42_4();
    }

    return 0;
}

void Scene_RunMiddleSequence(void)
{
    extern u8 gCell2[];

    s32 scene;
    s32 rec5;
    s32 rec6;
    s32 rec0;
    s32 kind;
    s32 bit;

    scene = Actor_unk3(0);
    rec5 = Actor_unk4(0x242);
    if (rec5 != 0) {
        Actor_unk105_4(64, 32, 0, 32, 32, 32);
        Actor_unk6_5(64, 32, 32, 32, 0, 0);
        kind = 20;
    } else {
        rec6 = Actor_unk5(0x241);
        if (rec6 != 0) {
            Actor_unk106_4(64, 0, 0, 32, 32, 32);
            Actor_unk7_5(64, 0, 32, 32, rec5, rec5);
            Actor_unk107_4(17);
            kind = 20;
        } else {
            rec0 = Actor_unk6(0x240);
            if (rec0 == 0) {
                goto L_020009b8;
            }
            Actor_unk108_4(0, 64, 0, 32, 32, 32);
            Actor_unk8_5(0, 64, 32, 32, rec6, rec6);
            Actor_unk109_4(16);
            kind = 17;
        }
    }
    Actor_unk110_4(kind);
    Actor_unk111_4(21);
    goto L_020009da;
    L_020009b8:;
    Actor_unk9_5(0, 32, 32, 32, rec0, rec0);
    Actor_unk112_4(15);
    Actor_unk113_4(16);
    Actor_unk114_4(17);
    L_020009da:;
    if (Actor_unk7(0x8ff) != 0) {
        Actor_unk115_4(18);
    } else {
        Actor_unk116_4(170);
        Actor_unk117_4(18, 2);
        Actor_unk118_4(18, 3);
        Actor_Run(0x2008d09, 0xc80);
    }
    if (gCell[225][0] == 3) {
        Actor_Do(0x12f);
    }
    Actor_unk10_5(0, 33, 4, 3, 20, 41);
    if (Actor_unk8(0x906) != 0) {
        Actor_unk12_3(19, 0x1680000, 0xa80000);
    }
    Actor_unk119_4((s32)Actor_unk8_4(19), 0);
    Actor_unk120_4(22, 15);
    Actor_unk2_4(23, 15);
    bit = 8;
    Actor_unk30(24, 15);
    *(u8 *)(Actor_unk31(22) + 89) |= bit;
    *(u8 *)(Actor_unk32(23) + 89) |= bit;
    {
        u8 *record = Actor_unk9_4(24);
        u8 value = *(volatile u8 *)&record[89];

        record[89] = (u8)(value | bit);
    }
    bit = 2;
    *(u8 *)(Actor_unk33(22) + 35) |= bit;
    *(u8 *)(Actor_unk34(23) + 35) |= bit;
    {
        u8 *record = Actor_unk11_4(24);
        u8 value = *(volatile u8 *)&record[35];

        record[35] = (u8)(value | bit);
    }
    Actor_unk35(22, 1);
    Actor_unk121_4(23, 1);
    Actor_unk36(24, 1);
    Actor_unk122_4(1);
    Actor_unk123_4();
    Actor_unk124_4(*(s32 *)(scene + 8), *(s32 *)(scene + 12), *(s32 *)(scene + 16), 0);
    Actor_unk125_4();
    Actor_unk126_4();
    Actor_unk127_4(1);
}

void Scene_RunScene3a3SequenceD(void)
{
    u32 i;
    u8 *record;

    if (Actor_unk9(0x240) == 0) {
        Actor_unk13_3(8, 0x3280000, 0x2d70000);
        record = Actor_unk12_4(8);
        {
            s32 shown = 0x3000;

            *(volatile u16 *)((s32)record + 6) = shown;
        }
        Actor_unk14_3(9, 0x31a0000, 0x3390000);
    }
    if (Actor_unk10(0x241) == 0) {
        Actor_unk15_3(10, 0x2300000, 0x2c60000);
        record = Actor_unk13_4(10);
        {
            s32 shown = 0x1000;

            *(volatile u16 *)((s32)record + 6) = shown;
        }
        Actor_unk16_3(11, 0x2400000, 0x2c60000);
    }
    if (Actor_unk11(0x242) == 0) {
        Actor_unk17_3(15, 0x1270000, 0x2e80000);
        record = Actor_unk14_4(15);
        {
            s32 shown = 0xb000;

            *(volatile u16 *)((s32)record + 6) = shown;
        }
    } else {
        record = Actor_unk12(15);
        {
            u8 value = *(volatile u8 *)&record[89];

            record[89] = (u8)(value | 4);
        }
    }
    record = Actor_unk13(17);
    if ((s32)record != 0) {
        {
            u8 value = *(volatile u8 *)&record[89];

            record[89] = (u8)(value | 4);
        }
    }
    record = Actor_unk14(16);
    if ((s32)record != 0) {
        {
            u8 value = *(volatile u8 *)&record[89];

            record[89] = (u8)(value | 4);
        }
    }
}

void Actor_ResetStateAndSpan(struct Actor02000c0c *actor)
{
    void Actor_unk128_4(struct Actor02000c0c *, s32);

    u8 *state = &actor->state;
    s32 clear = 0;
    u8 *attached;

    *state = (u8)clear;
    attached = actor->attached;
    clear -= 13;
    attached[9] = (clear & attached[9]) | 4;
    Actor_Apply12(actor, 3);
    Actor_unk128_4(actor, 0);
    actor->span = 0x4CCC;
    actor->reach = 0x4CCC;
}

void Effect_UpdateDriftingParticle(struct SceneMotion *work)
{
    u32 Actor_unk15_4(void);

    work->x += (work->timer << 12) +
        ((s16)((s32)((Actor_unk129_4() * 2) >> 16) - 1) << 15);
    if (work->timer <= 3) {
        work->z += -((Actor_unk15_4() * 0x8000) >> 16) - 0x10000;
        work->scale_x += 0x2666;
        work->scale_y += -0xa3d;
    } else {
        work->z += 0x20000;
        work->scale_x += 0x7ae;
        work->scale_y += 0x7ae;
    }
    if ((Actor_unk130_4() * work->timer) >> 16 == 0)
        Actor_Apply13(work, 7);
    if (work->timer != 0)
        work->timer--;
    else
        work->timer = ((Actor_unk131_3() * 5) >> 16) * 2 + 2;
    if (--work->active == 0) {
        work->callback = 0;
        Actor_unk18_2(work);
    }
}

void Effect_SpawnDriftingParticle(void)
{
    u32 Actor_unk15_4(void);

    struct SceneMotion *work;
    if ((gIw & 3) == 0) {
        work = CreateActor(Actor_unk6_4, 222, 0x400000, 0, 0x1900000);
        if (work != 0) {
            work->timer = 20;
            work->delay = 0;
            work->active = 20;
            Actor_unk19_2(work);
            work->callback = (void (*)(struct SceneMotion *))0x02008c45;
            Actor_Apply14(work, 1);
        }
    }
}

void Scene_RunScene3a3(void)
{
    u32 i;
    s32 record;

    Actor_unk132_3();
    Actor_unk3_4(0x6666, 0xccc);
    Actor_unk4_4(0x3f0000, -1, 0x1c20000, 1);
    Actor_unk133_3();
    Actor_unk134_3(30);
    Actor_unk135_3(18, 1);
    Actor_unk2_2(-1);
    Actor_unk3_2(0x2008d09);
    Actor_unk136_3(20);
    Actor_unk137_3(0, 18, 0);
    Actor_unk18_3(0, 0x4000, 0);
    Actor_unk138_3(18, 0, 20);
    Actor_unk19_3(18, 0xd000, 40);
    Actor_unk139_3(147);
    Actor_unk140_3(18, 2);
    Actor_unk141_3(20);
    Actor_unk20_3(18, 0xb000, 40);
    Actor_unk142_3();
    Actor_unk143_3(0, 1);
    Actor_unk144_3();
    Actor_unk145_3(14, 4);
    Actor_unk4_2(0x8ff);
    Actor_unk146_3();
}
