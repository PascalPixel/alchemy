#include "types.h"
#include "scene.h"

/* overlays/scene/actor/proximity/actor_proximity.c */
struct SceneActor {
    u8 unk_00[6];
    u16 facing;
    s32 x, y, z;
    u8 unk_14[71];
    u8 active;
};

s32 Actor_GetPositionDistance(s32 *, s32 *);
u32 Actor_GetAngle(s32, s32);
void Actor_SetMode(struct SceneActor *, s32);
struct SceneActor *Actor_Find(s32);

s32 Actor_UpdatePlayerProximity(struct SceneActor *actor,
                                    struct SceneActor *target,
                                    s32 range, s32 force)
{
    s32 result = 0;
    s32 *targetPos = &target->x;
    s32 *actorPos = &actor->x;

    if (Actor_GetPositionDistance(targetPos, actorPos) < range || force != 0) {
        u32 angle = (u16)Actor_GetAngle(target->z - actor->z,
                                            *targetPos - *actorPos);
        u32 farLeft = (angle - 0x2000) & 0xf000;
        u32 farRight = (angle + 0x2000) & 0xf000;
        u32 left = (angle - 0x1000) & 0xf000;
        u32 right = (angle + 0x1000) & 0xf000;
        u32 forward = angle & 0xf000;
        u32 facing = actor->facing & 0xf000;

        if (forward == facing || right == facing || left == facing || force != 0) {
            actor->active = 1;
            Actor_SetMode(actor, 1);
            result = 1;
        }
        if (target == Actor_Find(0) && (farRight == facing || farLeft == facing)) {
            actor->active = 1;
            Actor_SetMode(actor, 1);
            result = 1;
        }
    } else {
        actor->active = 0;
        Actor_SetMode(actor, 2);
    }
    return result;
}

/* overlays/scene/actor/proximity/actor_setup_and_events.c */
/*
 * Overlay resource_3b5: shared scene work plus the actor setup, dialogue and
 * event sequences that drive this scene, in address order.
 */

/*
 * Each macro below stands for one per-site call word held in the overlay
 * image and names the engine function that site reaches through the veneers,
 * keeping the site's own calling form. One word can serve two sites with
 * different targets, so the bindings are per call and not per name. Names
 * that have no binding in this tree are provisional.
 */

/*
 * Object-id slot at 0x1f4 of the shared scene work record, set once below to
 * the id of the object created earlier in the same function.
 */

struct SceneSetup {
    u16 *commands;
    u16 first;
    u16 second;
};

struct Rec_3b5 {
    u8 pad00[9];
    u8 lo9 : 2;
    u8 mode9 : 2;               /* +9,  bits 2..3 */
    u8 hi9 : 4;
    u8 pad0a[11];
    u8 lo15 : 2;
    u8 mode15 : 2;              /* +21, bits 2..3 */
    u8 hi15 : 4;
};

struct Work_3b5 {
    u8 pad00[35];
    u8 f35;                     /* +35 */
    u8 pad24[44];
    struct Rec_3b5 *volatile f80;   /* +80, re-read for the second store */
};

struct Actor {
    u8 pad00[0x23];
    u8 field23;
    u8 pad24[0x31];
    u8 field55;
};

extern u8 gVal[];
extern u8 gVal2[];
extern u8 gVal3[];
extern u8 gVal4[];
extern u8 gVal5[];
extern u8 gVal6[];
extern u8 gVal7[];
extern u8 gVal8[];
extern u8 gVal9[];
extern u8 gVal10[];
extern u8 gVal11[];
extern u8 gVal12[];
extern u8 gOv[];
extern u8 gOv2[];
extern u8 gOv3[];
extern u8 gVal13[];
extern struct SceneSetup gOv4[];
extern u8 gVal14[];

typedef s32(*IwramSqrt02000040)(s32);
u8 *Actor_unk2_4();

u8 *Actor_unk3_4();

u8 *Actor_unk4_4();

s32 Dialogue_ShowMessage();
s32 SceneFlag_Test();

struct Work_3b5 *Actor_unk5_4();

struct Actor *Actor_unk6_4(s32);

/*
 * These declarations are deliberately old-style: overlay import arities are
 * not fixed per name, and each call site names the veneer it reaches rather
 * than the import behind it, so one import reached from several sites carries
 * several names.
 *
 * Actor_unk7_4 is void because its result is discarded.
 *
 * Actor_unk15 returns s32 because callers test the result whole and never
 * truncate it; a u8 return would add a widening shift that is not wanted.
 */

static __inline__ void PlaceActor(s32 actor, s32 x, s32 y)
{
    Actor_unk26_3(actor, x, y);
}

static __inline__ void PlaceActor2(s32 actor, s32 x, s32 y)
{
    Actor_unk27_3(actor, x, y);
}

static __inline__ void bump_step(s32 amount)
{
    extern u8 gWork[];

    u8 *work = *(u8 **)gWork;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

static __inline__ void bump_step2(s32 amount)
{
    extern u8 gWork[];

    u8 *work = *(u8 **)gWork;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

static __inline__ void bump_step3(s32 amount)
{
    extern u8 gWork[];

    u8 *work = *(u8 **)gWork;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

static __inline__ void bump_step4(s32 amount)
{
    extern u8 gWork[];

    u8 *work = *(u8 **)gWork;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void State_SetValues31_2_4(void)
{
    Actor_unk28_3(0x1F, 2, 4);
}

s32 Actor_GetPositionDistance(s32 *a, s32 *b)
{
    s32 dx = (*a++ - *b++) >> 16;
    s32 dy = (*a++ - *b++) >> 16;
    s32 dz = (*a - *b) >> 16;
    s32 dxsq = dx *dx;
    s32 dysq = dy *dy;
    s32 dzsq = dz *dz;

    return ((IwramSqrt02000040) 0x030001D8)(dxsq + dysq + dzsq);
}

/*
 * Per-frame actor callback, whose argument is the owning actor record. The
 * initialiser stores this address plus the Thumb bit into field +0x6c of
 * actors 16 and 17. It returns a constant zero.
 *
 * The owner's eight-byte literal pool follows its return and belongs to it.
 * 0x03001e8c is a table of pointers whose entry 12 is 0x03001ebc, the overlay
 * work pointer the rest of this overlay loads directly.
 */
s32 Actor_UpdatePartnerProximity(u8 *self)
{
    u8 **globals = (u8 **)0x03001e8c;
    u8 *scene = globals[0];
    u8 *work = globals[12];        /* == *(u8 **)0x03001ebc */
    u16 *flags = (u16 *)(self + 100);
    s32 force = 0;
    s32 range = 18;
    u8 *partner;
    u8 *player;

    /*
     * Bit 0 of the actor's own flag halfword selects which partner to test.
     * The branch must stay two calls: as a conditional expression the
     * selector folds into arithmetic on the bit instead.
     */
    if ((*flags & 1) != 0) {
        partner = Actor_unk2_4(17);
    } else {
        partner = Actor_unk2_4(16);
    }
    if (Actor_unk15(self, partner, 32, 0) != 0) {
        return 0;
    }

    player = Actor_unk3_4(0);

    /*
     * Widen the test when the scene counter at work + 376 is already
     * running, or when the scene byte at scene + 0x0ea4 is set.
     */
    if (*(s16 *)(work + 376) != 0 || scene[0x0ea4] != 0) {
        range = 26;
        if ((*flags & 2) != 0) {
            force = 1;
        }
    }

    Actor_unk7_4(self, player, range, force);
    return 0;
}

/*
 * The eight-byte owner includes its one pool word, which holds the address
 * returned here. The word is loaded and returned, never dereferenced.
 */
u8 *SceneData_GetTable9060(void)
{
    return (u8 *)0x02009060;
}

/* Table slot with no data: reads nothing and returns zero. */
s32 SceneData_ReturnZero(void)
{
    return 0;
}

/* The eight-byte owner includes the pool word holding this address. */
u8 *SceneData_GetTable91f8(void)
{
    return (u8 *)0x020091f8;
}

/* The eight-byte owner includes the pool word holding this address. */
u8 *SceneData_GetTable9238(void)
{
    return (u8 *)0x02009238;
}

void Scene_RunScene3b5(void)
{
    extern u8 gWork[];
    void Actor_unk8_4();
    void Actor_unk9_4();

    u32 i;
    u8 *record;

    record = Actor_Check(8);
    if ((s32)record != 0) {
        record[89] = 0;
    }
    record = Actor_unk4_4(8);
    Actor_unk10_4((s32)record, 0);
    Actor_Run(0, 0x2200000, 0x1200000, 253);
    Actor_Do(0x200);
}

void ConfigureAndPlaceActorOneHundredTwo(void)
{
    s32 a = 3, b = 26;
    Actor_unk11_4(3, 32, 1, 1, a, b);
    PlaceActor(102, 0x00380000, 0x01a80000);
}

void Actor_unk12_4(void)
{
    s32 a = 3, b = 26;
    Actor_unk13_4(2, 25, 1, 1, a, b);
    PlaceActor2(102, -1, -1);
}

void Dialogue_RunMessage0e36(void)
{
    Actor_unk9_2(0x0E36);
    Actor_Apply(-1, 0);
}

void Dialogue_RunMessage0e37(void)
{
    Actor_unk10_2(0x0E37);
    Actor_Apply2(-1, 0);
}

void Scene_RunSupplementalSequenceTwo(void)
{
    extern u8 gWork[];

    u8 *__restrict p5;
    s32 rec;
    s32 p8;
    s32 msg;

    p5 = *(u8 **)gWork;
    rec = Actor_unk16(16);
    p8 = *(s16 *)(rec + 6);
    Actor_unk14_4();
    {
        volatile u16 *flags = (volatile u16 *)(rec + 100);
        u16 value = *flags;

        *flags = (u16)(value | 2);
    }
    if (*(s16 *)(p5 + 0x17e) == 0) {
        if (Actor_unk17(0x950) != 0) {
            msg = (s32)gVal9;
        } else if (Actor_unk18(0x962) != 0) {
            msg = (s32)gVal5;
        } else {
            msg = (s32)gVal;
        }
    } else {
        if (Actor_unk19(0x950) != 0) {
            msg = (s32)gVal11;
        } else if (Actor_unk20(0x962) != 0) {
            msg = (s32)gVal7;
        } else {
            msg = (s32)gVal3;
        }
    }
    Dialogue_ShowMessage(msg);
    Actor_unk15_4(16, 0);
    Actor_unk16_4(16, 0, 2);
    Actor_unk17_4(16, 0, 10);
    *(volatile u16 *)(rec + 6) = p8;
    Actor_unk18_4(1);
    *(volatile u16 *)(rec + 100) &= 1;
    Actor_unk19_4();
}

void Actor_unk20_4(void)
{
    extern u8 gWork[];

    u8 *__restrict p5;
    s32 rec;
    s32 p8;
    s32 msg;

    p5 = *(u8 **)gWork;
    rec = Actor_unk21(17);
    p8 = *(s16 *)(rec + 6);
    Actor_unk21_4();
    {
        volatile u16 *flags = (volatile u16 *)(rec + 100);
        u16 value = *flags;

        *flags = (u16)(value | 2);
    }
    if (*(s16 *)(p5 + 0x17e) == 0) {
        if (Actor_unk22(0x950) != 0) {
            msg = (s32)gVal10;
        } else if (SceneFlag_Test(0x962) != 0) {
            msg = (s32)gVal6;
        } else {
            msg = (s32)gVal2;
        }
    } else {
        if (Actor_unk23(0x950) != 0) {
            msg = (s32)gVal12;
        } else if (Actor_unk24(0x962) != 0) {
            msg = (s32)gVal8;
        } else {
            msg = (s32)gVal4;
        }
    }
    Actor_unk22_4(msg);
    Actor_unk23_4(17, 0);
    Actor_unk24_4(17, 0, 2);
    Actor_unk25_4(17, 0, 10);
    *(volatile u16 *)(rec + 6) = p8;
    Actor_unk26_4(1);
    *(volatile u16 *)(rec + 100) &= 1;
    Actor_unk27_4();
}

u8 *SceneData_SelectTable94a8ByFlags(void)
{
    if (Actor_unk25(0x950) != 0) {
        return gOv;
    }
    if (Actor_unk26(0x962) != 0) {
        return gOv2;
    }
    return gOv3;
}

void Dialogue_RunActor15Message1f92(void)
{
    Actor_unk28_4();
    Actor_unk11_2(0x1F92);
    Actor_Apply3(15, 0);
    Actor_unk29_4();
}

void Dialogue_RunActor24Message1f9d(void)
{
    Actor_unk30_4();
    Actor_unk12_2(0x1F9D);
    Actor_Apply4(24, 0);
    Actor_unk31_4();
}

void Scene_RunScene3b5(void)
{
    extern u8 gWork[];

    u32 i;
    s32 record;

    Actor_unk32_4();
    Actor_unk33_4((s32)gVal13);
    Actor_Place(25, 0xc000, 0);
    Actor_unk34_4(25, 0);
    Actor_unk2_3(25, 0x8000, 0);
    Actor_unk35_4(25, 0);
    Actor_unk36_4();
}

void Scene_RunScene3b5(void)
{
    extern u8 gWork[];

    u32 i;
    s32 record;

    Actor_unk37_4();
    Actor_unk3_3(26, 0x4000, 0);
    Actor_unk38_4(26, 2);
    Actor_unk2_2(0x1fa2);
    Actor_unk39_4(26, 0);
    Actor_unk40_4();
}

void Dialogue_RunActor27Message1fa3(void)
{
    Actor_unk41_4();
    Actor_unk13_2(0x1FA3);
    Actor_Apply5(0x1B, 0);
    Actor_unk42_4();
}

void Dialogue_RunActor24Message235f(void)
{
    Actor_unk43_4();
    Actor_unk14_2(0x235F);
    Actor_Apply6(24, 0);
    Actor_unk44_4();
}

void Scene_RunScene3b5(void)
{
    extern u8 gWork[];
    void Actor_unk8_4();
    void Actor_unk9_4();

    u32 i;
    s32 record;

    Actor_unk45_4();
    if (Actor_unk2(0x8bf) == 0) {
        Actor_unk3_2(0x8bf);
        Actor_unk4_2(0x2368);
        Actor_unk46_4(19, 0);
        Actor_unk47_4(233, 3);
        Actor_unk48_4(19, 0);
        Actor_unk8_4(0, 1);
        Actor_unk49_4(233, 0);
    } else {
        Actor_unk5_2(0x236a);
        Actor_unk50_4(19, 0);
    }
    Actor_unk51_4();
}

void SceneScript_SetupActors(void)
{
    extern u8 *gWork;
    u8 *Actor_Find(s32);

    u8 *work = gWork;
    u32 no;
    s32 index;

    Actor_unk52_4();
    for (no = 8; no <= 65; no++) {
        u8 *actor = Actor_Find(no);
        if (actor != NULL) {
            actor[85] = 0;
        }
    }
    index = *(s16 *)(work + 0x16c) - 1;
    Actor_unk6_2(158);
    Actor_unk4_3((s32)gOv4[index].commands, gOv4[index].first, gOv4[index].second);
    Actor_unk5_3(0, 0x8000, 0x4000);
    Actor_Find(0)[85] = 0;
    Actor_Apply7(0, 2);
    if (index != 6) {
        Actor_unk6_3(0, 2, -8);
        Actor_unk15_2(10);
    }
    Actor_unk16_2(*(s16 *)(work + 0x16c));
    Actor_unk53_4();
    Actor_unk54_4();
    Actor_unk55_4();
}

/*
 * Copy the player's two-bit mode into both mode fields of the given actor's
 * record and clear its flag byte at +35.
 *
 * f80 is volatile so that the pointer is loaded again for the second store;
 * without that the first load is reused and the second one disappears. The
 * two mode writes must stay bitfields so they share one 32-bit mask, which
 * explicit mask-and-or arithmetic does not produce.
 */
void Actor_CopyPlayerModeToActor(struct Work_3b5 *work)
{
    s32 bits;

    if (work != 0) {
        bits = Actor_unk5_4(0)->f80->mode9;
        work->f35 = 0;
        work->f80->mode9 = bits;
        work->f80->mode15 = bits;
    }
}

s32 Actor_unk56_4(s32 a0)
{
    extern u8 gWork[];

    u32 i;
    s32 record;
    s32 handler;
    s32 hidden;

    *(s32 *)((*(u8 *volatile *)gWork + 0x1c0)) = 0x100;
    Actor_unk7_3(16, 0x1600000, 0x1600000);
    Actor_unk3(16, 0x2008ec0);
    record = Actor_unk4(16);
    handler = 0x2008171;
    {
        volatile u16 *target = (volatile u16 *)(record + 100);
        s32 shown = 1;

        *target = shown;
    }
    *(volatile s32 *)(record + 108) = handler;
    hidden = 0;
    Actor_unk8_3(17, 0x1700000, 0x1400000);
    Actor_unk5(17, 0x2008f90);
    record = Actor_unk6(17);
    *(volatile u16 *)(record + 100) = hidden;
    *(volatile s32 *)(record + 108) = handler;
    record = Actor_unk27(14);
    *(volatile s32 *)(record + 108) = 0x20086e9;
    if (Actor_unk7(0x8c1) != 0) {
        Actor_unk9_3(28, 0x13c0000, 0x1480000);
    }
    if (Actor_unk8(0x201) != 0) {
        Actor_unk57_4();
    }
    if (Actor_unk9(0x200) != 0) {
        Actor_unk58_4();
        Actor_unk59_4(8, 4);
    }
    if (Actor_unk10(0x950) != 0) {
        Actor_unk10_3(20, 0x2080000, 0x2300000);
        Actor_unk11_3(21, 0x2080000, 0x2300000);
        Actor_unk12_3(22, 0x2080000, 0x2300000);
        Actor_unk13_3(24, 0x2080000, 0x2300000);
        Actor_unk14_3(25, 0x2080000, 0x2300000);
        Actor_unk15_3(26, 0x2080000, 0x2300000);
        Actor_unk16_3(27, 0x2080000, 0x2300000);
    } else {
        if (Actor_unk11(0x962) != 0) {
            Actor_unk17_3(27, 0x1180000, 0x500000);
            Actor_unk18_3(27, 0x2000, 0);
            Actor_unk60_4(27, 1);
        }
    }
    return 0;
}

void Scene_RunScene3b5SequenceA(void)
{
    extern u8 gWork[];
    void Actor_unk8_4();
    void Actor_unk9_4();

    u32 i;
    s32 record;

    Actor_unk61_4();
    Actor_unk19_3(0, 0x130, 0x138);
    Actor_unk20_3(0, 0xc000, 0);
    Actor_unk21_3(28, 0x4000, 0);
    Actor_unk62_4(20);
    Actor_unk7_2(0xe3d);
    Actor_unk12(28, 0);
    if (Actor_unk13(0, 0) == 0) {
        bump_step(1);
        Actor_unk63_4(28, 0);
        Actor_unk22_3(28, 0x10000, 0x8000);
        Actor_unk23_3(28, 0x140, 0x130);
        Actor_unk24_3(28, 0x13c, 0x148);
        Actor_unk25_3(28, 0xa000, 0);
        Actor_unk8_2(0x8c1);
    } else {
        Actor_unk64_4(28, 0);
    }
    Actor_unk65_4();
}

void State_SetValue30ThenCall(void)
{
    Actor_unk17_2(30);
    Actor_unk66_4();
}

void State_PassWorkHalfword16C(void)
{
    extern u8 *gWork;

    s16 *cnt = (s16 *)(gWork + 0x16C);

    Actor_unk18_2(*cnt);
}

/*
 * Set up objects 29, 30 and 32, branch on a query result to run one of two
 * near-identical sequences for objects 29, 30 and 20 with different
 * positions, sizes and speeds, then finish with shared placement calls.
 */
void Scene_RunPrimarySequence(void)
{
    extern u8 gWork[];

    u32 i;
    s32 object32_id;
    s32 list_00001fb6;

    Battle_Reset_1();
    Motion_CamBounds_1(-1, -1, -1, 0);
    Motion_SetSpeed_1(29, 0x10000, 0x8000);
    Motion_SetSpeed_2(30, 0x10000, 0x8000);
    list_00001fb6 = (s32)gVal14;
    SceneWork_SetStepValue_1(list_00001fb6);
    Motion_SetHPosTerrain_1(29, 0x480000, 0xd00000);
    Motion_SetHPosTerrain_2(30, 0x380000, 0xd00000);
    ObjectGroup_ConfigureChildValue_1(32, 15);
    object32_id = Scene_GetRecord_1(32);
    Actor_unk67_4(object32_id, 0); /* main:080091e0 */
    Motion_SetHPosTerrain_3(32, 0x5f0000, 0x280000);
    Motion_ResetPosMode2_1(29, 72, 248);
    Motion_ResetPosMode2_2(30, 56, 248);
    Motion_SetPosReset_1(0, 64, 0x108);
    Motion_ArmCb_1(0, 0xc000, 0);
    Motion_CommitPos_1(29);
    Object_SetModeById_1(29, 1);
    Object_SetModeById_2(30, 1);
    Object_SetModeById_3(0, 1);
    Motion_SetAngleToward_1(29, 0, 0);
    Motion_SetAngleToward_2(30, 0, 0);
    Battle_WaitMode0_1(20);
    Battle_WaitMode0_2(29, 0x102); /* main:0808a1f0 */
    Battle_WaitMode0_3(30, 0x102); /* main:0808a1f0 */
    Motion_SetVarCb_1(29, 2);
    Motion_SetVarCbObj_1(30, 2);
    Battle_WaitMode0_4(20);
    Motion_SetSpeed_3(29, 0); /* main:0808a178 */
    Battle_WaitMode0_5(25);
    Actor_unk14(52, 0, 12, 7); /* main:080150f8 */
    UiWork_Create_1((list_00001fb6 + 3), 11, 12, 2);
    SCENE_OBJECT_ID = 32;
    if (UiWork_WaitThenFinalizeCapacity_1(0, 0) == 0) { /* object_id 0, force 0 */
        Battle_WaitMode0_6(20);
        Motion_SetVarCbObj_2(30, 2);
        Battle_WaitMode0_7(30);
        Motion_ArmCb_2(30, 0, 0);
        Battle_WaitMode0_8(30);
        Battle_WaitMode0_9(10);
        Motion_CallWaitAnim_1(29, 3);
        Battle_WaitMode0_10(20);
        Motion_ArmCb_3(29, 0, 0);
        Battle_WaitMode0_11(30);
        BattleEv_RunWait_1(29, 0);
        Battle_WaitMode0_12(20);
        Motion_ArmCb_4(29, 0x4000, 0);
        Motion_ArmCb_5(30, 0x4000, 0);
        Battle_WaitMode0_13(30);
        Object_SetModeById_4(29, 3);
        Motion_CallWaitAnim_2(30, 3);
        Battle_WaitMode0_14(20);
        Motion_SetSpeed_4(29, 0x1cccc, 0xe666);
        Motion_SetSpeed_5(30, 0x1cccc, 0xe666);
        Motion_ResetPosMode2_3(29, 232, 248);
        Battle_WaitMode0_15(2);
        Motion_ResetPosMode2_4(30, 232, 248);
        Motion_CommitPos_2(29);
        Motion_ResetPosMode2_5(29, 248, 248);
        Motion_SetPosReset_2(30, 248, 248);
    } else {
        Battle_WaitMode0_16(20);
        Motion_SetVarCbObj_3(30, 2);
        Battle_WaitMode0_17(30);
        Motion_ArmCb_6(30, 0, 0);
        Battle_WaitMode0_18(30);
        Battle_WaitMode0_19(10);
        Motion_CallWaitAnim_3(29, 4);
        Battle_WaitMode0_20(20);
        Motion_ArmCb_7(29, 0, 0);
        Battle_WaitMode0_21(30);
        bump_step4(1);
        BattleEv_RunWait_2(29, 0);
        Battle_WaitMode0_22(20);
        Motion_ArmCb_8(29, 0x4000, 0);
        Motion_ArmCb_9(30, 0x4000, 0);
        Battle_WaitMode0_23(30);
        Object_SetModeById_5(29, 3);
        Motion_CallWaitAnim_4(30, 3);
        Battle_WaitMode0_24(20);
        Motion_SetSpeed_6(29, 0x19999, 0xcccc);
        Motion_SetSpeed_7(30, 0x19999, 0xcccc);
        Motion_ResetPosMode2_6(29, 72, 184);
        Motion_SetPosReset_3(30, 56, 184);
    }
    Motion_SetHPosTerrain_4(29, 0, 0);
    Motion_SetHPosTerrain_5(30, 0, 0);
    Motion_SetHPosTerrain_6(32, 0, 0);
    GameFlag_Set_1(0x8c0);
    Battle_SchedShoulder_1();
}

void Scene_ResetActor9AndDrawTiles(void)
{
    struct Actor *actor = Actor_unk6_4(9);
    if (actor != 0) {
        Actor_Apply8(actor, 0);
        actor->field23 = 2;
        actor->field55 = 0;
    }
    Actor_Apply9(9, 5);
    {
        s32 v5 = 34;
        s32 v6 = 16;
        Actor_SetRect(36, 16, 1, 1, v5, v6);
    }
    Actor_unk19_2(0x201);
}
