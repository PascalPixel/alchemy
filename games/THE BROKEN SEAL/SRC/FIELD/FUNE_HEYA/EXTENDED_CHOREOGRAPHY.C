#include "TYPES.H"
#include "FIELD_EVENT.H"

#define Scene_GetRecord_1(a0) Value1(Func_0200b642, a0)
#define Scene_GetRecord_2(a0) Value1(Func_0200b656, a0)
#define Scene_GetRecord_3(a0) Value1(Func_0200b66a, a0)
#define ObjectMotion_EnableActionAndSetCallback_2(a0, a1) Value2(Engine_ActorEnableActionCallback, a0, a1)
#define ObjectMotion_MarkActiveAndSetActionCallback_1(args...) Func_0200b7b8(args)

#include "STAGED_ACTOR.H"

/*
 * resource_3b1 owner at 0x020002f4, 76 bytes.
 * Puts an actor into its ready state: stage byte at +89, two mode fields on the
 * linked record, and the flag byte at +35 rewritten. Returns 1.
 *
 * The two mode writes are bitfields (the -13 mask stays 32-bit and is shared
 * between them); the +35 write is ordinary byte arithmetic, and its 0xfe mask
 * is narrowed to a byte, which is why the two are spelled differently.
 */

enum ExtendedChoreographyMessage {
    MSG_WONDER_COULD_HAVE_HAPPENED = 0x1d26,
    MSG_ITS_TOO_LATE_HIRE_MERCENARIES = 0x1d30,
    MSG_BUT_WE_CANT_SEND_SHIP = 0x1d31,
    MSG_LONGER_WE_SIT_HERE_MORE = 0x1d4e,
    MSG_IF_WE_ARENT_GOING_SET = 0x1d56,
    MSG_NOW_WANT_SEE_CAPTAIN_TOO = 0x1d91,
    MSG_YOURE_TRYING_LAUNCH_SHIP = 0x1d93,
    MSG_BAD_LUCK_LOSING_MY_LUCKY = 0x1dcd,
    MSG_IF_SHIP_FROM_TOLBI_HAD = 0x1dd4,
    MSG_ITS_MY_LUCKY_ANCHOR = 0x1ddb,
    MSG_WE_DONT_KNOW_MIGHT_HAPPEN = 0x1e06,
    MSG_THESE_PROUD_WARRIORS_NOT_GOING = 0x1e13,
    MSG_OUR_REPLACEMENT_NEVER_ARRIVED_BUT = 0x1e27,
    MSG_CAST_OFF = 0x1e3b,
    MSG_ROW_THOSE_OARS = 0x1e3c,
    MSG_WERE_OFF = 0x1e3d,
    MSG_IM_TURNING = 0x1e43,
    MSG_HEY_ARE_YOU_OK = 0x1e6e,
    MSG_OHHHH_NOOOO_GOING_MAKE_ME = 0x1e81,
    MSG_HA_HA_HA_ROWING_FEEL = 0x1e84,
    MSG_GIVES_ME_CHILLS_THINK_COULD = 0x1ea1,
    MSG_ROBIN_YOUVE_GOT_GOOD_EYE = 0x1ea2,
    MSG_HO_HO_PERSON_GOING_GET = 0x1ea6,
    MSG_OARSMAN_WAS_INJURED = 0x1eb2,
    MSG_WONDER_WHATS_WRONG_SHIP_SHOULDNT = 0x1ec1,
    MSG_THING_HAS_KAJA_HIS_MEN = 0x1ece,
    MSG_MONSTERS_EVERYWHERE_IM_STUCK_ROWING = 0x1ecf,
    MSG_SHIP_STARTING_LIST_IF_WE = 0x1ed0,
    MSG_HOW_MANY_MONSTERS_OUT_THERE = 0x1ed1,
    MSG_ANOTHER_MONSTER_ISNT_FIRST_CLASS = 0x1ed2,
    MSG_DONT_CARE_TAKES_JUST_HURRY = 0x1edb,
    MSG_IF_THOSE_MONSTERS_COME_BACK = 0x1edc,
    MSG_BOATS_ROCKING_MUCH_IM_CERTAIN = 0x1edd,
    MSG_HAD_IDEA_THERE_WERE_MANY = 0x1ede,
    MSG_WERE_SURROUNDED_BY_MONSTERS_STILL = 0x1edf,
    MSG_HATE_ARGUING = 0x1f48,
    MSG_SORRY_EVERYONE_BUT_WE_NEED = 0x1f78,
    MSG_IM_SPREADING_GOODWILL_WHEREVER_TRAVEL = 0x1f7b,
    MSG_SHIPS_CREW_READY_FOR_ANYTHING = 0x1f7d,
    MSG_SHIPS_CREW_READY_FOR_ANYTHING_2 = 0x1f7f,
    MSG_GOOD_SHIP_HAS_ARRIVED_SAFELY = 0x1f81
};

struct Rec_3b1 {
    u8 pad00[9];
    u8 lo9 : 2;
    u8 mode9 : 2;               /* +9,  bits 2..3 */
    u8 hi9 : 4;
    u8 pad0a[11];
    u8 lo15 : 2;
    u8 mode15 : 2;              /* +21, bits 2..3 */
    u8 hi15 : 4;
};

struct Work_3b1 {
    u8 pad00[35];
    u8 f35;                     /* +35 */
    u8 pad24[44];
    struct Rec_3b1 *f80;        /* +80 */
    u8 pad51[5];
    u8 f89;                     /* +89 */
};

struct SceneActor {
    u8 pad00[99];
    u8 mode;
};

struct EffectRecord {
    u8 pad00[6];
    u16 angle;
    u8 pad08[83];
    u8 state;
    u8 pad5c[6];
    u8 active;
};

struct SceneActor_02001144 {
    u8 pad00[10];
    s16 x;
    u8 pad0c[6];
    s16 y;
};

struct SceneWork {
    u8 pad00[52];
    struct SceneActor_02001144 *actors[58];
};

extern s16 Data_02000240[];
extern u8 Value_0000006f;
extern u8 Data_0200e984[];
extern u8 Data_0200e96c[];
extern u8 Data_0200eb94;
extern u8 Data_0200f420[];
extern u8 Data_0200f444[];
extern u8 Data_0200f570[];
extern u8 Data_0200f6fc[];
extern u8 Data_0200f81c[];
extern u8 Data_0200f930[];
extern u8 Data_0200f984[];
extern u8 Data_0200f9c0[];
extern u8 Data_0200fb58[];
extern u8 Data_0200fd44[];
extern u8 Data_0200fedc[];
extern u8 Value_00001f47;
extern u8 Value_00001ea0;
extern u8 Value_0000092b;
extern u8 Value_00001e7e;
extern u8 Value_00000993;
extern u8 Value_0000092a;
extern u8 Value_0000091a;
extern u8 Value_00000929;
extern u8 Value_00000938;
extern u8 Value_0000092f;
extern u8 Value_00001ea4;
extern u8 Value_00000996;
extern u8 Value_00001e8b;
extern u8 Value_0000091d;
extern u8 Value_0000093b;
extern u8 Value_00000932;
extern u8 Value_00001ea5;
extern u8 Value_00000997;
extern u8 Value_00001e8e;
extern u8 Value_0000091e;
extern u8 Value_0000093c;
extern u8 Value_00000933;
extern u8 Data_00001e40[];
extern u8 Data_0200e8e4[];
extern u8 Data_00001e46[];
extern u8 Data_0200e7c8[];
extern u8 Value_00001dd1;
extern u8 Value_00000925;
extern u8 Value_00001e19;
extern u8 Value_00001d50;
extern u8 Value_00001e9e;
extern u8 Value_00001e78;
extern u8 Value_00000917;
extern u8 Value_00000935;
extern u8 Value_0000092c;
extern u8 Value_00000928;
extern u8 Value_00001e9f;
extern u8 Value_00000992;
extern u8 Value_00001e7b;
extern u8 Value_00000919;
extern u8 Value_00000937;
extern u8 Value_0000092e;
extern u8 Value_00001ea3;
extern u8 Value_00000995;
extern u8 Value_00001e88;
extern u8 Value_0000091c;
extern u8 Value_0000093a;
extern u8 Value_00000931;
extern u32 Data_0200e668[];
extern u8 Data_0200e6a8[];
extern u8 Data_0200e6e4[];
extern u8 Value_0000093e;
extern u8 Value_00000911;
extern u8 Value_00000922;
extern u8 Data_0200e7f0[];
extern u8 Data_0200e818[];
extern const s32 Data_0200f2a0[];
extern const s32 Data_0200f300[];
extern const s32 Data_0200f360[];
extern const s32 Data_0200f3c0[];

s32 Func_020064e6(u8 *o);
struct SceneActor *Func_02006778(s32 actor_id);
void Func_02006cc0();
void Func_02001c60(u8 *o);
u8 *Func_02006fd2(s32 n);
void Func_02001d2e();
s32 Func_02001e74();
s32 Func_020070a0();
s32 Func_02007100();
void Func_02001e72();
s32 Func_02001fb8();
s32 Func_020071e4();
s32 Func_02007248();
void Func_0200208c(u8 *o);
u8 *Func_020073fe(s32 n);
void Func_02002158(u8 *o);
u8 *Func_020074ca(s32 n);
s32 Func_02007d24();
s32 Func_02007d82();
void Func_02007eb6();
void Func_02007ff4();
s32 Func_0200833a();
void Func_02008606();
void Func_020086b6();
void Func_020086e2();
void Func_02008710();
void Func_02008720();
void Func_0200872a();
void Func_02008734();
void Func_02008766();
void Func_020087b2();
void Func_020087c8();
void Func_0200880e();
void Func_0200885a();
void Func_02008f02();
void Func_020089de();
void Func_02008a1a();
void Func_02008a4a();
void Func_02008a74();
void Func_02008a8e();
void Func_02008a98();
void Func_02008aa6();
void Func_020092e8();
s32 Func_0200a018();
void Func_0200a1b2();
s32 Func_0200b642();
s32 Func_0200b656();
s32 Func_0200b66a();
void Func_0200b6b2();
void Func_0200b7b8();
void Func_0200b7ee();
s32 Func_0200a690_a();
s32 Func_0200a69a();
s32 Func_0200b0cc();
s32 Func_0200b0d6();
s32 Func_0200b0e0();
s32 Func_0200b0ea();
void Func_02001a98(u8 *obj);
u8 *Func_02006e0a(s32 n);
void Func_020080ba(void);
void Func_02006336_a();
void Func_02006326(s32 id, s32 value);
void Func_02006346(s32 id, s32 value);
void Func_02006366(s32 id, s32 value);
void Func_02006382(s32 id, s32 value);
void Func_02008892();
void Func_020088b6();
void Func_02008b0e();
void Func_02008b18();
u8 *Func_0200962c();
void Func_02008f84();
u8 *Func_0200966c();
u8 *Func_0200abc0();
u8 *Func_020096b6();
void Func_0200900e();
u8 *Func_020096f6();
u8 *Func_0200ac4a();
void Func_02009078();
void Func_02009082();
void Func_0200908c();
void Func_02009096();
s32 Func_0200a634();
void Func_02008e84();
s32 Func_0200a730();
s32 Func_0200a73a();
void Func_02008f8a();
void Func_02001b60(u8 *o);
u8 *Func_02006ed2(s32 n);
u8 *Func_02009ca8();
void Func_02001fc0(u8 *o);
u8 *Func_02007332(s32 n);
u8 *Func_02007772();
s32 Func_0200773c();
s32 Func_020062b6();
void Func_02006412();
void Func_02006418();
void Func_02006448();
void Func_020064a6();
void Func_020064b4_a();
void Func_02006514();
void Func_02006570();
void Func_0200658c();
void Func_020065ee();
void Func_0200819c();
void Func_020083e6();
u8 *Func_02008336_a();
u8 *Func_02008358_a();
void Func_02006814();
void Func_020068e0();
void Func_020068f4();
void Func_0200693e();
void Func_02006990();
void Func_02006a3c();
void Func_02006da8_a();
void Func_02006e34();
void Func_02006e58();
void Func_02006ea8();
void Func_02006ebe();
void Func_02006f16();
void Func_02006f32();
void Func_02008dc8();
void Func_020071cc();
void Func_020071e0();
void Func_02007232_a();
void Func_02007264();
void Func_0200727a();
void Func_0200729c();
void Func_020072ce_a();
void Func_020072ec();
void Func_020072fa();
void Func_02007434();
void Func_02007442();
void Func_02007508();
void Func_02007522();
void Func_0200758e();
void Func_020075c8();
void Func_020075de();
void Func_0200761a();
void Func_02007630();
void Func_0200768a();
void Func_0200776c();
void Func_02007814();
void Func_02008e2e();
u8 *Func_02008fbe();
u8 *Func_02008ffa();
u8 *Func_02009036();
u8 *Func_0200940c();
u8 *Func_0200943c();
void Func_02009452();
u8 *Func_020093dc_a();
void Func_02007e0e();
void Func_02007e32();
void Func_02007e40();
void Func_02007e4c();
void Func_02007e58();
void Func_02007e64();
void Func_02007e74();
void Func_02007f1a_a();
void Func_02007952();
void Func_02007978();
void Func_0200799e();
void Func_020079c4();
void Func_02007f2a();
void Func_02007f72();
void Func_02007fd6();
void Func_02007ffc();
void Func_02008022();
void Func_02008048();
void Func_02008188_a();
void Func_0200818e();
void Func_0200819e();
void Func_020081ae();
void Func_020081b8();
void Func_020081c6_a();
void Func_0200823c();
void Func_02008352();
void Func_0200835c_a();
void Func_02008366_a();
void Func_020083f6();
void Func_02008400();
void Func_02008446();
void Func_02008456();
void Func_0200847e();
void Func_0200849e();
void Func_020084ca();
void Func_020084fe();
void Func_0200858c();
void Func_02008596();
void Func_020085dc_a();
u8 *Func_02009eaa();
u8 *Func_02009ede();
u8 *Func_02009f14_a();
void Func_0200bade();
s32 Func_0200a7a2();
s32 Func_0200a7ac();
s32 Func_0200a7b6();
u8 *Func_0200bd42();
s32 Func_0200c01e();
void Func_0200c0f0();
u8 *Func_0200beea_a();
void Func_0200ade4();
void *Func_0200ad8a();
void Func_020082ba();
void Func_0200924a();
void Func_020092e8_a();
u8 *Func_0200af28();
u8 *Func_0200af38();
void Func_0200af4e();
void Value_0200e5d0();
u8 *Value_0200e4e8();
void Func_0200afc2();
void Func_0200afe4();
u8 *Func_0200b018();
u8 *Func_0200b024();
u8 *Func_0200b038();
u8 *Func_0200b050();
u8 *Func_0200b060();
u8 *Func_0200b074();
u8 *Func_0200b086();
u8 *Func_0200b090();
void Value_0200e640();
void Value_0200e550();
u8 *Func_0200b14e();
void Value_0200e5d0_b();
void Value_0200e4c0();
void Func_0200b402();
void Func_0200b41a();
void Func_0200b42c();
u8 *Func_0200b52c();

/* Scene-step dispatcher for overlay resource 0x3b1.
 *
 * The owner takes a step selector plus two step parameters and jumps through a
 * 26-entry table into one bounded block of scene setup calls per step. Step 18
 * finishes by re-entering the dispatcher with step 14, which the compiler turns
 * into a jump back to the range check.
 *
 * Uncertain: the roles of the two parameters differ per step (actor slot,
 * count, flag, upper loop bound), so they keep neutral names here. The record
 * fields written at +6 (halfword) and +24 (word) are the same scene-object
 * fields the neighbouring scene sources touch; their meaning is not recovered.
 */

/* Loader-relocated overlay calls: each Func_ symbol names the pre-relocation
 * call word the image holds.
 *
 * Three of those pre-relocation words repeat in this owner while reaching
 * different runtime helpers (0x0200af5a, 0x0200b0e8 and 0x0200b20c each cover
 * two distinct destinations), so one Func_ spelling cannot name both sites.
 * Those six sites are declared by their runtime address instead, which the
 * overlay symbol resolver binds directly. Registering this owner as a
 * translation unit with explicit absolute_symbols would let them go back to
 * suffixed Func_ spellings without changing a byte. */

/* The scene work record pointer; +0x1c0 holds the scene request word. */

/*
 * Actor slot search for resource_3b1.  The 48-byte owner at 0x02005038 has no
 * pool; the halfword at 0x02005066 is alignment before the next owner.
 */

/*
 * Field scene beat for overlay resource_3b1.  Each callee is named for its own
 * call site: every call reaches its target through its own local veneer, even
 * where the same logical callee is used from more than one site.
 */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
void FieldScene_CallPairWith10(s32 a, u16 b);

void SceneState_ScanTwoArraysAndCrossNotify(u8 *a, u8 *b);

void Scene_UpdateCueTimer(s32 a0, s32 a1, s32 a2);

static __inline__ void Call1(void (*f)(), s32 a0)
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

static __inline__ void Call1_02000b84(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call1_02000cc8(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1_02001a08(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call1_02003d10(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ void Call3_020040e8(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ s32 Value2_02004198(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ s32 Value2_02005068(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

static __inline__ s32 Value2_020056dc(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

/*
 * resource_3b1 helper: set bit 3 of the flags byte of actors 28 through 35.
 */

/*
 * The owner at 0x020037b4 is 36 bytes: 34 bytes of code and one alignment
 * halfword; it needs no literal pool.  The index, bit and zero are held in
 * locals initialised in that order.  The zero is an OR identity that folds
 * away unless it has its own local, and initialising the index in a for-init
 * emits it last.  Func_02009ca8 names the in-overlay entry point, not its
 * veneer.
 */
static __inline__ void SetDirection(s32 actor, s32 direction, s32 duration)
{
    Actor_FaceDirection(actor, direction, duration);
}

static __inline__ void Call1_02001b34(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call1_02001f3c(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ u8 *Pointer1(u8 *(*f)(), s32 id)
{
    return f(id);
}

/*
 * Flag-gated scene setup for overlay resource_3b1. Each callee name refers
 * to that call site's own call word rather than to a shared runtime
 * address.
 */

/* Scene selection chain of the resource_3b1 overlay: tests scene ids in turn and runs the matching call sequence. */
static __inline__ void Call3_0200351c(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call1_020038ac(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ u8 *Pointer1_020038ac(u8 *(*f)(), s32 a)
{
    return f(a);
}

static __inline__ void Call4_020048e8(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

/* The halfword field at +6 of a scene record is written from an int-sized
 * value; storing a plain constant through the cast would make the compiler
 * fetch a halfword literal from the pool instead. */
static __inline__ void SetPose(u8 *rec, s32 pose)
{
    *(u16 *)(rec + 6) = pose;
}

void Func_02002224();         /* site 0x1092 */

u8 *Func_02007596();          /* site 0x10ac; returns the actor record */

/* Contiguous unnamed state-owner run for resource_3b1. */
s32 ResetStagedActorState(struct StagedActor *actor)
{
    Object_SetAnimation(actor, 1);
    actor->x.value = 0;
    actor->y = 0;
    actor->z.value = 0;
    actor->unknown_24 = 0;
    actor->unknown_28 = 0;
    actor->unknown_2c = 0;
    actor->unknown_3c = 0x80000000;
    actor->unknown_38 = 0x80000000;
    return 0;
}

s32 UpdateStagedActorVerticalPosition(struct StagedActor *actor)
{
    s16 vertical_motion_direction;
    s32 descending_y;
    s32 ascending_y;

    if (actor->vertical_motion_direction != 0) {
        descending_y = (actor->y
            - ((u32)(Random_Next() << 0xF) >> 0x10)) + 0xFFFF8000;
        actor->y = descending_y;
        if (descending_y < 0) {
            vertical_motion_direction = 0;
            goto block_5;
        }
    } else {
        ascending_y = actor->y
            + ((u32)(Random_Next() << 0xF) >> 0x10) + 0x8000;
        actor->y = ascending_y;
        if (ascending_y > 0x80000) {
            vertical_motion_direction = 1;
block_5:
            actor->vertical_motion_direction = vertical_motion_direction;
        }
    }
    return 1;
}

s32 StagedActor_SetAngleByKind(u8 *o)
{
    u32 v = (u32)(Func_020064e6(o) << 6) >> 16;

    if (v == 6) {
        s32 k = 0x3000;
        *(u16 *)(o + 6) = k;
    } else if (v == 9) {
        s32 k = 0x5000;
        *(u16 *)(o + 6) = k;
    }
    return 1;
}

void StagedActor_AdvanceCounter98(u8 *o)
{
    u8 *p = o + 98;
    s32 n = *p + 1;

    *p = n;
    if ((u8)n > 80) {
        *(u16 *)(o + 102) += 1;
    }
}

s32 StagedActor_CountdownUntilPositionUnset(u8 *o)
{
    s32 n = *(s32 *)(o + 76);

    if (n != 0) {
        *(s32 *)(o + 76) = n - 1;
    } else {
        return 1;
    }
    if (*(s32 *)(o + 56) == 0x80000000
        && *(s32 *)(o + 60) == *(s32 *)(o + 56)
        && *(s32 *)(o + 64) == *(s32 *)(o + 60)) {
        return 1;
    }
    return 0;
}

void UpdateActorNineEffectMode(struct EffectRecord *record)
{
    struct SceneActor *actor;

    actor = Func_02006778(9);
    if (record->state != 0)
        return;
    if (actor->mode == 1) {
        record->angle = 0xd000;
        record->active = 1;
        actor->mode = 0;
    } else if (actor->mode == 2) {
        if (record->active != 0)
            Object_SetAnimation(record, 3);
        record->active = 0;
        actor->mode = 0;
    } else if (actor->mode == 3) {
        record->angle = 0;
        actor->mode = 0;
    }
}

s32 StagedActor_SetReadyState(struct Work_3b1 *work)
{
    struct Rec_3b1 *rec = work->f80;

    work->f89 = 8;
    Actor_SetSpriteFlags(work, 0);
    rec->mode9 = 1;
    rec->mode15 = 1;
    work->f35 = (work->f35 & ~1) | 2;
    Object_SetPalette(work, 15);
    return 1;
}

/*
 * Overlay resource_3b1. Picks the scene data table for the current scene
 * index, with two arms further narrowed by story flags.
 */
s32 SceneData_SelectTableByWord224(void)
{
    if (gGameState.scene == (s32)&Value_0000006f) {
        return (s32)Data_0200e984;
    }
    return (s32)Data_0200e96c;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

s32 SceneData_GetTableEB94(void)
{
    return (s32)&Data_0200eb94;
}

/*
 * The 292-byte owner at 0x0200054c covers the dispatcher, a 23-entry jump
 * table, the case bodies, an alignment halfword and the literal pool. Case
 * order and the shared arms reproduce the reference: 23 shares an arm with
 * 4 while 22 does not, and the 15/17/19 arm skips 16, 18 and 20. 2208 is
 * synthesised in the reference and stays decimal; 0x928 and 0x93e are pool
 * loads.
 */
u8 *SceneData_SelectTableBySceneIndexAndFlags(void)
{
    s16 *tbl = Data_02000240;
    s32 scene = tbl[225];

    switch (scene) {
    case 1:
    case 2:
        if (GameFlag_IsSet(2208) != 0) {
            return Data_0200f6fc;
        }
        if (GameFlag_IsSet(0x928) != 0 && GameFlag_IsSet(0x93e) == 0) {
            return Data_0200f570;
        }
        return Data_0200f444;
    case 4:
    case 23:
        if (GameFlag_IsSet(0x93e) != 0) {
            return Data_0200fedc;
        }
        return Data_0200f9c0;
    case 5:
        if (GameFlag_IsSet(2208) != 0) {
            return Data_0200f930;
        }
        if (GameFlag_IsSet(0x93e) != 0) {
            return Data_0200f984;
        }
        return Data_0200f81c;
    case 15:
    case 17:
    case 19:
        return Data_0200fb58;
    case 21:
        return Data_0200fd44;
    default:
        break;
    }

    return Data_0200f420;
}

void FieldScene_RunScene3b1_02000670(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    Func_02006cc0();
    if (GameFlag_IsSet(0x921) != 0) {
        Event_SetMessage(MSG_IF_SHIP_FROM_TOLBI_HAD);
        Event_ShowMessage(10, 0);
    } else {
        if (GameFlag_IsSet(0x922) != 0) {
            Event_SetMessage(MSG_NOW_WANT_SEE_CAPTAIN_TOO);
            Event_OpenMessage(10, 0);
            if (Event_ChooseYesNo(0, 0) == 0) {
                FieldScene_RunExtendedActorChoreography();
                goto L_020006ea;
            }
            Actor_StartRepeatedMotion(10, 2);
            Event_ShowMessage(10, 0);
            Actor_FaceDirection(10, 0xd000, 0);
        } else {
            Event_SetMessage(MSG_BUT_WE_CANT_SEND_SHIP);
            Event_ShowMessage(10, 0);
        }
    }
    L_020006ea:;
    Event_End();
}

void SceneDialogue_RunActor12Line(void)
{
    Event_Begin();
    Event_SetMessage((s32)&Value_00001dd1);
    Event_AskYesNo(12, 0);
    Event_End();
}

void FieldScene_RunScene3b1_02000728(void)
{
    Event_Begin();
    if (GameFlag_IsSet(0x928) != 0) {
        Event_SetMessage(MSG_OARSMAN_WAS_INJURED);
        FieldScene_RunStepThen10(8);
        Actor_FaceDirection(8, 0xd000, 60);
        Actor_SetAnimationAndWait(8, 4);
        FieldScene_RunStepThen10(8);
        Actor_SetAnimationAndWait(8, 3);
    } else if (GameFlag_IsSet(0x925) != 0) {
        Event_SetMessage(MSG_WE_DONT_KNOW_MIGHT_HAPPEN);
        Event_ShowMessage(8, 0);
    } else if (GameFlag_IsSet(0x921) != 0) {
        Event_SetMessage(MSG_BAD_LUCK_LOSING_MY_LUCKY);
        Event_ShowMessage(8, 0);
        if (GameFlag_IsSet(0x925) == 0 && GameFlag_IsSet(0x924) != 0) {
            gEventWork->unknown_172 = 1;
        }
    } else {
        Event_SetMessage(MSG_ITS_TOO_LATE_HIRE_MERCENARIES);
        Event_ShowMessage(8, 0);
    }
    Event_End();
}

void FieldScene_RunScene3b1_020007f8(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    if (GameFlag_IsSet(0x925) != 0) {
        Actor_StartRepeatedMotion(8, 2);
        Event_SetMessage(MSG_THESE_PROUD_WARRIORS_NOT_GOING);
        FieldScene_RunStepThen10(8);
        Actor_FaceActor(8, 0, 10);
        Event_OpenMessage(8, 0);
        if (Event_ChooseYesNo(0, 0) == 0) {
            Event_Wait(40);
            FieldScene_RunStepThen10(8);
            Value2(FieldScene_CallPairWith10, 8, 0x3000);
            Event_ShowMessage(8, 0);
            goto L_0200088e;
        }
        bump_step(2);
        Event_ShowMessage(8, 0);
        Actor_FaceDirection(8, 0x3000, 0);
    } else {
        Event_SetMessage(MSG_LONGER_WE_SIT_HERE_MORE);
        Event_ShowMessage(8, 0);
    }
    L_0200088e:;
    Event_End();
}

void SceneDialogue_ShowLine1E19Or1D50(void)
{
    Event_Begin();
    if (GameFlag_IsSet((s32)&Value_00000925) != 0) {
        Event_SetMessage((s32)&Value_00001e19);
        Event_AskYesNo(10, 0);
    } else {
        Event_SetMessage((s32)&Value_00001d50);
        Event_ShowMessage(10, 0);
    }
    Event_End();
}

/*
 * Flag-branched scene setup for overlay resource_3b1. Each callee name
 * refers to that call site's own call word rather than to a shared runtime
 * address.
 */

/*
 * Actors 24 and 25 setup for overlay resource_3b1. Each callee name refers
 * to its own call word rather than to a shared runtime address.
 */

/* Scene setup for resource_3b1: installs actors 10 through 17. */

/*
 * Set up actors 24 and 25 -- resource_3b1. A flat setter sequence with no
 * branches; the owner includes its one literal pool word.
 */

/* The pool word, referenced by address so that it is emitted. */

/*
 * The aliases name the call words encoded in the overlay image, and the
 * declarations are old-style because the call sites vary in arity.
 */

/*
 * Actors 24 and 25 setup for overlay resource_3b1. Each callee slot uses
 * its own local veneer, so the names are per call site and not the shared
 * main-image symbol.
 */
void SceneState_RunFlagBranchedActor8Setup(void)
{
    if (GameFlag_IsSet(0x300) != 0) {
        u8 *obj = SceneState_ApplyLevelFromFlags();
        u8 *p;

        Event_Begin();
        Func_02001a98(obj);
        Event_SetMessage((s32)&Value_00001e9e);
        FieldScene_RunStepThen10(8);
        Actor_SetAnimation(obj, 2);
        p = Func_02006e0a(0);
        if (p != 0) {
            Actor_SetDestination(obj, *(s16 *)(p + 10), *(s16 *)(p + 18));
        }
        Actor_WaitForMove(obj);
        Actor_SetPosition(obj, 0, 0);
        Event_End();
    } else if (GameFlag_IsSet((s32)&Value_0000092b) != 0) {
        FieldScene_RunPrimarySequence(8, (s32)&Value_00001e78, 0x990);
    } else if (GameFlag_IsSet((s32)&Value_0000092a) != 0) {
        FieldScene_RunPrimarySequence(8, (s32)&Value_00001e78, (s32)&Value_00000917);
    } else if (GameFlag_IsSet((s32)&Value_00000929) != 0) {
        FieldScene_RunPrimarySequence(8, (s32)&Value_00001e78, (s32)&Value_00000935);
    } else {
        FieldScene_RunPrimarySequence(8, (s32)&Value_00001e78, (s32)&Value_0000092c);
    }
}

void SceneDialogue_RunActorTenFlaggedDialogue(void)
{
    if (GameFlag_IsSet(0x300) != 0) {
        u8 *o = SceneState_ApplyLevelFromFlags();
        u8 *p;

        Event_Begin();
        Func_02001b60(o);
        Event_SetMessage((s32)&Value_00001e9f);
        FieldScene_RunStepThen10(10);
        Actor_SetAnimation(o, 2);
        p = Func_02006ed2(0);
        if (p != 0) {
            Actor_SetDestination(o, *(s16 *)(p + 10), *(s16 *)(p + 18));
        }
        Actor_WaitForMove(o);
        Actor_SetPosition(o, 0, 0);
        Event_End();
    } else if (GameFlag_IsSet((s32)&Value_0000092b) != 0) {
        FieldScene_RunPrimarySequence(10, (s32)&Value_00001e7b, (s32)&Value_00000992);
    } else if (GameFlag_IsSet((s32)&Value_0000092a) != 0) {
        FieldScene_RunPrimarySequence(10, (s32)&Value_00001e7b, (s32)&Value_00000919);
    } else if (GameFlag_IsSet((s32)&Value_00000929) != 0) {
        FieldScene_RunPrimarySequence(10, (s32)&Value_00001e7b, (s32)&Value_00000937);
    } else {
        FieldScene_RunPrimarySequence(10, (s32)&Value_00001e7b, (s32)&Value_0000092e);
    }
}

void FieldScene_RunActor11FlagDialogue(void)
{
    if (GameFlag_IsSet(0x8A0) != 0) {
        Event_Begin();
        Actor_SetAttachedEffect(11, 0x102);
        Event_Wait(40);
        Event_SetMessage((s32)&Value_00001f47);
        Event_ShowMessage(11, 0);
        Event_End();
    } else if (GameFlag_IsSet(0x300) != 0) {
        u8 *o = SceneState_ApplyLevelFromFlags();
        u8 *p;

        Event_Begin();
        Func_02001c60(o);
        Event_SetMessage((s32)&Value_00001ea0);
        FieldScene_RunStepThen10(11);
        Actor_SetAnimation(o, 2);
        p = Func_02006fd2(0);
        if (p != 0) {
            Actor_SetDestination(o, *(s16 *)(p + 10), *(s16 *)(p + 18));
        }
        Actor_WaitForMove(o);
        Actor_SetPosition(o, 0, 0);
        Event_End();
    } else if (GameFlag_IsSet((s32)&Value_0000092b) != 0) {
        FieldScene_RunPrimarySequence(11, (s32)&Value_00001e7e, (s32)&Value_00000993);
    } else if (GameFlag_IsSet((s32)&Value_0000092a) != 0) {
        FieldScene_RunPrimarySequence(11, (s32)&Value_00001e7e, (s32)&Value_0000091a);
    } else if (GameFlag_IsSet((s32)&Value_00000929) != 0) {
        FieldScene_RunPrimarySequence(11, (s32)&Value_00001e7e, (s32)&Value_00000938);
    } else {
        FieldScene_RunPrimarySequence(11, (s32)&Value_00001e7e, (s32)&Value_0000092f);
    }
}

void FieldScene_RunScene3b1SequenceA(void)
{
    u32 i;
    s32 rec7;
    s32 record;

    Event_Begin();
    if (GameFlag_IsSet(0x300) != 0) {
        rec7 = Value0(Func_02001e74);
        Func_02001d2e();
        Event_SetMessage(MSG_GIVES_ME_CHILLS_THINK_COULD);
        FieldScene_RunStepThen10(12);
        Actor_SetAnimation(rec7, 2);
        record = Value1(Func_020070a0, 0);
        if (record != 0) {
            Actor_SetDestination(rec7, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Actor_WaitForMove(rec7);
        Actor_SetPosition(rec7, 0, 0);
    } else {
        Actor_RunRepeatedMotion(12, 2);
        Event_Wait(20);
        Event_SetMessage(MSG_OHHHH_NOOOO_GOING_MAKE_ME);
        Event_OpenMessage(12, 0);
        if (Event_ChooseYesNo(0, 0) == 0) {
            FieldScene_RunStepThen10(12);
            Actor_SetAnimation(12, 2);
            record = Value1(Func_02007100, 0);
            if (record != 0) {
                Actor_SetDestination(12, *(s16 *)(record + 10), *(s16 *)(record + 18));
            }
            Actor_WaitForMove(12);
            Actor_SetPosition(12, 0, 0);
            GameFlag_Set(0x300);
            if (GameFlag_IsSet(0x92b) != 0) {
                GameFlag_Set(0x994);
                goto L_02000c9a;
            }
            if (GameFlag_IsSet(0x92a) != 0) {
                GameFlag_Set(0x91b);
                goto L_02000c9a;
            }
            if (GameFlag_IsSet(0x929) != 0) {
                GameFlag_Set(0x939);
                goto L_02000c9a;
            }
            GameFlag_Set(0x930);
        } else {
            bump_step(1);
            FieldScene_RunStepThen10(12);
        }
    }
    L_02000c9a:;
    Event_End();
}

void FieldScene_RunScene3b1SequenceB(void)
{
    u32 i;
    s32 rec7;
    s32 record;

    Event_Begin();
    if (GameFlag_IsSet(0x300) != 0) {
        rec7 = Value0(Func_02001fb8);
        Func_02001e72();
        Event_SetMessage(MSG_ROBIN_YOUVE_GOT_GOOD_EYE);
        FieldScene_RunStepThen10(9);
        Actor_SetAnimation(rec7, 2);
        record = Value1(Func_020071e4, 0);
        if (record != 0) {
            Actor_SetDestination(rec7, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        ((void (*)())Engine_ActorWaitForMove)(rec7);
        Actor_SetPosition(rec7, 0, 0);
    } else {
        Event_SetMessage(MSG_HA_HA_HA_ROWING_FEEL);
        ((void (*)())Engine_EventShowMessageAndWait)(9, 0, 60);
        Actor_RunRepeatedMotion(9, 1);
        Event_OpenMessage(9, 0);
        if (Event_ChooseYesNo(0, 0) == 0) {
            FieldScene_RunStepThen10(9);
            Actor_SetAnimation(9, 2);
            record = Value1(Func_02007248, 0);
            if (record != 0) {
                Actor_SetDestination(9, *(s16 *)(record + 10), *(s16 *)(record + 18));
            }
            Actor_WaitForMove(9);
            Actor_SetPosition(9, 0, 0);
            GameFlag_Set(0x300);
            if (GameFlag_IsSet(0x92b) != 0) {
                GameFlag_Set(0x991);
                goto L_02000de0;
            }
            if (GameFlag_IsSet(0x92a) != 0) {
                GameFlag_Set(0x918);
                goto L_02000de0;
            }
            if (GameFlag_IsSet(0x929) != 0) {
                GameFlag_Set(0x936);
                goto L_02000de0;
            }
            GameFlag_Set(0x92d);
        } else {
            bump_step(1);
            FieldScene_RunStepThen10(9);
        }
    }
    L_02000de0:;
    Event_End();
}

void SceneDialogue_RunActorThirteenFlag300Branch(void)
{
    if (GameFlag_IsSet(0x300) != 0) {
        u8 *o = SceneState_ApplyLevelFromFlags();
        u8 *p;

        Event_Begin();
        Func_02001fc0(o);
        Event_SetMessage((s32)&Value_00001ea3);
        FieldScene_RunStepThen10(13);
        Actor_SetAnimation(o, 2);
        p = Func_02007332(0);
        if (p != 0) {
            Actor_SetDestination(o, *(s16 *)(p + 10), *(s16 *)(p + 18));
        }
        Actor_WaitForMove(o);
        Actor_SetPosition(o, 0, 0);
        Event_End();
    } else if (GameFlag_IsSet((s32)&Value_0000092b) != 0) {
        FieldScene_RunPrimarySequence(13, (s32)&Value_00001e88, (s32)&Value_00000995);
    } else if (GameFlag_IsSet((s32)&Value_0000092a) != 0) {
        FieldScene_RunPrimarySequence(13, (s32)&Value_00001e88, (s32)&Value_0000091c);
    } else if (GameFlag_IsSet((s32)&Value_00000929) != 0) {
        FieldScene_RunPrimarySequence(13, (s32)&Value_00001e88, (s32)&Value_0000093a);
    } else {
        FieldScene_RunPrimarySequence(13, (s32)&Value_00001e88, (s32)&Value_00000931);
    }
}

void FieldScene_RunFlag300BranchDialogue(void)
{
    if (GameFlag_IsSet(0x300) != 0) {
        u8 *o = SceneState_ApplyLevelFromFlags();
        u8 *p;

        Event_Begin();
        Func_0200208c(o);
        Event_SetMessage((s32)&Value_00001ea4);
        FieldScene_RunStepThen10(14);
        Actor_SetAnimation(o, 2);
        p = Func_020073fe(0);
        if (p != 0) {
            Actor_SetDestination(o, *(s16 *)(p + 10), *(s16 *)(p + 18));
        }
        Actor_WaitForMove(o);
        Actor_SetPosition(o, 0, 0);
        Event_End();
    } else if (GameFlag_IsSet((s32)&Value_0000092b) != 0) {
        FieldScene_RunPrimarySequence(14, (s32)&Value_00001e8b, (s32)&Value_00000996);
    } else if (GameFlag_IsSet((s32)&Value_0000092a) != 0) {
        FieldScene_RunPrimarySequence(14, (s32)&Value_00001e8b, (s32)&Value_0000091d);
    } else if (GameFlag_IsSet((s32)&Value_00000929) != 0) {
        FieldScene_RunPrimarySequence(14, (s32)&Value_00001e8b, (s32)&Value_0000093b);
    } else {
        FieldScene_RunPrimarySequence(14, (s32)&Value_00001e8b, (s32)&Value_00000932);
    }
}

void FieldScene_RunActor15FlagDialogue(void)
{
    if (GameFlag_IsSet(0x300) != 0) {
        u8 *o = SceneState_ApplyLevelFromFlags();
        u8 *p;

        Event_Begin();
        Func_02002158(o);
        Event_SetMessage((s32)&Value_00001ea5);
        FieldScene_RunStepThen10(15);
        Actor_SetAnimation(o, 2);
        p = Func_020074ca(0);
        if (p != 0) {
            Actor_SetDestination(o, *(s16 *)(p + 10), *(s16 *)(p + 18));
        }
        Actor_WaitForMove(o);
        Actor_SetPosition(o, 0, 0);
        Event_End();
    } else if (GameFlag_IsSet((s32)&Value_0000092b) != 0) {
        FieldScene_RunPrimarySequence(15, (s32)&Value_00001e8e, (s32)&Value_00000997);
    } else if (GameFlag_IsSet((s32)&Value_0000092a) != 0) {
        FieldScene_RunPrimarySequence(15, (s32)&Value_00001e8e, (s32)&Value_0000091e);
    } else if (GameFlag_IsSet((s32)&Value_00000929) != 0) {
        FieldScene_RunPrimarySequence(15, (s32)&Value_00001e8e, (s32)&Value_0000093c);
    } else {
        FieldScene_RunPrimarySequence(15, (s32)&Value_00001e8e, (s32)&Value_00000933);
    }
}

/*
 * With story flag 0x300 set, opens a scripted sequence, creates a local
 * object, shows message 0x1ea6, configures it as slot 2, moves it onto actor
 * 0's signed halfword coordinates when actor 0 exists, then releases and
 * closes the sequence.  Otherwise flags 0x92b, 0x92a and 0x929 select the
 * setup call's third argument.  The 204-byte owner includes an alignment
 * halfword and its nine pool words.
 */
void FieldScene_RunActor16FlagDialogue(void)
{
    s32 obj;
    u8 *actor;

    if (GameFlag_IsSet(0x300) != 0) {
        obj = SceneState_ApplyLevelFromFlags();
        Event_Begin();
        Func_02002224(obj);
        Event_SetMessage(MSG_HO_HO_PERSON_GOING_GET);
        FieldScene_RunStepThen10(16);
        Actor_SetAnimation(obj, 2);

        actor = Func_02007596(0);
        if (actor != 0) {
            Actor_SetDestination(obj, *(s16 *)(actor + 10),
                          *(s16 *)(actor + 18));
        }

        Actor_WaitForMove(obj);
        Actor_SetPosition(obj, 0, 0);
        Event_End();
    } else {
        if (GameFlag_IsSet(0x92b) != 0) {
            FieldScene_RunPrimarySequence(16, 0x1e91, 0x998);
        } else if (GameFlag_IsSet(0x92a) != 0) {
            FieldScene_RunPrimarySequence(16, 0x1e91, 0x91f);
        } else if (GameFlag_IsSet(0x929) != 0) {
            FieldScene_RunPrimarySequence(16, 0x1e91, 0x93d);
        } else {
            FieldScene_RunPrimarySequence(16, 0x1e91, 0x934);
        }
    }
}

struct SceneActor_02001144 *FindActorNearPosition(s32 x, s32 y)
{
    struct SceneWork *work;
    struct SceneActor_02001144 **actor;
    struct SceneActor_02001144 *current;
    u32 i;
    s32 actor_x;
    s32 actor_y;
    s32 left;
    s32 top;
    s32 right;
    s32 bottom;

    work = *(struct SceneWork **)0x03001ebc;
    i = 8;
    left = x - 12;
    right = x + 12;
    top = y - 12;
    bottom = y + 12;
    actor = work->actors;
    while (i <= 65) {
        current = *actor++;
        actor_x = current->x;
        actor_y = current->y;
        if (left < actor_x && right > actor_x &&
            top < actor_y && bottom > actor_y)
            return current;
        i++;
    }
    return 0;
}

/* Actor placement check for resource_3b1. */

/* Bucket offsets, packed as {s16 hi; s16 lo} per entry. */

/*
 * Offset obj+10 and obj+18 by the bucket's packed hi/lo pair, test the
 * candidate point, and on success pack {x << 16, obj+12, z << 16} into a
 * stack struct for a second check.  Returns 1 only if both checks pass.  The
 * owner includes its one pool word, the bucket table base.  Callees are named
 * by the address their call site computes, not by a runtime address.
 */
s32 SceneActor_CheckBucketOffsetPoint(s32 bucket)
{
    u8 *obj = Func_02007772(0);
    u32 ofs = Data_0200e668[bucket];
    s32 x = *(s16 *)(obj + 10) + ((s32)ofs >> 16);
    s32 z = *(s16 *)(obj + 18) + (s32)(s16)ofs;

    if (FindActorNearPosition(x, z) != 0) {
        return 0;
    }

    {
        s32 point[3];
        point[0] = x << 16;
        point[1] = *(s32 *)(obj + 12);
        point[2] = z << 16;

        if (Func_0200773c(obj, point) != 0) {
            return 0;
        }
    }

    return 1;
}

/*
 * Level selection from scene flags, overlay resource_3b1. Each callee name
 * refers to its own call word rather than to a shared runtime address.
 */

/*
 * Actor 8 flag setup for overlay resource_3b1. Each callee name refers to
 * its own call word rather than to a shared runtime address.
 */

/* Scene state helper for overlay resource_3b1. */

/*
 * Picks a level from the highest flag that is set and applies it. The
 * 72-byte owner at 0x020012dc includes its three pool words, which are the
 * addresses taken as Value_0000092b, Value_0000092a and Value_00000929.
 */
s32 SceneState_ApplyLevelFromFlags(void)
{
    s32 ret = 0;

    if (GameFlag_IsSet((s32)&Value_0000092b) != 0) {
        ret = 3;
    } else if (GameFlag_IsSet((s32)&Value_0000092a) != 0) {
        ret = 2;
    } else if (GameFlag_IsSet((s32)&Value_00000929) != 0) {
        ret = 1;
    }

    return Func_020062b6(ret, 1);
}

/* Story selector owner at 0x02001324, 84 bytes; eight calls. Per-site call
 * veneers (raw asm confirms each callee slot uses a distinct local stub
 * even across the three near-identical "twin" owners at 0x1324/1378/13cc). */
void SceneDialogue_ShowLine1ECETo1ED0(void)
{
    Event_Begin();
    if (GameFlag_IsSet(0x92c)) Event_SetMessage(MSG_THING_HAS_KAJA_HIS_MEN);
    else if (GameFlag_IsSet(0x935)) Event_SetMessage(MSG_MONSTERS_EVERYWHERE_IM_STUCK_ROWING);
    else Event_SetMessage(MSG_SHIP_STARTING_LIST_IF_WE);
    Event_ShowMessage(0x12, 0); Event_End();
}

/* Story selector owner at 0x02001378, 84 bytes; eight calls. Per-site call
 * veneers (twin of 0x1324/0x13cc with distinct local stub addresses). */
void SceneDialogue_RunActor19TwoFlagLineA(void)
{
    Event_Begin();
    if (GameFlag_IsSet(0x92d)) Event_SetMessage(MSG_THING_HAS_KAJA_HIS_MEN);
    else if (GameFlag_IsSet(0x936)) Event_SetMessage(MSG_MONSTERS_EVERYWHERE_IM_STUCK_ROWING);
    else Event_SetMessage(MSG_SHIP_STARTING_LIST_IF_WE);
    Event_ShowMessage(0x13, 0); Event_End();
}

/* Story selector owner at 0x020013cc, 84 bytes; eight calls. Per-site call
 * veneers (twin of 0x1324/0x1378 with distinct local stub addresses). */
void SceneDialogue_RunActor20TwoFlagLine(void)
{
    Event_Begin();
    if (GameFlag_IsSet(0x92e)) Event_SetMessage(MSG_THING_HAS_KAJA_HIS_MEN);
    else if (GameFlag_IsSet(0x937)) Event_SetMessage(MSG_MONSTERS_EVERYWHERE_IM_STUCK_ROWING);
    else Event_SetMessage(MSG_SHIP_STARTING_LIST_IF_WE);
    Event_ShowMessage(0x14, 0); Event_End();
}

/* Story selector owner at 0x02001420, 60 bytes; six calls. */
void SceneDialogue_ShowLine1ED1Or1ED2(void)
{
    Event_Begin();
    if (GameFlag_IsSet(0x92f)) Event_SetMessage(MSG_HOW_MANY_MONSTERS_OUT_THERE);
    else Event_SetMessage(MSG_ANOTHER_MONSTER_ISNT_FIRST_CLASS);
    Event_ShowMessage(21, 0); Event_End();
}

/* Story selector owner at 0x0200145c, 84 bytes; eight calls. */
void SceneDialogue_RunActor22TwoFlagLine(void)
{
    Event_Begin();
    if (GameFlag_IsSet(0x930)) Event_SetMessage(MSG_THING_HAS_KAJA_HIS_MEN);
    else if (GameFlag_IsSet(0x939)) Event_SetMessage(MSG_MONSTERS_EVERYWHERE_IM_STUCK_ROWING);
    else Event_SetMessage(MSG_SHIP_STARTING_LIST_IF_WE);
    Event_ShowMessage(22, 0); Event_End();
}

/* Story selector owner at 0x020014b0, 84 bytes; eight calls. */
void SceneDialogue_RunActor23BranchedDialogue(void)
{
    Event_Begin();
    if (GameFlag_IsSet(0x931)) Event_SetMessage(MSG_THING_HAS_KAJA_HIS_MEN);
    else if (GameFlag_IsSet(0x93a)) Event_SetMessage(MSG_MONSTERS_EVERYWHERE_IM_STUCK_ROWING);
    else Event_SetMessage(MSG_SHIP_STARTING_LIST_IF_WE);
    Event_ShowMessage(23, 0); Event_End();
}

/* Story selector owner at 0x02001504, 84 bytes; eight calls. */
void SceneDialogue_RunActor24BranchedDialogue(void)
{
    Event_Begin();
    if (GameFlag_IsSet(0x932)) Event_SetMessage(MSG_THING_HAS_KAJA_HIS_MEN);
    else if (GameFlag_IsSet(0x93b)) Event_SetMessage(MSG_MONSTERS_EVERYWHERE_IM_STUCK_ROWING);
    else Event_SetMessage(MSG_SHIP_STARTING_LIST_IF_WE);
    Event_ShowMessage(24, 0); Event_End();
}

/* Story selector owner at 0x02001558, 60 bytes; six calls. */
void SceneDialogue_RunActor25FlaggedLine(void)
{
    Event_Begin();
    if (GameFlag_IsSet(0x933)) Event_SetMessage(MSG_HOW_MANY_MONSTERS_OUT_THERE);
    else Event_SetMessage(MSG_ANOTHER_MONSTER_ISNT_FIRST_CLASS);
    Event_ShowMessage(25, 0); Event_End();
}

/* Second-phase story selector at 0x02001594, 84 bytes; eight calls. */
void SceneDialogue_RunActor18TwoFlagLine(void)
{
    Event_Begin();
    if (GameFlag_IsSet(0x92c)) Event_SetMessage(MSG_DONT_CARE_TAKES_JUST_HURRY);
    else if (GameFlag_IsSet(0x935)) Event_SetMessage(MSG_IF_THOSE_MONSTERS_COME_BACK);
    else Event_SetMessage(MSG_BOATS_ROCKING_MUCH_IM_CERTAIN);
    Event_ShowMessage(18, 0); Event_End();
}

/* Second-phase story selector at 0x020015e8, 84 bytes; eight calls. */
void SceneDialogue_RunActor19TwoFlagLineB(void)
{
    Event_Begin();
    if (GameFlag_IsSet(0x92d)) Event_SetMessage(MSG_DONT_CARE_TAKES_JUST_HURRY);
    else if (GameFlag_IsSet(0x936)) Event_SetMessage(MSG_IF_THOSE_MONSTERS_COME_BACK);
    else Event_SetMessage(MSG_BOATS_ROCKING_MUCH_IM_CERTAIN);
    Event_ShowMessage(19, 0); Event_End();
}

/* Second-phase story selector at 0x0200163c, 84 bytes; eight calls. */
void SceneDialogue_ShowLine1EDBTo1EDDActor20(void)
{
    Event_Begin();
    if (GameFlag_IsSet(0x92e)) Event_SetMessage(MSG_DONT_CARE_TAKES_JUST_HURRY);
    else if (GameFlag_IsSet(0x937)) Event_SetMessage(MSG_IF_THOSE_MONSTERS_COME_BACK);
    else Event_SetMessage(MSG_BOATS_ROCKING_MUCH_IM_CERTAIN);
    Event_ShowMessage(20, 0); Event_End();
}

/* Second-phase story selector at 0x02001690, 60 bytes; six calls. */
void SceneDialogue_RunActor21FlaggedLine(void)
{
    Event_Begin();
    if (GameFlag_IsSet(0x92f)) Event_SetMessage(MSG_HAD_IDEA_THERE_WERE_MANY);
    else Event_SetMessage(MSG_WERE_SURROUNDED_BY_MONSTERS_STILL);
    Event_ShowMessage(21, 0); Event_End();
}

/* Second-phase story selector at 0x020016cc, 84 bytes; eight calls. */
void SceneDialogue_RunActor22BranchedDialogue(void)
{
    Event_Begin();
    if (GameFlag_IsSet(0x930)) Event_SetMessage(MSG_DONT_CARE_TAKES_JUST_HURRY);
    else if (GameFlag_IsSet(0x939)) Event_SetMessage(MSG_IF_THOSE_MONSTERS_COME_BACK);
    else Event_SetMessage(MSG_BOATS_ROCKING_MUCH_IM_CERTAIN);
    Event_ShowMessage(22, 0); Event_End();
}

/* Second-phase story selector at 0x02001720, 84 bytes; eight calls. */
void SceneDialogue_ShowLine1EDBTo1EDDActor23(void)
{
    Event_Begin();
    if (GameFlag_IsSet(0x931)) Event_SetMessage(MSG_DONT_CARE_TAKES_JUST_HURRY);
    else if (GameFlag_IsSet(0x93a)) Event_SetMessage(MSG_IF_THOSE_MONSTERS_COME_BACK);
    else Event_SetMessage(MSG_BOATS_ROCKING_MUCH_IM_CERTAIN);
    Event_ShowMessage(23, 0); Event_End();
}

/* Second-phase story selector at 0x02001774, 84 bytes; eight calls. */
void SceneDialogue_ShowLine1EDBTo1EDDActor24(void)
{
    Event_Begin();
    if (GameFlag_IsSet(0x932)) Event_SetMessage(MSG_DONT_CARE_TAKES_JUST_HURRY);
    else if (GameFlag_IsSet(0x93b)) Event_SetMessage(MSG_IF_THOSE_MONSTERS_COME_BACK);
    else Event_SetMessage(MSG_BOATS_ROCKING_MUCH_IM_CERTAIN);
    Event_ShowMessage(24, 0); Event_End();
}

/* Second-phase story selector at 0x020017c8, 60 bytes; six calls. */
void SceneDialogue_ShowLine1EDEOr1EDF(void)
{
    Event_Begin();
    if (GameFlag_IsSet(0x933)) Event_SetMessage(MSG_HAD_IDEA_THERE_WERE_MANY);
    else Event_SetMessage(MSG_WERE_SURROUNDED_BY_MONSTERS_STILL);
    Event_ShowMessage(25, 0); Event_End();
}

void FieldScene_RunPrimarySequence(s32 a0, s32 a1, s32 a2)
{
    u32 i;
    s32 record;

    Event_Begin();
    Event_SetMessage(a1);
    Event_OpenMessage(a0, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        FieldScene_RunStepThen10(a0);
        Actor_SetAnimation(a0, 2);
        record = Value1(Func_02007d24, 0);
        if (record != 0) {
            Actor_SetDestination(a0, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Actor_WaitForMove(a0);
        Actor_SetPosition(a0, 0, 0);
        GameFlag_Set(0x300);
        GameFlag_Set(a2);
    } else {
        bump_step(1);
        FieldScene_RunStepThen10(a0);
    }
    Event_End();
}

void FieldScene_RunScene3b1SequenceC(void)
{
    struct FieldActor *leader;

    leader = (struct FieldActor *)Func_02007d82(0);
    if ((u16)(leader->facing - 0x2000) > 0xc000) {
        if (GameFlag_IsSet(0x928) != 0 && GameFlag_IsSet(0x93e) == 0) {
            Sanctum_Open(17);
        } else {
            Sanctum_Open(15);
        }
    } else {
        Event_Begin();
        if (GameFlag_IsSet(0x93e) != 0) {
            Event_SetMessage(MSG_GOOD_SHIP_HAS_ARRIVED_SAFELY);
        } else if (GameFlag_IsSet(0x8a0) != 0) {
            Event_SetMessage(MSG_HATE_ARGUING);
        } else if (GameFlag_IsSet(0x928) != 0) {
            Event_SetMessage(MSG_SHIPS_CREW_READY_FOR_ANYTHING_2);
        } else if (GameFlag_IsSet(0x925) != 0) {
            Event_SetMessage(MSG_SHIPS_CREW_READY_FOR_ANYTHING);
        } else {
            Event_SetMessage(MSG_IM_SPREADING_GOODWILL_WHEREVER_TRAVEL);
        }
        if (GameFlag_IsSet(0x928) != 0 && GameFlag_IsSet(0x93e) == 0) {
            Event_ShowMessage(17, 0);
        } else {
            Event_ShowMessage(15, 0);
        }
        Event_End();
    }
}

void FieldScene_RunScene3b1SequenceD(void)
{
    u32 i;
    s32 record;

    if (GameFlag_IsSet(0x301) != 0) {
        Event_Begin();
        Func_02007ff4(8);
        Call3(Func_02007eb6, 0x1e48, 1, 8);
        Actor_SetSpeed(0, 0x19999, 0xcccc);
        Actor_WalkToAndWait(0, 0x198, 134);
        Value2(FieldScene_CallPairWith10, 0, 0x4000);
        Event_End();
    }
}

/*
 * Gated on flag 0x922.  Every callee slot has its own local call stub;
 * ConfigureSceneMotionFlags and Func_02006336_a are the same stub declared twice without
 * a prototype, because the two call sites pass different argument counts.
 */

/*
 * Scene setup for resource_3b1.  The 212-byte owner at 0x02001a60 includes
 * the alignment halfword at 0x02001b1a and the six pool words that follow it,
 * ending before the next owner's prologue at 0x02001b34.
 */
void FieldScene_RunFlagGatedThreeActorSetup(void)
{
    if (GameFlag_IsSet(0x922) == 0)
        return;

    Event_Begin();
    Func_020080ba();
    Camera_SetSpeed(0x19999, 0x3333);
    ConfigureSceneMotionFlags(0xe0 << 17, -1, 0x027e0000, 0x10000028u);
    Event_SetMessage(MSG_WONDER_COULD_HAVE_HAPPENED);

    FieldScene_RunStepThen10(8);
    FieldScene_RunStepThen10(10);
    Func_02006326(8, 0x3000);
    FieldScene_RunStepThen10(8);
    Func_02006336_a(10, 0xd000);
    FieldScene_RunStepThen10(10);
    Func_02006346(9, 0x5000);
    FieldScene_RunStepThen10(9);

    Actor_FaceDirection(8, 0, 20);
    FieldScene_RunStepThen10(8);
    Func_02006366(9, 0x8000);
    FieldScene_RunStepThen10(9);
    FieldScene_RunStepThen10(10);
    FieldScene_RunStepThen10(8);
    Func_02006382(10, 0xb000);
    FieldScene_RunStepThen10(8);

    GameFlag_Set(0x920);
    Event_End();
}

void FieldScene_RunThreeActorPresentation(void)
{
    s32 request_a;
    s32 request_b;
    s32 action;

    if (GameFlag_IsSet(0x911) == 0) {
    } else {
        Event_Begin();
        Func_0200819c();
        Camera_SetSpeed(0x26666, 0x4ccc);
        Func_02006418( 0x5b70000, -1, 0x1d00000, 0x10000014);
        Actor_RunRepeatedMotion(13, 1);
        Event_SetMessage(MSG_IF_WE_ARENT_GOING_SET);
        FieldScene_RunStepThen10(0x200d);
        Func_02006412(12, 0xd000);
        request_a = 0x800c;
        Actor_ShowEmote(12, 0x102, 20);
        Actor_StartRepeatedMotion(12, 2);
        FieldScene_RunStepThen10(request_a);
        Actor_RunRepeatedMotion(14, 1);
        Event_ShowMessageAndWait(0xa00e, 0, 20);
        Func_02006448(12, 0);
        request_b = 0xa00e;
        Actor_ShowEmote(12, 0x101, 40);
        Actor_ShowEmote(14, 0x103, 40);
        Actor_StartRepeatedMotion(14, 3);
        FieldScene_RunStepThen10(request_b);
        Actor_SetAttachedEffect(12, 0x102);
        Event_Wait(40);
        Actor_StartRepeatedMotion(12, 3);
        FieldScene_RunStepThen10(request_a);
        Actor_RunRepeatedMotion(14, 1);
        FieldScene_RunStepThen10(request_b);
        Func_020064a6(14, 0xb000);
        FieldScene_RunStepThen10(request_b);
        Func_020064b4_a(12, 0xd000);
        Actor_ShowEmote(12, 0x100, 30);
        Actor_StartRepeatedMotion(12, 1);
        FieldScene_RunStepThen10(request_a);
        Actor_SetAnimationAndWait(13, 4);
        FieldScene_RunStepThen10(0x200d);
        Actor_StartRepeatedMotion(13, 2);
        FieldScene_RunStepThen10(0x200d);
        Actor_SetAnimation(12, 4);
        FieldScene_RunStepThen10(request_a);
        Actor_SetAnimationAndWait(14, 4);
        FieldScene_RunStepThen10(request_b);
        Func_02006514(14, 0x8000);
        Actor_StartRepeatedMotion(14, 2);
        Event_ShowMessageAndWait(request_b, 0, 20);
        Actor_FaceDirection(12, 0, 0);
        Actor_ShowEmote(12, 0x102, 80);
        Event_ShowMessageAndWait(request_a, 0, 20);
        Actor_ShowEmote(14, 0x103, 0);
        Actor_ShowEmote(13, 0x103, 60);
        Actor_StartRepeatedMotion(14, 2);
        FieldScene_RunStepThen10(request_b);
        Func_02006570(14, 0xb000);
        Actor_RunRepeatedMotion(14, 1);
        FieldScene_RunStepThen10(request_b);
        Func_0200658c(13, 0x3000);
        Actor_ShowEmote(13, 0x101, 0);
        Actor_ShowEmote(12, 0x101, 60);
        Actor_RunRepeatedMotion(13, 1);
        FieldScene_RunStepThen10(13);
        Actor_ShowEmote(14, 0x103, 40);
        Actor_RunRepeatedMotion(14, 1);
        FieldScene_RunStepThen10(request_b);
        Actor_FaceDirection(12, 0xd000, 0);
        Actor_FaceDirection(13, 0x5000, 40);
        Actor_FaceDirection(12, 0, 0);
        Func_020065ee(13, 0x3000);
        Actor_RunRepeatedMotion(12, 2);
        Event_ShowMessageAndWait(request_a, 0, 20);
        Actor_FaceDirection(14, 0x4000, 40);
        FieldScene_RunStepThen10(request_b);
        Actor_StartRepeatedMotion(12, 2);
        Actor_RunRepeatedMotion(13, 2);
        Event_Wait(60);
        Actor_RunRepeatedMotion(13, 1);
        FieldScene_RunStepThen10(13);
        Actor_SetAnimationAndWait(14, 3);
        FieldScene_RunStepThen10(request_b);
        Actor_ShowEmote(12, 0x102, 40);
        Actor_RunRepeatedMotion(12, 2);
        FieldScene_RunStepThen10(request_a);
        Actor_SetAnimationAndWait(13, 3);
        FieldScene_RunStepThen10(13);
        Actor_FaceDirection(14, 0xb000, 40);
        Actor_SetAnimation(14, 3);
        Actor_SetAnimationAndWait(13, 3);
        Actor_SetSpeed(14, 0x19999, 0xcccc);
        Actor_SetSpeed(13, 0x19999, 0xcccc);
        action = (s32)Data_0200e6a8;
        Actor_EnableActionCallback(14, action);
        Actor_EnableActionCallback(13, action);
        Event_Wait(20);
        Actor_FaceDirection(12, 0x4000, 0);
        Actor_SetSpeed(0, 0x26666, 0x13333);
        *(u8 *)(Func_02008336_a(0) + 90) &= 254;
        Actor_WalkToAndWait(0, 184, 0x208);
        Event_Wait(1);
        {
            u8 *record = Func_02008358_a(0);
            u32 flag = 1;

            flag = flag | record[90];
            record[90] = (u8)flag;
        }
        Actor_FaceDirection(0, 0x8000, 20);
        Actor_FaceDirection(0, 0x4000, 0);
        Actor_Jump(12, 4, 20);
        Actor_FaceDirection(0, 0xa000, 20);
        Actor_StartRepeatedMotion(12, 2);
        FieldScene_RunStepThen10(12);
        Actor_SetSpeed(12, 0x19999, 0xcccc);
        Actor_EnableActionCallback(12, action);
        Event_Wait(40);
        Actor_FaceDirection(0, 0x4000, 0);
        Func_020083e6(12);
        GameFlag_Set(0x922);
        Event_End();
    }
}

void FieldScene_RunExtendedActorChoreography(void)
{
    s32 request_a;
    s32 request_b;
    s32 request_c;

    Audio_PlayCue(28);
    Camera_SetSpeed(0x26666, 0x4ccc);
    Func_02006814(0x1c80000, -1, 0x2880000, 0x10000014);
    Actor_RunRepeatedMotion(9, 1);
    Event_SetMessage(MSG_YOURE_TRYING_LAUNCH_SHIP);
    FieldScene_RunStepThen10(9);
    Actor_FaceDirection(0, 0xd000, 0);
    Actor_FaceDirection(10, 0xd000, 0);
    Actor_FaceDirection(11, 0, 0);
    Actor_FaceDirection(12, 0x3000, 0);
    Actor_FaceDirection(13, 0x8000, 40);
    Actor_ShowEmote(9, 0x103, 40);
    Actor_StartRepeatedMotion(9, 2);
    FieldScene_RunStepThen10(9);
    Actor_FaceDirection(12, 0, 0);
    Actor_FaceDirection(11, 0xd000, 0);
    Actor_FaceDirection(13, 0xd000, 20);
    Actor_RunRepeatedMotion(11, 1);
    FieldScene_RunStepThen10(0x100b);
    Actor_ShowEmote(13, 0x102, 20);
    Actor_StartRepeatedMotion(13, 2);
    FieldScene_RunStepThen10(13);
    Actor_ShowEmote(9, 0x105, 60);
    FieldScene_RunStepThen10(9);
    Actor_ShowEmote(12, 0x104, 20);
    Call1_02001f3c(FieldScene_RunStepThen10, 0x900c);
    Actor_RunRepeatedMotion(8, 1);
    Actor_SetAnimation(8, 3);
    FieldScene_RunStepThen10(8);
    Func_020068e0(12, 0x3000);
    Call1_02001f3c(FieldScene_RunStepThen10, 0x900c);
    Func_020068f4(11, 0xb000);
    Actor_SetAnimationAndWait(11, 3);
    Event_Wait(10);
    Actor_RunRepeatedMotion(13, 1);
    Actor_SetAnimationAndWait(13, 3);
    FieldScene_RunStepThen10(13);
    Actor_FaceDirection(13, 0x8000, 0);
    Actor_FaceDirection(12, 0x5000, 0);
    Func_0200693e(11, 0x5000);
    Actor_SetSpeed(13, 0x6666, 0x3333);
    Actor_SetSpeed(12, 0xcccc, 0x6666);
    Actor_WalkTo(12, 0x1bc, 0x29c);
    Actor_WalkToAndWait(13, 0x1d8, 0x29c);
    Actor_WaitForMove(12);
    Actor_SetAnimation(12, 1);
    Event_Wait(80);
    Func_02006990(12, 0xd000);
    Actor_ShowEmote(12, 0x101, 60);
    Actor_RunRepeatedMotion(11, 1);
    Event_Wait(20);
    Event_ShowMessageAndWait(0x400b, 0, 40);
    Actor_RunRepeatedMotion(11, 2);
    Actor_FaceDirection(11, 0xd000, 0);
    FieldScene_RunStepThen10(0x100b);
    Actor_FaceDirection(12, 0xd000, 0);
    Actor_ShowEmote(9, 0x101, 60);
    Actor_SetAnimation(11, 4);
    Event_Wait(20);
    FieldScene_RunStepThen10(0x100b);
    Actor_SetAnimation(9, 3);
    FieldScene_RunStepThen10(9);
    Actor_FaceDirection(13, 0xd000, 0);
    Actor_SetAttachedEffect(13, 0x102);
    Actor_Jump(13, 2, 20);
    FieldScene_RunStepThen10(13);
    Actor_SetAnimationAndWait(9, 3);
    request_a = 0x100c;
    FieldScene_RunStepThen10(9);
    Func_02006a3c(11, 0xd000);
    Actor_RunRepeatedMotion(12, 1);
    FieldScene_RunStepThen10(request_a);
    Actor_ShowEmote(8, 0x105, 40);
    Actor_SetAnimation(8, 3);
    FieldScene_RunStepThen10(8);
    Actor_ShowEmote(13, 0x102, 40);
    Actor_Jump(13, 4, 0);
    FieldScene_RunStepThen10(13);
    Actor_SetAnimation(9, 3);
    FieldScene_RunStepThen10(9);
    Actor_RunRepeatedMotion(11, 1);
    FieldScene_RunStepThen10(0x100b);
    Actor_ShowEmote(8, 0x102, 40);
    Event_ShowMessage(8, 0);
    Actor_StartRepeatedMotion(11, 2);
    Event_ShowMessageAndWait(0x100b, 0, 40);
    Actor_ShowEmote(9, 0x100, 0);
    Actor_FaceDirection(9, 0x5000, 20);
    Actor_StartRepeatedMotion(9, 2);
    Event_ShowMessageAndWait(9, 0, 20);
    Actor_SetAnimation(11, 3);
    Event_Wait(20);
    Actor_ShowEmote(12, 0x100, 40);
    Actor_StartRepeatedMotion(12, 2);
    request_b = 0x400b;
    FieldScene_RunStepThen10(request_a);
    Actor_FaceDirection(11, 0x5000, 20);
    FieldScene_RunStepThen10(request_b);
    Actor_StartRepeatedMotion(12, 2);
    FieldScene_RunStepThen10(request_a);
    Actor_SetAnimationAndWait(11, 3);
    Actor_RunRepeatedMotion(11, 1);
    FieldScene_RunStepThen10(request_b);
    Actor_ShowEmote(12, 0x102, 60);
    Actor_RunRepeatedMotion(9, 1);
    FieldScene_RunStepThen10(9);
    Actor_ShowEmote(11, 0x101, 40);
    Actor_FaceDirection(11, 0xd000, 20);
    Actor_SetAnimation(9, 3);
    FieldScene_RunStepThen10(9);
    Actor_ShowEmote(11, 0x103, 20);
    Actor_StartRepeatedMotion(11, 2);
    FieldScene_RunStepThen10(0x100b);
    Actor_ShowEmote(9, 0x108, 40);
    FieldScene_RunStepThen10(9);
    Actor_RunRepeatedMotion(8, 1);
    Actor_SetAnimation(8, 3);
    FieldScene_RunStepThen10(8);
    Actor_FaceDirection(9, 0xd000, 40);
    FieldScene_RunStepThen10(9);
    Actor_RunRepeatedMotion(12, 1);
    Event_Wait(20);
    FieldScene_RunStepThen10(request_a);
    Actor_FaceDirection(11, 0x5000, 0);
    Actor_FaceDirection(9, 0x5000, 0);
    Actor_FaceDirection(13, 0x8000, 0);
    Actor_FaceDirection(0, 0xc000, 0);
    Actor_FaceDirection(10, 0xb000, 40);
    Actor_RunRepeatedMotion(11, 1);
    FieldScene_RunStepThen10(request_b);
    Actor_SetAnimation(12, 3);
    Event_ShowMessageAndWait(request_a, 0, 20);
    Actor_RunRepeatedMotion(9, 2);
    FieldScene_RunStepThen10(9);
    Actor_ShowEmote(12, 0x108, 40);
    Actor_SetAnimation(12, 3);
    FieldScene_RunStepThen10(request_a);
    Actor_SetAnimationAndWait(8, 3);
    FieldScene_RunStepThen10(8);
    Actor_FaceDirection(8, 0x8000, 20);
    Audio_PlayCue(19);
    Actor_StartRepeatedMotion(8, 2);
    request_c = 0x8008;
    Actor_ShowEmote(8, 0x100, 80);
    FieldScene_RunStepThen10(request_c);
    Actor_ShowEmote(12, 0x101, 0);
    Actor_ShowEmote(11, 0x101, 0);
    Actor_ShowEmote(13, 0x101, 0);
    Actor_ShowEmote(10, 0x101, 0);
    Actor_ShowEmote(0, 0x101, 40);
    Actor_FaceDirection(12, 0xd000, 0);
    Actor_FaceDirection(11, 0xd000, 0);
    Actor_FaceDirection(13, 0xb000, 0);
    Actor_FaceDirection(10, 0xb000, 0);
    Actor_FaceDirection(0, 0xc000, 40);
    Actor_SetAttachedEffect(8, 0x102);
    Actor_Jump(8, 4, 40);
    Actor_StartRepeatedMotion(8, 2);
    Event_ShowMessage(request_c, 0);
    Actor_SetSpeed(8, 0x19999, 0xcccc);
    Actor_WalkToAndWait(8, 0x1db, 0x256);
    Actor_FaceDirection(8, 0x8000, 0);
    Actor_SetSpeed(9, 0x10000, 0x8000);
    Actor_WalkToAndWait(9, 0x1ce, 0x26a);
    Func_02006da8_a(9, 0xb000);
    Actor_ShowEmote(9, 0x100, 40);
    Actor_StartRepeatedMotion(9, 2);
    Call1_02001f3c(FieldScene_RunStepThen10, 0x8009);
    Actor_ShowEmote(11, 0x101, 60);
    FieldScene_RunStepThen10(11);
    Actor_ShowEmote(12, 0x102, 20);
    FieldScene_RunStepThen10(request_a);
    Actor_ShowEmote(8, 0x103, 20);
    Actor_Jump(8, 4, 0);
    Actor_FaceDirection(8, 0x5000, 20);
    FieldScene_RunStepThen10(8);
    Audio_PlayCue(28);
    Actor_StartRepeatedMotion(8, 3);
    FieldScene_RunStepThen10(8);
    Actor_ShowEmote(13, 0x101, 60);
    FieldScene_RunStepThen10(13);
    Func_02006e34(8, 0x3000);
    Actor_SetAnimationAndWait(8, 4);
    FieldScene_RunStepThen10(8);
    Actor_WalkToAndWait(12, 0x1bc, 0x274);
    Func_02006e58(12, 0xd000);
    Call1_02001f3c(FieldScene_RunStepThen10, 0x900c);
    Actor_FaceDirection(8, 0x5000, 20);
    Actor_SetAnimationAndWait(8, 3);
    FieldScene_RunStepThen10(8);
    Actor_ShowEmote(11, 0x102, 60);
    FieldScene_RunStepThen10(0x100b);
    Actor_ShowEmote(13, 0x107, 40);
    Actor_StartRepeatedMotion(13, 2);
    FieldScene_RunStepThen10(13);
    Func_02006ea8(9, 0x3000);
    Actor_SetAnimationAndWait(9, 4);
    Call1_02001f3c(FieldScene_RunStepThen10, 0x1009);
    Func_02006ebe(12, 0);
    Actor_RunRepeatedMotion(8, 1);
    FieldScene_RunStepThen10(8);
    Actor_FaceDirection(11, 0, 0);
    Actor_ShowEmote(12, 0x105, 0);
    Actor_ShowEmote(9, 0x105, 60);
    Camera_SetSpeed(0x13333, 0x2666);
    Func_02006f32(0x1d00000, -1, 0x2a80000, 0x10000000);
    Actor_RunRepeatedMotion(10, 1);
    Func_02006f16(10, 0);
    FieldScene_RunStepThen10(10);
    Actor_FaceDirection(0, 0, 0);
    Actor_ShowEmote(10, 0x102, 40);
    FieldScene_RunStepThen10(10);
    Actor_FaceDirection(10, 0x8000, 20);
    Actor_ShowEmote(10, 0x100, 0);
    Actor_Jump(10, 4, 40);
    FieldScene_RunStepThen10(10);
    Actor_RunRepeatedMotion(10, 1);
    FieldScene_RunStepThen10(10);
    Actor_SetSpeed(13, 0x10000, 0x8000);
    Actor_WalkTo(13, 0x1b6, 0x293);
    Actor_FaceDirection(8, 0x8000, 0);
    Actor_FaceDirection(9, 0xb000, 0);
    Actor_FaceDirection(12, 0x3000, 0);
    Actor_FaceDirection(11, 0xb000, 0);
    Audio_PlayCue(17);
    Actor_SetSpeed(10, 0x10000, 0x8000);
    Actor_WalkToAndWait(10, 0x1e8, 0x2ae);
    Actor_FaceDirection(10, 0xb000, 0);
    Actor_WaitForMove(13);
    Actor_SetAnimation(13, 1);
    Actor_FaceDirection(13, 0xd000, 0);
    Func_02008dc8();
    GameFlag_Set(0x921);
}

void FieldScene_RunBranchingActorPresentation(void)
{
    u8 *record;
    s32 request_a;
    s32 request_b;
    s32 value;
    s32 request_c;
    s32 action;

    Event_Begin();
    Func_02008e2e();
    Actor_FaceActor(8, 0, 0);
    Actor_ShowEmote(8, 0x100, 40);
    request_a = 0x1008;
    Actor_StartRepeatedMotion(8, 3);
    Event_SetMessage(MSG_ITS_MY_LUCKY_ANCHOR);
    FieldScene_RunStepThen10(request_a);
    Actor_StartRepeatedMotion(9, 1);
    Actor_StartRepeatedMotion(12, 1);
    Actor_StartRepeatedMotion(11, 1);
    Actor_StartRepeatedMotion(13, 1);
    Actor_RunRepeatedMotion(10, 1);
    Actor_FaceDirection(9, 0xd000, 0);
    Actor_FaceDirection(12, 0xd000, 0);
    Actor_FaceDirection(11, 0xd000, 0);
    Actor_FaceDirection(13, 0xd000, 0);
    Actor_FaceDirection(10, 0xb000, 20);
    Actor_RunRepeatedMotion(8, 1);
    Event_OpenMessage(request_a, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Actor_RunRepeatedMotion(9, 2);
        Call1_02001b34(FieldScene_RunStepThen10, 0x9009);
        Actor_ShowEmote(8, 0x108, 40);
        FieldScene_RunStepThen10(request_a);
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 2;
    } else {
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 2;
        Actor_RunRepeatedMotion(9, 1);
        Call1_02001b34(FieldScene_RunStepThen10, 0x9009);
        Actor_StartRepeatedMotion(8, 2);
        Call1_02001b34(FieldScene_RunStepThen10, 0x9008);
    }
    Actor_ShowEmote(13, 0x105, 40);
    Camera_SetSpeed(0xcccc, 0x1999);
    Camera_MoveTo(0x1d80000, -1, 0x27c0000, 1);
    Actor_SetSpeed(13, 0x10000, 0x8000);
    Actor_WalkToAndWait(13, 0x1d8, 0x296);
    Func_020071cc(13, 0xb000);
    FieldScene_RunStepThen10(13);
    Func_020071e0(8, 0x5000);
    Actor_SetAnimationAndWait(8, 3);
    Actor_SetAnimationAndWait(9, 3);
    Actor_FaceDirection(11, 0, 0);
    Actor_FaceDirection(13, 0x8000, 20);
    Actor_SetAnimation(11, 3);
    Actor_SetAnimationAndWait(13, 3);
    Event_Wait(20);
    Actor_RunRepeatedMotion(12, 1);
    Func_02007232_a(12, 0x3000);
    Event_ShowMessageAndWait(0x100c, 0, 20);
    Actor_FaceDirection(11, 0xb000, 20);
    Actor_ShowEmote(11, 0x101, 40);
    FieldScene_RunStepThen10(11);
    request_b = 0x900c;
    Func_02007264(12, 0xd000);
    Actor_SetAnimation(12, 4);
    FieldScene_RunStepThen10(request_b);
    Func_0200727a(13, 0xb000);
    Actor_RunRepeatedMotion(13, 1);
    FieldScene_RunStepThen10(13);
    Actor_ShowEmote(9, 0x100, 20);
    Func_0200729c(9, 0x3000);
    Actor_RunRepeatedMotion(9, 1);
    FieldScene_RunStepThen10(9);
    Actor_SetAnimationAndWait(12, 3);
    FieldScene_RunStepThen10(request_b);
    Actor_RunRepeatedMotion(8, 2);
    FieldScene_RunStepThen10(8);
    Func_020072ce_a(12, 0xd000);
    Actor_SetAnimationAndWait(12, 3);
    FieldScene_RunStepThen10(request_b);
    Actor_RunRepeatedMotion(11, 2);
    Func_020072ec(11, 0xb000);
    FieldScene_RunStepThen10(11);
    Func_020072fa(12, 0);
    FieldScene_RunStepThen10(request_b);
    Actor_FaceDirection(8, 0x3000, 0);
    Actor_FaceDirection(9, 0, 0);
    Actor_FaceDirection(11, 0xd000, 0);
    Actor_FaceDirection(13, 0xd000, 0);
    Actor_SetSpeed(0, 0x10000, 0x8000);
    Actor_WalkToAndWait(0, 0x1e6, 0x260);
    Actor_FaceDirection(0, 0x8000, 0);
    record = Pointer1(Func_02008fbe, 0);
    if (record != 0) {
        Actor_SetPosition(1, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Actor_SetSpeed(1, 0x10000, 0x8000);
    Actor_WalkToAndWait(1, 0x1e6, 0x270);
    Actor_FaceDirection(1, 0x8000, 0);
    record = Pointer1(Func_02008ffa, 1);
    if (record != 0) {
        Actor_SetPosition(2, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Actor_SetSpeed(2, 0x10000, 0x8000);
    Actor_WalkToAndWait(2, 0x1e6, 0x280);
    Actor_FaceDirection(2, 0x8000, 0);
    record = Pointer1(Func_02009036, 2);
    if (record != 0) {
        Actor_SetPosition(3, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Actor_SetSpeed(3, 0x10000, 0x8000);
    Actor_WalkToAndWait(3, 0x1e6, 0x290);
    Actor_FaceDirection(3, 0x8000, 20);
    Actor_ShowEmote(12, 0x108, 40);
    FieldScene_RunStepThen10(request_b);
    Actor_RunRepeatedMotion(9, 1);
    Call1_02001b34(FieldScene_RunStepThen10, 0x1009);
    Actor_SetAnimationAndWait(8, 3);
    Func_02007434(8, 0x5000);
    FieldScene_RunStepThen10(8);
    Func_02007442(8, 0x3000);
    Event_OpenMessage(8, 0);
    if (Event_ChooseYesNo(0, 0) == 1) {
        Actor_StartRepeatedMotion(8, 2);
        FieldScene_RunStepThen10(8);
        Actor_SetAnimationAndWait(12, 3);
        FieldScene_RunStepThen10(request_b);
        Actor_StartRepeatedMotion(9, 1);
        Event_ShowMessageAndWait(0x9009, 0, 40);
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    } else {
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 3;
        Actor_StartRepeatedMotion(8, 3);
        Event_ShowMessageAndWait(8, 0, 40);
    }
    Actor_RunRepeatedMotion(13, 1);
    FieldScene_RunStepThen10(13);
    Actor_RunRepeatedMotion(8, 1);
    value = 176;
    Func_02007508(8, 0x5000);
    FieldScene_RunStepThen10(8);
    Actor_RunRepeatedMotion(13, 1);
    Func_02007522(13, (value << 8));
    Event_ShowMessageAndWait(13, 0, 20);
    Actor_SetAnimationAndWait(8, 3);
    Actor_SetSpeed(8, 0xcccc, 0x6666);
    request_c = 0x4008;
    Actor_WalkToAndWait(8, 0x1d8, 0x278);
    FieldScene_RunStepThen10(request_c);
    Actor_ShowEmote(13, 0x103, 40);
    Actor_StartRepeatedMotion(13, 2);
    FieldScene_RunStepThen10(13);
    Actor_SetAnimation(8, 4);
    Event_ShowMessageAndWait(request_c, 0, 40);
    Actor_RunRepeatedMotion(11, 1);
    Func_0200758e(11, (value << 8));
    Call1_02001b34(FieldScene_RunStepThen10, 0x100b);
    Actor_ShowEmote(10, 0x102, 20);
    Actor_SetSpeed(10, 0x26666, 0x13333);
    Actor_Jump(10, 2, 0);
    Actor_WalkToAndWait(10, 0x1ce, 0x2a2);
    Func_020075c8(10, (value << 8));
    Actor_StartRepeatedMotion(10, 2);
    FieldScene_RunStepThen10(10);
    Func_020075de(9, 0x5000);
    Actor_SetAnimationAndWait(9, 4);
    FieldScene_RunStepThen10(9);
    Actor_SetAnimationAndWait(8, 3);
    FieldScene_RunStepThen10(request_c);
    Actor_ShowEmote(13, 0x102, 40);
    Event_ShowMessageAndWait(13, 0, 40);
    Func_0200761a(9, 0x3000);
    Actor_StartRepeatedMotion(9, 2);
    Call1_02001b34(FieldScene_RunStepThen10, 0x1009);
    Func_02007630(12, 0);
    Actor_FaceDirection(8, 0x8000, 0);
    Actor_FaceDirection(9, 0x5000, 0);
    Actor_FaceDirection(11, (value << 8), 0);
    Actor_FaceDirection(13, (value << 8), 0);
    Actor_FaceDirection(10, (value << 8), 20);
    Actor_RunRepeatedMotion(12, 1);
    Event_ShowMessageAndWait(0x100c, 0, 20);
    Actor_ShowEmote(8, 0x101, 40);
    Func_0200768a(8, 0xd000);
    Event_OpenMessage(0x1008, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Actor_SetAnimationAndWait(8, 3);
        Call1_02001b34(FieldScene_RunStepThen10, 0x1008);
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    } else {
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
        Call1_02001b34(FieldScene_RunStepThen10, 0x1008);
    }
    Actor_SetAnimationAndWait(0, 3);
    Actor_SetAnimationAndWait(8, 3);
    Call1_02001b34(FieldScene_RunStepThen10, 0x1008);
    Call2(FieldScene_CallPairWith10, 8, 0x8000);
    Call1_02001b34(FieldScene_RunStepThen10, 0x4008);
    Func_0200776c(2, 0, 0);
    Actor_SetAnimation(12, 3);
    Actor_SetAnimation(11, 3);
    Actor_SetAnimation(9, 3);
    Actor_StartRepeatedMotion(10, 2);
    Actor_RunRepeatedMotion(13, 2);
    Event_Wait(20);
    action = (s32)Data_0200e6e4;
    Actor_EnableActionCallback(10, action);
    Event_Wait(4);
    Actor_EnableActionCallback(11, action);
    Event_Wait(4);
    Actor_EnableActionCallback(12, action);
    Event_Wait(4);
    Actor_EnableActionCallback(9, action);
    Actor_SetAnimation(3, 2);
    record = Pointer1(Func_020093dc_a, 2);
    if (record != 0) {
        Actor_SetDestination(3, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Actor_WaitForMove(3);
    Actor_SetPosition(3, 0, 0);
    Actor_SetAnimation(2, 2);
    record = Pointer1(Func_0200940c, 1);
    if (record != 0) {
        Actor_SetDestination(2, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Actor_WaitForMove(2);
    Actor_SetPosition(2, 0, 0);
    Actor_SetAnimation(1, 2);
    record = Pointer1(Func_0200943c, 0);
    if (record != 0) {
        Actor_SetDestination(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Actor_WaitForMove(1);
    Actor_SetPosition(1, 0, 0);
    Actor_EnableActionCallback(13, action);
    Actor_WalkToAndWait(8, 0x1c8, 0x288);
    Func_02007814(8, 0);
    Func_02009452(232);
    GameFlag_Set(0x925);
    Event_End();
}

/*
 * A four-way gated cascade. Each gate either runs its own setter sequence
 * and returns or falls through to the next, and all four share one exit.
 * The 336-byte owner at 0x0200351c includes its eight trailing pool words.
 */
void SceneState_RunFlagGatedSetupCascade(void)
{
    if (GameFlag_IsSet((s32)&Value_0000093e) != 0) {
        Actor_SetPosition(8, 0, 0);
        Actor_SetPosition(9, 0, 0);
        Actor_SetPosition(10, 0, 0);
        Actor_SetPosition(11, 0, 0);
        Actor_SetPosition(12, 0, 0);
        FieldScene_RunSceneStep(14, 0, 0);
        return;
    }

    if (GameFlag_IsSet(0x8a0) != 0) {
        Func_02007e0e(8, 0x98, 0x1bc, 0x3000);
        Engine_ActorEnableActionCallback(8, 0x0200e958);
        Func_02007e32(10, 0xb8, 0x1e0, 0xb000);
        Func_02007e40(12, 0xaa, 0x1e8, 0xb000);
        Func_02007e4c(13, 0x88, 0x1e8, 0xd000);
        Func_02007e58(15, 0x78, 0x1e0, 0xd000);
        Func_02007e64(14, 0xb8, 0x20e, 0xb000);
        Func_02007e74(11, 0x88, 0x248, 0x8000);
        Engine_ActorEnableActionCallback(11, 0x0200e840);
        return;
    }

    {
        s32 t = GameFlag_IsSet((s32)&Value_00000928);
        if (t != 0) {
            SceneState_ApplyActor8FourFlags(t);
            return;
        }
    }

    if (GameFlag_IsSet((s32)&Value_00000925) != 0) {
        FieldScene_RunSceneStep(18, 0, 0);
        return;
    }

    if (GameFlag_IsSet((s32)&Value_00000911) != 0 &&
        GameFlag_IsSet((s32)&Value_00000922) != 0) {
        Func_02007f1a_a(14, 0, 0);
        Actor_SetPosition(12, 0, 0);
    }
}

void RunSceneSelectionChain(void)
{
    Task_Wait(1);
    SceneActor_SetFlagBit3ForActors28To35();
    if (GameFlag_IsSet(2366) != 0) {
        Func_02007f72(4, 4, 0);
        Func_02007f2a(8, 412, 222, 12288);
        OverlayObject_SetPositionAndHeading(9, 458, 161, 32768);
    } else {
        if (GameFlag_IsSet(2208) != 0) {
            Actor_SetPosition(8, 30932992, 9961472);
            Actor_SetAnimation(9, 5);
            FieldScene_RunSceneStep(4, 4, 0);
        } else {
            if (GameFlag_IsSet(2347) != 0) {
                Func_02007fd6(16, 0, 0);
                FieldScene_RunSceneStep(4, 4, 0);
                Func_02007952(3);
            } else {
                if (GameFlag_IsSet(2346) != 0) {
                    Func_02007ffc(16, 0, 0);
                    FieldScene_RunSceneStep(4, 3, 0);
                    Func_02007978(2);
                } else {
                    if (GameFlag_IsSet(2345) != 0) {
                        Func_02008022(16, 0, 0);
                        FieldScene_RunSceneStep(4, 2, 0);
                        Func_0200799e(1);
                    } else {
                        if (GameFlag_IsSet(2344) != 0) {
                            Func_02008048(16, 0, 0);
                            Actor_SetPosition(10, 0, 0);
                            Func_020079c4(0);
                        } else {
                            Actor_SetAnimation(9, 5);
                            if (GameFlag_IsSet(2341) != 0 && GameFlag_IsSet(2342) == 0) {
                                FieldScene_RunFourActorCoordinatePresentation();
                            }
                        }
                    }
                }
            }
        }
    }
}

void SceneActor_SetFlagBit3ForActors28To35(void)
{
    u32 i;
    u32 bit;
    u32 zero;

    i = 28;
    bit = 8;
    zero = 0;
    for (; i <= 35; i++) {
        u8 *obj = Func_02009ca8(i);
        u32 v = obj[0x59];
        obj[0x59] = (u8)((v | bit) | zero);
    }
}

void FieldScene_RunFourActorCoordinatePresentation(void)
{
    u8 *record;
    s32 mode;

    Event_Begin();
    Func_020081ae(25, 0, 0);
    Func_020081b8(24, 1, 0);
    Func_0200818e(0x1b80000, -1, 0xa80000, 0x1000001);
    Func_02008188_a(27, 0x1b8, 164, 0x5000);
    Func_0200819e(8, 0x1ac, 190, 0xd000);
    OverlayObject_SetPositionAndHeading(9, 0x1c4, 190, 0xb000);
    Actor_SetAnimation(9, 1);
    mode = 128;
    Func_020081c6_a(0, 0x1b8, 134, 0x8000);
    *(s32 *)((*(s32 *)0x03001ebc + 0x1c0)) = (mode << 1);
    Event_OpenScreen();
    Actor_SetSpeed(0, 0xcccc, 0x6666);
    Actor_WalkToAndWait(0, 0x198, 134);
    Actor_WalkToAndWait(0, 0x198, 148);
    Actor_WalkToAndWait(0, 0x1a8, 148);
    Actor_FaceDirection(0, 0x4000, 20);
    Actor_RunRepeatedMotion(27, 1);
    Event_SetMessage(MSG_OUR_REPLACEMENT_NEVER_ARRIVED_BUT);
    FieldScene_RunStepThen10(27);
    Actor_RunRepeatedMotion(8, 1);
    FieldScene_RunStepThen10(8);
    Actor_SetAnimationAndWait(27, 3);
    FieldScene_RunStepThen10(27);
    Func_0200823c(27, 0xd000);
    record = Pointer1_020038ac(Func_02009eaa, 0);
    if (record != 0) {
        Actor_SetPosition(1, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Actor_SetSpeed(1, 0xcccc, 0x6666);
    Actor_WalkToAndWait(1, 0x1b8, 148);
    Actor_FaceDirection(1, 0x4000, 0);
    record = Pointer1_020038ac(Func_02009ede, 1);
    if (record != 0) {
        Actor_SetPosition(2, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Actor_SetSpeed(2, 0xcccc, 0x6666);
    Actor_WalkToAndWait(2, 0x1c8, 148);
    Actor_FaceDirection(2, 0x4000, 0);
    record = Pointer1_020038ac(Func_02009f14_a, 2);
    if (record != 0) {
        Actor_SetPosition(3, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Actor_SetSpeed(3, 0xcccc, 0x6666);
    Actor_WalkToAndWait(3, 0x1d8, 148);
    Actor_FaceDirection(3, 0x4000, 20);
    Func_02008352(0, 0, 60);
    Func_0200835c_a(1, 0x4000, 20);
    Func_02008366_a(2, 1, 20);
    Actor_FaceDirection(27, 0x5000, 20);
    FieldScene_RunStepThen10(27);
    Actor_StartRepeatedMotion(9, 1);
    Actor_ShowEmote(9, (mode << 1), 40);
    FieldScene_RunStepThen10(9);
    Actor_StartRepeatedMotion(1, 3);
    Actor_ShowEmote(1, 0x103, 60);
    Actor_SetAnimationAndWait(27, 3);
    FieldScene_RunStepThen10(27);
    Actor_RunRepeatedMotion(10, 1);
    Actor_SetAnimation(10, 3);
    FieldScene_RunStepThen10(10);
    Actor_SetAnimation(8, 3);
    Actor_SetAnimation(9, 3);
    Actor_SetAnimation(11, 3);
    Actor_SetAnimation(12, 3);
    Actor_SetAnimationAndWait(13, 3);
    Func_020083f6(0, 0, 40);
    Func_02008400(2, 1, 0);
    FieldScene_RunSceneStep(1, 0x4000, 20);
    Actor_SetAnimationAndWait(27, 4);
    FieldScene_RunStepThen10(27);
    Actor_ShowEmote(8, 0x102, 60);
    Actor_StartRepeatedMotion(8, 1);
    FieldScene_RunStepThen10(8);
    Actor_SetAnimationAndWait(27, 3);
    FieldScene_RunStepThen10(27);
    Actor_FaceDirection(8, 0, 0);
    Actor_FaceDirection(9, 0x8000, 40);
    Actor_ShowEmote(8, 0x102, 0);
    Actor_ShowEmote(8, 0x102, 40);
    Actor_RunRepeatedMotion(27, 1);
    Actor_SetAnimation(27, 3);
    Event_ShowMessageAndWait(27, 0, 20);
    Actor_SetAnimation(8, 3);
    Actor_SetAnimationAndWait(9, 3);
    Event_Wait(40);
    Actor_ShowEmote(9, (mode << 1), 20);
    Func_02008446(9, 0xb000);
    FieldScene_RunStepThen10(9);
    Func_02008456(27, 0x3000);
    Actor_ShowEmote(27, 0x101, 60);
    Event_ShowMessageAndWait(27, 0, 60);
    Actor_ShowEmote(27, 0x106, 20);
    Func_0200847e(27, 0xb000);
    Actor_SetAnimationAndWait(27, 3);
    FieldScene_RunStepThen10(27);
    Func_020084fe(3, 2, 80);
    Func_0200849e(8, 0xd000);
    Actor_StartRepeatedMotion(8, 2);
    FieldScene_RunStepThen10(8);
    Actor_SetAnimationAndWait(9, 3);
    Actor_StartRepeatedMotion(9, 2);
    FieldScene_RunStepThen10(9);
    Func_020084ca(27, 0x5000);
    Actor_SetAnimationAndWait(27, 3);
    Actor_RunRepeatedMotion(27, 1);
    FieldScene_RunStepThen10(27);
    Actor_SetSpeed(27, 0xcccc, 0x6666);
    Actor_WalkToAndWait(27, 0x198, 158);
    Actor_WalkToAndWait(27, 0x198, 148);
    Actor_FaceDirection(27, 0, 20);
    Actor_RunRepeatedMotion(27, 1);
    FieldScene_RunStepThen10(27);
    Func_0200858c(1, 0x8000, 20);
    Func_02008596(2, 1, 0);
    Actor_WalkToAndWait(27, 0x198, 134);
    Actor_WalkTo(27, 0x1b8, 134);
    Event_Wait(40);
    Func_020085dc_a(9, 10, 0);
    GameFlag_Set(0x926);
}

void FieldScene_RunScene3b1_02003d10(void)
{
    u32 i;
    s32 rec7;
    s32 record;

    Event_Begin();
    Func_02008606(15, 0, 1);
    Actor_RunRepeatedMotion(8, 1);
    Event_Wait(20);
    Actor_SetSpeed(8, 0xcccc, 0x6666);
    Actor_WalkToAndWait(8, 0x1d4, 0x266);
    Actor_WalkToAndWait(8, 0x1d8, 0x254);
    Actor_FaceDirection(8, 0x8000, 20);
    Actor_Jump(8, 4, 20);
    rec7 = Value0(Func_0200833a);
    Event_Wait(20);
    Audio_PlayCue(214);
    Engine_ObjectSetScript(rec7, 0x200e738);
    Event_Wait(40);
    Actor_SetAnimationAndWait(8, 3);
    Event_Wait(20);
    Actor_WalkToAndWait(8, 0x1d2, 0x270);
    Value2(FieldScene_CallPairWith10, 8, 0x5000);
    Actor_StartRepeatedMotion(8, 2);
    Event_SetMessage(MSG_CAST_OFF);
    Event_ShowMessageAndWait(8, 0, 20);
    Func_020086b6(9, 11, 0);
}

void FieldScene_RunScene3b1_02003dec(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    Func_020086e2(15, 1, 1);
    Actor_FaceDirection(8, 0x5000, 40);
    Actor_StartRepeatedMotion(8, 2);
    Event_SetMessage(MSG_WERE_OFF);
    Event_ShowMessageAndWait(8, 0, 20);
    Func_02008710(9, 11, 0);
}

void FieldScene_RunScene3b1_02003e34(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    Func_0200872a(24, 0, 0);
    Func_02008734(18, 0, 0);
    Actor_SetPosition(0, 0, 0);
    Actor_SetPosition(16, 0x960000, 0x24a0000);
    Call4(Func_02008720, 0x9c0000, -1, 0x2180000, 0x1000001);
    Func_02008766(8, 0, 0);
    Actor_SetSpeed(16, 0xcccc, 0x6666);
    Actor_WalkToAndWait(16, 168, 0x242);
    Actor_WalkToAndWait(16, 168, 0x22a);
    Actor_FaceDirection(16, 0x8000, 20);
    Actor_StartRepeatedMotion(16, 2);
    Event_SetMessage(MSG_ROW_THOSE_OARS);
    Event_ShowMessageAndWait(16, 0, 20);
    Func_020087b2(9, 12, 0);
}

void FieldScene_RunScene3b1_02003eec(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    Actor_SetPosition(0, 0, 0);
    Func_02008f02();
    Actor_SetPosition(18, 0x960000, 0x24a0000);
    Call4(Func_020087c8, 0x9c0000, -1, 0x2180000, 0x1000001);
    Func_0200880e(8, 0, 0);
    Actor_SetSpeed(18, 0xcccc, 0x6666);
    Actor_WalkToAndWait(18, 168, 0x242);
    Actor_WalkToAndWait(18, 168, 0x22a);
    Actor_FaceDirection(18, 0x8000, 20);
    Actor_StartRepeatedMotion(18, 2);
    Event_SetMessage(MSG_ROW_THOSE_OARS);
    Event_ShowMessageAndWait(18, 0, 20);
    Func_0200885a(9, 12, 0);
}

/*
 * A flat setter cascade, one workspace-slot store, then a four-way gated
 * chain ending in an unconditional default arm. The store spells both its
 * offset and its stored value as 224 << 1 rather than folded constants.
 * The 340-byte owner at 0x02003f94 includes its trailing pool words.
 */
void FieldScene_RunFlagBranchedSetupCascade(void)
{
    extern u8 *Data_03001ebc;

    Event_Begin();
    Actor_SetAnimation(9, 5);
    Func_02008892(24, 1, 0);
    Actor_SetPosition(0, 0, 0);
    FieldScene_RunSceneStep(17, 0, 0);
    FieldScene_InstallFlaggedActors10To17(0);
    Func_020088b6(8, 1, 20);
    Camera_SetSpeed(0x6666, 0xccc);
    Camera_MoveTo(0x1b80000, -1, 0xb00000, 1);
    Event_Wait(20);
    Actor_SetAnimation(9, 7);
    Event_Wait(30);
    Audio_PlayCue(0xbc);
    Event_Wait(30);
    FieldScene_InstallFlaggedActors10To17(16);
    Event_Wait(0x50);
    FieldScene_InstallFlaggedActors10To17(0);
    Event_Wait(0x3c);
    Actor_SetAnimation(9, 7);
    Event_Wait(30);
    Audio_PlayCue(0xbc);
    Event_Wait(30);
    FieldScene_InstallFlaggedActors10To17(16);
    Event_Wait(0x50);
    FieldScene_InstallFlaggedActors10To17(0);
    Event_Wait(0x5a);
    Audio_PlayCue(0xbc);
    Event_Wait(30);

    *(u32 *)(Data_03001ebc + (224 << 1)) = (224 << 1) + 67;

    FieldScene_RunSceneStep(9, 0, 0);

    if (GameFlag_IsSet((s32)&Value_0000092b) != 0) {
        Event_RequestExit(20);
    } else if (GameFlag_IsSet((s32)&Value_0000092a) != 0) {
        Event_RequestExit(18);
    } else if (GameFlag_IsSet((s32)&Value_00000929) != 0) {
        Event_RequestExit(17);
    } else if (GameFlag_IsSet((s32)&Value_00000928) != 0) {
        Event_RequestExit(16);
    } else {
        Event_RequestExit(13);
    }
}

void FieldScene_RunScene3b1_020040e8(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    Func_020089de(15, 1, 1);
    Actor_RunRepeatedMotion(8, 1);
    Event_Wait(10);
    Actor_FaceDirection(8, 0x3000, 20);
    Actor_StartRepeatedMotion(8, 2);
    Event_SetMessage((s32)Data_00001e40);
    Event_ShowMessageAndWait(8, 0, 20);
    Func_02008a1a(9, 14, 0);
}

void FieldScene_RunScene3b1_0200413c(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    Camera_MoveTo(-1, -1, -1, 0);
    Task_Wait(1);
    Func_02008a4a(15, 1, 1);
    Actor_RunRepeatedMotion(8, 1);
    Event_SetMessage(MSG_IM_TURNING);
    FieldScene_RunStepThen10(8);
    Actor_FaceDirection(8, 0xd000, 40);
    Func_02008a74(9, 15, 0);
}

void FieldScene_RunScene3b1_02004198(void)
{
    u32 i;
    s32 record;
    s32 base5_200e8e4;

    Event_Begin();
    Func_02008a8e(24, 1, 0);
    Func_02008a98(25, 1, 0);
    SceneActor_SetFlagBit3ForActors28To35();
    Func_02008aa6(19, 11, 12);
    Actor_SetAnimation(10, 6);
    Value2_02004198(Engine_ActorEnableActionCallback, 12, 0x200e840);
    base5_200e8e4 = (s32)Data_0200e8e4;
    Actor_EnableActionCallback(36, base5_200e8e4);
    Value2_02004198(Engine_ActorEnableActionCallback, 37, base5_200e8e4);
    Value2_02004198(Engine_ActorEnableActionCallback, 38, base5_200e8e4);
    Actor_SetChildValue(36, 3);
    Actor_SetChildValue(37, 3);
    Actor_SetChildValue(38, 3);
    Func_020092e8();
    Event_End();
}

/*
 * A flat setter sequence, no branches; the final call takes no arguments.
 * The 60-byte owner at 0x02004218 includes its one pool word, the address
 * taken as Value_00000928.
 */
void FieldScene_RunActors24And25Setup(void)
{
    Event_Begin();
    Func_02008b0e(24, 1, 0);
    Func_02008b18(25, 0, 0);
    FieldScene_RunSceneStep(19, 11, 12);
    FieldScene_RunFormationAndEffectPresentation();
    GameFlag_Set((s32)&Value_00000928);
    Event_End();
}

/*
 * Four "install or fallback" blocks, each gated by a flag check, then four
 * unconditional installs.  src is threaded through every block unmodified,
 * and blocks two and four store the 0xffff0000 sentinel into the installed
 * object's +24 field.  The owner includes the trailing pool words that hold
 * the flag addresses and the sentinel.  Callees are named by the address
 * their call site computes, not by a runtime address.
 */
void FieldScene_InstallFlaggedActors10To17(u8 *src)
{
    if (GameFlag_IsSet((s32)&Value_00000928) != 0) {
        u8 *obj = Func_0200962c(0, 0);
        Actor_SetPosition(obj, 0xcd << 17, 0xac << 16);
        Func_02008f84(7, obj, src);
        Actor_SetPosition(10, 0, 0);
    } else {
        FieldScene_RunSceneStep(5, 10, src);
    }

    if (GameFlag_IsSet((s32)&Value_00000929) != 0) {
        u8 *obj = Func_0200966c(1, 0);
        Actor_SetPosition(obj, 0xeb << 17, 0xac << 16);
        *(u32 *)(Func_0200abc0(obj) + 24) = 0xffff0000;
        FieldScene_RunSceneStep(7, obj, src);
        Actor_SetPosition(11, 0, 0);
    } else {
        FieldScene_RunSceneStep(6, 11, src);
    }

    if (GameFlag_IsSet((s32)&Value_0000092a) != 0) {
        u8 *obj = Func_020096b6(2, 0);
        Actor_SetPosition(obj, 0xcd << 17, 0xcc << 16);
        Func_0200900e(7, obj, src);
        Actor_SetPosition(12, 0, 0);
    } else {
        FieldScene_RunSceneStep(5, 12, src);
    }

    if (GameFlag_IsSet((s32)&Value_0000092b) != 0) {
        u8 *obj = Func_020096f6(3, 0);
        Actor_SetPosition(obj, 0xeb << 17, 0xcc << 16);
        *(u32 *)(Func_0200ac4a(obj) + 24) = 0xffff0000;
        FieldScene_RunSceneStep(7, obj, src);
        Actor_SetPosition(13, 0, 0);
    } else {
        FieldScene_RunSceneStep(6, 13, src);
    }

    Func_02009078(5, 14, src);
    Func_02009082(6, 15, src);
    Func_0200908c(5, 16, src);
    Func_02009096(6, 17, src);
}

/*
 * Both callees live inside this overlay and are declared without a prototype,
 * so each call site fixes its own arity.
 */

/*
 * Resource 3b1 unindexed helper at 0x02004880 (16 bytes, 69 calls).
 *
 * Derived span: no inventory row (item 28). `push {lr}` at 0x02004880,
 * epilogue `pop {r0} / bx r0` at 0x0200488c-0x0200488e returns void.
 * Adjoins 0x0200486c below and 0x02004890 above with no pad or pool on
 * either side.
 *
 * The second argument is masked to 16 bits (`lsls`/`lsrs` #16) before the
 * call, so it is genuinely `u16`, not a truncated `s32`.
 */

/*
 * Shared helper for resource_3b1.  The owner at 0x0200486c is 18 bytes; the
 * halfword before the next owner at 0x02004880 is alignment, not part of it.
 */
void FieldScene_RunStepThen10(s32 a)
{
    Event_ShowMessage(a, 0);
    Event_Wait(10);
}

void FieldScene_CallPairWith10(s32 a, u16 b)
{
    Actor_FaceDirection(a, b, 10);
}

/*
 * Resource 3b1 unindexed helper at 0x02004890 (28 bytes, 55 calls).
 *
 * Derived span: no inventory row (item 28). `push {r5,r6,lr}` at
 * 0x02004890, epilogue `pop {r5,r6} / pop {r0} / bx r0` at
 * 0x020048a6-0x020048aa returns void. Adjoins 0x02004880 below and
 * 0x020048ac above with no pad or pool.
 *
 * `b` and `c` are shifted left 16 (16.16 fixed point) before the first
 * call but not masked, so they stay full `s32`. `Func_0200ad8a` returns a
 * pointer whose halfword at offset 6 is then set to `d`.
 */
void OverlayObject_SetPositionAndHeading(void *a, s32 b, s32 c, s32 d)
{
    Func_0200ade4(a, b << 16, c << 16, d);
    *(s16 *)((u8 *)Func_0200ad8a(a) + 6) = d;
}

void ConfigureSceneMotionFlags(s32 x, s32 y, s32 z, u32 flags)
{
    u32 selected;

    Camera_MoveTo(x, y, z, ~flags & 1);
    selected = flags & 0x1111;
    if ((flags & 0x10000000) != 0)
        Camera_WaitForMove();
    if ((flags & 0x01000000) != 0)
        Map_Redraw();
    Event_Wait(selected);
}

void FieldScene_RunSceneStep(s32 step, u32 arg, u32 opt)
{
    extern const s32 Data_0200e840[];

    u32 slot;

    switch (step) {
    case 0:
        Actor_FaceDirection(0, 0, 0);
        Actor_FaceDirection(1, 0x8000, 0);
        Value_0200e5d0(2, 0, 0);
        Actor_FaceDirection(3, 0x8000, opt);
        break;
    case 1:
        Actor_FaceDirection(0, arg, 0);
        Actor_FaceDirection(1, arg, 0);
        Actor_FaceDirection(2, arg, 0);
        Actor_FaceDirection(3, arg, opt);
        break;
    case 2:
        Actor_SetAnimation(0, 3);
        Actor_SetAnimation(1, 3);
        Actor_SetAnimation(2, 3);
        Actor_SetAnimation(3, 3);
        if (arg != 0) {
            Func_0200af4e(3);
        }
        if (opt == 0) {
            break;
        }
        Event_Wait(opt);
        break;
    case 3:
        Actor_SetAttachedEffect(0, 0x102);
        Actor_SetAttachedEffect(1, 0x102);
        Actor_SetAttachedEffect(2, 0x102);
        Actor_SetAttachedEffect(3, 0x102);
        Event_Wait(opt);
        break;
    case 4:
        for (slot = 0; slot < arg; slot++) {
            Actor_SetPosition(slot + 10, 0, 0);
        }
        break;
    case 5:
        {
            u8 *rec;

            rec = Func_0200af28(arg);
            SetPose(rec, 0x5000);
        }
        Actor_SetAnimation(arg, 5);
        Func_0200afc2(arg, opt);
        break;
    case 6:
        {
            u8 *rec;

            rec = Func_0200af38(arg);
            SetPose(rec, 0x5000);
            *(s32 *)(rec + 24) = -0x10000;
        }
        Actor_SetAnimation(arg, 5);
        Func_0200afc2(arg, opt);
        break;
    case 7:
        {
            u8 *rec;

            rec = Value_0200e4e8(arg);
            SetPose(rec, 0x5000);
        }
        Func_0200924a(arg);
        if (opt == 0) {
            Func_0200afe4(arg, 0);
        }
        break;
    case 8:
        gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
        Event_OpenScreen();
        if (arg != 0) {
            Value_0200e640();
        }
        Event_Wait(0);
        break;
    case 9:
        Event_CloseScreen();
        Event_WaitForScreen();
        if (arg == 0) {
            break;
        }
        Event_RequestExit(arg);
        break;
    case 10:
        FieldScene_RunSceneStep(24, 1, 0);
        FieldScene_RunSceneStep(25, 0, 0);
        Func_020082ba(0);
        OverlayObject_SetPositionAndHeading(0, 0x1b0, 168, 0x4000);
        OverlayObject_SetPositionAndHeading(1, 0x1c0, 168, 0x4000);
        OverlayObject_SetPositionAndHeading(2, 0x1a8, 152, 0x4000);
        OverlayObject_SetPositionAndHeading(3, 0x1ca, 152, 0x4000);
        break;
    case 11:
        if (arg != 0) {
            u8 *rec;

            Actor_SetAnimation(13, 1);
            rec = Func_0200b018(13);
            SetPose(rec, 0x3000);
            rec = Func_0200b024(13);
            *(s32 *)(rec + 24) = 0x10000;
        }
        {
            u8 *rec;

            Actor_SetAnimation(14, 1);
            rec = Func_0200b038(14);
            SetPose(rec, 0x5000);
            Actor_SetAnimation(15, 1);
            rec = Func_0200b050(15);
            SetPose(rec, 0x3000);
            rec = Func_0200b060(15);
            *(s32 *)(rec + 24) = 0x10000;
            Actor_SetAnimation(16, 1);
            rec = Func_0200b074(16);
            SetPose(rec, 0x5000);
            Value_0200e550(17, 1);
            rec = Func_0200b086(17);
            SetPose(rec, 0x3000);
            rec = Func_0200b090(17);
            *(s32 *)(rec + 24) = 0x10000;
        }
        Actor_SetPosition(28, 0x19a0000, 0xae0000);
        Actor_SetPosition(29, 0x1d60000, 0xae0000);
        Actor_SetPosition(30, 0x19a0000, 0xce0000);
        Actor_SetPosition(31, 0x1d60000, 0xce0000);
        Actor_SetPosition(32, 0x19a0000, 0x11e0000);
        Actor_SetPosition(33, 0x1d60000, 0x11e0000);
        Actor_SetPosition(34, 0x19a0000, 0x13c0000);
        Actor_SetPosition(35, 0x1d60000, 0x13c0000);
        Task_Wait(1);
        if (arg != 0) {
            Actor_FaceDirection(13, 0xb000, 0);
        }
        Call3(Value_0200e5d0_b, 14, 0xd000, 0);
        Actor_FaceDirection(15, 0xb000, 0);
        Actor_FaceDirection(16, 0xd000, 0);
        FieldScene_CallPairWith10(17, 0xb000);
        break;
    case 12:
        {
            u8 *rec;

            rec = Func_0200b14e(arg);
            Actor_SetAnimation(arg, 1);
            if (opt != 0) {
                SetPose(rec, 0x3000);
            } else {
                SetPose(rec, 0x5000);
            }
            *(s32 *)(rec + 24) = 0x10000;
        }
        break;
    case 13:
        Actor_SetPosition(9, 0, 0);
        Actor_SetPosition(12, 0, 0);
        Actor_SetPosition(11, 0, 0);
        Actor_SetPosition(13, 0, 0);
        Actor_SetPosition(10, 0, 0);
        break;
    case 14:
        Actor_SetPosition(14, 0, 0);
        Actor_SetPosition(13, 0, 0);
        break;
    case 15:
        FieldScene_RunSceneStep(24, 1, 0);
        Actor_SetPosition(9, 0, 0);
        Actor_SetPosition(10, 0, 0);
        OverlayObject_SetPositionAndHeading(8, 0x1bc, 0x266, 0xd000);
        Actor_SetPosition(0, 0, 0);
        if (arg != 0) {
            Func_020092e8_a();
        }
        ConfigureSceneMotionFlags(0x1c00000, 0x200000, 0x2700000, 0x1000001);
        if (opt == 0) {
            break;
        }
        gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
        Event_OpenScreen();
        Event_WaitForScreen();
        Value_0200e4c0(20);
        break;
    case 16:
        Actor_SetPosition(8, 0, 0);
        Actor_SetPosition(9, 0, 0);
        Actor_SetPosition(27, 0x1b60000, 0x980000);
        break;
    case 17:
        slot = 0;
        do {
            Actor_SetPosition(slot + 28, 0, 0);
            slot++;
        } while (slot <= 7);
        break;
    case 18:
        OverlayObject_SetPositionAndHeading(12, 152, 0x214, 0xb000);
        OverlayObject_SetPositionAndHeading(8, 134, 0x1ea, 0x3000);
        OverlayObject_SetPositionAndHeading(9, 166, 0x1ea, 0x5000);
        OverlayObject_SetPositionAndHeading(10, 182, 0x1f8, 0x5000);
        OverlayObject_SetPositionAndHeading(11, 118, 0x1f8, 0x3000);
        FieldScene_RunSceneStep(14, 0, 0);
        break;
    case 19:
        OverlayObject_SetPositionAndHeading(8, 0x1a0, 0x148, 0);
        OverlayObject_SetPositionAndHeading(9, 0x1c0, 0x160, 0xd000);
        OverlayObject_SetPositionAndHeading(10, 0x1c6, 248, 0x3000);
        OverlayObject_SetPositionAndHeading(arg, 0x198, 0x122, 0);
        OverlayObject_SetPositionAndHeading(opt, 0x198, 0x156, 0);
        OverlayObject_SetPositionAndHeading(13, 0x1a4, 0x164, 0xd000);
        OverlayObject_SetPositionAndHeading(14, 0x198, 0x130, 0);
        OverlayObject_SetPositionAndHeading(15, 0x1a2, 0x17a, 0xd000);
        OverlayObject_SetPositionAndHeading(16, 0x1b8, 0x106, 0x3000);
        OverlayObject_SetPositionAndHeading(17, 0x1c0, 0x17a, 0xd000);
        break;
    case 20:
        for (slot = arg; slot <= opt; slot++) {
            GameFlag_Clear(slot);
        }
        break;
    case 21:
        FieldScene_RunSceneStep(20, 0x92c, 0x93d);
        FieldScene_RunSceneStep(20, 0x917, 0x91f);
        FieldScene_RunSceneStep(20, 0x990, 0x998);
        GameFlag_Clear(0x300);
        GameFlag_Clear(0x301);
        GameFlag_Clear(0x302);
        break;
    case 22:
        Task_Wait(1);
        FieldScene_RunSceneStep(23, 0, 0);
        Actor_EnableActionCallback(12, Data_0200e840);
        break;
    case 23:
        Actor_Destroy(1);
        Actor_Destroy(2);
        Actor_Destroy(3);
        break;
    case 24:
        Camera_MoveTo(-1, -1, -1, 0);
        Task_Wait(1);
        if (arg != 0) {
            *(u8 *)(Func_0200b52c() + 0x55) = 0;
        }
        break;
    case 25:
        Func_0200b402(Data_0200f2a0);
        Task_Wait(1);
        if (arg == 1) {
            Func_0200b41a(Data_0200f300);
            Task_Wait(1);
        } else if (arg == 2) {
            Func_0200b41a(Data_0200f360);
            Task_Wait(1);
        } else if (arg == 3) {
            Func_0200b42c(Data_0200f3c0);
            Task_Wait(1);
        }
        break;
    }
}

/*
 * A flat setter sequence, no branches. The 52-byte owner at 0x02005004
 * includes its three pool words, which are plain numeric arguments and not
 * addresses of Value_ globals.
 */
void SceneState_ApplyActor8FourFlags(void)
{
    SceneActor_RunFirstMatchingSlot(8, 0x92c);
    SceneActor_RunFirstMatchingSlot(8, 0x935);
    SceneActor_RunFirstMatchingSlot(8, 0x917);
    SceneActor_RunFirstMatchingSlot(8, 0x990);
}

/*
 * Scan slots 0 through 8 inclusive.  On the first a1 that Engine_GameFlagIsSet
 * accepts, call Engine_ActorSetPosition and stop.  a0 and a1 advance together.
 */
void SceneActor_RunFirstMatchingSlot(s32 a0, s32 a1)
{
    unsigned int i = 0;

    do {
        if (GameFlag_IsSet(a1)!= 0) {
            Actor_SetPosition(a0, 0, 0);
            break;
        }
        i++;
        a0++;
        a1++;
    } while (i <= 8);
}

void FieldScene_RunScene3b1_02005068(void)
{
    extern u8 Data_0200e840[];
    u32 i;
    s32 rec8;
    s32 record;
    s32 base5_200e840;
    s32 base5_200e8e4;

    rec8 = Value2_02005068(Func_0200a018, 0, 0);
    Event_Begin();
    FieldScene_RunSceneStep(24, 1, 0);
    FieldScene_RunSceneStep(25, 2, 0);
    SceneActor_SetFlagBit3ForActors28To35();
    Value3(FieldScene_RunSceneStep, 19, rec8, 12);
    Actor_SetAnimation(10, 6);
    base5_200e840 = (s32)Data_0200e840;
    Actor_EnableActionCallback(rec8, base5_200e840);
    Actor_Destroy(11);
    Value2_02005068(Engine_ActorEnableActionCallback, 12, base5_200e840);
    base5_200e8e4 = (s32)Data_0200e8e4;
    Actor_EnableActionCallback(36, base5_200e8e4);
    Actor_EnableActionCallback(37, base5_200e8e4);
    Func_0200a1b2();
    Event_End();
}

/* Sets up three actor slots (2, 1, 3) with position/pose data pulled from a
 * per-slot lookup record (fields at +8 and +16), then drives a chain of
 * actor animation, camera, and text/dialog calls for the scene. */
void FieldScene_RunPositionTransferPresentation(void)
{
    u32 i;
    /* Per-slot lookup record; fields at +8 and +16 feed the setup call. */
    s32 record;
    s32 base5_1e46;
    s32 base5_200e7c8;

    ConfigureSceneMotionFlags(0x1b80000, -1, 0xb00000, 0x1000001);
    Actor_SetPosition(0, 0x1b80000, 0x860000);
    Event_OpenScreen(); /* main:0808a360 */
    Actor_SetSpeed(0, 0x19999, 0xcccc);
    Actor_SetAnimation(0, 5);
    Actor_MoveToAndWait(0, 0x198, 134);
    Actor_MoveToAndWait(0, 0x198, 152);
    Actor_MoveToAndWait(0, 0x1b0, 166);
    Actor_SetAnimation(0, 1);
    Actor_FaceDirection(0, 0x4000, 0);
    record = Scene_GetRecord_1(0);
    if (record != 0) {
        Actor_SetPosition(2, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    record = Scene_GetRecord_2(0);
    if (record != 0) {
        Actor_SetPosition(1, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    record = Scene_GetRecord_3(1);
    if (record != 0) {
        Actor_SetPosition(3, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Task_Wait(1); /* main:080000c0 */
    Actor_SetSpeed(2, 0x19999, 0xcccc);
    Actor_WalkTo(2, 0x1a8, 152);
    Actor_SetSpeed(1, 0x19999, 0xcccc);
    Actor_WalkTo(1, 0x1c0, 168);
    Actor_SetSpeed(3, 0x20000, 0x10000);
    Actor_WalkToAndWait(3, 0x1ca, 152);
    Actor_SetAnimation(1, 1);
    Actor_SetAnimation(2, 1);
    Actor_FaceDirection(2, 0x4000, 0);
    Actor_FaceDirection(1, 0x4000, 0);
    Actor_FaceDirection(3, 0x4000, 40);
    Func_0200b7ee(10); /* main:0808a1d8 */
    /* Text/dialog resource pointer, passed by base address and by base+1. */
    base5_1e46 = (s32)Data_00001e46;
    Func_0200b6b2(base5_1e46, 1, 10); /* main:08015210 */
    Event_Wait(10);
    FieldScene_RunSceneStep(0, 0, 40);
    Value3(FieldScene_RunSceneStep, 1, 0x4000, 20);
    Camera_SetSpeed(0x39999, 0x7333); /* main:0808a208 */
    Call4(ConfigureSceneMotionFlags, 0x1b80000, -1, 0x1400000, 0x10000014);
    Actor_RunRepeatedMotion(8, 2); /* main:0808a138 */
    Call2(FieldScene_CallPairWith10, 8, 0xd000);
    Event_SetMessage((base5_1e46 + 1)); /* main:0808a170 */
    FieldScene_RunStepThen10(8);
    Actor_FaceDirection(8, 0, 20);
    Call4(ConfigureSceneMotionFlags, 0x1b80000, -1, 0x860000, 0x10000000);
    /* Second text/dialog resource pointer, shared across three calls. */
    base5_200e7c8 = (s32)Data_0200e7c8;
    Actor_EnableActionCallback(1, base5_200e7c8);
    ObjectMotion_EnableActionAndSetCallback_2(2, base5_200e7c8);
    ObjectMotion_MarkActiveAndSetActionCallback_1(3, base5_200e7c8); /* main:0808a0b0 */
    Event_Wait(40);
    GameFlag_Set(0x301);
    FieldScene_RunSceneStep(23, 0, 0);
    GameFlag_Clear(FLAG_ARRIVAL_EVENT_PENDING);
}

void FieldScene_RunFormationAndEffectPresentation(void)
{
    s32 action;

    OverlayObject_SetPositionAndHeading(0, 0x1bc, 0x12c, 0);
    OverlayObject_SetPositionAndHeading(1, 0x1ca, 0x136, 0);
    OverlayObject_SetPositionAndHeading(2, 0x1bc, 0x14a, 0);
    OverlayObject_SetPositionAndHeading(3, 0x1b0, 0x136, 0);
    OverlayObject_SetPositionAndHeading(27, 0x1b8, 134, 0x8000);
    OverlayObject_SetPositionAndHeading(10, 0x1c6, 248, 0x3000);
    Actor_SetAnimation(10, 6);
    ConfigureSceneMotionFlags(0x1b80000, -1, 0x1340000, 0x1000001);
    Event_OpenScreen();
    Event_WaitForScreen();
    Event_Wait(20);
    Actor_FaceDirection(0, 0x2000, 0);
    Actor_FaceDirection(1, 0xa000, 0);
    Actor_FaceDirection(2, 0xa000, 0);
    Actor_FaceDirection(3, 0x2000, 40);
    FieldScene_RunSceneStep(2, 1, 20);
    Event_SetMessage(MSG_HEY_ARE_YOU_OK);
    FieldScene_RunStepThen10(27);
    FieldScene_RunSceneStep(1, 0xc000, 0);
    Camera_SetSpeed(0x26666, 0x4ccc);
    Camera_MoveTo(0x1b80000, -1, 0xb00000, 1);
    Actor_SetSpeed(27, 0x19999, 0xcccc);
    Actor_WalkToAndWait(27, 0x198, 134);
    Actor_WalkToAndWait(27, 0x198, 152);
    Actor_WalkToAndWait(27, 0x1a8, 164);
    Camera_SetSpeed(0x19999, 0x3333);
    Camera_MoveTo(0x1b80000, -1, 0x12c0000, 1);
    Actor_WalkToAndWait(27, 0x1a8, 222);
    Actor_WalkToAndWait(27, 0x1a8, 0x106);
    Actor_FaceDirection(27, 0x3000, 20);
    Actor_RunRepeatedMotion(27, 1);
    FieldScene_RunStepThen10(27);
    FieldScene_RunSceneStep(2, 1, 20);
    Actor_SetAnimationAndWait(27, 3);
    Actor_RunRepeatedMotion(27, 1);
    FieldScene_RunStepThen10(27);
    FieldScene_RunSceneStep(3, 2, 60);
    FieldScene_RunSceneStep(1, 0xe000, 60);
    Actor_FaceDirection(27, 0, 40);
    Actor_RunRepeatedMotion(27, 1);
    Actor_SetAnimation(27, 2);
    Actor_MoveToAndWait(27, 0x1b0, 0x10c);
    Actor_MoveToAndWait(27, 0x1c4, 0x10c);
    Actor_SetAnimation(27, 1);
    FieldScene_CallPairWith10(27, 0xd000);
    Actor_StartRepeatedMotion(27, 2);
    Event_ShowMessageAndWait(27, 0, 20);
    FieldScene_RunSceneStep(1, 0xc000, 20);
    Actor_SetAnimationAndWait(27, 4);
    Event_Wait(40);
    Event_ShowMessageAndWait(27, 0, 80);
    Actor_RunRepeatedMotion(27, 1);
    Event_Wait(20);
    Actor_SetAnimationAndWait(27, 3);
    Event_Wait(10);
    FieldScene_CallPairWith10(27, 0x5000);
    Event_OpenMessage(27, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Actor_SetAnimationAndWait(27, 3);
        FieldScene_RunStepThen10(27);
    } else {
        Actor_SetAnimationAndWait(27, 4);
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
        FieldScene_RunStepThen10(27);
        FieldScene_RunSceneStep(3, 2, 40);
        Actor_RunRepeatedMotion(27, 1);
        Actor_SetAnimation(27, 3);
        FieldScene_RunStepThen10(27);
    }
    FieldScene_RunSceneStep(2, 1, 20);
    action = (s32)Data_0200e7f0;
    Actor_EnableActionCallback(1, action);
    Actor_EnableActionCallback(2, action);
    Func_0200bade(3, action);
    Camera_SetSpeed(0x9999, 0x1333);
    Camera_MoveTo(0x1b80000, -1, 0xb00000, 1);
    Actor_SetSpeed(0, 0x10000, 0x8000);
    Actor_WalkToAndWait(0, 0x1a8, 0x110);
    Actor_WalkTo(0, 0x1a8, 164);
    Event_Wait(60);
    *(s32 *)((*(s32 *)0x03001ebc + 0x1c0)) = 0x209;
    FieldScene_RunSceneStep(9, 0, 0);
    GameFlag_Clear(0x301);
    GameFlag_Clear(0x927);
    Event_RequestExit(4);
}

void FieldScene_RunActors24And25SetupWithValue929(void)
{
    s32 handle = Func_0200a634(0, 0);

    Event_Begin();
    FieldScene_RunSceneStep(24, 1, 0);
    FieldScene_RunSceneStep(25, 0, 0);
    Func_02008e84(0);
    FieldScene_RunSceneStep(19, handle, 12);
    Actor_SetPosition(11, 0, 0);
    FieldScene_RunFormationAndEffectPresentation();
    GameFlag_Set((s32)&Value_00000929);
    Event_End();
}

void FieldScene_RunScene3b1_020056dc(void)
{
    extern u8 Data_0200e840[];
    u32 i;
    s32 rec2;
    s32 rec8;
    s32 record;
    s32 base5_200e840;
    s32 base5_200e8e4;

    rec8 = Value2_020056dc(Func_0200a690_a, 0, 0);
    rec2 = Value2_020056dc(Func_0200a69a, 1, 0);
    Event_Begin();
    FieldScene_RunSceneStep(24, 1, 0);
    FieldScene_RunSceneStep(25, 3, 0);
    SceneActor_SetFlagBit3ForActors28To35();
    Value3(FieldScene_RunSceneStep, 19, rec8, rec2);
    Actor_SetAnimation(10, 6);
    base5_200e840 = (s32)Data_0200e840;
    Actor_EnableActionCallback(rec8, base5_200e840);
    Actor_Destroy(11);
    Actor_EnableActionCallback(rec2, base5_200e840);
    Actor_Destroy(12);
    base5_200e8e4 = (s32)Data_0200e8e4;
    Actor_EnableActionCallback(36, base5_200e8e4);
    Value2_020056dc(Engine_ActorEnableActionCallback, 37, base5_200e8e4);
    Actor_SetChildValue(36, 3);
    Actor_SetChildValue(37, 3);
    FieldScene_RunPositionTransferPresentation();
    Event_End();
}

/*
 * A flat setter sequence, no branches. The 108-byte owner at 0x02005780
 * includes its one pool word, the address taken as Value_0000092a.
 */
void FieldScene_RunActors24And25SetupWithValue92a(void)
{
    s32 handle = Func_0200a730(0, 0);
    s32 other = Func_0200a73a(1, 0);

    Event_Begin();
    FieldScene_RunSceneStep(24, 1, 0);
    FieldScene_RunSceneStep(25, 0, 0);
    Func_02008f8a(0);
    FieldScene_RunSceneStep(19, handle, other);
    Actor_SetPosition(11, 0, 0);
    Actor_SetPosition(12, 0, 0);
    FieldScene_RunFormationAndEffectPresentation();
    GameFlag_Set((s32)&Value_0000092a);
    Event_End();
}

void FieldScene_RunExtendedFormationPresentation(void)
{
    s32 slot_b;
    s32 slot_c;
    s32 slot_a;
    u8 *record;
    s32 action;

    slot_a = Value2(Func_0200a7a2, 0, 0);
    slot_b = Func_0200a7ac(1, 0);
    slot_c = Value2(Func_0200a7b6, 2, 0);
    Event_Begin();
    FieldScene_RunSceneStep(10, 0, 0);
    FieldScene_RunSceneStep(17, 0, 0);
    Actor_SetPosition(8, 0x1d80000, 0x980000);
    Actor_SetAnimation(9, 5);
    Actor_SetPosition(27, 0x1b80000, 0x860000);
    Actor_SetChildValue(27, 15);
    record = Func_0200bd42(27);
    Actor_SetSpriteFlags(record, 0);
    FieldScene_InstallFlaggedActors10To17(16);
    ConfigureSceneMotionFlags(0x1b60000, -1, 0xae0000, 0x1000001);
    FieldScene_RunSceneStep(8, 1, 20);
    Audio_PlayCue(19);
    Audio_PlayCue(181);
    Work_SetValuesIfNonNegative(0x20000, 0x20000, 0x10000);
    Event_Wait(10);
    Work_SetValuesIfNonNegative(-1, -1, 0xe666);
    Event_Wait(80);
    Audio_PlayCue(181);
    Work_SetValuesIfNonNegative(0x20000, 0x20000, 0x10000);
    Event_Wait(10);
    Work_SetValuesIfNonNegative(-1, -1, 0xe666);
    Audio_PlayCue(63);
    GameFlag_Set(0x11a);
    Actor_SetAttachedEffect(3, 0x102);
    Event_Wait(40);
    FieldScene_CallPairWith10(3, 0x6000);
    Event_SetMessage(MSG_WONDER_WHATS_WRONG_SHIP_SHOULDNT);
    Event_ShowMessageAndWait(3, 0, 40);
    FieldScene_RunStepThen10(27);
    Actor_FaceDirection(0, 0x2000, 0);
    Actor_FaceDirection(1, 0xa000, 0);
    Actor_FaceDirection(2, 0, 0);
    Actor_FaceDirection(3, 0xe000, 40);
    Actor_FaceDirection(0, 0x6000, 0);
    Actor_FaceDirection(1, 0xe000, 0);
    Actor_FaceDirection(2, 0x6000, 0);
    Actor_FaceDirection(3, 0x8000, 40);
    Actor_FaceDirection(2, 0xe000, 0);
    Actor_ShowEmote(2, 0x100, 60);
    FieldScene_CallPairWith10(2, 0x2000);
    Actor_RunRepeatedMotion(2, 1);
    FieldScene_RunStepThen10(2);
    Actor_StartRepeatedMotion(0, 1);
    Actor_StartRepeatedMotion(1, 1);
    Actor_RunRepeatedMotion(3, 1);
    Event_Wait(10);
    Actor_FaceDirection(0, 0xc000, 0);
    Actor_FaceDirection(1, 0xc000, 0);
    Actor_FaceDirection(2, 0xe000, 0);
    Actor_FaceDirection(3, 0xa000, 20);
    Actor_SetChildValue(27, 0);
    record = Func_0200beea_a(27);
    Actor_SetSpriteFlags(record, 1);
    Actor_SetSpeed(27, 0x10000, 0x8000);
    Actor_WalkToAndWait(27, 0x1ae, 134);
    FieldScene_CallPairWith10(27, 0x3000);
    Actor_StartRepeatedMotion(27, 2);
    FieldScene_RunStepThen10(27);
    Actor_StartRepeatedMotion(slot_a, 1);
    Actor_StartRepeatedMotion(slot_b, 1);
    Actor_StartRepeatedMotion(slot_c, 1);
    Actor_RunRepeatedMotion(13, 1);
    Actor_SetAttachedEffect(slot_a, 0x102);
    Actor_SetAttachedEffect(slot_b, 0x102);
    Actor_SetAttachedEffect(slot_c, 0x102);
    Actor_SetAttachedEffect(13, 0x102);
    Event_Wait(40);
    FieldScene_RunSceneStep(12, slot_a, 0);
    FieldScene_RunSceneStep(12, slot_b, 1);
    FieldScene_RunSceneStep(12, slot_c, 0);
    FieldScene_RunSceneStep(11, 1, 0);
    Actor_FaceDirection(slot_a, 0xd000, 0);
    Actor_FaceDirection(slot_b, 0xb000, 0);
    Actor_FaceDirection(slot_c, 0xd000, 0);
    Actor_FaceDirection(0, 0, 0);
    Actor_FaceDirection(1, 0x8000, 0);
    Actor_FaceDirection(2, 0, 0);
    Actor_FaceDirection(3, 0x8000, 40);
    Actor_StartRepeatedMotion(27, 2);
    Event_ShowMessage(27, 0);
    Actor_FaceDirection(0, 0xc000, 0);
    Actor_FaceDirection(1, 0xc000, 0);
    Actor_FaceDirection(2, 0xc000, 0);
    Actor_FaceDirection(3, 0xc000, 20);
    Actor_WalkToAndWait(27, 0x1b8, 134);
    Actor_SetPosition(27, 0, 0);
    FieldScene_CallPairWith10(1, 0x8000);
    Actor_RunRepeatedMotion(1, 1);
    FieldScene_RunStepThen10(1);
    Actor_FaceDirection(2, 0, 0);
    FieldScene_CallPairWith10(3, 0x8000);
    Actor_SetAnimation(0, 3);
    Actor_SetAnimation(1, 3);
    Actor_SetAnimation(2, 3);
    Actor_SetAnimationAndWait(3, 3);
    Actor_SetSpeed(1, 0x10000, 0x8000);
    Actor_SetSpeed(2, 0x10000, 0x8000);
    Actor_SetSpeed(3, 0x10000, 0x8000);
    action = (s32)Data_0200e818;
    Actor_EnableActionCallback(1, action);
    Actor_EnableActionCallback(2, action);
    Func_0200c0f0(3, action);
    GameFlag_Set(0x302);
    *(s32 *)0x0200ff84 = 0;
    Value2(Func_0200c01e, (s32)Scene_UpdateCueTimer, 0xc80);
    FieldScene_RunSceneStep(23, 0, 0);
    Actor_Destroy(27);
    GameFlag_Clear(FLAG_ARRIVAL_EVENT_PENDING);
    GameFlag_Clear(0x927);
    Event_End();
}

void RunActorsEightAndNineMapEvent(void)
{
    Event_Begin();
    FieldScene_RunSceneStep(15, 1, 0);
    OverlayObject_SetPositionAndHeading(9, 468, 616, 32768);
    FieldScene_RunSceneStep(8, 1, 20);
    Actor_RunRepeatedMotion(9, 2);
    Event_Wait(20);
    SetDirection(8, 53248, 80);
    Actor_FaceDirection(8, 0, 20);
    Actor_SetAnimationAndWait(8, 3);
    Event_Wait(20);
    FieldScene_RunSceneStep(9, 21, 0);
}

void FieldScene_RunScene3b1_02006110(void)
{
    s32 rec2;
    s32 rec4;
    s32 rec7;
    s32 rec8;

    rec2 = Value2(Func_0200b0cc, 0, 0);
    rec8 = Func_0200b0d6(1, 0);
    rec7 = Func_0200b0e0(2, 0);
    rec4 = Value2(Func_0200b0ea, 3, 0);
    Event_Begin();
    FieldScene_RunSceneStep(10, 0, 0);
    OverlayObject_SetPositionAndHeading(8, 0x1d8, 144, 0x5000);
    OverlayObject_SetPositionAndHeading(27, 0x198, 142, 0x3000);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
    Event_OpenScreen();
    Event_WaitForScreen();
    Event_Wait(40);
    Actor_RunRepeatedMotion(27, 1);
    Event_SetMessage(MSG_SORRY_EVERYONE_BUT_WE_NEED);
    FieldScene_RunStepThen10(27);
    Actor_StartRepeatedMotion(rec2, 2);
    Actor_StartRepeatedMotion(rec8, 2);
    Actor_StartRepeatedMotion(rec7, 2);
    Actor_RunRepeatedMotion(rec4, 2);
    Event_Wait(20);
    Actor_FaceDirection(rec2, 0, 0);
    Actor_FaceDirection(rec8, 0x8000, 0);
    Actor_FaceDirection(rec7, 0, 0);
    Actor_FaceDirection(rec4, 0x8000, 40);
    Actor_SetSpeed(rec2, 0x10000, 0x8000);
    Actor_SetSpeed(rec8, 0x10000, 0x8000);
    Actor_SetSpeed(rec7, 0x10000, 0x8000);
    Actor_SetSpeed(rec4, 0x10000, 0x8000);
    Actor_WalkTo(rec2, 0x1d6, 172);
    Actor_WalkTo(rec8, 0x19a, 172);
    Actor_WalkTo(rec7, 0x1d6, 204);
    Actor_WalkToAndWait(rec4, 0x19a, 204);
    Actor_SetAnimation(rec2, 1);
    Actor_SetAnimation(rec8, 1);
    Actor_SetAnimation(rec7, 1);
    Actor_FaceDirection(rec8, 0xd000, 0);
    Actor_FaceDirection(rec2, 0xb000, 0);
    Actor_FaceDirection(rec4, 0xd000, 0);
    Actor_FaceDirection(rec7, 0xb000, 20);
    Actor_RunRepeatedMotion(27, 1);
    FieldScene_RunStepThen10(27);
    Actor_SetAnimation(rec2, 3);
    Actor_SetAnimation(rec8, 3);
    Actor_SetAnimation(rec7, 3);
    Actor_SetAnimationAndWait(rec4, 3);
    FieldScene_RunStepThen10(27);
    Actor_SetAnimation(rec2, 3);
    Actor_SetAnimation(rec8, 3);
    Actor_SetAnimation(rec7, 3);
    Actor_SetAnimationAndWait(rec4, 3);
    Actor_FaceDirection(27, 0, 0);
    FieldScene_CallPairWith10(0, 0x8000);
    Actor_SetAnimationAndWait(0, 3);
    Actor_SetAnimationAndWait(27, 3);
    Actor_SetSpeed(27, 0x10000, 0x8000);
    Actor_WalkToAndWait(27, 0x198, 132);
    Actor_WalkToAndWait(27, 0x1bc, 132);
    Actor_SetPosition(27, 0, 0);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
    Event_CloseScreen();
    Event_WaitForScreen();
    Call2(SceneState_ScanTwoArraysAndCrossNotify, 0x92c, 0x935);
    Call2(SceneState_ScanTwoArraysAndCrossNotify, 0x917, 0x990);
    GameFlag_Clear(0x8a0);
    Event_RequestExit(10);
}

/*
 * Runs two first-match linear scans over indices 0 to 8, each breaking on its
 * first hit and calling a per-element handler, then cross-pairs the miss
 * counts: the count from scanning `a` indexes into `b`, and the count from
 * scanning `b` indexes into `a`.  Each callee is named for its own call site,
 * because every call reaches its target through its own local veneer and two
 * of the sites share one veneer.
 */
void SceneState_ScanTwoArraysAndCrossNotify(u8 *a, u8 *b)
{
    s32 cnt_a = 0;
    s32 cnt_b = 0;
    u32 i;

    for (i = 0; i <= 8; i++) {
        u8 *p = a + i;
        if (GameFlag_IsSet(p)!= 0) {
            GameFlag_Clear(p);
            break;
        }
        cnt_a++;
    }

    for (i = 0; i <= 8; i++) {
        u8 *p = b + i;
        if (GameFlag_IsSet(p)!= 0) {
            GameFlag_Clear(p);
            break;
        }
        cnt_b++;
    }

    GameFlag_Set(b + cnt_a);
    GameFlag_Set(a + cnt_b);
}
