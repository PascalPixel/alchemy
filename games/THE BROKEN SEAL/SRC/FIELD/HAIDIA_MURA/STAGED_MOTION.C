#include "TYPES.H"

#define Scene_GetRecord_1(a0) Value1(Engine_ActorGet, a0)
#define Scene_GetRecord_2(a0) Value1(Engine_ActorGet, a0)
#define Scene_GetRecord_1_02002f14(a0) Value1(Engine_ActorGet, a0)
#define Scene_GetRecord_2_02002f14(a0) Value1(Engine_ActorGet, a0)
#define ObjectMotion_EnableActionAndSetCallback_2(a0, a1) Value2(Engine_ActorEnableActionCallback, a0, a1)
#define ObjectMotion_MarkActiveAndSetActionCallback_1(a0, a1) Value2(Func_02009242, a0, a1)
#define ACTOR_A 12
#define ACTOR_B 13
#define ACTOR_C 14
#define ACTOR_D 11
#define SHARED_DATA ((s32)Data_0200e65c)
#define GetOrbitingSceneObject Func_0200bb38
#define AllocateEffectTransfer Func_0200ba98_a
#define UpdateOrbitingSceneObject Value_0200dae1

#include "STAGED_ACTOR.H"
#include "FIELD_EVENT.H"

/*
 * Reset or tear down a consecutive run of scene actors -- resource_373.
 */
struct Resource373Actor {
    u8 unknown_00[8];
    s32 field08;
    s32 field0c;
    s32 field10;
    u8 unknown_14[0x41];
    u8 flag55;
};

struct SourceEntity {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct ParticleRecord {
    u8 f00[9];
    u8 f09;
};

struct StagedParticle {
    u8 f00[0x48];
    s32 f48;
    s32 f4c;
    struct ParticleRecord *f50;
    u8 f54;
    u8 f55;
    u8 f56[8];
    u16 f5e;
};

/* Emitter fields are named by offset; the layout is not verified. */
struct Resource373Emitter {
    u8 unknown_00[6];
    u16 field06;
    s32 x;                          /* 0x08 */
    s32 y;                          /* 0x0c */
    s32 z;                          /* 0x10 */
    u8 unknown_14[4];
    s32 field18;
    s32 field1c;
    u8 unknown_20[4];
    s32 field24;
    s32 field28;
    s32 field2c;
    u8 unknown_30[8];
    s32 field38;
    s32 field3c;
    s32 field40;
};

struct Resource373Particle {
    u8 unknown_00[0x28];
    s32 lifetime;                   /* 0x28 */
    u8 unknown_2c[4];
    s32 field30;
    s32 field34;
    u8 unknown_38[0x10];
    s32 field48;
    u8 unknown_4c[9];
    u8 field55;
};

struct StagedVerticalEffect {
    s32 pad0[2];
    s32 f8;
    s32 fc;
    s32 f10;
    s32 pad14;
    s32 f18;
    s32 f1c;
    s32 pad20[17];
    s16 f64;
    s16 pad66;
    s32 *f68;
};

typedef struct OrbitingSceneObjectSprite {
    u8 padding_00[5];
    u8 flags_05_low : 5;
    u8 flags_05_bit_5 : 1;
    u8 flags_05_high : 2;
    u8 padding_06[3];
    u8 flags_09_low : 2;
    u8 flags_09_mode : 2;
    u8 flags_09_high : 4;
    u8 padding_0a[18];
    u8 palette;
    u8 padding_1d[10];
    u8 state;
} OrbitingSceneObjectSprite;

typedef struct OrbitingSceneObject {
    u8 padding_00[8];
    s32 x;
    s32 y;
    u8 padding_10[19];
    u8 flags_23;
    u8 padding_24[12];
    s32 orbit_angle;
    u8 padding_34[4];
    s32 orbit_center_x;
    s32 orbit_center_y;
    u8 padding_40[16];
    OrbitingSceneObjectSprite *sprite;
    u8 padding_54;
    u8 mode;
    u8 state;
    u8 padding_57[5];
    u8 active;
    u8 padding_5d[4];
    u8 visible;
    u8 padding_62[10];
    u32 callback;
} OrbitingSceneObject;

extern s32 Data_0200e708[];
extern s32 Data_0200e870[];
extern u8 Data_02000240[];
extern s32 Data_0200ee48[];
extern s32 Data_0200ecc8[];
extern s32 Data_0200eab8[];
extern s32 Data_0200e8a8[];
extern s32 Data_0200f334[];
extern s32 Data_0200f100[];
extern s32 Data_0200ef38[];
extern u8 Value_0200f570;
extern u8 Value_0200f55a;
extern u8 Data_00000f4d[];
extern u8 Data_00001c45[];
extern u8 Data_00001c60[]; /* Buffer passed by address to Engine_EventSetMessage. */
extern u8 Data_0200e65c[];
extern u8 Data_00000ee8[];
extern u8 Data_0200e590[];
extern u8 Value_00000ee4;
extern s32 Data_0200f63c[];
extern s32 Data_0200f6cc[];
extern s32 Data_0200f748[];
extern s32 Data_0200f7c4[];
extern s32 Data_0200f55a[];
extern s32 Data_0200e6e0[];
extern u8 Value_0000087a;
extern u8 Value_0200dae1;

void Func_020073a4();
void Func_020073c4();
void Func_020074ee();
void Func_020074f0();
s32 Func_02009242();
void Func_0200955e();
void Func_02009576();
void Func_0200958a();
s32 Func_020095fa();
void Func_02009794();
void Func_020097e0();
s32 Func_020097ee();
void Func_020098d2();
void Func_02009966();
void Func_0200996c();
void Func_020099a6();
s32 Func_0200ba42();
s32 Func_0200ba60();
s32 Func_0200451a(s32, s32, s32, s32);
void Func_02005a06(void);
void Func_0200b296(void);
s32 Func_02004612(s32, s32, s32, s32);
void Func_020078a2();
void Func_02007b8a();
void Func_02007c4a();
void Func_0200882c();
void Func_02008860();
void Func_0200976e();
void Func_0200a080();
s32 Func_0200a0a8();
void Func_0200a0b4_a();
void Func_0200a114();
void Func_0200a11a();
void Func_0200a120();
void Func_0200a1c2();
s32 Func_0200a1ea();
void Func_0200a1f6();
u8 *Func_0200a1fa();
void Func_0200a256();
void Func_0200a25c();
void Func_0200a262();
void Func_0200a312();
s32 Func_0200a384();
s32 Func_0200a390();
void Func_0200a3ec();
void Func_0200a3f2();
void Func_0200a3f8();
u8 *Func_0200a9a4_a();
void Func_0200d5c1();
void Func_0200d5e1();
void Func_0200d5f1();
void Func_0200d601();
void Func_0200da09();
s32 Func_0200b60e(struct SourceEntity *);
void Func_0200b656(s32, s32, s32 *);
struct StagedParticle *Func_0200b6a2(s32, s32, s32, s32);
struct Resource373Particle *Func_0200b754();
void Func_0200b7d4(s32, s32, s32 *);
void Func_0200b838(s32, s32, s32, s32);
void Func_0200b6d4(void);
void Func_0200ad9e(s32, s32, s32, s32);
OrbitingSceneObject *Func_0200bb38();
u8 *Func_0200ba98_a(s32, s32);
void Func_0200b9b8(s32);
s32 Func_0200baf8(s32, s32);
void Func_0200b9f4(s32);
s32 Func_0200bb34(s32, s32);

/*
 * Distance between two three-component 16.16 fixed-point positions.
 *
 * Each argument walks three consecutive 16.16 words in x, y, z order. The
 * per-axis deltas are taken in fixed point, shifted down to integers, squared,
 * and summed; the total is passed to the resident IWRAM integer square root.
 *
 * Expressions are preserved exactly as reconstructed: the walking-pointer form
 * is load-bearing for byte-identity and must not become struct field access.
 */

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */

/* Actor IDs the scene sets up: three actors share a common data pointer,
 * a fourth actor (11) is driven through its own sequence of position/speed
 * calls. */

/* Shared data used for actors A, B, C. */

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
void Effect_UpdateParticlePosition(s32 *particle, s32 delta_x, s32 delta_z);

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
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

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call3_02000dc0(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call3_02000e54(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call3_02000e84(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers, matching the reference codegen. */
static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

static __inline__ void Call1_02000f5c(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call3_02000f8c(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* Runs four fixed steps in order: a single-argument call, a call that takes
 * the address of the byte at Value_0200f55a plus two constants, another
 * three-constant call, and a final single-argument call. */
static __inline__ s32 Value2_02000fbc(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call1_02000fbc(void (*f)(), s32 a0)
{
    f(a0);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ s32 Value1_020011d8(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ s32 Value2_02002cb0(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

/* Newly identified engine calls: each macro below names the site's own
 * engine function from its reconstructed C source, and keeps the site's
 * own calling form. */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1_02002f14(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value2_02002f14(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ s32 Value2_020031b4(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ s32 Value1_020034c8(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ s32 Value2_020034c8(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

static __inline__ void Call11(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7, s32 a8, s32 a9, s32 a10)
{
    f(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10);
}

/*
 * Staged actor cutscene tail -- resource_373. The long scene script plus the
 * particle, sound, and vertical-motion effects it drives.
 */

/*
 * Each alias names the loader-relocated call word the image holds, not a
 * runtime address. The declarations are old-style because the call sites vary
 * in arity.
 */

/*
 * Calls spelled through these wrappers pass their constants straight into the
 * argument registers. A direct call instead precomputes a costly constant into
 * a pseudo shared with later uses in the block, and a value-returning call
 * sets r0 last of its arguments.
 */
static __inline__ void Call1_02003fb0(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call6_02003fb0(
    void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

static __inline__ s32 Value2_02003fb0(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

s32 MapStagedScene_SelectPrimaryData(void)
{
    return (s32)Data_0200e708;
}

s32 MapStagedScene_GetEmptyData(void)
{
    return 0;
}

s32 MapStagedScene_SelectSecondaryData(void)
{
    return (s32)Data_0200e870;
}

s32 MapStagedScene_SelectTertiaryData(void)
{
    u8 *scene_state = Data_02000240;
    if (*(s16 *)(scene_state + 0x1c2) == 16)
        return (s32)Data_0200ee48;
    if (GameFlag_IsSet(0x87a) != 0)
        return (s32)Data_0200ecc8;
    if (GameFlag_IsSet(0x815) != 0)
        return (s32)Data_0200eab8;
    return (s32)Data_0200e8a8;
}

void SceneDialogue_RunActor181Scene(void)
{
    Event_Begin();
    Actor_SetPosition(26, 0, 0);
    GameFlag_Set(0xfd0);
    Item_ShowFound(0xb5, 3);
    Party_GiveItem(0xb5, 0);
    Event_End();
}

void FieldScene_RunActor181Scene(void)
{
    Event_Begin();
    Actor_SetPosition(20, 0, 0);
    GameFlag_Set(0xfd0);
    Item_ShowFound(0xb5, 3);
    Party_GiveItem(0xb5, 0);
    Event_End();
}

s32 MapStagedScene_SelectQuaternaryData(void)
{
    if (GameFlag_IsSet(0x87a) != 0)
        return (s32)Data_0200f334;
    if (GameFlag_IsSet(0x815) != 0)
        return (s32)Data_0200f100;
    return (s32)Data_0200ef38;
}

void SceneDialogue_RunActorTenFlaggedDialogue(void)
{
    Event_Begin();
    if (GameFlag_IsSet(0x815) != 0) {
        Event_SetMessage(0x11cc);
        Event_ShowMessage(10, 0);
    } else {
        Event_SetMessage(0xf81);
        Actor_FaceEachOther(10, 0, 4);
        Event_AskYesNo(10, 0);
    }
    Event_End();
}

void SceneDialogue_RunActorFourteenTalk(void)
{
    s32 flag = 0x806;
    Event_Begin();
    if (GameFlag_IsSet(0x815) != 0) {
        Event_SetMessage(0x11c9);
        Event_ShowMessage(14, 0);
    } else if (GameFlag_IsSet(flag) == 0) {
        GameFlag_Set(flag);
        Event_SetMessage(0xf7c);
        Actor_FaceEachOther(14, 0, 4);
        Event_AskYesNo(14, 0);
    } else {
        Event_SetMessage(0xf7e);
        Actor_FaceEachOther(14, 0, 4);
        Event_ShowMessage(14, 0);
    }
    Event_End();
}

void FieldScene_RunFlag807BranchSequence(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    if (GameFlag_IsSet(0x807) == 0) {
        GameFlag_Set(0x807);
        Event_SetMessage(0xf63);
        Actor_ShowEmote(18, 0x103, 0);
        Actor_FaceEachOther(0, 18, 20);
        Event_ShowMessageAndWait(18, 0, 6);
        Actor_FaceDirection(18, 0x8000, 30);
        Actor_Jump(18, 2, 20);
        Event_ShowMessageAndWait(18, 0, 6);
        Actor_FaceEachOther(18, 0, 10);
        Actor_ShowEmote(18, 0x103, 0);
        Event_ShowMessageAndWait(18, 0, 10);
        Actor_ShowEmote(0, 0x102, 60);
    } else {
        Actor_ShowEmote(18, 0x103, 0);
        Event_SetMessage(0xf66);
        Event_ShowMessageAndWait(18, 0, 20);
    }
    Event_End();
}

void SceneDialogue_RunActor21FlaggedLine(void)
{
    Event_Begin();
    if (GameFlag_IsSet(0x202) != 0) {
        Event_SetMessage(0xf68);
    } else {
        Event_SetMessage(0xf69);
    }
    Event_ShowMessage(21, 0);
    Event_End();
}

void SceneDialogue_RunActor10LineAndFlag81f(void)
{
    Event_Begin();
    Actor_FaceEachOther(0, 10, 20);
    Event_SetMessage(0x1c8d);
    Event_ShowMessage(10, 0);
    GameFlag_Set(0x81f);
    Event_End();
}

void FieldScene_RunScene373_02000cd0(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    Work_SetValuesIfNonNegative(0x30000, 0x30000, 0x10000);
    Task_Wait(10);
    Work_SetValuesIfNonNegative(-1, -1, 0xe666);
    Event_SetMessage(0x1c9a);
    Event_ShowMessageAndWait(17, 0, 20);
    Actor_FaceActor(17, 0, 20);
    Event_ShowMessage(17, 0);
    Event_End();
}

void SceneDialogue_RunActorNineteenDialogue(void)
{
    Event_Begin();
    Actor_RunRepeatedMotion(19, 2);
    Event_Wait(20);
    Actor_FaceActor(19, 0, 20);
    Event_SetMessage(0x1c9d);
    Event_AskYesNo(19, 0);
    GameFlag_Set(0x307);
    Event_End();
}

void SceneState_Send210AndApplyRectAt40x84(void)
{
    s32 m, n;
    GameFlag_Set(0x210);
    m = 10;
    n = 84;
    Map_CopyCellAttributes(40, 84, 7, 4, m, n);
}

void SceneState_Send210AndApplyRect(void)
{
    s32 m, n;
    GameFlag_Clear(0x210);
    m = 10;
    n = 84;
    Map_CopyCellAttributes(40, 89, 7, 4, m, n);
}

void FieldScene_RunScene373_02000dc0(void)
{
    u32 i;
    s32 record;

    Audio_PlayCue(188);
    Map_AnimateCells(0x200f544, 45, 11);
    Actor_WalkTo(0, 0x101, 0x1a4);
    Event_RequestExit(11);
}

void SceneState_ApplyFlag801Branch(void)
{
    if (GameFlag_IsSet(0x801) == 0) {
        FieldScene_RunScene373SequenceC();
    } else {
        Audio_PlayCue(123);
        Event_RequestExit(1);
    }
}

void SceneState_SetValue123Mode3(void)
{
    Audio_PlayCue(123);
    Event_RequestExit(3);
}

void SceneState_SetValue123Mode4(void)
{
    Audio_PlayCue(123);
    Event_RequestExit(4);
}

void SceneState_ApplyValues123And2(void)
{
    Audio_PlayCue(123);
    Event_RequestExit(2);
}

void FieldScene_RunScene373_02000e54(void)
{
    u32 i;
    s32 record;

    Audio_PlayCue(158);
    Map_AnimateCells(0x200f55a, 54, 32);
    Actor_WalkTo(0, 0x196, 0x2d7);
    Event_RequestExit(5);
}

void FieldScene_RunScene373_02000e84(void)
{
    u32 i;
    s32 record;

    Audio_PlayCue(158);
    Map_AnimateCells(0x200f570, 45, 39);
    Actor_WalkTo(0, 0x106, 0x325);
    Event_RequestExit(6);
}

void SceneDialogue_RunFlag815GatedStep(void)
{
    if (GameFlag_IsSet(0x815) != 0 && GameFlag_IsSet(0x87a) == 0) {
        Event_Begin();
        Event_SetMessage(0x11b6);
        Event_OpenMessage(21, 0);
        if (Event_ChooseYesNo(0, 0) == 0) {
            Event_ShowMessageAndWait(21, 0, 60);
            Event_ShowMessage(21, 0);
        } else {
            u8 *b = *(u8 **)0x03001ebc;
            u16 *h = (u16 *)(b + 0x1d8);
            *h = *h + 2;
            Event_Wait(40);
            Event_ShowMessage(21, 0);
        }
        Event_End();
    } else {
        Audio_PlayCue(0x9e);
        Map_AnimateCells((s32)Data_0200f55a, 50, 44);
        Actor_WalkTo(0, 0x154, 0x378);
        Event_RequestExit(7);
    }
}

/* Runs four scene primitives in sequence: one single-argument call, one call
 * passing the address of Value_0200f570 plus two small constants, one call
 * with a byte-flag-sized first argument (0) and two larger constants, and a
 * final single-argument call. */
void FieldScene_RunPrimarySequence(void)
{
    Audio_PlayCue(158);
    Map_AnimateCells((s32)&Value_0200f570, 49, 69); /* main:08009178 */
    Actor_WalkTo(0, 326, 0x466); /* object_id 0, x 326, z 0x466 */
    Event_RequestExit(8); /* main:0808a248 */
}

/* Runs a short scripted step, then two 3-argument setup calls, then another
 * short scripted step; none of the callees' effects are visible here. */
void FieldScene_RunScene373SequenceA(void)
{
    Audio_PlayCue(158);
    Map_AnimateCells(0x200f586, 52, 76); /* main:08009178 */
    Actor_WalkTo(0, 0x176, 0x4d6); /* object_id 0, x 0x176, z 0x4d6 */
    Event_RequestExit(9); /* main:0808a248 */
}

void FieldScene_RunPrimarySequenceSecond(void)
{
    Audio_PlayCue(158);
    Map_AnimateCells((s32)&Value_0200f55a, 35, 74); /* main:08009178 */
    Actor_WalkTo(0, 102, 0x4b6);
    Event_RequestExit(10); /* main:0808a248 */
}

void FieldScene_RunScene373SequenceC(void)
{
    u32 i;
    s32 rec7;
    s32 rec8;
    s32 record;

    rec8 = Value1(Engine_ActorGet, 0);
    rec7 = Value1(Engine_ActorGet, 5);
    Event_Begin();
    *(s32 *)(rec7 + 8) = *(s32 *)(rec8 + 8);
    *(s32 *)(rec7 + 12) = *(s32 *)(rec8 + 12);
    *(s32 *)(rec7 + 16) = *(s32 *)(rec8 + 16);
    *(s32 *)(rec7 + 56) = -0x80000000;
    *(s32 *)(rec7 + 60) = -0x80000000;
    *(s32 *)(rec7 + 64) = -0x80000000;
    *(s32 *)(rec7 + 36) = 0;
    *(s32 *)(rec7 + 40) = 0;
    *(s32 *)(rec7 + 44) = 0;
    *(s32 *)(rec7 + 20) = *(s32 *)(rec8 + 12);
    Task_Wait(1);
    Actor_SetSpeed(5, 0x10000, 0x8000);
    Actor_WalkToAndWait(5, 110, 0x11b);
    Actor_FaceEachOther(0, 5, 2);
    Event_SetMessage(0xf39);
    if (*(s32 *)(rec8 + 8) < *(s32 *)(rec7 + 8)) {
        Event_ShowMessageAndWait(0xa005, 0, 2);
    } else {
        Event_ShowMessageAndWait(0x8005, 0, 2);
    }
    Actor_SetAnimationAndWait(0, 3);
    Event_Wait(2);
    Actor_SetAnimation(5, 2);
    record = Value1(Engine_ActorGet, 0);
    if (record != 0) {
        Actor_SetDestination(5, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Actor_WaitForMove(5);
    Actor_SetPosition(5, 0, 0);
    Actor_WalkToAndWait(0, 110, 0x12f);
    Event_End();
}

void FieldScene_RunScene373SequenceD(void)
{
    u32 i;
    s32 record;
    s32 base5_f4d;

    if (GameFlag_IsSet(0x808) == 0) {
        Event_Begin();
        Actor_SetSpeed(0, 0x10000, 0x8000);
        base5_f4d = (s32)Data_00000f4d;
        Event_SetMessage(base5_f4d);
        Event_ShowMessageAndWait(15, 0, 2);
        Event_ShowMessageAndWait(16, 0, 2);
        Message_ShowCentered((base5_f4d + 2), 1);
        Event_Wait(6);
        Actor_WalkToAndWait(0, 69, 0x366);
        Event_End();
    }
}

void SceneState_RunFlag204Step(void)
{
    s32 m, n;
    Event_Begin();
    m = 20;
    n = 50;
    Map_CopyCellAttributes(49, 53, 8, 4, m, n);
    Func_0200451a(0, 10, 11, 1);
    GameFlag_Set(0x204);
    Event_End();
}

void SceneState_SetFlag204AndConfigureRegion49_46(void)
{
    s32 p5, p6;
    Event_Begin();
    Func_02004612(0, 13, 10, 1);
    GameFlag_Clear(0x204);
    p5 = 20;
    p6 = 50;
    Map_CopyCellAttributes(49, 46, 8, 4, p5, p6);
    Event_End();
}

void FieldScene_RunScene373SequenceE(void)
{
    u32 i;
    u8 *rec7;
    s32 record;

    rec7 = Value1(Engine_ActorGet, 22);
    Event_Begin();
    Actor_SetSpeed(0, 0x20000, 0x20000);
    Actor_Jump(0, 5, 0);
    Actor_WalkTo(0, 215, 0x193);
    rec7[90] |= 1;
    Actor_SetPosition(22, 0xa60000, 0x1770000);
    Actor_FaceDirection(22, 0x2000, 20);
    rec7[90] = (rec7[90] ^ 1);
    Actor_SetSpeed(22, 0x28000, 0x28000);
    Actor_Jump(22, 4, 0);
    Actor_WalkToAndWait(22, 202, 0x18b);
    Actor_SetAnimation(0, 1);
    Event_Wait(10);
    Actor_FaceDirection(0, 0xb000, 0);
    Actor_FaceDirection(22, 0x3000, 24);
    Actor_ShowEmote(0, 0x100, 0);
    Actor_StartRepeatedMotion(0, 2);
    Event_Wait(20);
    Actor_SetSpeed(22, 0x18000, 0x10000);
    Actor_EnableActionCallback(0, 0x200f59c);
    Event_Wait(10);
    Actor_ShowEmote(22, 0x103, 0);
    Actor_EnableActionCallback(22, 0x200f5ec);
    Func_020073a4(0);
    Actor_WalkToAndWait(0, 0x100, 0x1da);
    Actor_FaceDirection(0, 0xc000, 0);
    Func_020073c4(22);
    Actor_WalkToAndWait(22, 0x100, 0x1c8);
    Actor_SetAnimation(0, 1);
    Actor_FaceDirection(22, 0x4000, 20);
    Actor_StartRepeatedMotion(22, 2);
    Event_Wait(20);
    Event_SetMessage(0xfce);
    Event_AskYesNo(22, 0);
    record = Actor_Get(22);
    *(s32 *)(record + 108) = 0x200d72d;
    Actor_EnableActionCallback(22, 0x200e248);
    GameFlag_Set(0x823);
    Event_End();
}

void SceneState_RunTablePairWhenActor22State1(void)
{
    u8 *p = Actor_Get(22);
    if (GameFlag_IsSet(0x823) != 0) {
        u8 *q = p;
        q += 100;
        if (*(s16 *)q == 1) {
            FieldScene_RunScene373_02001490((s32)Data_0200f63c, (s32)Data_0200f6cc);
        }
    }
}

void FieldScene_RunScene373_02001490(s32 a0, s32 a1)
{
    u32 i;
    s32 p8;
    s32 rec8;
    s32 record;

    p8 = a1;
    rec8 = Value1(Engine_ActorGet, 22);
    Event_Begin();
    Actor_StartRepeatedMotion(22, 2);
    Actor_ShowEmote(22, 0x100, 20);
    Actor_StartRepeatedMotion(0, 2);
    Actor_ShowEmote(0, 0x102, 40);
    Actor_EnableActionCallback(0, a0);
    Event_Wait(10);
    Actor_ShowEmote(22, 0x103, 0);
    Func_020074f0(22, p8);
    Func_020074ee(0);
    Event_Wait(20);
    Actor_RunRepeatedMotion(22, 2);
    *(s32 *)(rec8 + 24) = 0x10000;
    *(s32 *)(rec8 + 28) = 0x10000;
    record = Value1(Engine_ActorGet, 0);
    *(s32 *)(record + 24) = 0x10000;
    *(s32 *)(record + 28) = 0x10000;
    Event_SetMessage(0xfce);
    Event_AskYesNo(22, 0);
    record = Actor_Get(22);
    *(s32 *)(record + 108) = 0x200d72d;
    Actor_EnableActionCallback(22, 0x200e248);
    Event_End();
}

void SceneState_RunTablePairWhenActor22State2(void)
{
    u8 *p = Actor_Get(22);
    if (GameFlag_IsSet(0x823) != 0) {
        u8 *q = p;
        q += 100;
        if (*(s16 *)q == 2) {
            FieldScene_RunScene373_02001490((s32)Data_0200f748, (s32)Data_0200f7c4);
        }
    }
}

void SceneState_RunTablePairByActor22State(void)
{
    u8 *rec = Actor_Get(22);
    if (GameFlag_IsSet(0x823) != 0) {
        u8 *q = rec;
        s32 v;
        q += 100;
        v = *(s16 *)q;
        if (v == 1) {
            FieldScene_RunScene373_02001490((s32)Data_0200f748, (s32)Data_0200f6cc);
        } else if (v == 2) {
            FieldScene_RunScene373_02001490((s32)Data_0200f748, (s32)Data_0200f7c4);
        }
    }
}

/* Runs a scripted movement/pose sequence for actors 0, 1 and 8, reading two
 * lookup records along the way (one 32-bit-field record, one 16-bit-field
 * record) to copy their stored values onto actor 1. */
void FieldScene_RunSecondaryActorSequence(void)
{
    u32 i;
    s32 record;
    s32 slot_table;

    Event_Begin();
    Actor_SetChildValue(0, 0);
    Actor_FaceActor(8, 0, 20);
    slot_table = (s32)Data_00001c45;
    Event_SetMessage(slot_table);
    Actor_StartRepeatedMotion(8, 2);
    Event_ShowMessageAndWait(8, 0, 20);
    Camera_SetSpeed(0x10000, 0x2000); /* main:0808a208 */
    Camera_MoveTo(0x18e0000, -1, 0x2460000, 1);
    Actor_SetSpeed(0, 0xcccc, 0x6666);
    Actor_SetSpeed(1, 0xcccc, 0x6666);
    Actor_WalkToAndWait(0, 0x1a4, 0x260);
    Actor_FaceDirection(0, 0xa000, 0);
    Actor_FaceDirection(8, 0x3000, 0);
    record = Scene_GetRecord_1(0);
    if (record != 0) {
        /* Copy the record's fields at +8 and +16 onto actor 1. */
        Actor_SetPosition(1, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Actor_WalkToAndWait(1, 0x192, 0x260);
    Actor_FaceDirection(1, 0xd000, 20);
    Event_ShowMessage(0x1001, 0);
    Actor_FaceDirection(8, 0x5000, 20);
    Actor_SetAnimationAndWait(8, 3); /* main:0808a110 */
    Event_ShowMessage(0x4008, 0);
    Actor_SetAnimation(0, 3);
    Actor_SetAnimationAndWait(1, 3); /* main:0808a110 */
    Event_Wait(20);
    Actor_RunRepeatedMotion(8, 2); /* main:0808a138 */
    Event_OpenMessage(0x4008, 0); /* main:0808a178 */
    if (Event_ChooseYesNo(0, 0) == 1) { /* main:0808a070 */
        bump_step(1);
        Actor_StartRepeatedMotion(8, 1);
    }
    Event_ShowMessageAndWait(0x4008, 0, 40); /* main:0808a188 */
    Actor_ShowEmote(8, 0x105, 60);
    /* Pass the slot table's field at +6 for the slot passed above. */
    Event_SetMessage((slot_table + 6)); /* main:0808a170 */
    Event_ShowMessageAndWait(0x4008, 0, 20); /* main:0808a188 */
    Actor_RunRepeatedMotion(1, 1); /* main:0808a138 */
    Event_Wait(40);
    Event_ShowMessageAndWait(0x1001, 0, 40); /* main:0808a188 */
    Actor_RunRepeatedMotion(8, 1); /* main:0808a138 */
    Actor_FaceDirection(8, 0xd000, 20);
    Event_ShowMessage(0x4008, 0);
    Actor_SetAnimationAndWait(1, 3); /* main:0808a110 */
    Event_ShowMessageAndWait(0x1001, 0, 120); /* main:0808a188 */
    Event_ShowMessageAndWait(0x4008, 0, 20); /* main:0808a188 */
    Actor_ShowEmote(1, 0x105, 40);
    Event_ShowMessageAndWait(0x1001, 0, 40); /* main:0808a188 */
    Actor_SetAnimationAndWait(8, 4); /* main:0808a110 */
    Event_ShowMessageAndWait(0x4008, 0, 20);
    Actor_SetAnimationAndWait(1, 3); /* main:0808a110 */
    Event_Wait(40);
    Actor_FaceDirection(8, 0x5000, 20);
    Event_ShowMessageAndWait(0x4008, 0, 10); /* main:0808a188 */
    Actor_SetAnimation(0, 3);
    Actor_SetAnimationAndWait(1, 3); /* main:0808a110 */
    Event_Wait(20);
    Actor_SetAnimationAndWait(8, 3);
    Actor_SetAnimation(1, 2);
    record = Scene_GetRecord_2(0);
    if (record != 0) {
        /* Copy the record's fields at +10 and +18 onto actor 1. */
        Actor_SetDestination(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Actor_WaitForMove(1);
    Actor_SetPosition(1, 0, 0);
    GameFlag_Set(0x303);
    Event_End(); /* main:0808a020 */
}

/* Runs the actor 0 / 1 / 9 setup sequence: position, speed and animation
 * calls in a fixed order. Two steps look up a record for one actor and
 * copy a coordinate pair out of it (offsets +8/+16 as s32, then +10/+18
 * as s16) into the call configuring another actor. */
void FieldScene_RunPrimaryActorSequence(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    Camera_MoveTo(0x1650000, -1, 0x2e20000, 1);
    Actor_WalkToAndWait(0, 0x16f, 0x2e9);
    Actor_FaceDirection(0, 0xa000, 0);
    record = Scene_GetRecord_1_02002f14(0);
    if (record != 0) {
        /* Copy the s32 coordinate pair at +8/+16 of the looked-up record. */
        Actor_SetPosition(1, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Actor_WalkToAndWait(1, 0x15a, 0x2e9);
    Actor_FaceDirection(1, 0xd000, 20);
    Event_SetMessage(0x1c53);
    Event_ShowMessage(1, 0);
    Actor_RunRepeatedMotion(9, 2);
    Actor_ShowEmote(9, 0x100, 0);
    Actor_FaceDirection(9, 0x3000, 10);
    Actor_FaceDirection(9, 0x5000, 10);
    Actor_FaceDirection(9, 0x3000, 40);
    Event_ShowMessageAndWait(9, 0, 20);
    Actor_SetAnimation(0, 3);
    Actor_SetAnimationAndWait(1, 3);
    Actor_RunRepeatedMotion(9, 1);
    Actor_FaceDirection(9, 0x5000, 10);
    Event_ShowMessageAndWait(9, 0, 20);
    Actor_StartRepeatedMotion(1, 1);
    Actor_ShowEmote(1, 0x103, 40);
    Event_ShowMessageAndWait(1, 0, 10);
    Actor_SetAnimationAndWait(9, 3);
    Event_ShowMessageAndWait(9, 0, 20);
    Actor_FaceDirection(0, 0x8000, 0);
    Actor_FaceDirection(1, 0x1000, 40);
    Actor_SetAnimationAndWait(9, 4);
    Event_ShowMessage(9, 0);
    Actor_FaceDirection(0, 0xb000, 0);
    Actor_FaceDirection(1, 0xd000, 10);
    Actor_RunRepeatedMotion(1, 2);
    Event_Wait(20);
    Event_ShowMessageAndWait(1, 0, 10);
    Actor_SetAnimationAndWait(9, 3);
    Event_ShowMessageAndWait(9, 0, 20);
    Actor_ShowEmote(0, 0x102, 0);
    Actor_ShowEmote(1, 0x102, 80);
    Event_ShowMessageAndWait(1, 0, 20);
    Actor_RunRepeatedMotion(9, 2);
    Event_ShowMessageAndWait(9, 0, 20);
    Actor_FaceDirection(1, 0x1000, 20);
    Event_OpenMessage(1, 0); /* main:0808a178 */
    if (Event_ChooseYesNo(0, 0) == 0) {
        Actor_ShowEmote(1, 0x105, 60);
    } else {
        bump_step(1);
    }
    Event_ShowMessageAndWait(1, 0, 20);
    Actor_FaceDirection(1, 0xd000, 10);
    Event_SetMessage((s32)Data_00001c60);
    Event_ShowMessage(1, 0);
    Actor_SetAnimationAndWait(9, 3);
    Event_ShowMessageAndWait(9, 0, 20);
    Actor_SetAnimationAndWait(1, 3);
    Event_Wait(20);
    Actor_FaceDirection(1, 0x1000, 20);
    Actor_SetAnimationAndWait(1, 3);
    Actor_SetAnimationAndWait(0, 3);
    Actor_SetAnimation(1, 2);
    record = Scene_GetRecord_2_02002f14(0);
    if (record != 0) {
        /* Copy the s16 coordinate pair at +10/+18 of the looked-up record. */
        Actor_SetDestination(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Actor_WaitForMove(1);
    Actor_SetPosition(1, 0, 0);
    GameFlag_Set(0x304);
    Event_End();
}

/* Sets up actors 12, 13, 14 and 20 with shared data and movement/speed
 * parameters, then drives actor 11 through a further sequence of moves. */
void FieldScene_RunCompanionActorSequence(void)
{
    u32 i;
    s32 actor_data;
    s32 shared_data;

    Event_Begin();
    actor_data = Actor_Get(ACTOR_A);
    Actor_SetSpriteFlags(actor_data, 0);
    actor_data = Actor_Get(ACTOR_B);
    Actor_SetSpriteFlags(actor_data, 0);
    actor_data = Actor_Get(ACTOR_C);
    Actor_SetSpriteFlags(actor_data, 0);
    Actor_SetAnimation(ACTOR_A, 0);
    Actor_SetAnimation(ACTOR_B, 0);
    Actor_SetAnimation(ACTOR_C, 0);
    Task_Wait(20);
    Work_SetValuesIfNonNegative(0x20000, 0x20000, 0x10000);
    shared_data = SHARED_DATA;
    Actor_EnableActionCallback(ACTOR_A, shared_data);
    Task_Wait(10);
    ObjectMotion_EnableActionAndSetCallback_2(ACTOR_B, shared_data);
    Work_SetValuesIfNonNegative(-1, -1, 0xe666);
    Task_Wait(20);
    ObjectMotion_MarkActiveAndSetActionCallback_1(ACTOR_C, shared_data);
    Actor_ShowEmote(ACTOR_D, 0x100, 40);
    Actor_RunRepeatedMotion(ACTOR_D, 2);
    Actor_FaceDirection(ACTOR_D, 0xd000, 10);
    Event_SetMessage(0x1c90);
    Event_ShowMessageAndWait(ACTOR_D, 0, 40);
    Actor_FaceActor(ACTOR_D, 0, 20);
    Event_ShowMessage(ACTOR_D, 0);
    Actor_FaceDirection(ACTOR_D, 0x8000, 10);
    GameFlag_Set(0x305);
    Event_End();
}

/*
 * The owner extends through its one literal pool word. The run length is
 * unsigned and the selector advances once per iteration. The Func_ aliases
 * name the call words encoded in the overlay image, not verified runtime
 * addresses.
 */
void SceneActor_ResetActorRun(s32 first, u32 count, s32 mode)
{
    s32 selector = first;
    u32 i;

    if (mode == 0) {
        for (i = 0; i < count; i++) {
            struct Resource373Actor *actor = Actor_Get(selector);

            actor->flag55 = 0;
            Actor_SetSpriteFlags(actor, 0);
            actor->field08 = 0x01860000;   /* 0xc3 << 17. */
            actor->field0c = 0x00a00000;   /* 0xa0 << 16. */
            actor->field10 = 0x034a0000;   /* The literal pool word. */
            selector++;
        }
        return;
    }

    for (i = 0; i < count; i++) {
        Actor_SetPosition(selector, 0, 0);
        selector++;
    }
}

void FieldScene_RunLargeStagingSequence(void)
{
    u32 i;
    s32 rec = 0;
    s32 rec3;
    u8 *rec8;
    u8 *record;
    s32 none2;
    s32 base7_200e590;
    s32 base5_ee8;
    s32 p1;
    s32 k;
    s32 p2;
    s32 facing;

    rec3 = Value1_020034c8(Engine_ActorGet, 0);
    rec8 = Value1_020034c8(Engine_ActorGet, 14);
    Event_Begin();
    Camera_MoveTo(-1, -1, -1, 0);
    Task_Wait(1);
    *(u8 *)(Func_020095fa() + 85) = 0;
    Task_Wait(1);
    Map_CopyCellAttributes(49, 53, 8, 4, 20, 50);
    Map_CopyCellsTo(2, 102, 84, 41, 2, 1);
    Map_CopyCellsTo(1, 102, 83, 41, 1, 1);
    Map_CopyCellsTo(0, 103, 82, 42, 1, 1);
    rec = Engine_ActorGet(11);
    *(u8 *)(rec + 85) = 0;
    k = 0x1840000;
    *(s32 *)(rec + 12) = 0xa00000;
    *(s32 *)(rec + 16) = 0x3480000;
    *(s32 *)(rec + 8) = k;
    Actor_SetSpriteFlags(rec, 0);
    rec = Engine_ActorGet(12);
    *(u8 *)(rec + 85) = 0;
    *(s32 *)(rec + 12) = 0xa00000;
    *(s32 *)(rec + 16) = 0x34c0000;
    *(s32 *)(rec + 8) = k;
    Actor_SetSpriteFlags(rec, 0);
    rec = Engine_ActorGet(13);
    *(u8 *)(rec + 85) = 0;
    *(s32 *)(rec + 16) = 0x3500000;
    *(s32 *)(rec + 12) = 0xa00000;
    *(s32 *)(rec + 8) = k;
    Actor_SetSpriteFlags(rec, 0);
    record = Engine_ActorGet(11);
    Actor_SetSpriteFlags((s32)record, 0);
    record = Engine_ActorGet(12);
    Actor_SetSpriteFlags((s32)record, 0);
    Actor_SetAnimation(0, 11);
    base7_200e590 = (s32)Data_0200e590;
    Actor_EnableActionCallback(0, base7_200e590);
    Func_0200955e();
    base5_ee8 = (s32)Data_00000ee8;
    Func_0200958a(base5_ee8, 0, 0);
    Func_02009576();
    Camera_MoveTo(0x1530000, 0xa00000, 0x4950000, 0);
    Map_Redraw();
    Task_Wait(1);
    Camera_SetSpeed(0x547a, 0xa8f);
    Camera_MoveTo(0x1280000, 0xa00000, 0x3990000, 1);
    Actor_SetPosition(5, 0x1990000, 0x46e0000);
    Task_Wait(1);
    Actor_SetSpeed(5, 0xb333, 0x5999);
    Actor_WalkTo(5, 0x1a4, 0x42c);
    Func_02009794();
    gEventWork->transition_frames = 60;
    Event_OpenScreen();
    Actor_WaitForMove(5);
    Actor_SetSpeed(5, 0x10000, 0x8000);
    Actor_WalkToAndWait(5, 0x155, 0x428);
    Actor_SetSpeed(5, 0xcccc, 0x6666);
    Actor_WalkToAndWait(5, 0x167, 0x409);
    Actor_SetSpeed(8, 0x8000, 0x4000);
    Actor_WalkTo(8, 0x13e, 0x3b3);
    Actor_SetAnimation(8, 2);
    Actor_WalkToAndWait(5, 0x19c, 0x409);
    Actor_WalkToAndWait(5, 0x19c, 0x3fb);
    Actor_WalkToAndWait(5, 0x176, 0x3f0);
    Actor_WalkToAndWait(5, 0x15b, 0x3bb);
    Actor_WalkToAndWait(8, 0x13e, 0x3b3);
    Actor_FaceEachOther(5, 8, 40);
    Actor_RunRepeatedMotion(8, 2);
    Actor_SetAnimationAndWait(5, 3);
    Event_Wait(10);
    Actor_WalkTo(8, 0x17b, 0x3f9);
    Camera_SetSpeed(0x8000, 0x1000);
    Actor_WalkToAndWait(5, 0x14d, 0x398);
    Actor_WalkToAndWait(5, 0x12b, 0x39c);
    Camera_WaitForMove();
    Event_Wait(10);
    Actor_FaceDirection(5, 0xf000, 30);
    Actor_RunRepeatedMotion(5, 2);
    Event_Wait(20);
    Camera_SetSpeed(0x20000, 0x4000);
    Camera_MoveTo(0x1830000, 0xa00000, 0x3620000, 1);
    Camera_WaitForMove();
    Actor_Jump(10, 2, 20);
    Event_SetMessage((base5_ee8 + 1));
    Event_ShowMessageAndWait(0x100a, 0, 10);
    *(s32 *)(rec3 + 24) = 0x10000;
    *(s32 *)(rec3 + 28) = 0x10000;
    Actor_EnableActionCallback(0, 1);
    Actor_FaceEachOther(10, 0, 40);
    Actor_SetAttachedEffect(0, 0x102);
    Actor_RunRepeatedMotion(0, 2);
    Event_Wait(40);
    Actor_StartRepeatedMotion(10, 2);
    Event_Wait(40);
    Event_ShowMessageAndWait(0x100a, 0, 40);
    Actor_SetAnimation(0, 11);
    Actor_EnableActionCallback(0, base7_200e590);
    Camera_FollowActor(5, 1);
    Camera_WaitForMove();
    Event_Wait(40);
    Actor_SetAnimationAndWait(5, 3);
    Actor_FaceDirection(5, 0xd000, 10);
    Actor_WalkToAndWait(5, 0x138, 0x2f7);
    Actor_WalkToAndWait(5, 0x169, 0x2f8);
    Event_Wait(20);
    Actor_FaceDirection(5, 0x8000, 40);
    Actor_FaceDirection(5, 0, 40);
    Event_ShowMessageAndWait(0x6001, 0, 10);
    Actor_ShowEmote(5, 0x100, 0);
    Actor_Jump(5, 4, 40);
    Actor_FaceDirection(5, 0xc000, 30);
    Actor_RunRepeatedMotion(5, 2);
    Event_Wait(40);
    Event_ShowMessageAndWait(5, 0, 30);
    Actor_SetAnimationAndWait(5, 3);
    Event_Wait(10);
    Camera_SetSpeed(0x40000, 0x8000);
    Camera_MoveTo(0x18c0000, -1, 0x24c0000, 1);
    Actor_WalkToAndWait(5, 0x1c8, 0x2e3);
    Camera_WaitForMove();
    Event_Wait(10);
    Actor_ShowEmote(1, 0x105, 40);
    Actor_RunRepeatedMotion(1, 2);
    Event_Wait(20);
    FieldScene_RunStep8C();
    Actor_SetAnimation(1, 17);
    Event_ShowMessageAndWait(0x2001, 0, 20);
    Audio_PlayCue(131);
    for (i = 0; i < 60; i++) {
        OverlayObject_UpdateOnFrameBit1(Engine_ActorGet(1));
        Task_Wait(1);
    }
    Actor_SetSpritePriority(1, 1);
    p1 = 0x200d5b1;
    Call2(Func_020097e0, p1, 0xc80);
    p2 = 0x200d5d1;
    Value2_020034c8(Func_020097ee, p2, 0xc80);
    record = Engine_ActorGet(14);
    Actor_SetSpriteFlags((s32)record, 0);
    none2 = 0;
    rec8[85] = none2;
    *(s32 *)((s32)rec8 + 8) = 0x1ac0000;
    *(s32 *)((s32)rec8 + 12) = 0xd00000;
    *(s32 *)((s32)rec8 + 16) = 0x2480000;
    facing = 0x8000;
    *(u16 *)((s32)rec8 + 6) = facing;
    *(s32 *)((s32)rec8 + 108) = 0x200d75d;
    Event_Wait(4);
    Actor_SetSpeed(14, 0x20000, 0x20000);
    Call4(Func_020098d2, (s32)rec8, 0x1980000, 0xd00000, 0x2480000);
    Event_Wait(40);
    Actor_SetSpeed(9, 0x2666, 0x1333);
    Actor_SetSpeed(14, 0x2666, 0x1333);
    Engine_ActorGet(9);
    Call4(Func_020099a6, (s32)rec8, 0x1880000, 0xd00000, 0x2480000);
    Actor_MoveToAndWait(9, 0x17a, 0x248);
    Event_Wait(20);
    Event_ShowMessageAndWait(0x2005, 0, 10);
    *(s32 *)((s32)rec8 + 108) = none2;
    Actor_SetSpritePriority(1, 2);
    Actor_Get(1)->priority_flags |= ACTOR_PRIORITY_AUTOMATIC;
    Func_02009966(p1);
    Func_0200996c(p2);
    Task_Wait(1);
    Actor_SetChildValue(1, 0);
    Actor_SetChildValue(9, 0);
    Actor_FaceDirection(1, 0x3000, 0);
    Actor_SetAnimation(1, 1);
    Effect_SpawnRisingDustBurst((s32)rec8);
    FieldScene_RunSingleStep();
    Event_Wait(10);
    Actor_WalkToAndWait(5, 0x1a8, 0x270);
    Actor_FaceEachOther(1, 5, 60);
    Actor_RunRepeatedMotion(1, 2);
    Event_Wait(10);
    Event_ShowMessageAndWait(0x6001, 0, 20);
    Actor_SetAnimationAndWait(5, 3);
    Event_Wait(10);
    Event_ShowMessageAndWait(5, 0, 20);
    Actor_ShowEmote(1, 0x102, 40);
    Actor_SetAnimationAndWait(5, 4);
    Event_Wait(10);
    Event_ShowMessageAndWait(5, 0, 20);
    Actor_ShowEmote(1, 0x101, 80);
    Actor_RunRepeatedMotion(5, 1);
    Event_Wait(10);
    Event_ShowMessageAndWait(5, 0, 20);
    Actor_ShowEmote(1, 0x102, 80);
    Actor_SetAnimationAndWait(5, 4);
    Event_Wait(10);
    Event_ShowMessageAndWait(5, 0, 20);
    Actor_SetAnimationAndWait(1, 3);
    Event_Wait(10);
    Actor_WalkToAndWait(5, 0x184, 0x25c);
    Actor_FaceDirection(5, 0xa000, 20);
    Actor_RunRepeatedMotion(5, 2);
    Event_Wait(10);
    Event_ShowMessageAndWait(0x1005, 0, 20);
    Actor_SetAnimationAndWait(5, 4);
    Event_Wait(10);
    Actor_ShowEmote(1, 0x105, 40);
    Actor_RunRepeatedMotion(1, 2);
    Event_Wait(10);
    Actor_FaceDirection(5, 0, 0);
    Actor_FaceDirection(1, facing, 40);
    Call11(Engine_EventShowTwoMessagesAndWait, 1, 1, 2, 25, 2, 5, 10, 14, 4, 14, none2);
    Event_Wait(40);
    Actor_SetAttachedEffect(5, 0x102);
    Actor_SetAttachedEffect(1, 0x102);
    Event_Wait(80);
    Actor_ShowEmote(5, 0x101, 40);
    Event_ShowMessageAndWait(0x1005, 0, 20);
    Actor_SetAttachedEffect(1, 0x102);
    Event_Wait(80);
    Actor_SetAnimationAndWait(1, 4);
    Event_Wait(10);
    Actor_ShowEmote(5, 0x101, 40);
    Event_ShowMessageAndWait(0x1005, 0, 20);
    Actor_RunRepeatedMotion(1, 2);
    Event_Wait(40);
    Actor_SetAnimationAndWait(1, 4);
    Event_Wait(20);
    Actor_ShowEmote(5, 0x101, 80);
    Actor_SetAnimationAndWait(5, 4);
    Event_Wait(10);
    Event_ShowMessageAndWait(0x1005, 0, 10);
    Actor_FaceDirection(5, 0x1000, 40);
    Actor_FaceActor(5, 1, 40);
    Actor_RunRepeatedMotion(1, 2);
    Event_Wait(10);
    Actor_WalkToAndWait(5, 0x17c, 0x26c);
    Event_Wait(10);
    Event_ShowMessageAndWait(0x1005, 0, 10);
    Actor_FaceDirection(1, 0x5000, 30);
    Actor_SetAttachedEffect(1, 0x102);
    Event_Wait(80);
    Actor_RunRepeatedMotion(1, 2);
    Event_Wait(10);
    Event_ShowMessageAndWait(0x6001, 0, 20);
    Actor_SetAnimationAndWait(5, 4);
    Event_Wait(10);
    Actor_ShowEmote(1, 0x101, 80);
    Actor_SetAnimationAndWait(5, 4);
    Event_Wait(10);
    Actor_SetSpeed(1, 0xcccc, 0x6666);
    Actor_WalkToAndWait(1, 0x19c, 0x25c);
    Actor_FaceDirection(1, 0x5000, 20);
    Actor_RunRepeatedMotion(5, 1);
    Event_Wait(10);
    Event_ShowMessageAndWait(0x1005, 0, 20);
    Actor_ShowEmote(1, 0x101, 80);
    Actor_Jump(5, 4, 30);
    Event_ShowMessageAndWait(0x1005, 0, 20);
    Actor_RunRepeatedMotion(1, 3);
    Event_Wait(10);
    Event_ShowMessageAndWait(0x6001, 0, 20);
    Event_Wait(30);
    Actor_FaceDirection(5, 0xe000, 40);
    Actor_SetAnimationAndWait(5, 3);
    Event_ShowMessageAndWait(0x1005, 0, 20);
    Actor_ShowEmote(1, 0x101, 80);
    Actor_ShowEmote(5, 0x103, 40);
    Actor_SetSpeed(5, 0xcccc, 0x6666);
    Actor_WalkTo(5, 0x1ac, 0x274);
    Event_Wait(20);
    Actor_FaceDirection(1, 0x3000, 0);
    Actor_WaitForMove(5);
    Event_ShowMessage(0x5001, 0);
    Actor_SetAnimation(5, 1);
    Event_Wait(60);
    Actor_FaceDirection(5, 0xb000, 30);
    Actor_RunRepeatedMotion(1, 2);
    Actor_WalkToAndWait(5, 0x1ac, 0x274);
    Actor_FaceDirection(5, 0xb000, 20);
    Actor_RunRepeatedMotion(1, 3);
    Event_Wait(10);
    Actor_SetAnimationAndWait(5, 4);
    Event_Wait(10);
    Event_ShowMessageAndWait(0x2005, 0, 20);
    Actor_SetAttachedEffect(1, 0x102);
    Event_Wait(40);
    Event_ShowMessageAndWait(0x5001, 0, 20);
    Actor_SetAnimation(1, 3);
    Actor_SetAnimationAndWait(5, 3);
    Event_Wait(10);
    Actor_SetSpeed(5, 0x8000, 0x4000);
    Actor_SetSpeed(1, 0x8000, 0x4000);
    Actor_WalkTo(5, 0x1c2, 0x2ee);
    Actor_WalkTo(1, 0x1c2, 0x2ee);
    Event_Wait(60);
    gEventWork->transition_frames = 60;
    Event_CloseScreen();
    Event_WaitForScreen();
    Event_RequestExit(12);
    Event_End();
}

void FieldScene_RunStagedActorCutsceneSequence(void)
{
    u8 *scene;
    u8 *rec;
    u32 i;
    s32 turn_back;
    s32 turn_side;
    u8 *turned;
    s32 none;
    s32 flag;
    s32 callback_a;
    s32 callback_b;
    s32 callback_c;
    s32 callback_d;
    s32 callback_e;
    s32 callback_f;

    scene = Actor_Get(0);
    Event_Begin();
    Camera_MoveTo(-1, -1, -1, 0);
    Task_Wait(1);
    Map_CopyCellAttributes(49, 53, 8, 4, 20, 50);
    Map_CopyCellsTo(2, 102, 84, 41, 2, 1);
    Map_CopyCellsTo(1, 102, 83, 41, 1, 1);
    Map_CopyCellsTo(0, 103, 82, 42, 1, 1);
    Actor_SetPosition(21, 0x1880000, 0x3800000);
    turned = Actor_Get(21);
    /*
     * Overwritten at once, but the store must stay: its zero halfword
     * temporary is what the record byte stores below reuse out of a high
     * register.
     */
    *(u16 *)(turned + 6) = 0;
    turn_back = 0xc000;
    *(u16 *)(turned + 6) = turn_back;
    Actor_SetPosition(1, 0x12a0000, 0x2e00000);
    turned = Actor_Get(1);
    turn_side = 0x4000;
    *(u16 *)(turned + 6) = turn_side;
    Actor_SetPosition(5, 0x12a0000, 0x2f80000);
    turned = Actor_Get(5);
    *(u16 *)(turned + 6) = turn_side;
    Actor_SetAnimation(0, 11);
    Engine_ActorEnableActionCallback(0, 0x200e590);
    rec = Actor_Get(23);
    rec[85] = 0;
    *(s32 *)(rec + 8) = 0x1840000;
    *(s32 *)(rec + 12) = 0xa00000;
    *(s32 *)(rec + 16) = 0x3480000;
    Actor_SetSpriteFlags(rec, 0);
    rec = Actor_Get(24);
    rec[85] = 0;
    *(s32 *)(rec + 8) = 0x1840000;
    *(s32 *)(rec + 12) = 0xa00000;
    *(s32 *)(rec + 16) = 0x34c0000;
    Actor_SetSpriteFlags(rec, 0);
    rec = Actor_Get(25);
    rec[85] = 0;
    *(s32 *)(rec + 8) = 0x1840000;
    *(s32 *)(rec + 12) = 0xa00000;
    *(s32 *)(rec + 16) = 0x3500000;
    Actor_SetSpriteFlags(rec, 0);
    Func_0200a1fa()[85] = 0;
    Task_Wait(1);
    Camera_MoveTo(0x17f0000, 0xa00000, 0x36d0000, 0);
    Map_Redraw();
    Task_Wait(1);
    *(s32 *)(*(u8 **)0x03001ebc + 0x1c8) = 32;
    Event_OpenScreen();
    Actor_SetSpeed(5, 0x8000, turn_side);
    Actor_SetSpeed(1, 0x8000, turn_side);
    Engine_ActorEnableActionCallback(5, 0x200e614);
    Engine_ActorEnableActionCallback(1, 0x200e5cc);
    Event_Wait(40);
    Engine_ActorEnableActionCallback(0, 1);
    *(s32 *)(scene + 24) = 0x10000;
    *(s32 *)(scene + 28) = 0x10000;
    Call4(Engine_ActorFaceDirection, 0, 0xb000, 40, 0x10000);
    Actor_SetAnimationAndWait(0, 3);
    Event_Wait(10);
    Actor_SetSpeed(0, 0x4ccc, 0x2666);
    Actor_WalkToAndWait(0, 400, 840);
    Event_Wait(10);
    Actor_FaceDirection(0, turn_back, 30);
    Actor_RunRepeatedMotion(0, 1);
    Event_Wait(20);
    Actor_FaceDirection(0, 0x8000, 40);
    Func_0200976e();
    Actor_SetAnimation(0, 17);
    Func_0200a080((s32)Func_0200da09, 3200);
    for (i = 0; i < 40; i++) {
        OverlayObject_UpdateOnFrameBit1(scene);
        Task_Wait(1);
    }
    Actor_SetSpritePriority(0, 1);
    /* Callback symbols are pooled loads that stay after the preceding call. */
    callback_a = (s32)Func_0200d5c1;
    Value2_02003fb0(Func_0200a0a8, callback_a, 3200);
    callback_b = (s32)Func_0200d5e1;
    Call2(Func_0200a0b4_a, callback_b, 3200);
    Actor_SetSpeed(23, 0x3333, 0x1999);
    Actor_MoveToAndWait(23, 390, 832);
    Actor_FaceDirection(0, 0xc000, 0);
    Actor_MoveToAndWait(23, 400, 826);
    Event_Wait(20);
    {
        u8 *record = Actor_Get(0);
        u8 value = *(volatile u8 *)&record[35]; /* Keeps the byte in its own register. */

        record[35] = (u8)(value | 1);
    }
    Actor_SetAnimation(0, 1);
    Func_0200a114((s32)Func_0200da09);
    Func_0200a11a(callback_a);
    Func_0200a120(callback_b);
    Task_Wait(1);
    Actor_SetChildValue(0, 0);
    Actor_SetChildValue(23, 0);
    Actor_SetPosition(23, 0, 0);
    Event_Wait(20);
    Actor_SetAnimation(0, 11);
    Engine_ActorEnableActionCallback(0, 0x200e590);
    Event_Wait(120);
    Map_CopyCellsTo(7, 102, 84, 41, 2, 1);
    Engine_ActorEnableActionCallback(0, 1);
    *(s32 *)(scene + 24) = 0x10000;
    *(s32 *)(scene + 28) = 0x10000;
    Actor_SetAnimation(0, 1);
    Event_Wait(40);
    Actor_SetAnimationAndWait(0, 3);
    Actor_WalkToAndWait(0, 377, 843);
    Actor_FaceDirection(0, 0xc000, 40);
    Actor_FaceDirection(0, 0, 20);
    Actor_SetAnimation(0, 17);
    Call2(Func_0200a1c2, (s32)Func_0200da09, 3200);
    for (i = 0; i < 40; i++) {
        OverlayObject_UpdateOnFrameBit1(scene);
        Task_Wait(1);
    }
    Actor_SetSpritePriority(0, 1);
    callback_c = (s32)Func_0200d5c1;
    Value2_02003fb0(Func_0200a1ea, callback_c, 3200);
    callback_d = (s32)Func_0200d5f1;
    Call2(Func_0200a1f6, callback_d, 3200);
    Actor_SetSpeed(24, 0x3333, 0x1999);
    Actor_MoveToAndWait(24, 390, 832);
    Actor_FaceDirection(0, 0xc000, 0);
    Actor_MoveToAndWait(24, 377, 828);
    Event_Wait(20);
    {
        u8 *record = Actor_Get(0);
        u8 value = *(volatile u8 *)&record[35];

        record[35] = (u8)(value | 1);
    }
    Actor_SetAnimation(0, 1);
    Func_0200a256((s32)Func_0200da09);
    Func_0200a25c(callback_c);
    Func_0200a262(callback_d);
    Task_Wait(1);
    Actor_SetChildValue(0, 0);
    Actor_SetChildValue(24, 0);
    Actor_SetPosition(24, 0, 0);
    Event_Wait(20);
    Actor_SetAnimation(0, 11);
    Engine_ActorEnableActionCallback(0, 0x200e590);
    Event_Wait(120);
    Map_CopyCellsTo(6, 102, 83, 41, 1, 1);
    Engine_ActorEnableActionCallback(0, 1);
    *(s32 *)(scene + 24) = 0x10000;
    *(s32 *)(scene + 28) = 0x10000;
    Actor_SetAnimation(0, 1);
    Event_Wait(40);
    Actor_SetAnimationAndWait(0, 3);
    Actor_WalkToAndWait(0, 360, 855);
    Actor_FaceDirection(21, 0xb000, 10);
    Actor_FaceDirection(0, 0xc000, 30);
    Actor_FaceDirection(0, 0xd000, 20);
    Actor_SetAnimation(0, 17);
    Call2(Func_0200a312, (s32)Func_0200da09, 3200);
    for (i = 0; i < 40; i++) {
        OverlayObject_UpdateOnFrameBit1(scene);
        Task_Wait(1);
    }
    Actor_SetSpritePriority(0, 1);
    callback_e = (s32)Func_0200d5c1;
    Value2_02003fb0(Func_0200a384, callback_e, 3200);
    callback_f = (s32)Func_0200d601;
    Value2_02003fb0(Func_0200a390, callback_f, 3200);
    Actor_SetSpeed(25, 0x3333, 0x1999);
    Actor_MoveToAndWait(25, 390, 832);
    Actor_FaceDirection(0, 0xc000, 0);
    Actor_MoveToAndWait(25, 360, 837);
    Event_Wait(20);
    {
        u8 *record = Actor_Get(0);
        u8 value = *(volatile u8 *)&record[35];

        record[35] = (u8)(value | 1);
    }
    Actor_SetAnimation(0, 1);
    Func_0200a3ec((s32)Func_0200da09);
    Func_0200a3f2(callback_e);
    Func_0200a3f8(callback_f);
    Task_Wait(1);
    Actor_SetChildValue(0, 0);
    Actor_SetChildValue(25, 0);
    Actor_SetPosition(25, 0, 0);
    Event_Wait(20);
    Actor_SetAnimation(0, 11);
    Engine_ActorEnableActionCallback(0, 0x200e590);
    Event_Wait(120);
    FieldScene_RunSingleStep();
    Map_CopyCellsTo(5, 103, 82, 42, 1, 1);
    Engine_ActorEnableActionCallback(0, 1);
    *(s32 *)(scene + 24) = 0x10000;
    *(s32 *)(scene + 28) = 0x10000;
    Actor_Jump(21, 2, 20);
    Event_SetMessage(0xf03);
    Event_ShowMessageAndWait(21, 0, 10);
    Actor_FaceDirection(0, 0x1000, 10);
    Func_020078a2(21, 5, 6, 0);
    Actor_SetSpeed(21, 0x4ccc, 0x2666);
    Actor_WalkToAndWait(21, 397, 832);
    Event_Wait(20);
    Actor_FaceDirection(21, 0x4000, 60);
    Actor_FaceDirection(21, 0xc000, 60);
    Actor_SetAnimationAndWait(21, 3);
    Actor_SetAnimationAndWait(21, 3);
    Event_ShowMessageAndWait(21, 0, 10);
    Actor_WalkToAndWait(21, 372, 832);
    Event_Wait(20);
    Actor_FaceDirection(21, 0x4000, 40);
    Actor_FaceDirection(21, 0x8000, 40);
    Actor_SetAnimationAndWait(21, 3);
    Actor_SetAnimationAndWait(21, 3);
    Event_ShowMessageAndWait(21, 0, 20);
    Actor_FaceDirection(21, 0x5000, 20);
    Actor_SetAnimationAndWait(21, 3);
    Event_ShowMessageAndWait(21, 0, 10);
    Actor_RunRepeatedMotion(0, 2);
    Event_Wait(20);
    Actor_SetAnimationAndWait(21, 4);
    Event_OpenMessage(21, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Actor_SetAnimationAndWait(21, 3);
        bump_step(1);
    } else {
        Actor_SetAnimationAndWait(21, 4);
    }
    Event_ShowMessageAndWait(21, 0, 20);
    Event_SetMessage(0xf0a);
    Actor_WalkToAndWait(21, 386, 841);
    Event_Wait(10);
    Actor_FaceDirection(21, 0xd000, 60);
    Actor_RunRepeatedMotion(21, 2);
    Event_ShowMessageAndWait(21, 0, 20);
    Actor_FaceDirection(21, 0x5000, 30);
    Event_OpenMessage(21, 0);
    if (Event_ChooseYesNo(0, 0) == 1) {
        bump_step(1);
    }
    Event_ShowMessageAndWait(21, 0, 20);
    Actor_FaceDirection(21, 0xd000, 60);
    Actor_RunRepeatedMotion(21, 2);
    Event_SetMessage(0xf0e);
    Event_ShowMessageAndWait(21, 0, 20);
    Actor_WalkToAndWait(21, 386, 825);
    Event_Wait(10);
    Actor_SetAnimationAndWait(21, 4);
    Event_Wait(60);
    Event_ShowMessageAndWait(21, 0, 60);
    Actor_FaceDirection(21, 0x5000, 10);
    Event_ShowMessageAndWait(21, 0, 10);
    Actor_WalkToAndWait(21, 372, 832);
    Actor_FaceDirection(21, 0x5000, 10);
    Actor_RunRepeatedMotion(0, 2);
    Event_Wait(20);
    Actor_SetAnimationAndWait(21, 3);
    Event_Wait(20);
    Event_ShowMessageAndWait(21, 0, 20);
    Actor_ShowEmote(0, 258, 60);
    Actor_RunRepeatedMotion(21, 2);
    Event_ShowMessageAndWait(21, 0, 20);
    Actor_SetAnimationAndWait(0, 3);
    Actor_SetAnimationAndWait(21, 3);
    Event_ShowMessageAndWait(21, 0, 10);
    Camera_SetSpeed(0x6666, 0xccc);
    Camera_MoveTo(0x1790000, 0xa00000, 0x35c0000, 1);
    Actor_SetSpeed(5, 0x10000, 0x8000);
    Actor_SetSpeed(1, 0x10000, 0x8000);
    Actor_WalkTo(1, 369, 904);
    Actor_WalkToAndWait(5, 392, 904);
    Actor_SetAnimation(1, 1);
    Func_02007b8a(5, 10, 11, 0);
    Actor_FaceDirection(5, 0xa000, 0);
    Event_ShowMessageAndWait(5, 0, 10);
    Actor_RunRepeatedMotion(21, 2);
    Event_Wait(10);
    Actor_FaceDirection(21, 0x3000, 0);
    Actor_FaceDirection(0, 0x1000, 20);
    Actor_Jump(5, 4, 0);
    Actor_WalkToAndWait(5, 392, 843);
    Actor_FaceDirection(5, 0x9000, 0);
    Actor_FaceDirection(21, 0x3000, 0);
    Actor_FaceDirection(0, 0xd000, 20);
    Call2((void (*)())Engine_ActorSetAnimationAndWait, 21, 3);
    Event_ShowMessageAndWait(21, 0, 10);
    Actor_SetAnimationAndWait(5, 3);
    Event_ShowMessageAndWait(5, 0, 10);
    Actor_SetAnimation(21, 3);
    Actor_SetAnimationAndWait(0, 3);
    Event_ShowMessageAndWait(21, 0, 20);
    Func_02007c4a(1, 10, 11, 0);
    Actor_SetSpeed(5, 0x4ccc, 0x2666);
    Actor_SetSpeed(1, 0x4ccc, 0x2666);
    Actor_WalkTo(1, 392, 843);
    Func_0200a9a4_a(5)[90] &= 0xfe;
    Actor_WalkToAndWait(5, 408, 843);
    Event_Wait(1);
    {
        u8 *record = Actor_Get(5);
        u8 value = *(volatile u8 *)&record[90];

        record[90] = (u8)(value | 1);
    }
    Actor_FaceDirection(5, 0x8000, 0);
    Actor_WaitForMove(1);
    Actor_SetAnimation(1, 1);
    Actor_FaceDirection(1, 0x8000, 30);
    Actor_Jump(21, 4, 30);
    Event_ShowMessageAndWait(21, 0, 20);
    Actor_SetAnimationAndWait(1, 3);
    Event_Wait(20);
    Actor_FaceDirection(21, 0x5000, 20);
    Event_ShowMessageAndWait(21, 0, 10);
    Actor_FaceDirection(0, 0xd000, 0);
    Actor_Jump(0, 2, 30);
    Actor_SetAttachedEffect(0, 258);
    Event_Wait(60);
    Actor_FaceDirection(21, 0x3000, 40);
    Actor_RunRepeatedMotion(1, 2);
    Event_ShowMessageAndWait(1, 0, 20);
    Actor_ShowEmote(21, 257, 80);
    Actor_FaceDirection(21, 0x5000, 30);
    Actor_ShowEmote(0, 258, 60);
    Actor_FaceDirection(21, 0x3000, 0);
    Actor_FaceDirection(0, 0xd000, 20);
    Event_ShowMessageAndWait(21, 0, 10);
    Actor_SetAnimationAndWait(1, 3);
    Event_Wait(80);
    Actor_FaceEachOther(5, 1, 30);
    Actor_StartRepeatedMotion(1, 2);
    Actor_RunRepeatedMotion(5, 2);
    Event_Wait(10);
    Actor_ShowEmote(21, 261, 60);
    Event_ShowMessageAndWait(21, 0, 10);
    Actor_FaceDirection(5, 0x8000, 0);
    Actor_FaceDirection(1, 0x8000, 10);
    Actor_RunRepeatedMotion(5, 2);
    Event_Wait(20);
    Event_ShowMessageAndWait(5, 0, 10);
    Actor_StartRepeatedMotion(21, 2);
    Event_Wait(40);
    Event_ShowMessageAndWait(21, 0, 10);
    Actor_SetAnimation(1, 3);
    Actor_SetAnimationAndWait(5, 3);
    Event_Wait(20);
    Actor_SetAnimationAndWait(21, 4);
    Event_ShowMessageAndWait(21, 0, 10);
    Actor_SetAnimation(1, 3);
    Actor_SetAnimationAndWait(5, 3);
    Actor_SetAnimationAndWait(21, 3);
    Event_ShowMessageAndWait(21, 0, 10);
    Actor_SetAnimation(1, 3);
    Actor_SetAnimationAndWait(5, 3);
    Event_Wait(20);
    Actor_RunRepeatedMotion(21, 2);
    Event_ShowMessageAndWait(21, 0, 20);
    Camera_SetSpeed(0x9999, 0x1333);
    Camera_MoveTo(0x1750000, 0xa00000, 0x3450000, 1);
    Actor_WalkToAndWait(21, 364, 816);
    Actor_FaceDirection(0, 0xd000, 0);
    Actor_FaceDirection(21, 0x3000, 10);
    Event_ShowMessageAndWait(21, 0, 40);
    Actor_FaceEachOther(5, 1, 30);
    Actor_StartRepeatedMotion(1, 2);
    Actor_RunRepeatedMotion(5, 2);
    Event_Wait(20);
    Actor_FaceDirection(1, 0x8000, 0);
    Call3((void (*)())Engine_ActorFaceDirection, 5, 0x8000, 20);
    Actor_FaceDirection(21, 0x5000, 20);
    Event_ShowMessageAndWait(21, 0, 10);
    Actor_ShowEmote(0, 261, 60);
    Actor_SetAnimationAndWait(21, 4);
    Event_OpenMessage(21, 0);
    none = 0; /* One zero shared by the placement call and the byte store. */
    if (Event_ChooseYesNo(0, 0) == 1) {
        bump_step(1);
    }
    Event_ShowMessageAndWait(21, 0, 20);
    Event_SetMessage(0xf27);
    Actor_ShowEmote(21, 259, 0);
    Actor_RunRepeatedMotion(21, 3);
    Event_ShowMessageAndWait(21, 0, 10);
    Actor_Jump(21, 4, 0);
    Actor_RunRepeatedMotion(21, 3);
    Actor_SetAnimation(21, 7);
    Event_Wait(5);
    Call11(Engine_EventShowTwoMessagesAndWait, 21, 14, 2, 24, 2, 1, 10, 14, 4, 14, none);
    Audio_PlayCue(161);
    rec = Actor_Get(21);
    {
        u8 value = *(volatile u8 *)&rec[90];

        *(u8 *)(*(s32 *)(rec + 80) + 38) = none;
        rec[90] = (u8)(value & 0xfe);
    }
    Actor_SetSpeed(21, 0x30000, 0x18000);
    Actor_WalkToAndWait(21, 364, 815);
    Event_Wait(4);
    for (i = 0; i != 4; i++) {
        *(s32 *)(rec + 16) += 0x18000;
        *(s32 *)(rec + 28) += -0x1999;
        Event_Wait(1);
    }
    Actor_SetPosition(21, 0, 0);
    Actor_SetSpeed(1, 0x30000, 0x18000);
    Actor_Jump(1, 6, 0);
    Actor_WalkToAndWait(1, 374, 827);
    Event_ShowMessage(5, 0);
    Actor_FaceDirection(1, 0xb000, 0);
    Actor_ShowEmote(5, 256, 0);
    Actor_StartRepeatedMotion(5, 2);
    Actor_ShowEmote(1, 256, 10);
    Actor_SetAnimation(1, 13);
    Actor_Jump(1, 2, 5);
    Audio_PlayCue(143);
    Work_SetValuesIfNonNegative(0, 0x40000, 0x10000);
    Map_CopyCellsTo(1, 102, 83, 41, 1, 1);
    Actor_SetSpriteFlags(Actor_Get(1), 0);
    Actor_FaceDirection(0, 0xd000, 10);
    Actor_StartRepeatedMotion(1, 3);
    Work_SetValuesIfNonNegative(-1, -1, 0xe666);
    MapRender_WaitForValues();
    Actor_ShowEmote(1, 258, 80);
    Actor_SetAnimation(21, 8);
    *(s32 *)(rec + 28) = 0x8000;
    Actor_SetPosition(21, 0x16c0000, 0x32b0000);
    for (i = 0; i != 5; i++) {
        *(s32 *)(rec + 28) += 0x1999;
        Event_Wait(1);
    }
    none = 0; /* Refreshed after the loops for the closing scene store. */
    Event_Wait(60);
    Actor_RunRepeatedMotion(1, 2);
    Actor_FaceDirection(1, 0x5000, 30);
    Actor_StartRepeatedMotion(1, 2);
    Actor_RunRepeatedMotion(5, 2);
    Event_Wait(60);
    Actor_RunRepeatedMotion(21, 2);
    Event_Wait(20);
    Event_ShowMessageAndWait(21, 0, 20);
    Camera_SetSpeed(0x4ccc, 0x999);
    Camera_MoveTo(0x1740000, 0xa00000, 0x35b0000, 1);
    Actor_SetSpeed(21, 0x30000, 0x18000);
    Actor_Jump(21, 6, 0);
    Actor_WalkToAndWait(21, 359, 835);
    Event_Wait(20);
    Actor_FaceDirection(21, 0x4000, 20);
    Actor_RunRepeatedMotion(21, 2);
    rec[35] &= 0xfe;
    Event_ShowMessageAndWait(21, 0, 80);
    Actor_ShowEmote(21, 257, 80);
    Actor_FaceDirection(21, 0, 60);
    Actor_RunRepeatedMotion(21, 3);
    Event_ShowMessageAndWait(21, 0, 10);
    Actor_SetAttachedEffect(21, 258);
    Event_Wait(80);
    Actor_FaceDirection(1, 0x5000, 40);
    Actor_ShowEmote(1, 258, 80);
    Actor_RunRepeatedMotion(1, 2);
    Event_ShowMessageAndWait(1, 0, 20);
    Actor_FaceDirection(1, 0x8000, 20);
    Actor_RunRepeatedMotion(1, 3);
    Event_Wait(10);
    Actor_RunRepeatedMotion(1, 3);
    Actor_SetSpriteFlags(Actor_Get(1), 1);
    Actor_Jump(1, 6, 0);
    Actor_SetAnimation(1, 1);
    Actor_SetSpeed(1, 0x40000, 0x20000);
    rec = Actor_Get(1);
    rec[90] &= 0xfe;
    Actor_SetDestination(1, 403, 827);
    Actor_SetAttachedEffect(5, 258);
    Actor_FaceDirection(5, 0xc000, 20);
    Event_ShowMessageAndWait(5, 0, 1);
    Actor_WaitForMove(1);
    Actor_FaceDirection(1, 0x5000, 20);
    Event_ShowMessageAndWait(1, 0, 20);
    Actor_ShowEmote(1, 256, 0);
    Actor_SetAnimation(1, 13);
    Actor_Jump(1, 2, 5);
    Actor_SetSpriteFlags(Actor_Get(1), 0);
    Map_CopyCellsTo(2, 102, 84, 41, 2, 1);
    Audio_PlayCue(143);
    Work_SetValuesIfNonNegative(0, 0x40000, 0x10000);
    Actor_RunRepeatedMotion(1, 3);
    Work_SetValuesIfNonNegative(-1, -1, 0xe666);
    MapRender_WaitForValues();
    Actor_ShowEmote(1, 258, 30);
    Actor_SetSpeed(5, 0x4ccc, 0x2666);
    Actor_WalkToAndWait(5, 408, 855);
    Event_Wait(60);
    Actor_RunRepeatedMotion(1, 2);
    Actor_ShowEmote(21, 261, 60);
    Actor_StartRepeatedMotion(5, 3);
    Actor_RunRepeatedMotion(0, 3);
    Event_Wait(80);
    Actor_FaceDirection(1, 0x4000, 30);
    Actor_RunRepeatedMotion(1, 3);
    Event_Wait(10);
    Actor_SetAnimationAndWait(5, 4);
    Event_Wait(80);
    Actor_SetAnimationAndWait(21, 3);
    Event_Wait(10);
    Event_ShowMessageAndWait(21, 0, 10);
    Actor_FaceDirection(5, 0xb000, 0);
    Actor_FaceDirection(0, 0xc000, 0);
    Actor_FaceDirection(21, 0, 60);
    Actor_FaceDirection(21, 0x4000, 60);
    Actor_SetAnimationAndWait(21, 4);
    Event_Wait(60);
    Actor_FaceDirection(21, 0, 80);
    Actor_ShowEmote(21, 261, 80);
    Event_ShowMessageAndWait(21, 0, 60);
    Actor_FaceDirection(0, 0xc000, 0);
    Actor_ShowEmote(0, 257, 0);
    Actor_ShowEmote(5, 257, 0);
    Actor_ShowEmote(1, 257, 60);
    Actor_SetAnimationAndWait(21, 4);
    Event_Wait(30);
    Event_ShowMessageAndWait(21, 0, 60);
    Actor_StartRepeatedMotion(1, 2);
    Actor_RunRepeatedMotion(5, 2);
    Event_Wait(20);
    Actor_FaceDirection(5, 0x8000, 60);
    Actor_FaceDirection(21, 0x4000, 30);
    Event_ShowMessageAndWait(21, 0, 30);
    Actor_SetAnimationAndWait(5, 3);
    Event_Wait(10);
    Event_ShowMessageAndWait(5, 0, 20);
    Actor_FaceDirection(21, 0, 30);
    Actor_SetAnimationAndWait(21, 4);
    Event_Wait(20);
    Event_ShowMessageAndWait(21, 0, 20);
    Actor_SetAnimationAndWait(5, 3);
    Event_Wait(20);
    Actor_RunRepeatedMotion(1, 3);
    Event_Wait(10);
    Event_ShowMessageAndWait(1, 0, 20);
    Actor_FaceDirection(0, 0xd000, 0);
    Actor_ShowEmote(21, 256, 0);
    Actor_RunRepeatedMotion(21, 3);
    Event_Wait(30);
    Event_ShowMessageAndWait(21, 0, 60);
    Actor_RunRepeatedMotion(1, 3);
    Actor_SetSpeed(1, 0x10000, 0x8000);
    Actor_SetSpriteFlags(Actor_Get(1), 0);
    Actor_Jump(1, 4, 0);
    Call3((void (*)())Engine_ActorWalkToAndWait, 1, 398, 828);
    Event_Wait(60);
    Actor_FaceDirection(21, 0x4000, 0);
    Actor_FaceDirection(0, 0xc000, 60);
    Actor_SetAnimationAndWait(0, 3);
    Event_Wait(60);
    Actor_SetAnimationAndWait(21, 3);
    Event_Wait(60);
    rec = Actor_Get(1);
    flag = 1; /* One shared mark bit for the three record flags. */
    rec[90] |= flag;
    rec = Actor_Get(5);
    rec[90] |= flag;
    rec = Actor_Get(0);
    Actor_SetSpeed(1, 0x10000, 0x8000);
    Actor_SetSpeed(5, 0x10000, 0x8000);
    Actor_FaceDirection(0, 0, 0);
    Actor_WalkTo(5, *(s16 *)(rec + 10) + 16, *(s16 *)(rec + 18));
    Actor_WalkToAndWait(1, *(s16 *)(rec + 10) + 16, *(s16 *)(rec + 18) - 16);
    Actor_WaitForMove(1);
    Actor_FaceDirection(1, 0x5000, 30);
    Actor_SetAnimation(1, 3);
    Actor_SetAnimation(5, 3);
    Actor_SetAnimationAndWait(0, 3);
    Event_Wait(40);
    Actor_WalkToAndWait(5, *(s16 *)(rec + 10), *(s16 *)(rec + 18));
    Actor_SetPosition(5, 0, 0);
    Actor_WalkToAndWait(1, *(s16 *)(rec + 10), *(s16 *)(rec + 18));
    Actor_SetPosition(1, 0, 0);
    Party_AddMembers(1, 5);
    Camera_MoveTo(0x1790000, 0xa00000, 0x3770000, 1);
    Func_0200882c(0, 13, 10, 0);
    Actor_WalkToAndWait(0, 376, 912);
    Actor_FaceDirection(0, 0xc000, 0);
    {
        u8 *record = Actor_Get(21);
        u8 value = *(volatile u8 *)&record[90];

        record[90] = (u8)(value | flag);
    }
    Call4(Func_02008860, 21, 6, 5, 0);
    Actor_WalkToAndWait(21, 373, 887);
    Actor_FaceDirection(21, 0x4000, 0);
    Actor_FaceDirection(0, 0xc000, 40);
    Actor_SetAnimation(21, 3);
    Actor_SetAnimationAndWait(0, 3);
    Event_Wait(20);
    Camera_FollowActor(0, 1);
    Camera_WaitForMove();
    Event_Wait(100);
    Map_CopyCellAttributes(49, 46, 8, 4, 20, 50);
    GameFlag_Set(514);
    GameFlag_Clear(303);
    scene[85] = 3;
    *(s32 *)(scene + 12) = 0xa00000;
    *(s32 *)(scene + 60) = 0x80000000;
    *(s32 *)(scene + 40) = none;
    Event_End();
}

void FieldScene_RunStep8C(void)
{
    Psynergy_Begin(0x8c, 0);
}

void FieldScene_RunSingleStep(void)
{
    Func_0200b6d4();
}

void SceneState_SetValue1ThenCall(void)
{
    Actor_Get(1);
    SceneEffect_UpdateObjectOnOddFrames();
}

void SceneState_SetValue0ThenCall(void)
{
    Actor_Get(0);
    SceneEffect_UpdateObjectOnOddFrames();
}

void FieldScene_RunStep9(void)
{
    Actor_Get(9);
    SceneEffect_UpdateObjectOnOddFramesOnly();
}

void FieldScene_RunStep17(void)
{
    Actor_Get(0x17);
    SceneEffect_UpdateObjectOnOddFramesOnly();
}

void SceneState_SetValue24ThenCall(void)
{
    Actor_Get(0x18);
    SceneEffect_UpdateObjectOnOddFramesOnly();
}

void SceneState_SetValue25ThenCall(void)
{
    Actor_Get(0x19);
    SceneEffect_UpdateObjectOnOddFramesOnly();
}

s32 Runtime_ComputeFixedPointDistance(s32 *first_position, s32 *second_position)
{
    typedef s32(*FixedPointSqrt)(s32);

    s32 delta_x = (*first_position++ - *second_position++) >> 16;
    s32 delta_y = (*first_position++ - *second_position++) >> 16;
    s32 delta_z = (*first_position - *second_position) >> 16;
    s32 delta_x_squared = delta_x *delta_x;
    s32 delta_y_squared = delta_y *delta_y;
    s32 delta_z_squared = delta_z *delta_z;

    return ((FixedPointSqrt) 0x030001D8)(delta_x_squared + delta_y_squared + delta_z_squared);
}

s32 SceneActor_RunStep18WhenTargetSet(s32 *p)
{
    s32 t = Actor_Get(0);
    if (p[14] == (s32)0x80000000 && p[16] == (s32)0x80000000)
        return 0;
    Func_0200ad9e((s32)p, t, 18, 0);
    return 0;
}

void Effect_ConfigureSpawnedParticle(struct SourceEntity *source)
{
    s32 spawn_position[3];
    s32 particle_index;
    struct StagedParticle *particle;
    spawn_position[0] = source->f08;
    spawn_position[1] = source->f0c - (Func_0200b60e(source) << 4) + (s32)0xfff80000;
    spawn_position[2] = source->f10;
    particle_index = Random_Next();
    Func_0200b656(((particle_index << 1) + particle_index) << 4, Random_Next(), spawn_position);
    particle = Func_0200b6a2(0x11d, spawn_position[0], spawn_position[1], spawn_position[2]);
    if (particle != 0) {
        particle->f55 = 2;
        particle->f48 = 0x1999;
        particle->f5e = 12;
        Actor_SetSpriteFlags(particle, 0);
        Object_SetAnimation(particle, 0);
        Object_SetScript(particle, (s32)Data_0200e6e0);
        {
            struct ParticleRecord *record = particle->f50;
            s32 record_flags = ~12;
            record_flags &= record->f09;
            record_flags |= 4;
            record->f09 = record_flags;
        }
    }
    Audio_PlayCue(0x8a);
}

/*
 * Play a rising-dust burst from a source entity. The owner extends through its
 * four literal pool words. Both loops count down inclusively, so they run 31
 * lift steps and 8 particles. The address handed to Engine_ObjectSetScript is passed
 * through as a plain pointer; whether it is animation data or a callback is
 * not established.
 */
void Effect_SpawnRisingDustBurst(struct Resource373Emitter *emitter)
{
    s32 frame_countdown;

    Audio_PlayCue(154);

    for (frame_countdown = 30; frame_countdown >= 0; frame_countdown--) {
        emitter->y += 0x10000;              /* 0x80 << 9. */
        emitter->field06 = (u16)(emitter->field06 + 0x2000);  /* 0x80 << 6. */
        emitter->field18 += -2048;          /* The pool word 0xfffff800. */
        emitter->field1c += -2048;
        Task_Wait(1);
    }

    for (frame_countdown = 7; frame_countdown >= 0; frame_countdown--) {
        struct Resource373Particle *particle =
            Func_0200b754(0x11d, emitter->x, emitter->y, emitter->z);

        if (particle != 0) {
            s32 vertical_speed;

            Actor_SetSpriteFlags(particle, 0);
            Object_SetScript(particle, (const void *)0x0200e6e4);

            vertical_speed = Random_Next() + 0x10000;
            particle->field34 = 0x10000;
            particle->field30 = vertical_speed;
            particle->field55 = 2;
            particle->field48 = 0x0a3d;

            particle->lifetime = Random_Next() - Random_Next();

            Effect_UpdateParticlePosition(
                particle,
                ((Random_Next() * 3) << 3) + 0x80000,
                Random_Next());
        }
    }

    Audio_PlayCue(131);

    emitter->x = 0;
    emitter->y = 0;
    emitter->z = 0;
    emitter->field38 = (s32)0x80000000;
    emitter->field3c = (s32)0x80000000;
    emitter->field40 = (s32)0x80000000;
    emitter->field24 = 0;
    emitter->field28 = 0;
    emitter->field2c = 0;
}

void Effect_UpdateParticlePosition(s32 *particle, s32 delta_x, s32 delta_z)
{
    s32 position[3];
    if (particle != 0) {
        position[0] = particle[2];
        position[1] = particle[3];
        position[2] = particle[4];
        Func_0200b7d4(delta_x, delta_z, position);
        Func_0200b838((s32)particle, position[0], position[1], position[2]);
    }
}

void SceneState_ApplyRectAndRunTwo(void)
{
    s32 e = 22;
    s32 f = 36;
    Map_CopyCellAttributes(17, 0, 3, 1, e, f);
    Func_02005a06();
    Func_0200b296();
}

void Effect_PlayStepSound(void)
{
    if ((*(u32 *)0x03001e40 & 15) == 0)
        Audio_PlayCue(0x83);
}

void FieldScene_RunScriptedStepEE4(void)
{
    Event_Begin();
    Message_ShowCentered((s32)&Value_00000ee4, 1);
    Event_End();
}

void FieldScene_RunScene373SequenceB(void)
{
    u32 i;
    s32 rec7;
    s32 record;

    if (GameFlag_IsSet(0x241) != 0) {
        rec7 = GameFlag_IsSet(0x106);
        if (rec7 != 0) {
            goto L_02005a8a;
        }
        *(u8 *)(Func_0200ba42(22) + 91) = rec7;
        GameFlag_Clear(0x241);
    } else {
        if (GameFlag_IsSet(0x106) != 0) {
            *(u8 *)(Func_0200ba60(22) + 91) = 1;
            GameFlag_Set(0x241);
        }
    }
    L_02005a8a:;
}

void SceneActor_SetFlagByteBySlotZeroPosition(void)
{
    s32 *g = Actor_Get(0);
    u8 *q;
    if (GameFlag_IsSet((s32)&Value_0000087a) != 0)
        q = Actor_Get(21);
    else
        q = Actor_Get(20);
    if (q != 0) {
        if (g[3] > 0xc80000)
            q[0x23] = 3;
        else
            q[0x23] = 1;
    }
}

s32 SceneEffect_UpdateOrbitPosition(s32 *p)
{
    s16 *q = (s16 *)p[20];
    s32 a, b;
    s32 d = Math_Sin(p[12]) * 2;
    if (d > 0)
        d = -d;
    p[2] = p[14] + Math_Cos(p[12]) * 2;
    p[3] = p[15] + d;
    q[15] = Math_Cos(p[12] + 0x8000) / 8;
    a = Random_Next();
    b = Random_Next();
    p[12] = p[12] + ((((u32)a << 9) >> 16) + (((u32)b << 9) >> 16)) + 0x400;
    return 0;
}

void InitializeStagedActorSceneOrbitingEffect(void)
{
    OrbitingSceneObject *actor;
    OrbitingSceneObjectSprite *sprite;
    u8 *transfer;
    s32 zero;

    actor = GetOrbitingSceneObject();
    sprite = actor->sprite;
    sprite->flags_09_mode = 1;
    sprite->flags_05_bit_5 = 0;
    sprite->flags_09_high = 0;

    zero = 0;
    sprite->state = zero;
    Actor_SetSpriteFlags(actor, zero);
    actor->active = zero;
    actor->mode = zero;

    if (GameFlag_IsSet(0x109) == 0)
        actor->y += 0x200000;

    actor->flags_23 &= 0xfe;
    actor->visible = 1;

    transfer = AllocateEffectTransfer(17, 0x608);
    Item_LoadIcon(181);
    transfer += 0x400;
    Vram_Load(sprite->palette, 128, transfer);
    Heap_Release(17);

    actor->orbit_center_x = actor->x;
    actor->orbit_angle = zero;
    actor->orbit_center_y = actor->y;
    actor->active = 1;
    actor->callback = (u32)&UpdateOrbitingSceneObject;
    actor->state = zero;
}

void OverlayObject_UpdateOnFrameBit1(s32 p)
{
    extern s32 Data_03001e40;

    if ((Data_03001e40 & 2) != 0)
        Object_SetPartPalettes(p, 7);
    else
        Object_SetPartPalettes(p, 0);
    if ((Data_03001e40 & 0xf) == 0)
        Func_0200b9b8(p);
}

void SceneEffect_UpdateObjectOnOddFrames(s32 p)
{
    extern volatile u32 Data_03001e40;

    if ((Data_03001e40 & 1) != 0)
        Object_SetPartPalettes(p, Func_0200baf8(Data_03001e40 >> 1, 6));
    if ((Data_03001e40 & 0xf) == 0)
        Func_0200b9f4(p);
}

void SceneEffect_UpdateObjectOnOddFramesOnly(s32 p)
{
    extern volatile u32 Data_03001e40;

    if ((Data_03001e40 & 1) != 0)
        Object_SetPartPalettes(p, Func_0200bb34(Data_03001e40 >> 1, 6));
}

void Effect_AnimateVerticalPositive(struct StagedVerticalEffect *effect)
{
    s32 *anchor = effect->f68;
    s32 frame = ++effect->f64;
    if (frame > 31) {
        Engine_ObjectDispatchRelease((s32)effect);
    } else {
        s32 amplitude = Math_Sin(frame << 10);
        s32 offset;
        effect->f18 = amplitude;
        effect->f1c = amplitude;
        effect->f8 = anchor[2];
        effect->fc += 0x10000;
        offset = 0x10000 - amplitude;
        effect->f10 = anchor[4] + offset * 5 + 0x80000;
    }
}

void Effect_AnimateVerticalNegative(struct StagedVerticalEffect *effect)
{
    s32 *anchor = effect->f68;
    s32 frame = ++effect->f64;
    if (frame > 31) {
        Engine_ObjectDispatchRelease((s32)effect);
    } else {
        s32 amplitude = Math_Sin(frame << 10);
        s32 offset;
        effect->f18 = amplitude;
        effect->f1c = -amplitude;
        effect->f8 = anchor[2];
        effect->fc += 0x10000;
        offset = 0x10000 - amplitude;
        effect->f10 = anchor[4] - offset * 5 + 0x100000;
    }
}
