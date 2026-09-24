#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

enum MotionParticleMessage {
    MSG_YOUR_FIRST_TIME_VISIT_ALTIN = 0x18bd,
    MSG_WE_CANT_DRINK_WATER_MONSTERS = 0x18c7,
    MSG_TRYING_FIND_YOUR_WAY_WEST = 0x18d9,
    MSG_DEFEATED_THOSE_MONSTERS_DIDNT = 0x18e1,
    MSG_DO_WANT_WEAPONS = 0x18e7,
    MSG_THANK_GOODNESS_WATER_HAS_RECEDED = 0x18ea,
    MSG_MY_STORE_SUBMERGED_WANT_SELL = 0x18ed,
    MSG_ITS_GREAT_CAN_SELL_ARMOR = 0x18ee,
    MSG_WILL_DO_IF_MY_MERCHANDISE = 0x18f1,
    MSG_NONE_MY_GOODS_WERE_DAMAGED = 0x18f2,
    MSG_WE_GOT_LITTLE_DAMP_BUT = 0x18f5,
    MSG_DID_SEE_WATER_GUSHING_OUT = 0x1918,
    MSG_ONES_WHO_DEFEATED_WATER_BEASTS = 0x191f,
    MSG_TRUE_FOUND_ANCIENT_RUINS_IN = 0x1924,
    MSG_GIRL_FROM_XIAN_WAS_ASKING = 0x1932,
    MSG_YOULL_HAVE_FIND_PASSAGE_IN = 0x1941,
    MSG_HOW_ABOUT_ARENT_IMPRESSED_BY = 0x1943,
    MSG_GIRL_FROM_XIAN_BOUGHT_LOT = 0x1945,
    MSG_THERE_SMALL_TEMPLE_WEST_ALTIN = 0x1947,
    MSG_THERE_FEW_BEASTS_IN_MINE = 0x194a
};


#define Scene_GetRecord_1(args...) Func_020010e0(args)
#define RuntimeBlock_GetOffset1e0Pointer_1(args...) Func_020011a6(args)
#define Scene_GetRecord_2(args...) Func_0200112a(args)

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
extern u8 Data_02009120[];
extern u8 Data_02009288[];
extern u8 Data_020090f0[];
extern u8 Data_0200940c[], Data_020095bc[], Data_020093f4[];
extern u8 Data_02009730[];
extern u8 Data_020099f4[];
extern u8 Data_02009724[];
extern u8 *Data_03001ebc;   /* pointer cell holding the overlay work */
extern u8 Data_02009e70[];
extern u32 Data_03001e40;
extern s16 Data_02000240_t[][1];

void Func_02000f9e(void *);
u8 *Func_020010e0();
u8 *Func_0200112a();
s32 Func_020011a6();
struct Slot02000338 *Func_02001206(s32);
s32 Func_0200140a();
s32 Func_020014e8();
s32 Func_0200157a();
u32 Func_02001a82(void);
u32 Func_02001ae4(void);
u32 Func_02001b0a(void);
struct SceneMotion *Func_02001b6c(s32, s32, s32, s32);
s32 Func_02001686();
struct Actor *Func_0200171a(s32);
s32 Func_020017d4();
void Func_02001832();
u8 *Func_0200191a();
s32 Func_0200193e();
s32 Func_0200194e();
u8 *Func_0200195c();
s32 Func_0200196a();
s32 Func_0200197a();
u8 *Func_02001988_b();
void Func_02001998();
u8 *Func_02001a12();
u8 *Func_02001a3e();
u8 *Func_02001a6c();
s32 Func_02001a7a();
s32 Func_02001a8c();
s32 Func_02001aa2();
void Func_02001bc6();
void Func_02001d38();

/* Each Func_ symbol names the call word the overlay image holds before loader
 * relocation, not a runtime address. One word can serve two sites with
 * different targets, so the sites stay separate and must not be merged onto a
 * shared name. Names that describe what a target does are provisional. */

/* Calls spelled through these wrappers pass their constants straight into the
 * argument registers. Spelling them as direct calls instead precomputes a
 * costly constant into a temporary that is then shared with later uses in the
 * same block. A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1(void (*f)(), s32 a0)
{

    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{

    return f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{

    f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{

    f(a0, a1, a2);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

static __inline__ void Call3_020006f0(void (*func)(), s32 a, s32 b, s32 c)
{
    u32 Func_02001aaa(void);

    func(a, b, c);
}

static __inline__ struct SceneMotion *CreateActor(
    struct SceneMotion *(*func)(s32, s32, s32, s32),
    s32 kind, s32 x, s32 y, s32 z)
{
    u32 Func_02001aaa(void);

    return func(kind, x, y, z);
}

static __inline__ s32 Value6(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    return f(a0, a1, a2, a3, a4, a5);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

u8 *Func_0200138a();        /* scene/actor record accessor */

s32 SceneData_SelectTableByWord224(void)
{
    s16 v = gGameState.scene;

    if (v == (s32)&Value_0000004b) {
        return (s32)Data_02009120;
    }
    if (v == (s32)&Value_0000004c) {
        return (s32)Data_02009288;
    }
    return (s32)Data_020090f0;
}

s32 Func_02000070(void)
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
    s32 id = gGameState.scene;
    if (id == (s32)&Value_0000004b) {
        if (GameFlag_IsSet(0x909)) {
            Data_0200940c[142] = 0;
            Data_0200940c[166] = 0;
        }
        return Data_0200940c;
    }
    if (id == (s32)&Value_0000004c) {
        if (GameFlag_IsSet(0x8fd))
            Data_020095bc[46] = 1;
        if (GameFlag_IsSet(0x8fe) || GameFlag_IsSet(0x907))
            Data_020095bc[94] = 1;
        Func_02000f9e(Data_020095bc);
        return Data_020095bc;
    }
    return Data_020093f4;
}

s32 SceneData_GetPrimaryTable(void)
{
    s16 v = gGameState.scene;

    if (v == (s32)&Value_0000004b) {
        return (s32)Data_02009730;
    }
    if (v == (s32)&Value_0000004c) {
        return (s32)Data_020099f4;
    }
    return (s32)Data_02009724;
}

void FieldScene_RunActorEightPromptDialogue(void)
{
    u8 *work;

    Event_Begin();
    Event_SetMessage(MSG_YOUR_FIRST_TIME_VISIT_ALTIN);
    /* r1 is set before r0; the argument order is unchanged. */
    Event_OpenMessage(8, 0);

    if (Event_ChooseYesNo(0, 0) == 1) {
        Event_ShowMessage(8, 0);
    } else {
        work = Data_03001ebc;
        *(u16 *)(work + 472) = (u16)(*(u16 *)(work + 472) + 1);
        Event_AskYesNo(8, 0);
    }

    Event_End();
}

void SceneDialogue_ShowLine1918(void)
{
    Event_Begin();
    Event_SetMessage(MSG_DID_SEE_WATER_GUSHING_OUT);
    Event_AskYesNo(9, 0);
    Event_End();
}

/*
 * One of two branches of the opening setup: a short branch that only moves
 * actor 14, or a longer branch that positions actor 18 and a second record
 * from their x/y/z fields at +8/+12/+16, clearing a byte at +85 of a
 * separately looked-up record on the way.
 */
void FieldScene_RunOpeningAuxiliarySequence(void)
{

    u8 *rec18;
    u8 *ready_flag;
    u8 *record;

    Event_Begin();
    if (GameFlag_IsSet(0x909) != 0) {
        Event_SetMessage(MSG_ONES_WHO_DEFEATED_WATER_BEASTS);
        Event_AskYesNo(14, 0); /* object 14, action 0 */
    } else {
        Actor_SetAnimation(14, 4); /* object 14, action 4 */
        Event_SetMessage(MSG_WE_CANT_DRINK_WATER_MONSTERS);
        Event_ShowMessageAndWait(14, 0, 10);
        ready_flag = GameFlag_IsSet(0x8ff);
        if (ready_flag == 0) {
            rec18 = Scene_GetRecord_1(18);
            /* Clear the byte at +85 of the lookup result. */
            *(u8 *)(RuntimeBlock_GetOffset1e0Pointer_1() + 85) = ready_flag;
            Camera_SetSpeed(0x10000, 0x2000);
            Camera_MoveTo(*(s32 *)(rec18 + 8), *(s32 *)(rec18 + 12), *(s32 *)(rec18 + 16), 1); /* use_setter 1 */
            Actor_FaceActor(ACTOR_PARTY_LEADER, 0x4000, 0);
            Actor_FaceDirection(14, 0x3000, 0);
            Camera_WaitForMove();
            Event_Wait(120); /* should_wait 120 */
            record = Scene_GetRecord_2(0);
            Camera_MoveTo(*(s32 *)(record + 8), *(s32 *)(record + 12), *(s32 *)(record + 16), 1); /* use_setter 1 */
            Camera_WaitForMove();
        }
        Actor_SetAnimationAndWait(14, 4);
    }
    Event_End();
}

void SceneDialogue_RunActor17Message1924(void)
{
    void Event_End(void);

    Event_Begin();
    Event_SetMessage(MSG_TRUE_FOUND_ANCIENT_RUINS_IN);
    Event_AskYesNo(17, 0);
    Event_End();
}

void SceneDialogue_RunActor9Message1932(void)
{

    Event_Begin();
    Event_SetMessage(MSG_GIRL_FROM_XIAN_WAS_ASKING);
    Event_AskYesNo(9, 0);
    Event_End();
}

void SceneDialogue_RunActor10Message18d9(void)
{
    void Event_Begin(void);
    void Event_End(void);

    Event_Begin();
    Event_SetMessage(MSG_TRYING_FIND_YOUR_WAY_WEST);
    Event_AskYesNo(10, 0);
    Event_End();
}

void SceneDialogue_RunActor14Message18e1(void)
{

    Event_Begin();
    Event_SetMessage(MSG_DEFEATED_THOSE_MONSTERS_DIDNT);
    Event_AskYesNo(14, 0);
    Event_End();
}

void SceneDialogue_RunActor21Message194a(void)
{

    Event_Begin();
    Event_SetMessage(MSG_THERE_FEW_BEASTS_IN_MINE);
    Event_AskYesNo(21, 0);
    Event_End();
}

s32 SceneActor_IsSlotZeroAngleInRange(void)
{
    struct Slot02000338 *slot = Func_02001206(0);

    if ((u32)((slot->angle + 0x5FFF) << 16) <= 0x3FFE0000) {
        return 1;
    }
    return 0;
}

void FieldScene_RunActorFifteenFlagBranch(void)
{
    void Event_SetMessage();
    void Shop_Open();

    if (GameFlag_IsSet(0x242) == 0) {
        Event_Begin();
        Event_SetMessage(MSG_DO_WANT_WEAPONS);
        /* r1 is set before r0 here; the argument order is unchanged. */
        Event_AskYesNo(15, 0);
        Event_End();
        return;
    }

    if (SceneActor_IsSlotZeroAngleInRange() != 0) {
        Shop_Open(19, 15);
        return;
    }

    Event_Begin();
    Event_SetMessage(MSG_THANK_GOODNESS_WATER_HAS_RECEDED);
    if (GameFlag_IsSet(0x909) != 0) {
        Event_SetMessage(MSG_YOULL_HAVE_FIND_PASSAGE_IN);
    }
    Event_ShowMessage(15, 0);
    Event_End();
}

void FieldScene_RunActorTwentyFlagBranch(void)
{
    void Event_End(void);
    void Event_End(void);
    void Event_SetMessage(s32);

    if (GameFlag_IsSet(0x241) == 0) {
        Event_Begin();
        Event_SetMessage(MSG_MY_STORE_SUBMERGED_WANT_SELL);
        Event_ShowMessage(20, 0);
        Event_End();
        return;
    }

    if (SceneActor_IsSlotZeroAngleInRange() != 0) {
        Shop_Open(20, 17);
        return;
    }

    Event_Begin();
    Event_SetMessage(MSG_ITS_GREAT_CAN_SELL_ARMOR);
    if (GameFlag_IsSet(0x909) != 0) {
        Event_SetMessage(MSG_HOW_ABOUT_ARENT_IMPRESSED_BY);
    }
    Event_ShowMessage(17, 0);
    Event_End();
}

void FieldScene_RunActorTwentyOneFlagBranch(void)
{
    if (GameFlag_IsSet(0x240) == 0) {
        Event_Begin();
        Event_SetMessage(MSG_WILL_DO_IF_MY_MERCHANDISE);
        Event_ShowMessage(21, 0);
        Event_End();
        return;
    }

    if (SceneActor_IsSlotZeroAngleInRange() != 0) {
        Shop_Open(21, 16);
        return;
    }

    Event_Begin();
    Event_SetMessage(MSG_NONE_MY_GOODS_WERE_DAMAGED);
    if (GameFlag_IsSet(0x909) != 0) {
        Event_SetMessage(MSG_GIRL_FROM_XIAN_BOUGHT_LOT);
    }
    Event_ShowMessage(16, 0);
    Event_End();
}

void FieldScene_RunFacingGatedDialogue18(void)
{
    u8 *rec;

    rec = Func_0200138a(0);

    if ((u32)((*(u16 *)(rec + 6) + 0x5fff) << 16) <= 0x3ffe0000) {
        Inn_Open(6, 18);
        return;
    }

    Event_Begin();

    if (GameFlag_IsSet(0x909) != 0) {
        Event_SetMessage(MSG_THERE_SMALL_TEMPLE_WEST_ALTIN);
        Event_ShowMessage(18, 0);
    } else {
        Event_SetMessage(MSG_WE_GOT_LITTLE_DAMP_BUT);
        Event_AskYesNo(18, 0);
    }

    Event_End();
}

void FieldScene_RunEarlySequence(void)
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

    work = Data_03001ebc;
    Event_Begin();
    for (i = 8; i <= 65; i++) {
        record = (u8 *)Value1(Func_0200140a, i);
        if (record != 0) {
            record[85] = 0;
        }
    }
    tbl = Data_02009e70;
    idx = ((s32)((s32)(*(u16 *)(work + 0x16c) - 2) << 16) >> 16);
    off = idx << 3;
    o4 = off + 4;
    a = *(s16 *)(tbl + o4);
    b = *(s16 *)(tbl + o4 + 2);
    if (idx == 1) {
        Audio_PlayCue(188);
        Map_CopyCellsTo(42, 33, a, b, 2, 2);
        c = a + 2;
        Map_CopyCellsTo(42, 35, c, b, 2, 2);
        Event_Wait(4);
        Map_CopyCellsTo(40, 33, a, b, 2, 2);
        Map_CopyCellsTo(40, 35, c, b, 2, 2);
        Event_Wait(4);
    } else {
        Audio_PlayCue(158);
        if (idx == 3) {
            Map_CopyCellsTo(33, 42, 8, 17, 1, 2);
        }
        Map_AnimateCells(*(s32 *)(tbl + off), a, b);
    }
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x4000);
    *(s32 *)(Data_03001ebc + 0x1c0) = 0x100;
    *(u8 *)(Func_020014e8(0) + 85) = 0;
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 2);
    if (idx == 6) {
        Actor_CenterAndWalk(ACTOR_PARTY_LEADER, 2, 0);
    } else {
        if (idx != 1) {
            Actor_CenterAndWalk(ACTOR_PARTY_LEADER, 2, -4);
        } else {
            Actor_SetSpritePriority(ACTOR_PARTY_LEADER, 2);
            Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, -4);
        }
    }
    Event_Wait(10);
    Event_RequestExit(*(s16 *)(work + 0x16c));
    Event_CloseScreen();
    Event_WaitForScreen();
    Event_End();
}

void FieldScene_RunScene3a3SequenceB(void)
{
    u8 *work;

    work = Data_03001ebc;
    Event_Begin();
    *(u8 *)(Func_0200157a(0) + 85) = 0;
    Audio_PlayCue(123);
    Actor_CenterAndWalk(ACTOR_PARTY_LEADER, 2, -16);
    Event_RequestExit(*(s16 *)(work + 0x16c));
    Event_CloseScreen();
    Event_WaitForScreen();
    Event_End();
}

void SceneMotion_UpdateTimedActor(struct SceneMotion *work)
{
    u32 Func_02001aaa(void);

    if (work->delay != 0) {
        if (--work->delay == 1)
            Work_SetValuesIfNonNegative(-1, -1, 0xe666);
    }
    if (work->velocity == 0) {
        Object_SetAnimation(work, 1);
        work->y += -0x18000;
        if (work->y < work->ground) {
            if (work->active != 0) {
                Audio_PlayCue(229);
                work->active = 0;
                work->delay = 4;
                Work_SetValuesIfNonNegative(0, 0x10000, 0x10000);
            }
            work->y = work->ground;
        }
        work->state = 1;
    } else {
        work->state = 0;
    }
    if (work->timer == 0) {
        Audio_PlayCue(152);
        work->active = 1;
        Object_SetAnimation(work, 2);
        work->velocity = 0x30000;
    }
    if (++work->timer == 60)
        work->timer = 0;
}

void FieldScene_RunScene3a3SequenceC(void)
{
    struct SceneMotion *motion;

    motion = (struct SceneMotion *)Value1(Func_02001686, 18);
    motion->timer = 0;
    motion->delay = 0;
    *(s32 *)((u8 *)motion + 72) = 0x6666;
    motion->callback = SceneMotion_UpdateTimedActor;
    Actor_SetSpeed(18, 0x13333, 0x9999);
    Actor_MoveToAndWait(18, 28, 0x1cc);
    Actor_MoveToAndWait(18, 24, 0x1c0);
    Audio_PlayCue(229);
    Actor_Destroy(18);
    Work_SetValuesIfNonNegative(0, 0x10000, 0x10000);
    Event_Wait(4);
    Work_SetValuesIfNonNegative(-1, -1, 0xe666);
    Event_Wait(40);
    Actor_SetAnimation(18, 1);
}

void SceneState_SetFlag906ByActorNineteenX(void)
{
    struct Actor *p = Func_0200171a(19);

    if ((p->f08 >> 20) == 22) {
        GameFlag_Set(0x906);
    } else {
        GameFlag_Clear(0x906);
    }
}

/*
 * Mirror three progress flags into three scene flags, then let the scene id
 * at Data_02000240[224] select one of two continuations. Every call here
 * leaves through its own veneer, so the sites stay separate.
 */
s32 SceneState_SyncProgressFlagsAndDispatch(void)
{
    s16 scene;

    if (GameFlag_IsSet(0x8fd) != 0) {
        GameFlag_Set(0x240);
    }

    if (GameFlag_IsSet(0x8fe) != 0 || GameFlag_IsSet(0x907) != 0) {
        GameFlag_Set(0x241);
    }

    if (GameFlag_IsSet(0x8fe) != 0 && GameFlag_IsSet(0x907) != 0) {
        GameFlag_Set(0x242);
    }

    scene = gGameState.scene;
    if (scene == (s32)&Value_0000004b) {
        FieldScene_RunMiddleSequence();
    } else if (scene == (s32)&Value_0000004c) {
        FieldScene_RunScene3a3SequenceD();
    }

    return 0;
}

void FieldScene_RunMiddleSequence(void)
{
    s32 scene;
    s32 rec5;
    s32 rec6;
    s32 rec0;
    s32 kind;

    scene = Value1(Func_020017d4, 0);
    rec5 = GameFlag_IsSet(0x242);
    if (rec5 != 0) {
        Map_CopyCellsTo(64, 32, 0, 32, 32, 32);
        Map_CopyCellAttributes(64, 32, 32, 32, 0, 0);
        kind = 20;
    } else {
        rec6 = GameFlag_IsSet(0x241);
        if (rec6 != 0) {
            Map_CopyCellsTo(64, 0, 0, 32, 32, 32);
            Value6(Engine_MapCopyCellAttributes, 64, 0, 32, 32, rec5, rec5);
            Actor_Destroy(17);
            kind = 20;
        } else {
            rec0 = GameFlag_IsSet(0x240);
            if (rec0 == 0) {
                goto L_020009b8;
            }
            Map_CopyCellsTo(0, 64, 0, 32, 32, 32);
            Value6(Engine_MapCopyCellAttributes, 0, 64, 32, 32, rec6, rec6);
            Actor_Destroy(16);
            kind = 17;
        }
    }
    Actor_Destroy(kind);
    Actor_Destroy(21);
    goto L_020009da;
L_020009b8:
    Map_CopyCellAttributes(0, 32, 32, 32, rec0, rec0);
    Actor_Destroy(15);
    Actor_Destroy(16);
    Actor_Destroy(17);
L_020009da:
    if (GameFlag_IsSet(0x8ff) != 0) {
        Actor_Destroy(18);
    } else {
        Func_02001998(170);
        Actor_SetChildValue(18, 2);
        Actor_SetAnimation(18, 3);
        Call2(Func_02001832, 0x2008d09, 0xc80);
    }
    if (Data_02000240_t[225][0] == 3) {
        GameFlag_Clear(FLAG_ARRIVAL_EVENT_PENDING);
    }
    Map_CopyCellAttributes(0, 33, 4, 3, 20, 41);
    if (GameFlag_IsSet(0x906) != 0) {
        Actor_SetPosition(19, 0x1680000, 0xa80000);
    }
    Actor_SetSpriteFlags((s32)Func_0200191a(19), 0);
    Actor_SetChildValue(22, 15);
    Call2((void (*)())Engine_ActorSetChildValue, 23, 15);
    Actor_SetChildValue(24, 15);
    {
        u8 bits = 8;
        u8 *flags = (u8 *)Func_0200193e(22) + 89;
        u8 value = *flags;

        value |= bits;
        *flags = value;
        flags = (u8 *)Func_0200194e(23) + 89;
        value = *flags;
        value |= bits;
        *flags = value;
        flags = (u8 *)Func_0200195c(24) + 89;
        bits |= *flags;
        *flags = bits;
    }
    {
        u8 bits = 2;
        u8 *flags = (u8 *)Func_0200196a(22) + 35;
        u8 value = *flags;

        value |= bits;
        *flags = value;
        flags = (u8 *)Func_0200197a(23) + 35;
        value = *flags;
        value |= bits;
        *flags = value;
        flags = (u8 *)Func_02001988_b(24) + 35;
        bits |= *flags;
        *flags = bits;
    }
    Actor_SetSpritePriority(22, 1);
    Actor_SetSpritePriority(23, 1);
    Actor_SetSpritePriority(24, 1);
    Task_Wait(1);
    Event_Begin();
    Camera_MoveTo(*(s32 *)(scene + 8), *(s32 *)(scene + 12), *(s32 *)(scene + 16), 0);
    Map_Redraw();
    Event_End();
    Task_Wait(1);
}

void FieldScene_RunScene3a3SequenceD(void)
{
    u8 *actor;
    s32 facing;

    if (GameFlag_IsSet(0x240) == 0) {
        Actor_SetPosition(8, 0x3280000, 0x2d70000);
        actor = Func_02001a12(8);
        facing = 0x3000;
        *(u16 *)(actor + 6) = facing;
        Actor_SetPosition(9, 0x31a0000, 0x3390000);
    }
    if (GameFlag_IsSet(0x241) == 0) {
        Actor_SetPosition(10, 0x2300000, 0x2c60000);
        actor = Func_02001a3e(10);
        facing = 0x1000;
        *(u16 *)(actor + 6) = facing;
        Actor_SetPosition(11, 0x2400000, 0x2c60000);
    }
    if (GameFlag_IsSet(0x242) == 0) {
        Actor_SetPosition(15, 0x1270000, 0x2e80000);
        actor = Func_02001a6c(15);
        facing = 0xb000;
        *(u16 *)(actor + 6) = facing;
    } else {
        u8 flags;

        actor = (u8 *)Value1(Func_02001a7a, 15);
        flags = 4;
        flags |= actor[89];
        actor[89] = flags;
    }
    actor = (u8 *)Value1(Func_02001a8c, 17);
    if (actor != 0) {
        u8 flags = 4;

        flags |= actor[89];
        actor[89] = flags;
    }
    actor = (u8 *)Value1(Func_02001aa2, 16);
    if (actor != 0) {
        u8 flags = 4;

        flags |= actor[89];
        actor[89] = flags;
    }
}

void SceneActor_ResetStateAndSpan(struct Actor02000c0c *actor)
{
    void Func_02001aaa_local(struct Actor02000c0c *, s32);

    u8 *state = &actor->state;
    s32 clear = 0;
    u8 *attached;

    *state = (u8)clear;
    attached = actor->attached;
    clear -= 13;
    attached[9] = (clear & attached[9]) | 4;
    Object_SetPalette(actor, 3);
    Func_02001aaa_local(actor, 0);
    actor->span = 0x4CCC;
    actor->reach = 0x4CCC;
}

void SceneEffect_UpdateDriftingParticle(struct SceneMotion *work)
{
    u32 Func_02001aaa(void);

    work->x += (work->timer << 12) +
        ((s16)((s32)((Func_02001a82() * 2) >> 16) - 1) << 15);
    if (work->timer <= 3) {
        work->z += -((Func_02001aaa() * 0x8000) >> 16) - 0x10000;
        work->scale_x += 0x2666;
        work->scale_y += -0xa3d;
    } else {
        work->z += 0x20000;
        work->scale_x += 0x7ae;
        work->scale_y += 0x7ae;
    }
    if ((Func_02001ae4() * work->timer) >> 16 == 0)
        Object_SetPalette(work, 7);
    if (work->timer != 0)
        work->timer--;
    else
        work->timer = ((Func_02001b0a() * 5) >> 16) * 2 + 2;
    if (--work->active == 0) {
        work->callback = 0;
        Engine_ObjectDispatchRelease(work);
    }
}

void SceneEffect_SpawnDriftingParticle(void)
{
    u32 Func_02001aaa(void);

    struct SceneMotion *work;
    if ((Data_03001e40 & 3) == 0) {
        work = CreateActor(Func_02001b6c, 222, 0x400000, 0, 0x1900000);
        if (work != 0) {
            work->timer = 20;
            work->delay = 0;
            work->active = 20;
            SceneActor_ResetStateAndSpan(work);
            work->callback = (void (*)(struct SceneMotion *))0x02008c45;
            Object_SetAnimation(work, 1);
        }
    }
}

void FieldScene_RunScene3a3_02000d58(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    Camera_SetSpeed(0x6666, 0xccc);
    Camera_MoveTo(0x3f0000, -1, 0x1c20000, 1);
    Camera_WaitForMove();
    Event_Wait(30);
    Actor_SetAnimation(18, 1);
    Call1(Func_02001d38, -1);
    Call1(Func_02001bc6, 0x2008d09);
    Event_Wait(20);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 18, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 0);
    Actor_FaceDirection(18, 0, 20);
    Actor_FaceDirection(18, 0xd000, 40);
    Audio_PlayCue(147);
    Actor_RunRepeatedMotion(18, 2);
    Event_Wait(20);
    Actor_FaceDirection(18, 0xb000, 40);
    FieldScene_RunScene3a3SequenceC();
    Camera_FollowActor(ACTOR_PARTY_LEADER, 1);
    Camera_WaitForMove();
    Actor_SetAnimationAndWait(14, 4);
    GameFlag_Set(0x8ff);
    Event_End();
}
