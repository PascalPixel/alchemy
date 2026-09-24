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

extern s16 Data_02000240[];
extern u8 Data_0200dcc8[];
extern u8 Data_0200dab8[];
extern u8 LinkedMessage_MasterHammetIsntOnlyOne;
extern u8 Data_0200e1fc[];
extern u8 Data_0200e250[];
extern u8 Data_0200de30[];
extern u8 Data_0200cf2c[];
extern u8 LinkedMessage_YouWereSuchGreatHelp[];
extern u8 Value_00000854;
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
struct Object *Func_02004e84(s32);
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
T *Func_0200533e(s32);
void Func_02005418(s32);
struct Rec_383 *Func_02007926();
struct Rec_383 *Func_0200792e();
struct Rec_383 *Func_02007936();
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

static __inline__ void Call1_0200082c(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call1_0200091c(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

static __inline__ void Call1_020019a4(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call0(void (*f)())
{
    f();
}

static __inline__ void Call1_020019e4(void (*f)(), s32 a0)
{
    f(a0);
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

static __inline__ void Call1_02001ba0(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call6_02001ba0(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

static __inline__ s32 Value1_02001e80(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call1_020036f8(void (*f)(), s32 a0)
{
    f(a0);
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

/*
 * Head export stub at 0x02000030. The eight-byte owner includes its one pool
 * word at 0x02000034, which the load reads and which sits past the return.
 * The word is an address returned without being dereferenced, so this is a
 * getter for an in-image table; it names bytes only while this overlay is
 * resident at 0x02000000.
 */
u8 *SceneData_GetTableD8f8(void)
{
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
    return (u8 *)0x0200da80;
}

u8 *SceneData_SelectTableBySceneRange(void)
{
    s16 *tbl = Data_02000240;
    u8 *ret;
    s32 v = tbl[225];
    s32 low = 15;

    if (v <= 17) {
        if (v >= low) {
            ret = Data_0200dcc8;
        } else {
            ret = Data_0200dab8;
        }
    } else {
        ret = Data_0200dab8;
    }
    Func_02004dbc(ret);
    return ret;
}

void ActorPresentation_SetSceneCellByAngle(void)
{
    s32 x;
    s32 z;

    if (*(u16 *)(Func_02004e04(0) + 6) >= 0xa000
        && *(u16 *)(Func_02004e14(0) + 6) <= 0xe000) {
        Leader_CheckAhead();
        x = 42;
        z = 85;
        Map_CopyCellAttributes(41, 85, 1, 1, x, z);
    } else if (*(u16 *)(Func_02004e3e(0) + 6) >= 0x2000
               && *(u16 *)(Func_02004e4e(0) + 6) <= 0x6000) {
        Leader_CheckAhead();
        x = 42;
        z = 85;
        Map_CopyCellAttributes(43, 85, 1, 1, x, z);
    }
}

void FieldScene_RunObjectTwentySixPositionCheck(void)
{
    struct Object *obj;
    s32 x;
    s32 z;

    Event_Begin();
    obj = Func_02004e84(26);
    if ((obj->x >> 20) == 42) {
        x = 41;
        z = 24;
        Map_CopyCellAttributes(101, 24, 3, 4, x, z);
        GameFlag_Set(0x859);
    }
    Event_End();
}

void FieldScene_RunActorNineteenAngleDialogue(void)
{
    s32 v = *(u16 *)(Func_02004ebe(0) + 6);

    Event_Begin();
    if (v >= 0xa001 && v <= 0xdfff) {
        Shop_Open(4, 19);
    } else {
        if (GameFlag_IsSet(0x855) == 0) {
            Event_SetMessage((s32)&LinkedMessage_MasterHammetIsntOnlyOne);
        } else {
            Event_SetMessage(MSG_WE_FOUND_OUR_STOLEN_WEAPONS);
        }
        Event_ShowMessage(19, 0);
    }
    Event_End();
}

void FieldScene_RunActorTwentyAngleDialogue(void)
{
    s32 v = *(u16 *)(Func_02004f1e(0) + 6);

    Event_Begin();
    if (v >= 0xa001 && v <= 0xdfff) {
        Shop_Open(5, 20);
    } else {
        if (GameFlag_IsSet(0x855) == 0) {
            Event_SetMessage(MSG_THIEVES_DIDNT_HIT_OUR_HOUSE);
        } else {
            Event_SetMessage(MSG_IF_YOURE_GONNA_HEAD_INTO);
        }
        Event_ShowMessage(20, 0);
    }
    Event_End();
}

void FieldScene_RunActorTwentyThreeAngleDialogue(void)
{
    s32 v = *(u16 *)(Func_02004f7e(0) + 6);

    Event_Begin();
    if (v >= 0xa001 && v <= 0xdfff) {
        Inn_Open(1, 23);
    } else {
        if (GameFlag_IsSet(0x855) == 0) {
            Event_SetMessage(MSG_EVERYONE_THINKS_OUR_GUESTS_THIEVES);
        } else {
            Event_SetMessage(MSG_THEY_HID_THOSE_STOLEN_GOODS);
        }
        Event_ShowMessage(23, 0);
    }
    Event_End();
}

void FieldScene_RunActorEighteenConditionalScene(void)
{
    Event_Begin();
    if (Func_02004f98() == 0) {
        Actor_SetAnimationAndWait(18, 4);
        Event_Wait(20);
        Event_SetMessage(MSG_WOW_HAVE_MANY_THINGS_ARENT);
        Event_ShowMessage(18, 0);
    } else {
        Item_ShowFound(ITEM_BONE, 3);
        Party_GiveItem(ITEM_BONE, 0);
    }
    Event_End();
}

void SceneDialogue_ShowLine12BB(void)
{
    Func_0200513c();
    Event_SetMessage(MSG_WE_DONT_HAVE_TIME_FOR);
    Event_ShowMessage(ACTOR_GERALD, 0);
}

void SceneState_SetFlags92bAnd94b(void)
{
    Event_Begin();
    Message_ShowCentered(MSG_ROBIN_CHECKED_BARREL, 1);
    Message_ShowCentered(MSG_BUT_DIDNT_FIND_ANYTHING, 1);
    Event_End();
}

void SceneState_SetFlags929And949(void)
{
    Event_Begin();
    Message_ShowCentered(MSG_ROBIN_CHECKED_CHEST, 1);
    Message_ShowCentered(MSG_BUT_CHEST_WAS_EMPTY, 1);
    Event_End();
}

u8 *SceneData_SelectTableE1fcBySceneRangeOrFlag855(void)
{
    s16 *tbl = Data_02000240;
    s32 v = tbl[225];
    s32 lo = 15;

    if (v <= 17) {
        if (v >= lo) {
            return Data_0200e1fc;
        }
    }
    if (GameFlag_IsSet(0x855) != 0) {
        return Data_0200e250;
    }
    return Data_0200de30;
}

void ActorPresentation_RunActorModeOneThenZeroWithStep(s32 x)
{
    Actor_SetAnimation(x, 1);
    SceneActor_SetPairZeroAndValue(x, 0, 2);
    Event_ShowMessage(x, 0);
}

void SceneState_RunGuardedActorStep(s32 x)
{
    u8 *flag = (u8 *)Func_020050fe() + 91;
    s32 zero = 0;

    *flag = 1;
    Event_Begin();
    Actor_SetAnimation(x, 1);
    Event_Wait(2);
    Event_ShowMessage(x, 0);
    Event_End();
    *flag = zero;
}

void SceneDialogue_PromptAndCountSkip(s32 x)
{
    SceneActor_SetPairZeroAndValue(x, 0, 2);
    Event_OpenMessage(x, 0);
    if (Event_ChooseYesNo(0, 0) != 0) {
        gEventWork->message += 1;
    }
    Event_ShowMessage(x, 0);
}

void SceneDialogue_RunActorElevenDialogue(void)
{
    Event_Begin();
    Event_SetMessage(MSG_MISTER_FUN_SEE_STRANGE_NEW);
    Actor_SetAnimation(11, 1);
    SceneDialogue_PromptAndCountSkip(11);
    Event_End();
}

void FieldScene_RunScene383_02000428(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    Event_SetMessage(MSG_IVAN_HAS_GREAT_POWERS_WOULDNT);
    SceneDialogue_PromptAndCountSkip(15);
    Actor_FaceDirection(15, 0x8000, 0);
    Event_End();
}

void SceneState_BranchOnSlotZeroFacingAndFlag855(void)
{
    s32 value = *(u16 *)(Func_020051d6(0) + 6);

    Event_Begin();
    if (value >= 0xa001 && value <= 0xdfff) {
        Shop_Open(6, 21);
    } else if (GameFlag_IsSet(0x855) == 0) {
        Event_SetMessage(MSG_DID_JUST_ARRIVE_IN_TOWN);
        SceneDialogue_PromptAndCountSkip(21);
    } else {
        Event_SetMessage(MSG_WITH_BRIDGE_OUT_WILL_QUITE);
        Event_ShowMessage(21, 0);
    }
    Event_End();
}

void SceneDialogue_RunActor9FlaggedLine(void)
{
    Event_Begin();
    if (GameFlag_IsSet(0x855) == 0) {
        Event_SetMessage(MSG_IF_ONLY_THESE_ROCKS_WERE);
    } else {
        Event_SetMessage(MSG_THANK_GOODNESS_THOSE_THIEVES_WERE);
    }
    ActorPresentation_RunActorModeOneThenZeroWithStep(9);
    Event_End();
}

void SceneDialogue_RunActorTwelveFlaggedDialogue(void)
{
    Event_Begin();
    if (GameFlag_IsSet(0x855) != 0) {
        Event_SetMessage(MSG_THIEVES_HID_STOLEN_TREASURE_IN);
    } else {
        Event_SetMessage(MSG_JUST_ME_OR_AM_MISSING);
    }
    ActorPresentation_RunActorModeOneThenZeroWithStep(12);
    Event_End();
}

void FieldScene_RunFlag856DialogueBranch(void)
{
    s32 g;
    g = 0x851;
    Event_Begin();
    if (GameFlag_IsSet(0x856) != 0) {
        if (GameFlag_IsSet(g) == 0) {
            Event_SetMessage(MSG_YOURE_GOING_HELP_IVAN);
            ActorPresentation_RunActorModeOneThenZeroWithStep(16);
            Event_Wait(10);
            SceneEffect_ApplyThreeValuesAndFinish(16, 3, 20);
            GameFlag_Set(g);
        } else {
            Event_SetMessage(MSG_PLEASE_LOOK_AFTER_IVAN);
        }
    } else {
        Event_SetMessage(MSG_COULD_SOMEONE_PLEASE_HELP_IVAN);
    }
    ActorPresentation_RunActorModeOneThenZeroWithStep(16);
    Event_End();
}

void SceneDialogue_ShowLine128E(void)
{
    Event_Begin();
    Event_SetMessage(MSG_THOSE_THREE_STRANGERS_SURE_HAVE);
    ActorPresentation_RunActorModeOneThenZeroWithStep(18);
    Event_End();
}

void SceneActor_StepActor24AnimationByFacing(void)
{
    T *p;
    s16 *q;
    s32 v;
    s32 n;

    p = Func_0200533e(24);
    Event_Begin();
    Actor_RunRepeatedMotion(24, 2);
    Event_SetMessage(MSG_OW_STOP);
    Event_ShowMessage(24, 0);
    Actor_SetSpeed(24, 0x40000, 0x20000);
    if ((u32)((p->unk6 & 0xf000) - 0x5000) <= 0x6000) {
        q = (s16 *)((u8 *)p + 100);
        v = *q;
        if (v <= 2) {
            Actor_EnableActionCallback(24, Data_0200e4a8[v]);
            *(u16 *)q = *(u16 *)q + 1;
            goto clamp;
        }
    } else {
        q = (s16 *)((u8 *)p + 100);
        v = *q;
        if (v > 2) {
            Actor_EnableActionCallback(24, Data_0200e4a8[v]);
            *(u16 *)q = *(u16 *)q + 1;
            goto clamp;
        }
    }
    Actor_EnableActionCallback(24, Data_0200e4c0[v]);
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
    Func_02005418(24);
    Event_End();
}

void SceneDialogue_RunActor10Line(void)
{
    Event_Begin();
    Event_SetMessage(MSG_DID_THOSE_THIEVES_COME_FROM);
    SceneDialogue_PromptAndCountSkip(10);
    Event_End();
}

void SceneDialogue_RunActor11Line(void)
{
    Event_Begin();
    Event_SetMessage(MSG_MY_FATHER_WORRIED_ABOUT_THOSE);
    ActorPresentation_RunActorModeOneThenZeroWithStep(11);
    Event_End();
}

void SceneDialogue_RunActor14Line(void)
{
    Event_Begin();
    Event_SetMessage(MSG_HEARD_DEFEATED_THOSE_THIEVES);
    SceneDialogue_PromptAndCountSkip(14);
    Event_End();
}

void FieldScene_RunScene383SequenceC(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    if (GameFlag_IsSet(0x857) == 0) {
        Event_SetMessage((s32)LinkedMessage_YouWereSuchGreatHelp);
        Func_02004e3e_a(16, 20);
        Func_02004e90(16, 3, 20);
        Func_02004e50(16, 30);
        Actor_FaceDirection(16, 0, 0);
        Event_Wait(30);
        Actor_RunRepeatedMotion(16, 2);
        Event_Wait(30);
        Func_02004ea8(0, 16, 20);
        SceneEffect_ApplyThreeValuesAndFinish(16, 3, 20);
        bump_step(1);
        if (Value0(Func_020055d6) == 0) {
            Event_SetMessage(((s32)LinkedMessage_YouWereSuchGreatHelp + 3));
            Func_02004ea8_a(16, 20);
            Event_End();
            goto L_02000906;
        }
        GameFlag_Set(0x857);
        Party_GiveItem(ITEM_WATER_OF_LIFE, 0);
    }
    Event_SetMessage(MSG_HEADING_OUT_BEYOND_GOMA_RANGE);
    Event_OpenMessage(16, 0);
    Event_Wait(20);
    if (Event_ChooseYesNo(0, 0) != 0) {
        bump_step(1);
    }
    Event_ShowMessage(16, 0);
    Event_End();
    L_02000906:;
}

void FieldScene_RunScene383_0200091c(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    SceneActor_SetPairZeroAndValue(18, 0, 2);
    if (GameFlag_IsSet(0x85b) == 0) {
        Event_SetMessage(MSG_HAVE_LOT_LEFTOVER_BONES_FROM);
        Event_OpenMessage(18, 0);
    } else {
        Event_SetMessage(MSG_WANT_MORE_BONES);
        Event_OpenMessage(18, 0);
    }
    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_Wait(20);
        Event_ShowMessage(18, 0);
        Event_Wait(20);
        Actor_RunRepeatedMotion(18, 2);
        Event_Wait(20);
        if (Value0(Func_020056b8) == 0) {
            Actor_SetAnimationAndWait(18, 4);
            Event_Wait(20);
            Event_SetMessage(MSG_WOW_HAVE_MANY_THINGS_ARENT);
            Event_ShowMessage(18, 0);
            goto L_020009ec;
        }
        Item_ShowFound(ITEM_BONE, 3);
        Party_GiveItem(ITEM_BONE, 0);
        GameFlag_Set(0x85b);
    } else {
        bump_step(1);
        Event_Wait(20);
        Actor_SetAnimationAndWait(18, 3);
        Event_Wait(20);
        Event_ShowMessage(18, 0);
    }
    L_020009ec:;
    Actor_FaceDirection(18, 0x4000, 0);
    Event_End();
}

void SceneDialogue_RunActorNineFlaggedDialogue(void)
{
    Event_Begin();
    if (GameFlag_IsSet(0x855) == 0) {
        Event_SetMessage(MSG_OK_MISTER_LET_ME_SEE);
    } else {
        Event_SetMessage(MSG_IF_ROCK_WORTHLESS_MAYBE_THATS);
    }
    SceneState_RunGuardedActorStep(9);
    Event_End();
}

void SceneDialogue_RunActorElevenFlaggedDialogue(void)
{
    Event_Begin();
    if (GameFlag_IsSet(0x855) == 0) {
        Event_SetMessage(MSG_WONDER_OUTSIDE_WORLD_LIKE);
    } else {
        Event_SetMessage(MSG_FATHER_LOOKS_SAD_WORRYING_LIKE);
    }
    SceneState_RunGuardedActorStep(11);
    Event_End();
}

void SceneDialogue_ShowLine124EOr135E(void)
{
    Event_Begin();
    if (GameFlag_IsSet(0x855) == 0) {
        Event_SetMessage(MSG_COULD_THEY_THIEVES_GOOD_DONT);
    } else {
        Event_SetMessage(MSG_GUESS_NOTHING_IN_OUR_HOUSE);
    }
    SceneState_RunGuardedActorStep(12);
    Event_End();
}

void SceneDialogue_RunActor16FlaggedLine(void)
{
    Event_Begin();
    if (GameFlag_IsSet(0x855) == 0) {
        Event_SetMessage(MSG_TICKLES_BEING_TICKLED_BY_BOY);
    } else {
        Event_SetMessage(MSG_CAVE_IN_GOMA_RANGE_DANGEROUS);
    }
    SceneState_RunGuardedActorStep(16);
    Event_End();
}

void SceneDialogue_RunActorEighteenBranchedDialogue(void)
{
    Event_Begin();
    if (GameFlag_IsSet(0x855) == 0) {
        Event_SetMessage(MSG_MASTER_HIS_WIFE_BLINDED_BY);
    } else if (GameFlag_IsSet(0x85b) == 0) {
        Event_SetMessage(MSG_GEE_ALWAYS_GET_HUNGRY_WHEN);
    } else {
        Event_SetMessage(MSG_HE_REALLY_LIKES_BONES_WONDER);
    }
    SceneState_RunGuardedActorStep(18);
    Event_End();
}

/* Configures actors 0, 1, 2 (position, movement, and animation timing), then
 * branches on whether actor 0 is already set up: one path sets up actors 0-2
 * with poses and movement, the other only advances actor 2's animation. Both
 * paths converge to check actor 0's record and, depending on that check,
 * configure either actor 2 or actor 1 from it before the scene finishes. */
void FieldScene_RunSetupSequence(void)
{
    u8 *Func_02005ae2();
    u8 *Func_020067d8();

    u8 *record;

    Event_Begin();
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0xcccc, 0x6666);
    Actor_SetSpeed(ACTOR_GERALD, 0xcccc, 0x6666);
    Actor_SetSpeed(ACTOR_IVAN, 0xcccc, 0x6666);
    Audio_PlayCue(19);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x180, 0x198);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Actor_SetPosition(ACTOR_GERALD, 0x1800000, 0x1980000);
    Actor_WalkToAndWait(ACTOR_GERALD, 0x170, 0x198);
    Actor_FaceDirection(ACTOR_GERALD, 0xd000, 20);
    if (GameFlag_IsSet(0x850) != 0) {
    } else {
        GameFlag_Set(0x850);
        SceneEffect_ApplyPairWithValue141(2, 0);
        Event_Wait(40);
        SceneState_SetValue2ThenFinish();
        Event_SetMessage(MSG_DO_POSSESS_STRANGE_POWERS);
        Audio_PlayCue(60);
        Event_Wait(30);
        Func_02005232(2, 3, 30);
        Func_020051f2(2, 30);
        Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
        Actor_RunRepeatedMotion(ACTOR_GERALD, 1);
        Event_Wait(20);
        SceneEffect_ApplyPairWithValue141(2, 0);
        Event_Wait(40);
        SceneState_SetValue2ThenFinish();
        Func_02005222(2, 30);
        Func_0200525c(0, 1, 50);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
        Actor_FaceDirection(ACTOR_GERALD, 0xd000, 0);
        Event_Wait(20);
        SceneEffect_ApplyPairWithValue141(2, 0);
        Event_Wait(40);
        SceneState_SetValue2ThenFinish();
        Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
        Func_0200526c(2, 50);
        Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
        Func_020052c6(1, 3, 20);
        Func_020052d0(2, 3, 20);
        Func_02005290(2, 40);
        Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
        Event_Wait(30);
        Call3((void (*)())Engine_ActorFaceDirection, 2, 0xc000, 0);
        Event_Wait(30);
        Actor_WalkToAndWait(ACTOR_IVAN, 0x178, 0x178);
        Event_Wait(40);
        Func_020052fe(0, 1, 50);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
        Actor_FaceDirection(ACTOR_GERALD, 0xd000, 0);
        Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
        Event_Wait(50);
        Func_02005346(2, 3, 30);
        Actor_FaceDirection(ACTOR_IVAN, 0x4000, 0);
        Event_Wait(10);
        Event_ShowMessage(ACTOR_IVAN, 0);
        Event_OpenMessage(ACTOR_IVAN, 0);
        goto L_join_setup_paths;
    }
    Audio_PlayCue(60);
    Event_SetMessage(MSG_WOULD_REALLY_WOULD_HELP_ME);
    Event_OpenMessage(ACTOR_IVAN, 0);
    L_join_setup_paths:;
    if (Event_ChooseYesNo(0, 0) == 0) {
        Func_02001b36();
        GameFlag_Set(0x856);
        Actor_SetAnimation(ACTOR_IVAN, 2);
        record = Scene_GetRecord_1(0);
        if (record != 0) {
            /* Read the two s16 fields at +10 and +18 of the record. */
            Actor_SetDestination(ACTOR_IVAN, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Actor_WaitForMove(ACTOR_IVAN);
        Actor_SetPosition(ACTOR_IVAN, 0, 0);
    } else {
        Event_ShowMessage(ACTOR_IVAN, 0);
    }
    Actor_SetAnimation(ACTOR_GERALD, 2);
    record = Scene_GetRecord_2(0);
    if (record != 0) {
        /* Read the two s16 fields at +10 and +18 of the record. */
        Actor_SetDestination(ACTOR_GERALD, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Actor_WaitForMove(ACTOR_GERALD);
    Actor_SetPosition(ACTOR_GERALD, 0, 0);
    Audio_PlayCueForPartyMember_1();
    Event_End();
}

void SceneState_SetValue123Mode11(void)
{
    Audio_PlayCue(123);
    Event_RequestExit(11);
}

/* Sets up the opening sequence: two calls with fixed argument pairs, a
 * write to the scene work record, and two more calls with fixed args. */
void FieldScene_RunOpeningSequenceHead(void)
{
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 32768, 16384); /* object_id 0, speed_limit 32768, acceleration 16384 */
    Actor_WalkTo(ACTOR_PARTY_LEADER, 728, 408); /* object_id 0, x 728, z 408 */
    SCENE_WORD_1C8 = 16;
    Audio_PlayCue_1_020019a4(123);
    Event_RequestExit(15); /* main:0808a248 */
}

/* Runs the second part of an opening sequence: two parameterised calls, then
 * (only if a flag read via Value_00000854 is clear) a short block of setup
 * calls, then unconditionally sets a scene work field and two more calls. */
void FieldScene_RunOpeningSequenceSecond(void)
{
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 32768, 16384); /* object 0, speed_limit 32768, acceleration 16384 */
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 744, 408); /* object 0, x 744, z 408 */
    if (GameFlag_IsSet((s32)&Value_00000854) == 0) {
        Event_Begin();
        SceneWork_SetStepValue_1_020019e4((s32)&Value_000012c3);
        Event_ShowMessage(8, 0); /* action 8 */
        Event_End();
    }
    SCENE_WORK_FIELD_456 = 16;
    Audio_PlayCue_1_020019e4(123);
    Event_RequestExit(14); /* main:0808a248 */
}

/* Third scene step: sets up actors 24 and 25 (fetching each one's record),
 * runs a shared series of configuration calls touching actors 0-2, 10, 14,
 * 20, 24 and 25, then marks the two fetched records with a byte flag. */
void FieldScene_RunOpeningSequenceThird(void)
{
    void *Func_02005ae2();
    void *Func_020067d8();

    void *actor24;
    void *actor25;

    actor24 = Scene_GetRecord_1_02001a4c(24);
    actor25 = Scene_GetRecord_2_02001a4c(25);
    Event_Begin();
    Actor_SetSpeed(ACTOR_PARTY_LEADER, RATIO_HI, RATIO_LO);
    Actor_SetSpeed(ACTOR_GERALD, RATIO_HI, RATIO_LO);
    Actor_SetSpeed(ACTOR_IVAN, RATIO_HI, RATIO_LO);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 232, 696);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 200, 696);
    Event_Wait(10);
    Actor_ShowEmote(25, 256, 0);
    Actor_ShowEmote(24, 256, 0);
    Event_Wait(60);
    Call3(FieldScene_RunSplitTripleSteps, 25, 0, 10);
    Actor_RunRepeatedMotion(24, 2);
    Event_Wait(20);
    SceneWork_SetStepValue_1_02001a4c(4758);
    Call2(SceneActor_SetModeZeroAndValue, 24, 20);
    Actor_SetAttachedEffect(25, 258); /* main:0808a1f0 */
    Event_Wait(60);
    Call2(SceneActor_SetModeZeroAndValue, 25, 20);
    Actor_RunRepeatedMotion(24, 1);
    Call2(SceneActor_SetModeZeroAndValue, 24, 30);
    Actor_SetSpeed(24, 262144, 131072);
    Actor_SetSpeed(25, 229376, 114688);
    ObjectMotion_EnableActionAndSetCallback_1(25, 33609776);
    ObjectMotion_EnableActionAndSetCallback_2(24, 33609056);
    Object_LookupAndStep_1(24);
    Map_CopyCellAttributes(14, 45, 3, 1, 14, 44); /* main:080091c0 */
    GameFlag_Set(2130);
    GameFlag_Set(768);
    Call2(Func_020067dc, 33598369, 3200); /* main:080000d0 */
    /* Byte flag at +100 of each record: 1 for actor 24's, 3 for actor 25's. */
    ((struct SceneRecord *)actor24)->value100 = 1;
    ((struct SceneRecord *)actor25)->value100 = 3;
    BattleRuntime_ScheduleShoulderButtonModeUpdate_1_02001a4c();
}

/* Sets up actors 24 and 25 (pose picked from a per-actor threshold field at
 * +100), positions and moves actors 0-2 and 24-25, and advances the shared
 * scene step counter partway through. */
void FieldScene_RunScene383SequenceB(void)
{
    s32 actor24;
    s32 actor25;
    s32 record_00001299;

    actor24 = Scene_GetRecord_1_02001ba0(24);
    actor25 = Scene_GetRecord_2_02001ba0(25);
    Event_Begin();
    Call1_02001ba0(Func_02006838, 0x200aba1);
    GameFlag_Clear(0x300);
    if (*(s16 *)(actor24 + 100) <= 3) {
        Engine_ActorEnableActionCallback(24, 0x200d678);
    } else {
        Engine_ActorEnableActionCallback(24, 0x200d650);
    }
    if (*(s16 *)(actor25 + 100) <= 2) {
        Engine_ActorEnableActionCallback(25, 0x200d768);
    } else {
        Engine_ActorEnableActionCallback(25, 0x200d650);
    }
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0xcccc, 0x6666);
    Actor_SetSpeed(ACTOR_GERALD, 0xcccc, 0x6666);
    Actor_SetSpeed(ACTOR_IVAN, 0xcccc, 0x6666);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 248, 0x2d8);
    Actor_SetPosition(ACTOR_IVAN, 0xf80000, 0x2d80000);
    Actor_SetPosition(ACTOR_GERALD, 0xf80000, 0x2d80000);
    Actor_WalkTo(ACTOR_IVAN, 0x108, 0x2e8);
    Actor_WalkToAndWait(ACTOR_GERALD, 232, 0x2e8);
    Actor_WaitForMove(ACTOR_IVAN);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 0);
    Actor_FaceActor(ACTOR_GERALD, ACTOR_PARTY_LEADER, 0);
    Func_02006290(2, 0, 30);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
    record_00001299 = (s32)LinkedMessage_TheyreActingSuspiciousSomethingsNot;
    Event_SetMessage(record_00001299);
    Event_ShowMessage(ACTOR_IVAN, 0);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
    Func_020062bc(1, 2, 20);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Func_020062fe(1, 3, 20);
    Func_020062f0(0, 1, 10);
    Event_OpenMessage(ACTOR_GERALD, 0);
    if (Event_ChooseYesNo(0, 0) != 0) {
        bump_step(1);
    }
    Func_020062ec(1, 30);
    Event_SetMessage((record_00001299 + 4));
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
    Func_0200631e(1, 2, 50);
    Actor_ShowEmote(ACTOR_IVAN, 0x100, 0);
    Event_Wait(60);
    Func_02006352(0, 1, 50);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
    Func_0200634e(1, 2, 30);
    Func_02006388(2, 3, 10);
    Func_02006348(2, 20);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x102, 0);
    Event_Wait(60);
    Func_020063b8(2, 3, 20);
    Func_02006378(2, 30);
    Event_SetMessage(MSG_ROBIN_TAKE_LEAD);
    Event_ShowMessage(ACTOR_GERALD, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 0);
    Event_Wait(40);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimation(ACTOR_GERALD, 3);
    Func_020063fa(2, 3, 50);
    Actor_WalkTo(ACTOR_IVAN, 248, 0x2d8);
    Actor_WalkToAndWait(ACTOR_GERALD, 248, 0x2d8);
    Actor_SetPosition(ACTOR_GERALD, 0, 0);
    Actor_SetPosition(ACTOR_IVAN, 0, 0);
    Actor_SetPosition(24, 0x680000, 0x2b80000);
    Actor_SetPosition(25, 0x780000, 0x2b80000);
    Actor_FaceDirection(24, 0, 0);
    Actor_FaceDirection(25, 0x8000, 0);
    Map_CopyCellAttributes(14, 50, 3, 1, 14, 44);
    Event_End();
}

void FieldScene_RunSteps107And250(void)
{
    GameFlag_Set(0x107);
    GameFlag_Set(0x250);
    FieldScene_RunScene383_02004b2c();
}

/* Configures actor records 24 and 25 (their +100 "shown" half words end up
 * set to 1 and 3 respectively) and finishes with a six-argument call that
 * repeats actor id 14. */
void FieldScene_ConfigurePairedActors(void)
{
    u32 i;
    u8 *record;

    Event_Wait(30);
    Actor_RunRepeatedMotion(24, 1);
    Event_Wait(20);
    Event_SetMessage((s32)LinkedMessage_TheyreBack);
    Func_02006498(24, 20);
    Actor_FaceDirection(25, 0, 20);
    Actor_SetAttachedEffect(25, 0x102);
    Actor_RunRepeatedMotion(25, 2);
    Func_020064bc(25, 20);
    Actor_SetAnimationAndWait(24, 4);
    Event_Wait(20);
    Func_020064d2(24, 20);
    Actor_SetSpeed(24, 0x40000, 0x20000);
    Actor_SetSpeed(25, 0x38000, 0x1c000);
    ObjectMotion_EnableActionAndSetCallback_1_02001e80(25, 0x200d830);
    Actor_EnableActionCallback(24, 0x200d560);
    Object_LookupAndStep_1_02001ba0(24);
    {
        u8 *record = Scene_GetRecord_1_02001e80(24);
        s32 shown = 1;

        *(u16 *)(record + ACTOR_SHOWN_OFFSET) = shown;
    }
    {
        u8 *record = Scene_GetRecord_2_02001e80(25);
        s32 shown = 3;

        *(u16 *)(record + ACTOR_SHOWN_OFFSET) = shown;
    }
    Map_CopyCellAttributes(14, 48, 4, 1, 14, 44);
}

void SceneActor_UpdateAnimationOnStateMatch(s32 actor, s32 expected, s32 next, const u8 *desc)
{
    u8 *rec = Func_02007472(actor);

    if (*(s16 *)(rec + 100) == expected) {
        Actor_EnableActionCallback(actor, desc);
        *(u16 *)(rec + 100) = (u16)next;
    }
}

void SceneState_SetFlagByActorPosition(void)
{
    u8 *p0 = Func_0200749e(0);
    s32 rx = *(s32 *)(p0 + 8);
    s32 x;
    s32 z;

    u8 *p1 = Func_020074a6(0);
    x = rx >> 20;
    z = *(s32 *)(p1 + 16);
    x = x - 34;
    z = z >> 20;

    if ((u32)x <= 1 && z > 40 && z <= 42) {
        GameFlag_Set(148 << 2);
    } else {
        GameFlag_Clear(148 << 2);
    }
}

void SceneActor_FaceActors24And25TowardActorZero(void)
{
    struct Rec_383 *origin = Func_02007926(0);
    struct Rec_383 *first = Func_0200792e(24);
    struct Rec_383 *second = Func_02007936(25);

    first->f6 = Func_02007858(origin->f16 - first->f16, origin->f8 - first->f8);
    second->f6 = Func_0200786c(origin->f16 - second->f16, origin->f8 - second->f8);
}

void FieldScene_RunLateSequence(void)
{
    u32 i;
    s32 record;
    s32 v5;

    Event_Begin();
    Actor_SetPosition(10, 0x3180000, 0x1a00000);
    Actor_SetPosition(11, 0x3200000, 0x1900000);
    Actor_SetPosition(12, 0x3080000, 0x1a00000);
    Actor_FaceDirection(10, 0x3000, 0);
    Actor_FaceDirection(11, 0x3000, 0);
    Actor_FaceDirection(12, 0x3000, 0);
    Actor_SetSpeed(11, 0xcccc, 0x6666);
    Actor_SetSpeed(12, 0xcccc, 0x6666);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0x3100000, 0x1c00000);
    Actor_SetPosition(ACTOR_GERALD, 0x3280000, 0x1b00000);
    Actor_SetPosition(ACTOR_IVAN, 0x3080000, 0x1b00000);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 19);
    Actor_SetAnimation(ACTOR_GERALD, 19);
    Actor_SetAnimation(ACTOR_IVAN, 19);
    v5 = 2;
    *(u8 *)(Func_02007a1a(0) + 35) = v5;
    *(u8 *)(Func_02007a26(1) + 35) = v5;
    *(u8 *)(Func_02007a30_a(2) + 35) = v5;
    record = Func_02007a3a(0);
    Actor_SetSpriteFlags(record, 0);
    record = Func_02007a46(2);
    Actor_SetSpriteFlags(record, 0);
    record = Func_02007a52(1);
    Actor_SetSpriteFlags(record, 0);
    Actor_FaceDirection(8, 0xb000, 0);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
    Camera_FollowActor(ACTOR_PARTY_LEADER, 0);
    Camera_WaitForMove();
    Map_Redraw();
    SceneState_SetWord1c0To209AndRun();
    Event_Wait(60);
    SceneEffect_ApplyThreeValuesAndFinish(10, 3, 20);
    Event_SetMessage(MSG_THESE_KIDS_NOTHING_WORRY_ABOUT);
    Func_0200731c(10, 30);
    Func_02007324(8, 30);
    Actor_WalkTo(11, 0x328, 0x1c8);
    Actor_WalkTo(12, 0x318, 0x1c8);
    Actor_WaitForMove(12);
    Actor_FaceDirection(12, 0, 0);
    Actor_WaitForMove(11);
    Actor_FaceDirection(11, 0, 0);
    Event_Wait(30);
    Func_020073b8(11, 3, 20);
    Func_02007378(11, 20);
    Func_0200739a(12, 0, 30);
    Func_0200738a(12, 60);
    Event_End();
}

s32 OverlayObject_GetObjectTwoByte118(void)
{
    return Func_02007c8e_a(2)[0x118];
}

s32 OverlayObject_RunObjectTwoWhenFlagged(void)
{
    u8 *o;

    Func_02007eb8();
    o = Func_02007ca6(2);
    o += 0xf8;
    if (*(s32 *)o & 1) {
        Func_02007ce6(2, 0, 0, 0);
        Audio_PlayCue(126);
        Func_02007cd2(0);
        return Func_02007cd8(2);
    }
}

/* Sets up actors 10, 11, 12 (position, pose, flags) and actors 0-2 and 8-9 and
 * 13-14 in several later waves, moving and animating them through a long
 * scripted sequence, branches once on an actor-12 state check, then advances
 * the shared scene phase twice before returning. */
void RunEventScript01(void)
{
    extern u8 *Data_03001ebc;

    u32 i;
    s32 record;
    u8 *work;
    s32 base5_200d17c;
    u8 *actor12_record;

    record = Scene_GetRecord_1_02002fd4(12);
    actor12_record = *(u8 **)(record + 80);
    Event_Begin();
    Actor_SetPosition(10, 0x3180000, 0x1a00000);
    Actor_SetPosition(11, 0x3200000, 0x1900000);
    Actor_SetPosition(12, 0x3080000, 0x1880000);
    record = Scene_GetRecord_2_02002fd4(10);
    Actor_SetSpriteFlags(record, 0);
    record = Scene_GetRecord_3(11);
    Actor_SetSpriteFlags(record, 0);
    record = Scene_GetRecord_4(12);
    Actor_SetSpriteFlags(record, 0);
    Actor_SetAnimation(10, 9);
    Actor_SetAnimation(11, 9);
    Actor_SetAnimation(12, 9);
    /* Clear the low bit of the flag byte at +35. */
    *(u8 *)(Scene_GetRecord_5(12) + 35) &= 254;
    /* Set flag bits 0x0c of the byte at +9. */
    actor12_record[9] |= 12;
    base5_200d17c = (s32)Data_0200d17c;
    Actor_EnableActionCallback(10, base5_200d17c);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0x3180000, 0x1b80000);
    Actor_SetPosition(ACTOR_GERALD, 0x3280000, 0x1b00000);
    Actor_SetPosition(ACTOR_IVAN, 0x3080000, 0x1b80000);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xb000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0xb000, 0);
    Actor_SetSpeed(8, 0xcccc, 0x6666);
    Actor_FaceDirection(8, 0xb000, 0);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
    Camera_FollowActor(ACTOR_PARTY_LEADER, 0);
    Camera_WaitForMove();
    Map_Redraw();
    Func_020076ba();
    Actor_EnableActionCallback(11, base5_200d17c);
    Event_Wait(30);
    Actor_EnableActionCallback(12, base5_200d17c);
    Event_Wait(30);
    Event_SetMessage(MSG_THEY_THEY_GOT_US);
    Func_0200770c(10, 20);
    Actor_ShowEmote(8, 0x102, 0);
    Event_Wait(60);
    Func_02007726(8, 20);
    Actor_WalkToAndWait(8, 0x328, 0x1c8);
    Actor_SetSpeed(ACTOR_GERALD, 0xcccc, 0x6666);
    Actor_WalkToAndWait(ACTOR_GERALD, 0x318, 0x1b0);
    Actor_FaceDirection(ACTOR_GERALD, 0, 0);
    Actor_WalkToAndWait(8, 0x328, 0x198);
    Actor_FaceDirection(8, 0x8000, 0);
    Actor_WalkToAndWait(ACTOR_GERALD, 0x328, 0x1b0);
    Actor_FaceDirection(ACTOR_GERALD, 0xc000, 0);
    Event_Wait(20);
    Func_020077e2(8, 3, 20);
    Func_020077a2(8, 20);
    Actor_WalkToAndWait(8, 0x300, 0x198);
    Event_Wait(20);
    Actor_FaceActor(8, ACTOR_PARTY_LEADER, 0);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 8, 0);
    Actor_FaceActor(ACTOR_GERALD, 8, 0);
    Func_020077f6(2, 8, 40);
    Func_020077e6(8, 30);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimation(ACTOR_GERALD, 3);
    Func_02007848(2, 3, 20);
    Actor_WalkToAndWait(8, 0x2e8, 0x198);
    Event_Wait(50);
    Actor_RunRepeatedMotion(11, 2);
    Func_02007824(11, 20);
    Actor_EnableActionCallback(11, base5_200d17c);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 1);
    Event_Wait(20);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 11, 0);
    Actor_FaceActor(ACTOR_GERALD, 11, 0);
    Func_02007870(2, 11, 20);
    Func_02007860(1, 20);
    Actor_RunRepeatedMotion(12, 2);
    Func_02007870_a(12, 30);
    Actor_EnableActionCallback(12, base5_200d17c);
    Actor_ShowEmote(ACTOR_GERALD, 0x103, 0);
    Event_Wait(60);
    Func_02007890(1, 30);
    Func_020078b2(2, 0, 30);
    Func_020078bc(0, 2, 30);
    Func_020078f6(2, 3, 20);
    Func_020078b6(2, 20);
    Func_020078d8(1, 2, 30);
    Func_02007912(0, 3, 30);
    Func_020078d2(2, 20);
    Actor_ShowEmote(ACTOR_GERALD, 0x100, 0);
    Event_Wait(60);
    Func_020078ec(1, 30);
    Actor_ShowEmote(ACTOR_IVAN, 0x101, 0);
    Event_Wait(60);
    Func_02007936_a(0, 1, 20);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Func_02007960(1, 3, 20);
    Func_02007984(2, 0);
    Func_0200822c();
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
    Func_02007950(1, 2, 10);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 1);
    Event_Wait(20);
    Actor_SetAttachedEffect(ACTOR_GERALD, 0x102);
    Event_Wait(60);
    Func_02007980(0, 1, 10);
    Event_OpenMessage(ACTOR_GERALD, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_Wait(20);
        Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
        Func_020079ae(1, 2, 20);
        Func_02007a2a();
        Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
        Actor_RunRepeatedMotion(ACTOR_GERALD, 1);
        ((void (*)())Engine_EventWait)(20);
        Func_020079b8(1, 20);
    } else {
        bump_step(1);
        Actor_ShowEmote(ACTOR_GERALD, 0x102, 0);
        Event_Wait(60);
        Func_020079fe(1, 20);
        Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
        Func_02007a2a_a(1, 2, 20);
        Func_02007aa6();
        Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
        Actor_RunRepeatedMotion(ACTOR_GERALD, 1);
        Event_Wait(20);
    }
    Actor_ShowEmote(ACTOR_IVAN, 0x102, 0);
    Event_Wait(60);
    SceneEffect_ApplyThreeValuesAndFinish(2, 4, 20);
    Event_SetMessage(MSG_SEE_THATS_HAPPENED);
    Func_02007a56_a(2, 20);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Func_02007ab0(1, 3, 40);
    Actor_WalkToAndWait(8, 0x328, 0x198);
    Actor_FaceDirection(8, 0x8000, 0);
    Event_Wait(30);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 8, 0);
    Actor_FaceActor(ACTOR_GERALD, 8, 0);
    Func_02007abe(2, 8, 20);
    Actor_RunRepeatedMotion(8, 1);
    Event_Wait(20);
    Func_02007abc(8, 20);
    Actor_SetSpeed(9, 0xcccc, 0x6666);
    Actor_SetSpeed(13, 0xcccc, 0x6666);
    Actor_SetSpeed(14, 0xcccc, 0x6666);
    Actor_SetPosition(9, 0x2e80000, 0x1980000);
    Actor_WalkToAndWait(9, 0x300, 0x198);
    Func_02007b18(9, 10, 30);
    Actor_SetPosition(13, 0x2e80000, 0x1980000);
    Actor_WalkToAndWait(13, 0x300, 0x198);
    Actor_SetPosition(14, 0x2e80000, 0x1980000);
    Actor_WalkTo(14, 0x310, 0x190);
    Actor_WalkToAndWait(13, 0x308, 0x1a8);
    Actor_WaitForMove(14);
    Actor_FaceActor(13, 10, 0);
    Func_02007b78(14, 10, 20);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 9, 0);
    Actor_FaceActor(ACTOR_GERALD, 9, 0);
    Func_02007b96(2, 9, 20);
    Func_02007bd0(9, 4, 20);
    Func_02007b90(9, 20);
    Func_02007b98_a(11, 20);
    Func_02007ba0(12, 30);
    Func_02007bda(9, 13, 20);
    Actor_RunRepeatedMotion(13, 1);
    Event_Wait(20);
    Func_02007bc0(13, 20);
    Func_02007c12(9, 3, 30);
    Func_02007c04(9, 14, 20);
    Func_02007c26(9, 3, 30);
    Func_02007c00(9, 10, 20);
    Func_02007bf0(9, 20);
    Actor_SetAnimation(13, 3);
    Func_02007c4a(14, 3, 20);
    Func_02007c3c(13, 14, 20);
    Actor_SetAnimation(13, 3);
    Func_02007c66(14, 3, 20);
    Actor_WalkTo(14, 0x318, 0x188);
    Actor_WalkToAndWait(13, 0x310, 0x190);
    Actor_FaceActor(13, 12, 0);
    Actor_WaitForMove(14);
    Func_02007c6c(14, 11, 20);
    Actor_RunRepeatedMotion(13, 1);
    Event_Wait(20);
    Func_02007c6a(13, 20);
    Func_02007cbc(14, 4, 20);
    Func_02007c7c(14, 30);
    Func_02007cb6(13, 0, 20);
    Func_02007c8e(13, 20);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimation(ACTOR_GERALD, 3);
    Func_02007cf0(2, 3, 50);
    /* Write the field at +0x1c8, then the phase/status word at +0x1c0, of
     * the shared scene work record. */
    work = Data_03001ebc;
    *(s32 *)(work + 0x1c8) = 30;
    *(s32 *)(work + 0x1c0) = 0x201;
    Event_CloseScreen();
    Event_WaitForScreen();
    Event_Wait(60);
    Event_End();
}

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

void SceneState_SetWord1c0To209AndRun(void)
{
    u8 *state;

    state = *(u8 **)0x03001ebc;
    *(s32 *)(state + 0x1c0) = 0x209;
    Event_OpenScreen();
    Event_WaitForScreen();
    Event_Wait(1);
}

void SceneActor_SetModeZeroAndValue(s32 a, s32 b)
{
    Event_ShowMessage(a, 0);
    Event_Wait(b);
}

void FieldScene_RunSplitTripleSteps(s32 a, s32 b, s32 c)
{
    Actor_FaceActor(a, b, 0);
    Event_Wait(c);
}

void SceneActor_SetPairZeroAndValue(s32 a, s32 b, s32 c)
{
    Actor_FaceEachOther(a, b, 0);
    Event_Wait(c);
}

void SceneEffect_ApplyThreeValuesAndFinish(s32 a, s32 b, s32 c)
{
    Func_02009418_a(a, b, c);
    Func_0200942e(a);
    Event_Wait(c);
}

void SceneEffect_ApplyPairWithValue141(s32 a, s32 b)
{
    Psynergy_Begin(141, 1);
    Psynergy_SetTarget(a, b);
    Psynergy_RaiseHands();
    Psynergy_PlayEffect(1);
    Task_Wait(1);
}

void SceneState_SetValue2ThenFinish(void)
{
    Psynergy_PlayEffect(2);
    Psynergy_LowerHands();
    Func_0200957e();
}

void OverlayObject_ConfigureObject22WithResource17(s32 a)
{
    u8 *o;
    u8 *q;
    u8 *p;
    u8 *v;
    s32 z;
    s32 m;

    z = 0;
    o = Func_02009356(22);
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
        v = Func_02009370(17, 0x608);
        Item_LoadIcon(a);
        v += 0x400;
        Vram_Load(q[28], 0x80, v);
        Heap_Release(17);
    }
}

u8 *SceneData_FindEntryAtPosition(s32 *o)
{
    s32 x = (o[0] + (s32)0xFFC00000) >> 19;
    s32 y = (o[2] + (s32)0xFD900000) >> 19;
    u8 *e = Data_0200cf2c;
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

s32 SceneActor_CheckTileFreeOfKinds(u8 *p)
{
    s32 x;
    s32 y;

    if (p == 0) {
        return 1;
    }
    x = (p[0] << 19) + 0x480000;
    y = (p[1] << 19) + 0x2780000;
    if (Func_02009062(x, y, 0) != 0 || Func_02009030_a(x, y, 2) != 0
        || Func_0200903e_a(x, y, 24) != 0 || Func_0200904c(x, y, 25) != 0) {
        return -1;
    }
    return 0;
}

void SceneActor_ApplyScaledBytePairPosition(s32 a, u8 *p)
{
    Func_0200958c(a, (p[0] << 19) + 0x480000, 0, (p[1] << 19) + 0x2780000);
}

void FieldScene_RunScene383_02004b2c(void)
{
    Event_Begin();
    Camera_MoveTo(0xa80000, -1, 0x2900000, 1);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0xcccc, 0x6666);
    Actor_SetSpeed(ACTOR_GERALD, 0xcccc, 0x6666);
    Actor_SetSpeed(ACTOR_IVAN, 0xcccc, 0x6666);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 248, 0x2b8);
    Actor_SetPosition(ACTOR_GERALD, 0xf80000, 0x2b80000);
    Actor_SetPosition(ACTOR_IVAN, 0xf80000, 0x2b80000);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 200, 0x2b8);
    Actor_WalkTo(ACTOR_GERALD, 248, 0x2c8);
    Actor_WalkToAndWait(ACTOR_IVAN, 232, 0x2b8);
    Actor_WaitForMove(ACTOR_GERALD);
    Actor_FaceDirection(ACTOR_GERALD, 0xc000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0x8000, 0);
    Actor_WaitForMove(ACTOR_PARTY_LEADER);
    Actor_SetAnimation(ACTOR_GERALD, 12);
    FieldScene_ConfigurePairedActors();
    Call4(Func_02009a54, 0x300000, 0x2400000, 0x1200000, 0x2e00000);
    Actor_SetSpeed(ACTOR_GERALD, 0x10000, 0x8000);
    Actor_SetSpeed(ACTOR_IVAN, 0xc000, 0x6000);
    Actor_SetSpeed(24, 0x10000, 0x13333);
    Actor_SetSpeed(25, 0x18000, 0x18000);
    Data_0200e4f8 = 0;
    Call2(Func_020098a8, 0x200c8c9, 0xc94);
    Engine_GameFlagClear(0x1ff);
    Event_End();
    Audio_PlayCue(9);
}
