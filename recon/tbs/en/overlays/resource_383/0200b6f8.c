/* Draft of RunDialoguePromptScene, resource_383 at 0x0200b6f8 (split from FIELD/KUUPUAPPU_HEYA/PROMPT.C).
 * Remaining difference: it loads constants through address-derived symbols (Value_/Data_0000/LinkedMessage_ names) that no link defines, so the overlay keeps its listing rows. */
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

enum {
    /* Message 0x182 + 189. */
    ITEM_WATER_OF_LIFE = 189,
    /* Message 0x182 + 231. */
    ITEM_BONE = 231
};

enum PromptMessage {
    MSG_ROBIN_CHECKED_CHEST = 0x929,
    MSG_ROBIN_CHECKED_BARREL = 0x92b,
    MSG_BUT_CHEST_WAS_EMPTY = 0x949,
    MSG_BUT_DIDNT_FIND_ANYTHING = 0x94b,
    MSG_IF_ONLY_THESE_ROCKS_WERE = 0x1243,
    MSG_OK_MISTER_LET_ME_SEE = 0x1245,
    MSG_MISTER_FUN_SEE_STRANGE_NEW = 0x1247,
    MSG_WONDER_OUTSIDE_WORLD_LIKE = 0x124b,
    MSG_JUST_ME_OR_AM_MISSING = 0x124c,
    MSG_COULD_THEY_THIEVES_GOOD_DONT = 0x124e,
    MSG_COULD_SOMEONE_PLEASE_HELP_IVAN = 0x1250,
    MSG_IVAN_HAS_GREAT_POWERS_WOULDNT = 0x1253,
    MSG_DO_POSSESS_STRANGE_POWERS = 0x1256,
    MSG_WOULD_REALLY_WOULD_HELP_ME = 0x125d,
    MSG_YOURE_GOING_HELP_IVAN = 0x1276,
    MSG_PLEASE_LOOK_AFTER_IVAN = 0x1278,
    MSG_TICKLES_BEING_TICKLED_BY_BOY = 0x127c,
    MSG_THIEVES_DIDNT_HIT_OUR_HOUSE = 0x1282,
    MSG_DID_JUST_ARRIVE_IN_TOWN = 0x1284,
    MSG_EVERYONE_THINKS_OUR_GUESTS_THIEVES = 0x128d,
    MSG_THOSE_THREE_STRANGERS_SURE_HAVE = 0x128e,
    MSG_MASTER_HIS_WIFE_BLINDED_BY = 0x1294,
    MSG_ROBIN_TAKE_LEAD = 0x129f,
    MSG_OW_STOP = 0x12ac,
    MSG_WE_DONT_HAVE_TIME_FOR = 0x12bb,
    MSG_THESE_KIDS_NOTHING_WORRY_ABOUT = 0x12dd,
    MSG_THEY_THEY_GOT_US = 0x12e4,
    MSG_SEE_THATS_HAPPENED = 0x12f2,
    MSG_WAIT_DONT_WANT_TAKE_YOUR = 0x132a,
    MSG_THANK_GOODNESS_THOSE_THIEVES_WERE = 0x1353,
    MSG_IF_ROCK_WORTHLESS_MAYBE_THATS = 0x1355,
    MSG_DID_THOSE_THIEVES_COME_FROM = 0x1356,
    MSG_MY_FATHER_WORRIED_ABOUT_THOSE = 0x1359,
    MSG_FATHER_LOOKS_SAD_WORRYING_LIKE = 0x135b,
    MSG_THIEVES_HID_STOLEN_TREASURE_IN = 0x135c,
    MSG_GUESS_NOTHING_IN_OUR_HOUSE = 0x135e,
    MSG_HEADING_OUT_BEYOND_GOMA_RANGE = 0x1364,
    MSG_HEARD_DEFEATED_THOSE_THIEVES = 0x1368,
    MSG_CAVE_IN_GOMA_RANGE_DANGEROUS = 0x136c,
    MSG_WE_FOUND_OUR_STOLEN_WEAPONS = 0x1370,
    MSG_IF_YOURE_GONNA_HEAD_INTO = 0x1372,
    MSG_WITH_BRIDGE_OUT_WILL_QUITE = 0x1374,
    MSG_THEY_HID_THOSE_STOLEN_GOODS = 0x137b,
    MSG_HAVE_LOT_LEFTOVER_BONES_FROM = 0x137c,
    MSG_GEE_ALWAYS_GET_HUNGRY_WHEN = 0x1382,
    MSG_WOW_HAVE_MANY_THINGS_ARENT = 0x1384,
    MSG_WANT_MORE_BONES = 0x1385,
    MSG_HE_REALLY_LIKES_BONES_WONDER = 0x1cf4
};

#define Scene_GetRecord_1(args...) Func_02005ae2(args)
#define Scene_GetRecord_2(args...) Func_02005b1c(args)
#define Audio_PlayCueForPartyMember_1(args...) Func_02005cca(args)
#define Audio_PlayCue_1_020019a4(a0) Value1(Engine_AudioPlayCue, a0)
#define SCENE_WORD_1C8 (*(u32 *)(*(u8 **)0x03001ebc + 456))
#define SceneWork_SetStepValue_1_020019e4(a0) Value1(Engine_EventSetMessage, a0)
#define Audio_PlayCue_1_020019e4(a0) Value1(Engine_AudioPlayCue, a0)
#define SCENE_WORK_FIELD_456 (*(u32 *)(*(u8 **)0x03001ebc + 456))
#define Scene_GetRecord_1_02001a4c(args...) Func_020067d0(args)
#define Scene_GetRecord_2_02001a4c(args...) Func_020067d8(args)
#define SceneWork_SetStepValue_1_02001a4c(a0) Value1(Engine_EventSetMessage, a0)
#define ObjectMotion_EnableActionAndSetCallback_1(a0, a1) Value2(Engine_ActorEnableActionCallback, a0, a1)
#define ObjectMotion_EnableActionAndSetCallback_2(a0, a1) Value2(Engine_ActorEnableActionCallback, a0, a1)
#define Object_LookupAndStep_1(a0) Call1(Func_020068d0, a0)
#define BattleRuntime_ScheduleShoulderButtonModeUpdate_1_02001a4c() Value0(Engine_EventEnd)
#define RATIO_HI 52428
#define RATIO_LO 26214
#define Scene_GetRecord_1_02001ba0(a0) Value1(Func_02006924, a0)
#define Scene_GetRecord_2_02001ba0(a0) Value1(Func_0200692c, a0)
#define ObjectMotion_EnableActionAndSetCallback_1_02001e80(a0, a1) Value2(Engine_ActorEnableActionCallback, a0, a1)
#define Object_LookupAndStep_1_02001ba0(args...) Func_02006ca4(args)
#define Scene_GetRecord_1_02001e80(args...) Func_02006c92(args)
#define Scene_GetRecord_2_02001e80(a0) Value1(Func_02006c9e, a0)
#define ACTOR_SHOWN_OFFSET 100
#define Scene_GetRecord_1_02002fd4(a0) Value1(Func_02007d56, a0)
#define Scene_GetRecord_2_02002fd4(args...) Func_02007d8c(args)
#define Scene_GetRecord_3(args...) Func_02007d98(args)
#define Scene_GetRecord_4(args...) Func_02007da4(args)
#define Scene_GetRecord_5(args...) Func_02007dc8_a(args)

/*
 * resource_383 owner at 0x02002ba0, 80 bytes.
 * Points two records at a third: each gets the angle from its own offset to the
 * reference record, stored as a halfword at +6.
 */
extern s16 Data_02000240[];
extern u8 Data_0200dcc8[];
extern u8 Data_0200dab8[];
extern u8 LinkedMessage_MasterHammetIsntOnlyOne;
extern u8 Data_0200e1fc[];
extern u8 Data_0200e250[];
extern u8 Data_0200de30[];
extern u8 Data_0200cf2c[];
extern u8 LinkedMessage_YouWereSuchGreatHelp[];
extern u8 MsgNoEffect;
extern u8 Value_000012c3;
extern s32 Data_0200e4a8[];
extern s32 Data_0200e4c0[];
extern u8 LinkedMessage_TheyreActingSuspiciousSomethingsNot[];
extern u8 LinkedMessage_TheyreBack[];
extern u8 Data_0200d17c[];
extern u8 LinkedMessage_YouRobinRightWontForget[];
extern u8 LinkedMessage_IvanGotShamansRod[];
extern u8 Data_0200d354[];
extern u8 Data_0200d4c8[];
extern u16 Data_0200e4f8;

void Func_02004dbc(u8 *);
u8 *Func_02004e04(s32);
u8 *Func_02004e14(s32);
u8 *Func_02004e3e(s32);
u8 *Func_02004e4e(s32);
struct FieldActor *Func_02004e84(s32);
u8 *Func_02004ebe(s32);
u8 *Func_02004f1e(s32);
u8 *Func_02004f7e(s32);
s32 Func_020050fe(void);
s32 Func_02004f98(void);
void Func_0200513c(void);
u8 *Func_02007472(s32 actor);
u8 *Func_0200749e(s32 actor);
u8 *Func_020074a6(s32 actor);
void Func_0200957e(void);
void Func_02004e3e_a();
void Func_02004e50();
void Func_02004e90();
void Func_02004ea8();
void Func_02004ea8_a();
s32 Func_020055d6();
s32 Func_020056b8();
void Func_02001b36();
void Func_020051f2();
void Func_02005222();
void Func_02005232();
void Func_0200525c();
void Func_0200526c();
void Func_02005290();
void Func_020052c6();
void Func_020052d0();
void Func_020052fe();
void Func_02005346();
void Func_02005cca();
u8 *Func_02005b1c();
void Func_020067dc();
void Func_020068d0();
void *Func_020067d0();
u8 *Func_020051d6(s32);
struct FieldActor *Func_0200533e(s32);
void Func_02005418(s32);
struct FieldActor *Func_02007926();
struct FieldActor *Func_0200792e();
struct FieldActor *Func_02007936();
s32 Func_02007858();
s32 Func_0200786c();
void Func_02006290();
void Func_020062bc();
void Func_020062ec();
void Func_020062f0();
void Func_020062fe();
void Func_0200631e();
void Func_02006348();
void Func_0200634e();
void Func_02006352();
void Func_02006378();
void Func_02006388();
void Func_020063b8();
void Func_020063fa();
void Func_02006838();
s32 Func_02006924();
s32 Func_0200692c();
void Func_02006498();
void Func_020064bc();
void Func_020064d2();
u8 *Func_02006c92();
s32 Func_02006c9e();
void Func_02006ca4();
void Func_0200731c();
void Func_02007324();
void Func_02007378();
void Func_0200738a();
void Func_0200739a();
void Func_020073b8();
s32 Func_02007a1a();
s32 Func_02007a26();
s32 Func_02007a30_a();
s32 Func_02007a3a();
s32 Func_02007a46();
s32 Func_02007a52();
void Func_020076ba();
void Func_0200770c();
void Func_02007726();
void Func_020077a2();
void Func_020077e2();
void Func_020077e6();
void Func_020077f6();
void Func_02007824();
void Func_02007848();
void Func_02007860();
void Func_02007870();
void Func_02007870_a();
void Func_02007890();
void Func_020078b2();
void Func_020078b6();
void Func_020078bc();
void Func_020078d2();
void Func_020078d8();
void Func_020078ec();
void Func_020078f6();
void Func_02007912();
void Func_02007936_a();
void Func_02007950();
void Func_02007960();
void Func_02007980();
void Func_02007984();
void Func_020079ae();
void Func_020079b8();
void Func_020079fe();
void Func_02007a2a();
void Func_02007a2a_a();
void Func_02007a56_a();
void Func_02007aa6();
void Func_02007ab0();
void Func_02007abc();
void Func_02007abe();
void Func_02007b18();
void Func_02007b78();
void Func_02007b90();
void Func_02007b96();
void Func_02007b98_a();
void Func_02007ba0();
void Func_02007bc0();
void Func_02007bd0();
void Func_02007bda();
void Func_02007bf0();
void Func_02007c00();
void Func_02007c04();
void Func_02007c12();
void Func_02007c26();
void Func_02007c3c();
void Func_02007c4a();
void Func_02007c66();
void Func_02007c6a();
void Func_02007c6c();
void Func_02007c7c();
void Func_02007c8e();
void Func_02007cb6();
void Func_02007cbc();
void Func_02007cf0();
s32 Func_02007d56();
s32 Func_02007d8c();
s32 Func_02007d98();
s32 Func_02007da4();
s32 Func_02007dc8_a();
void Func_0200822c();
void Func_02007ea2();
void Func_02007ec2();
void Func_02007f02();
void Func_02007f1c();
void Func_02007fbe();
void Func_02007fc4_a();
void Func_02007fdc();
void Func_02007ffe();
void Func_02008010_a();
void Func_02008016();
void Func_02008020();
void Func_02008026();
void Func_02008044_a();
void Func_02008070_a();
void Func_02008084();
void Func_020080e4();
void Func_020080ee_a();
void Func_02008128();
void Func_020081b8();
void Func_020081de();
void Func_020081e4();
void Func_020081f2();
void Func_0200820a();
void Func_0200821e_a();
void Func_020082b6_b();
void Func_020082f0_a();
void Func_02008300_a();
void Func_02008326();
void Func_0200833a();
void Func_0200834a();
void Func_02008378();
void Func_02008382();
void Func_020083aa_a();
void Func_020083b0();
void Func_020083bc();
void Func_020083d6();
void Func_020083ea();
void Func_020083fa_a();
void Func_02008418();
void Func_02008422();
void Func_02008430();
void Func_0200843a();
void Func_02008442();
void Func_0200847c();
u8 *Func_02008492();
void Func_02008494();
void Func_020084b6();
void Func_020084bc();
void Func_020084e0();
void Func_020084ea();
void Func_020084f0();
void Func_02008516();
u8 *Func_02008526();
void Func_0200852a();
void Func_0200852c_a();
u8 *Func_02008532();
u8 *Func_0200853e();
void Func_02008562();
void Func_02008592();
void Func_020085a2_a();
void Func_020085c2();
void Func_020085d2_a();
void Func_020085e6();
void Func_020085ea();
void Func_020085fc();
void Func_0200860e_a();
void Func_02008614();
void Func_02008620();
void Func_02008648();
void Func_0200864e();
void Func_020086fe();
void Func_02008734();
void Func_02008738();
void Func_0200876e();
void Func_0200879c();
void Func_020087a6();
void Func_020087a8();
void Func_020087ea();
void Func_020087f6_a();
void Func_020087f8();
void Func_020087f8_a();
void Func_0200880c_a();
void Func_02008814_a();
void Func_02008830();
void Func_0200885c();
void Func_02008866();
void Func_02008870_a();
void Func_0200888c();
void Func_0200889c();
void Func_020088bc();
void Func_020088cc();
void Func_020088ce();
void Func_020088fc();
void Func_020088fe();
void Func_0200890e();
void Func_02008910();
void Func_02008950();
void Func_02008978();
s32 Func_020089a6();
void Func_02008a30();
void Func_02008a66_a();
void Func_02008a68_a();
void Func_02008aee();
void Func_02008afc();
void Func_02008b16();
void Func_02008b4e();
s32 Func_02008e26();
void Func_02008e2e();
void Func_0200920a();
s32 Func_02009278();
s32 Func_0200929c();
void Func_020098a8();
void Func_02009a54();
u8 *Func_02007c8e_a(s32);
void Func_02007eb8(void);
u8 *Func_02007ca6(s32);
void Func_02007ce6(s32, s32, s32, s32);
void Func_02007cd2(s32);
s32 Func_02007cd8(s32);
u8 *Func_02009356(s32);
u8 *Func_02009370(s32, s32);
void Func_02009418_a(s32, s32, s32);
void Func_0200942e(s32);
s32 Func_02009062(s32, s32, s32);
s32 Func_02009030_a(s32, s32, s32);
s32 Func_0200903e_a(s32, s32, s32);
s32 Func_0200904c(s32, s32, s32);
void Func_0200958c(s32, s32, s32, s32);

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

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
void SceneActor_SetModeZeroAndValue(s32 a, s32 b);

void FieldScene_RunSplitTripleSteps(s32 a, s32 b, s32 c);

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* Moves the next dialogue line on by amount messages. */
static __inline__ void bump_step(s32 amount)
{
    gEventWork->message += amount;
}

static __inline__ s32 Value0(s32 (*f)())
{
    return f();
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

static __inline__ void Call0(void (*f)())
{
    f();
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ s32 Value0_02001a4c(s32 (*f)())
{
    void *Func_02005ae2();
    void *Func_020067d8();

    return f();
}

static __inline__ void Call1_02001a4c(void (*f)(), s32 a0)
{
    void *Func_02005ae2();
    void *Func_020067d8();

    f(a0);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    void *Func_02005ae2();
    void *Func_020067d8();

    f(a0, a1, a2, a3, a4, a5);
}

/* The "shown" half word at +100 of an actor record. */

/* Phase/status word at 0x1c0 of the shared scene work record. */

static __inline__ void Call6_02001ba0(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step_020036f8(s32 off, s32 amount)
{
    extern u8 Data_03001ebc[];

    u8 *work = *(u8 **)Data_03001ebc;
    u16 *slot = (u16 *)((s32)work + off);
    s32 next = *slot + amount;

    *slot = next;
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}
s32 OverlayObject_GetObjectTwoByte118(void);
s32 OverlayObject_RunObjectTwoWhenFlagged(void);

void RunDialoguePromptScene(void)
{
    extern u8 Data_03001ebc[];

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

    p7 = *(volatile s32 *)Data_03001ebc;
    GameFlag_Set(0x855);
    Event_Begin();
    {
        u8 *record = Func_02008492(12);
        u8 value = *(volatile u8 *)&record[35];

        record[35] = (u8)(value | 1);
    }
    Actor_MoveToAndWait(15, 0x368, 0x1a9);
    Actor_MoveToAndWait(16, 0x368, 0x199);
    Actor_MoveToAndWait(17, 0x368, 0x179);
    Actor_SetPosition(11, 0x3080000, 0x1880000);
    Actor_SetPosition(10, 0x3180000, 0x1880000);
    Actor_SetPosition(12, 0x3280000, 0x1880000);
    Actor_SetAnimation(10, 5);
    Actor_SetAnimation(11, 5);
    Actor_SetAnimation(12, 5);
    Actor_FaceActor(11, ACTOR_PARTY_LEADER, 0);
    Actor_FaceActor(10, ACTOR_PARTY_LEADER, 0);
    Actor_FaceActor(12, ACTOR_PARTY_LEADER, 0);
    record = Func_02008526(10);
    Actor_SetSpriteFlags((s32)record, 1);
    record = Func_02008532(11);
    Actor_SetSpriteFlags((s32)record, 1);
    record = Func_0200853e(12);
    Actor_SetSpriteFlags((s32)record, 1);
    Actor_SetPosition(13, 0x3000000, 0x1980000);
    Actor_SetPosition(14, 0x3000000, 0x1a80000);
    Actor_MoveToAndWait(9, 0x310, 0x1a8);
    Actor_SetPosition(8, 0x3280000, 0x1980000);
    Actor_FaceActor(13, 9, 0);
    Actor_FaceActor(8, 9, 0);
    Actor_FaceActor(14, 10, 0);
    Actor_FaceActor(9, 10, 0);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0x3180000, 0x1b80000);
    Actor_SetPosition(ACTOR_GERALD, 0x3280000, 0x1b80000);
    Actor_SetPosition(ACTOR_IVAN, 0x3080000, 0x1b80000);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 10, 0);
    Actor_FaceActor(ACTOR_GERALD, 10, 0);
    Actor_FaceActor(ACTOR_IVAN, 10, 0);
    work = *(u8 *volatile *)Data_03001ebc;
    off1c8 = 0x1c8;
    *(volatile s32 *)((s32)work + off1c8) = 30;
    *(volatile s32 *)(((s32)work + 0x1c0)) = 0x201;
    Event_OpenScreen();
    Event_WaitForScreen();
    Event_Wait(40);
    Actor_RunRepeatedMotion(10, 2);
    base6_12fc = (s32)LinkedMessage_YouRobinRightWontForget;
    Event_SetMessage(base6_12fc);
    Func_02007ea2(10, 20);
    Actor_RunRepeatedMotion(9, 1);
    Event_Wait(20);
    Func_02007f02(9, 4, 20);
    Func_02007ec2(9, 20);
    Actor_SetAnimation(13, 3);
    Func_02007f1c(8, 3, 20);
    Actor_StartRepeatedMotion(11, 2);
    Actor_StartRepeatedMotion(12, 2);
    Event_Wait(60);
    Actor_SetSpeed(13, 0xcccc, 0x6666);
    Actor_WalkTo(13, 0x2ea, 0x198);
    Actor_FaceDirection(9, 0xb000, 0);
    Actor_FaceDirection(14, 0xb000, 0);
    base5_200d354 = (s32)Data_0200d354;
    Actor_EnableActionCallback(11, base5_200d354);
    Event_Wait(20);
    Actor_EnableActionCallback(10, base5_200d354);
    Event_Wait(15);
    Actor_EnableActionCallback(12, base5_200d354);
    Event_Wait(35);
    Actor_EnableActionCallback(8, 0x200d2fc);
    Event_Wait(20);
    Actor_WaitForMove(13);
    Actor_SetPosition(13, 0, 0);
    Event_Wait(40);
    Actor_SetSpeed(9, 0xcccc, 0x6666);
    Actor_SetSpeed(14, 0xcccc, 0x6666);
    Actor_WalkToAndWait(9, 0x310, 0x198);
    Actor_FaceActor(9, ACTOR_PARTY_LEADER, 0);
    Actor_WalkToAndWait(14, 0x300, 0x198);
    Func_02007fc4_a(14, 0, 20);
    Func_02007ffe(9, 3, 20);
    Func_02007fbe(9, 20);
    Func_02008010_a(14, 3, 20);
    Actor_FaceDirection(14, 0x2000, 10);
    Func_02007fdc(14, 20);
    Func_02008016(0, 1, 50);
    Func_02008020(0, 2, 50);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 9, 0);
    Actor_FaceActor(ACTOR_GERALD, 9, 0);
    Func_02008026(2, 9, 20);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimation(ACTOR_GERALD, 3);
    Func_02008070_a(2, 3, 50);
    Actor_FaceEachOther(9, 14, 0);
    Func_02008084(9, 3, 20);
    Func_02008044_a(9, 20);
    Actor_EnableActionCallback(14, 0x200d3ac);
    Event_Wait(50);
    Value2(Engine_ActorEnableActionCallback, 9, 0x200d444);
    Actor_SetSpeed(ACTOR_GERALD, 0xcccc, 0x6666);
    Actor_SetSpeed(ACTOR_IVAN, 0xcccc, 0x6666);
    Actor_WalkToAndWait(ACTOR_GERALD, 0x318, off1c8);
    Actor_FaceDirection(ACTOR_GERALD, 0xd000, 0);
    Event_Wait(30);
    Actor_WalkToAndWait(ACTOR_IVAN, 0x318, 0x198);
    Actor_FaceDirection(ACTOR_IVAN, 0, 0);
    Actor_WalkToAndWait(ACTOR_GERALD, 0x328, off1c8);
    Actor_FaceDirection(ACTOR_GERALD, 0xd000, 0);
    Event_Wait(100);
    Func_020080e4(14, 9, 60);
    Func_020080ee_a(9, 14, 40);
    Func_02008128(9, 3, 40);
    Actor_FaceDirection(9, 0, 0);
    Event_Wait(20);
    Actor_RunRepeatedMotion(9, 2);
    Audio_PlayCue(124);
    Actor_SetAnimation(15, 4);
    Actor_SetPosition(18, 0x3680000, 0x1a80000);
    Actor_SetSpritePriority(18, 1);
    Actor_SetSpeed(18, 0xcccc, 0x6666);
    Actor_SetDestinationOffset(18, 0, -8);
    Actor_WaitForMove(18);
    Actor_RunRepeatedMotion(18, 2);
    Event_Wait(60);
    Message_ShowCentered((base6_12fc + 5), 1);
    Actor_SetAnimation(15, 2);
    Actor_SetPosition(18, 0, 0);
    {
        u8 *work0 = *(u8 **)Data_03001ebc;
        u16 *slot0;
        s32 next0;

        off1d8 = 0x1d8;
        slot0 = (u16 *)((s32)work0 + off1d8);
        next0 = *slot0 + 1;
        *slot0 = next0;
    }
    Actor_RunRepeatedMotion(14, 1);
    Func_020081b8(14, 20);
    Func_020081f2(0, 1, 40);
    Func_020081e4(9, 14, 20);
    Func_0200821e_a(9, 3, 20);
    Func_020081de(9, 30);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 14, 0);
    Func_0200820a(1, 14, 40);
    Actor_FaceDirection(14, 0, 0);
    Event_Wait(40);
    Actor_RunRepeatedMotion(14, 2);
    Audio_PlayCue(124);
    Actor_SetAnimation(16, 4);
    {
        u8 *rec;
        s32 t = 0;
        u16 zero_sym = (u16)(t + t);

        rec = (u8 *)Func_020089a6(19);
        rec[85] = zero_sym;
    }
    Actor_SetSpritePriority(19, 1);
    Actor_SetPosition(19, 0x3680000, 0x1980000);
    Actor_SetSpeed(19, 0xcccc, 0x6666);
    Actor_SetDestinationOffset(19, 0, -8);
    Actor_WaitForMove(19);
    Actor_RunRepeatedMotion(19, 2);
    Event_Wait(60);
    Message_ShowCentered((base6_12fc + 8), 1);
    Actor_SetAnimation(16, 2);
    Actor_SetPosition(19, 0, 0);
    bump_step_020036f8(off1d8, 1);
    Actor_ShowEmote(9, 0x102, 0);
    Event_Wait(60);
    Func_020082b6_b(9, 20);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Actor_StartRepeatedMotion(ACTOR_GERALD, 2);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
    Event_Wait(20);
    Func_02008326(14, 3, 50);
    Func_02008300_a(9, 0, 20);
    Func_020082f0_a(9, 30);
    Actor_FaceDirection(14, 0xd000, 0);
    Event_Wait(30);
    Actor_ShowEmote(14, 0x100, 0);
    Event_Wait(60);
    Actor_WalkToAndWait(14, 0x358, 0x178);
    Event_Wait(20);
    Func_0200834a(14, 9, 20);
    Func_0200833a(14, 20);
    Actor_FaceActor(9, 14, 0);
    Actor_ShowEmote(ACTOR_IVAN, 0x100, 0);
    Event_Wait(60);
    Func_02008378(2, 14, 30);
    Func_02008382(9, 2, 20);
    Func_020083bc(9, 3, 20);
    Event_ShowMessage(9, 0);
    Actor_FaceDirection(14, 0x5000, 0);
    Event_Wait(30);
    Func_020083b0(2, 9, 20);
    Func_020083ea(2, 3, 20);
    Func_020083aa_a(9, 20);
    Actor_FaceEachOther(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
    Func_020083d6(1, 2, 20);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Func_02008418(1, 3, 40);
    Func_02008422(2, 3, 30);
    Actor_RunRepeatedMotion(9, 1);
    Event_Wait(20);
    Func_0200843a(9, 4, 20);
    Func_020083fa_a(9, 20);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 9, 0);
    Actor_FaceActor(ACTOR_GERALD, 9, 0);
    Func_02008430(2, 9, 20);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    Actor_StartRepeatedMotion(ACTOR_GERALD, 1);
    Actor_ShowEmote(ACTOR_IVAN, 0x102, 0);
    Event_Wait(60);
    Func_02008442(2, 40);
    Func_02008494(9, 3, 20);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 4);
    Actor_SetAnimationAndWait(ACTOR_GERALD, 4);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 3);
    Event_Wait(30);
    Func_020084bc(9, 4, 20);
    Func_0200847c(9, 20);
    Func_020084b6(0, 1, 20);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
    Func_020084e0(2, 4, 30);
    Actor_SetSpeed(ACTOR_IVAN, 0x18000, 0xc000);
    Actor_WalkToAndWait(ACTOR_IVAN, 0x320, 0x198);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
    Event_ShowMessage(ACTOR_IVAN, 0);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 9, 0);
    Func_020084f0(1, 9, 30);
    Func_0200852a(9, 4, 20);
    Func_020084ea(9, 20);
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
    Actor_SetAttachedEffect(ACTOR_GERALD, 0x102);
    Actor_SetAttachedEffect(ACTOR_IVAN, 0x102);
    Event_Wait(60);
    Func_02008516(2, 20);
    Actor_RunRepeatedMotion(9, 1);
    Event_Wait(20);
    Func_0200852c_a(9, 20);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x101, 0);
    Actor_ShowEmote(ACTOR_IVAN, 0x101, 0);
    Event_Wait(60);
    Func_020085a2_a(9, 4, 20);
    Func_02008562(9, 20);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    Actor_StartRepeatedMotion(ACTOR_GERALD, 1);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
    Event_Wait(20);
    Func_020085d2_a(9, 3, 20);
    Func_02008592(9, 40);
    Actor_ShowEmote(ACTOR_IVAN, 0x105, 0);
    Event_Wait(60);
    Actor_WalkToAndWait(9, 0x348, 0x1a8);
    Func_020085ea(9, 0, 20);
    Func_020085c2(9, 20);
    Func_02008614(0, 3, 20);
    Actor_FaceDirection(9, 0x5000, 0);
    Event_Wait(20);
    Func_020085e6(9, 20);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 1);
    Event_Wait(20);
    Func_020085fc(1, 20);
    Func_0200864e(9, 3, 20);
    Func_0200860e_a(9, 30);
    Func_02008648(9, 14, 20);
    Func_02008620(9, 20);
    Actor_WalkToAndWait(14, 0x358, 0x198);
    base5_200d4c8 = (s32)Data_0200d4c8;
    Actor_EnableActionCallback(9, base5_200d4c8);
    Actor_EnableActionCallback(14, base5_200d4c8);
    Actor_WalkTo(ACTOR_GERALD, 0x318, off1c8);
    Actor_SetSpeed(ACTOR_IVAN, 0xcccc, 0x6666);
    Actor_WalkTo(ACTOR_IVAN, 0x308, 0x1b0);
    Actor_WaitForMove(ACTOR_GERALD);
    Actor_FaceDirection(ACTOR_GERALD, 0xd000, 0);
    Actor_WaitForMove(ACTOR_IVAN);
    Actor_FaceDirection(ACTOR_IVAN, 0xd000, 0);
    Func_02008e2e(9);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 9, 0);
    rec8 = Value1(Func_02008e26, 14);
    {
        u8 *target = rec8 + 91;
        s32 shown = 1;

        *target = shown;
    }
    *(s32 *)((s32)rec8 + 56) = -0x80000000;
    *(s32 *)((s32)rec8 + 60) = -0x80000000;
    *(s32 *)((s32)rec8 + 64) = -0x80000000;
    Actor_ShowEmote(9, 0x100, 0);
    Actor_SetAnimation(14, 1);
    Event_Wait(50);
    Actor_FaceActor(9, ACTOR_PARTY_LEADER, 0);
    Actor_WalkToAndWait(ACTOR_GERALD, 0x328, 0x1b8);
    Actor_FaceDirection(ACTOR_GERALD, 0xb000, 0);
    Event_Wait(20);
    Func_020086fe(9, 20);
    Func_02008738(0, 1, 50);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 9, 0);
    Func_02008734(1, 9, 20);
    Func_0200876e(9, 3, 20);
    Actor_WalkTo(9, 0x2e8, 0x198);
    Actor_WalkTo(14, 0x2e8, 0x198);
    Actor_WaitForMove(9);
    Actor_WaitForMove(14);
    Event_Wait(30);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x318, 0x198);
    Func_0200879c(0, 1, 30);
    Actor_ShowEmote(ACTOR_IVAN, 0x105, 0);
    Event_Wait(50);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
    Func_020087a8(1, 2, 20);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 1);
    Event_Wait(20);
    Func_020087a6(1, 40);
    Func_020087f8(2, 3, 30);
    Func_020087ea(0, 1, 40);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 4);
    Func_02008814_a(1, 4, 30);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
    Func_020087f8_a(1, 2, 20);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 1);
    Event_Wait(20);
    Func_020087f6_a(1, 40);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
    Event_Wait(20);
    Func_0200880c_a(2, 20);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Func_02008866(1, 3, 20);
    Func_02008870_a(2, 4, 20);
    Func_02008830(2, 20);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x101, 0);
    Event_Wait(60);
    Func_0200889c(2, 3, 20);
    Func_0200885c(2, 20);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x100, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x100, 0);
    Event_Wait(60);
    Func_020088cc(2, 3, 20);
    Func_0200888c(2, 20);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x102, 0);
    Event_Wait(60);
    Func_020088fc(2, 4, 20);
    Func_020088bc(2, 20);
    Func_0200890e(1, 3, 20);
    Func_020088ce(1, 30);
    Event_OpenMessage(ACTOR_IVAN, 0);
    if (Event_ChooseYesNo(0, 0) != 0) {
        Event_Wait(20);
        Actor_RunRepeatedMotion(ACTOR_GERALD, 1);
        Event_Wait(20);
        Func_020088fe(1, 20);
        Func_02008950(2, 4, 20);
        Func_02008910(2, 20);
    }
    Func_02008978(2, 3, 30);
    Actor_WalkToAndWait(ACTOR_IVAN, 0x320, 0x1c8);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
    Actor_FaceActor(ACTOR_GERALD, ACTOR_IVAN, 0);
    Actor_WalkToAndWait(ACTOR_IVAN, 0x358, 0x1c8);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
    Actor_FaceActor(ACTOR_GERALD, ACTOR_IVAN, 0);
    Actor_WalkToAndWait(ACTOR_IVAN, 0x358, 0x178);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
    Actor_FaceActor(ACTOR_GERALD, ACTOR_IVAN, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0, 0);
    Event_Wait(20);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
    rec8 = Value3(Func_02009278, 2, 17, 65);
    Event_Wait(60);
    base5_1324 = (s32)LinkedMessage_IvanGotShamansRod;
    Message_ShowCentered(base5_1324, 1);
    Engine_ObjectDispatchRelease((s32)rec8);
    Actor_SetAnimation(17, 2);
    Event_Wait(20);
    Func_02008a30(2, 3, 20);
    Actor_WalkToAndWait(ACTOR_IVAN, 0x358, 0x1c8);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
    Actor_FaceActor(ACTOR_GERALD, ACTOR_IVAN, 0);
    Actor_WalkToAndWait(ACTOR_IVAN, 0x320, 0x1c8);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
    Actor_FaceActor(ACTOR_GERALD, ACTOR_IVAN, 0);
    Actor_WalkToAndWait(ACTOR_IVAN, 0x308, 0x1a8);
    Actor_FaceEachOther(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
    Func_02008a66_a(1, 2, 30);
    SceneEffect_ApplyThreeValuesAndFinish(2, 3, 20);
    Event_SetMessage((base5_1324 + 1));
    Func_02008a68_a(2, 20);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    {
        u16 *slot = (u16 *)((s32)p7 + 0x1d8);
        s32 saved = *(s16 *)slot;

        if (Value0(OverlayObject_GetObjectTwoByte118)!= 0) {
            Event_SetMessage(MSG_WAIT_DONT_WANT_TAKE_YOUR);
            Event_ShowMessage(ACTOR_IVAN, 0);
            OverlayObject_RunObjectTwoWhenFlagged();
        }
        Func_0200920a(2);
        *slot = saved;
    }
    Func_02008aee(1, 3, 50);
    Actor_WalkToAndWait(ACTOR_IVAN, 0x308, 0x198);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
    Actor_FaceActor(ACTOR_GERALD, ACTOR_IVAN, 0);
    Actor_WalkToAndWait(ACTOR_IVAN, 0x2e8, 0x198);
    Event_Wait(40);
    Func_02008b16(0, 1, 20);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 1);
    Event_Wait(20);
    Func_02008afc(1, 20);
    Func_02008b4e(0, 3, 20);
    Actor_SetAnimation(ACTOR_GERALD, 2);
    record = Value1(Func_0200929c, 0);
    if ((s32)record != 0) {
        Actor_SetDestination(ACTOR_GERALD, *(s16 *)((s32)record + 10), *(s16 *)((s32)record + 18));
    }
    Actor_WaitForMove(ACTOR_GERALD);
    Actor_SetPosition(ACTOR_GERALD, 0, 0);
    Event_Wait(30);
    Actor_SetPosition(8, 0, 0);
    Actor_SetPosition(9, 0, 0);
    Actor_SetPosition(13, 0, 0);
    Actor_SetPosition(14, 0, 0);
    Actor_SetPosition(10, 0, 0);
    Actor_SetPosition(11, 0, 0);
    Actor_SetPosition(12, 0, 0);
    Actor_SetPosition(ACTOR_IVAN, 0, 0);
    *(s32 *)((*(u8 *volatile *)Data_03001ebc + 0x1c0)) = 0x209;
    Event_End();
}
void SceneEffect_ApplyThreeValuesAndFinish(s32 a, s32 b, s32 c);
