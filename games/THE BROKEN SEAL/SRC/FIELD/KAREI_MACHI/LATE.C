#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))
#define SCENE_FIELD_1C8 (*(s32 *)(*(u8 **)Data_03001ebc + 0x1c8))
#define SCENE_FIELD_1C0 (*(s32 *)(*(u8 **)Data_03001ebc + 0x1c0))

#include "RESOURCE_3A8_EFFECT.H"
extern u8 MsgKareiLordHammetsPalaceLordAway[];
extern u8 MsgKareiWeveArrivedHammet[];

extern const s32 KareiMachi_ActionScript01[];
extern const s32 KareiMachi_Script02[];

enum {
    /* Message 0x182 + 181. */
    ITEM_NUT = 181
};


struct Obj {
    u8 filler00[6];
    u16 f06;
};

struct Obj_02000040 {
    u8 filler00[100];
    u16 f64;
    u16 f66;
};

struct Object {
    u8 filler00[6];
    u16 x;
    u8 filler08[94];
    s16 cnt;
};

typedef struct Effect {
    unsigned char pad00[0xC];
    s32 y;
    unsigned char pad10[0x13];
    s8 state23;
} Effect;

struct SceneWork {
    u8 unknown_000[0x1c0];
    s32 request;
    u8 unknown_1c4[4];
    s32 setup;
    u8 unknown_1cc[12];
    u16 step;
};

struct Obj_020036f8 {
    u8 filler00[8];
    s32 f08;
    s32 f0c;
    u8 filler10[8];
    s32 f18;
    s32 f1c;
    s32 f20;
    s32 f24;
    s32 f28;
    u8 filler2c[56];
    s16 f64;
};

typedef struct RenderData {
    unsigned char pad00[0x1E];
    s16 rotation;
} RenderData;

typedef struct Effect_0200390c {
    unsigned char pad00[8];
    s32 x;
    s32 y;
    unsigned char pad10[0x20];
    s32 angle;
    unsigned char pad34[4];
    s32 base_x;
    s32 base_y;
    unsigned char pad40[0x10];
    RenderData *render;
} Effect_0200390c;

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
extern s32 KareiMachi_Data01[];
extern const u8 KareiMachi_Data02[];
extern const u8 KareiMachi_Data03[];
void Object_SetActionCallbackAndRefreshById();
s32 Object_GetByIdFar();
void FieldScene_RunScene3a8SequenceB();
s32 Object_CheckMovementCollision();
void Object_SetPosition();
void Object_CommitPosition();
void BattleFx_PlayQueuedSound();
void FieldScene_RunScene3a8SequenceA();
s32 Object_CreateFar();
void BattleFx_SetQueuedSoundAndPlay();
u8 *Battle_GetWorkObject1e0();
void Object_Destroy(void);

/* Two early long branches share the scene-skip tail. Four polling loops
 * wait on signed actor fields; calls bind at loader runtime addresses. */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

/* Verified scene siblings use these call forms for independently evaluated
 * large constants in repeated actor operations. */
static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{

    f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{

    f(a0, a1, a2);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step_0200164c(s32 amount)
{
    extern u8 Data_03001ebc[];

    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ s32 Value0(s32 (*f)())
{
    return f();
}

static __inline__ s32 Value4(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    return f(a0, a1, a2, a3);
}

void SceneState_LinkRecordZeroWhenFlag200Clear(void);

/* Runs a scripted sequence for two actors (8 and 9): sets up their sprite
 * records, moves and animates them in lockstep through a series of timed
 * steps, then hands off to a third actor (2) and a couple of standalone
 * calls (5) before advancing the shared scene step counter and phase word. */
void FieldScene_RunTwoActorCutsceneSequence(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    u8 *record;

    Event_Begin();
    Camera_MoveTo(-1, -1, -1, 0);
    Task_Wait(1);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0xc00000, 0x1560000);
    Task_Wait(1);
    Camera_SetSpeed(0x3333, 0x666);
    Camera_MoveTo(0xc00000, -1, 0xfc0000, 1);
    SCENE_FIELD_1C8 = 40;
    Event_OpenScreen();
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0xcccc, 0x6666);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 192, 0x116);
    Actor_SetSpeed(8, 0x10000, 0x8000);
    Actor_SetSpeed(9, 0x10000, 0x8000);
    record = Actor_Get(8);
    {
        /* Field at +6 of the record: a visibility/state word. */
        s32 shown = 0x3000;

        *(u16 *)(record + 6) = shown;
    }
    record = Actor_Get(9);
    {
        /* Field at +6 of the record: a visibility/state word. */
        s32 shown = 0x5000;

        *(u16 *)(record + 6) = shown;
    }
    Task_Wait(1);
    /* Clear bit 0 of the flag byte at +90. */
    *(u8 *)(Object_GetByIdFar(8) + 90) &= 254;
    *(u8 *)(Object_GetByIdFar(9) + 90) &= 254;
    Actor_WalkTo(8, 184, 232);
    Actor_WalkToAndWait(9, 198, 232);
    Actor_SetAnimation(8, 1);
    Event_Wait(20);
    {
        /* Set bit 0 of the flag byte at +90. */
        u8 bits = 1;
        u8 *flags = Object_GetByIdFar(8) + 90;
        u8 value = *flags;

        value |= bits;
        *flags = value;
        flags = Object_GetByIdFar(9) + 90;
        bits |= *flags;
        *flags = bits;
    }
    Event_Wait(20);
    Actor_SetAnimationAndWait(8, 4);
    Event_SetMessage((s32)MsgKareiLordHammetsPalaceLordAway);
    Event_ShowMessageAndWait(8, 0, 10);
    Actor_FaceDirection(9, 0x5000, 10);
    Actor_SetAnimationAndWait(9, 3);
    Event_ShowMessageAndWait(9, 0, 10);
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
    Event_Wait(60);
    record = Value1(Engine_ActorGet, 0);
    if (record != 0) {
        Actor_SetPosition(ACTOR_IVAN, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Actor_SetSpeed(ACTOR_IVAN, 0xcccc, 0x6666);
    Actor_WalkToAndWait(ACTOR_IVAN, 212, 0x10c);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xe000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0x6000, 20);
    Event_ShowMessageAndWait(0x4002, 0, 20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Actor_WalkToAndWait(ACTOR_IVAN, 202, 254);
    Event_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_IVAN, 3);
    Event_Wait(10);
    Event_ShowMessageAndWait(ACTOR_IVAN, 0, 10);
    Actor_ShowEmote(8, 0x101, 0);
    Actor_ShowEmote(9, 0x101, 40);
    Actor_FaceDirection(8, 0, 0);
    Actor_FaceDirection(9, 0x8000, 40);
    Actor_FaceDirection(8, 0x3000, 0);
    Actor_FaceDirection(9, 0x3000, 20);
    Actor_ShowEmote(ACTOR_IVAN, 0x102, 60);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
    Event_ShowMessageAndWait(ACTOR_IVAN, 0, 10);
    Actor_RunRepeatedMotion(8, 2);
    Event_ShowMessageAndWait(8, 0, 10);
    Actor_ShowEmote(8, 0x105, 60);
    Actor_StartRepeatedMotion(8, 2);
    Event_ShowMessageAndWait(8, 0, 10);
    Actor_RunRepeatedMotion(9, 2);
    Event_ShowMessageAndWait(9, 0, 20);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
    Event_Wait(10);
    Actor_FaceDirection(ACTOR_IVAN, 0x6000, 10);
    Event_AskYesNo(0x4002, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(8, 1);
    Actor_SetAnimationAndWait(8, 4);
    Event_ShowMessageAndWait(8, 0, 10);
    Actor_ShowEmote(ACTOR_IVAN, 0x101, 40);
    Actor_FaceDirection(ACTOR_IVAN, 0xa000, 20);
    Actor_RunRepeatedMotion(9, 1);
    Actor_SetAnimationAndWait(9, 3);
    Actor_FaceDirection(ACTOR_IVAN, 0xc000, 0);
    Event_ShowMessageAndWait(9, 0, 10);
    Actor_FaceDirection(ACTOR_IVAN, 0x6000, 60);
    Actor_FaceDirection(ACTOR_IVAN, 0xc000, 10);
    Event_ShowMessageAndWait(ACTOR_IVAN, 0, 10);
    Actor_FaceDirection(8, 0, 0);
    Actor_FaceDirection(9, 0x8000, 40);
    Actor_FaceDirection(8, 0x3000, 0);
    Actor_FaceDirection(9, 0x3000, 10);
    Actor_RunRepeatedMotion(8, 1);
    Event_OpenMessage(8, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0x6000, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
        Event_ShowMessageAndWait(0x4002, 0, 10);
        bump_step_0200164c(1);
    } else {
        bump_step_0200164c(1);
        Actor_SetAnimationAndWait(ACTOR_IVAN, 4);
        Event_ShowMessageAndWait(0x4002, 0, 10);
    }
    Actor_RunRepeatedMotion(9, 2);
    Event_ShowMessageAndWait(9, 0, 10);
    Actor_FaceDirection(ACTOR_IVAN, 0xc000, 10);
    Actor_SetAnimationAndWait(ACTOR_IVAN, 4);
    Event_ShowMessageAndWait(ACTOR_IVAN, 0, 10);
    Actor_SetAttachedEffect(8, 0x102);
    Event_Wait(60);
    Event_ShowMessageAndWait(8, 0, 10);
    Actor_FaceDirection(ACTOR_IVAN, 0xa000, 10);
    Actor_SetAnimationAndWait(ACTOR_IVAN, 3);
    Event_ShowMessageAndWait(ACTOR_IVAN, 0, 10);
    Actor_SetAttachedEffect(8, 0x102);
    Actor_SetAttachedEffect(9, 0x102);
    Event_Wait(60);
    Actor_RunRepeatedMotion(8, 2);
    Event_ShowMessageAndWait(8, 0, 10);
    Actor_RunRepeatedMotion(9, 2);
    Actor_FaceDirection(9, 0x8000, 10);
    Event_ShowMessageAndWait(9, 0, 10);
    Actor_FaceDirection(8, 0, 10);
    Actor_SetAnimationAndWait(8, 3);
    Actor_FaceDirection(8, 0x3000, 10);
    Event_ShowMessage(8, 0);
    Actor_FaceDirection(9, 0x3000, 10);
    Actor_SetAnimationAndWait(ACTOR_IVAN, 3);
    Actor_FaceDirection(ACTOR_IVAN, 0x6000, 10);
    Event_AskYesNo(0x4002, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
    Event_ShowMessageAndWait(0x4002, 0, 10);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimationAndWait(ACTOR_IVAN, 3);
    Actor_FaceDirection(ACTOR_IVAN, 0xc000, 10);
    Actor_SetAnimationAndWait(ACTOR_IVAN, 3);
    Call3((void (*)())Engine_ActorSetSpeed, 8, 0xcccc, 0x6666);
    Call3((void (*)())Engine_ActorSetSpeed, 9, 0xcccc, 0x6666);
    record = Actor_Get(8);
    {
        /* Field at +6 of the record: a visibility/state word. */
        s32 shown = 0;

        *(u16 *)(record + 6) = shown;
    }
    record = Actor_Get(9);
    {
        s32 shown = 0x8000;

        *(u16 *)(record + 6) = shown;
    }
    /* Clear bit 0 of the flag byte at +90. */
    *(u8 *)(Object_GetByIdFar(8) + 90) &= 254;
    *(u8 *)(Object_GetByIdFar(9) + 90) &= 254;
    Actor_WalkTo(8, 168, 232);
    Actor_WalkToAndWait(9, 212, 232);
    Actor_SetAnimation(8, 1);
    Event_Wait(20);
    {
        /* Set bit 0 of the flag byte at +90. */
        u8 bits = 1;
        u8 *flags = Object_GetByIdFar(8) + 90;
        u8 value = *flags;

        value |= bits;
        *flags = value;
        flags = Object_GetByIdFar(9) + 90;
        bits |= *flags;
        *flags = bits;
    }
    Actor_WalkToAndWait(ACTOR_IVAN, 192, 232);
    Actor_FaceDirection(ACTOR_IVAN, 0xc000, 0);
    Audio_PlayCue(188);
    Map_CopyCellsTo(36, 23, 43, 12, 2, 2);
    Task_Wait(5);
    Map_CopyCellsTo(39, 23, 43, 12, 2, 2);
    Task_Wait(5);
    Actor_WalkToAndWait(ACTOR_IVAN, 192, 222);
    Actor_SetPosition(ACTOR_IVAN, 0, 0);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x13333, 0x9999);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 192, 222);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
    Actor_SetAnimation(8, 3);
    Actor_SetAnimationAndWait(9, 3);
    Actor_WalkTo(8, 184, 232);
    Actor_WalkToAndWait(9, 198, 232);
    Actor_WalkTo(8, 188, 212);
    Actor_WalkToAndWait(9, 194, 212);
    Actor_SetPosition(8, 0, 0);
    Actor_SetPosition(9, 0, 0);
    SCENE_FIELD_1C8 = 24;
    SCENE_FIELD_1C0 = 0x201;
    Event_CloseScreen();
    Event_WaitForScreen();
    Event_RequestExit(5);
    Event_End();
}

void FieldScene_RunSecondaryGroupSequence(void)
{
    extern struct SceneWork *Data_03001ebc;

    u8 *fieldActor;
    u8 *object;
    struct SceneWork *work;
    u32 random;
    u32 motionPhase;
    const u8 *motionActions;
    s32 scale;
    const u8 *exitActions;

    Event_Begin();
    Camera_MoveTo(-1, -1, -1, 0);
    Task_Wait(1);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
    Actor_SetPosition(9, 0x1b80000, 0x20a0000);
    object = Actor_Get(9);
    Actor_SetSpriteFlags(object, 0);
    Camera_MoveTo(0x1b80000, -1, 0x20a0000, 0);
    Map_Redraw();
    Task_Wait(1);
    Event_OpenScreen();
    Camera_SetSpeed(0x19999, 0x3333);
    Camera_MoveTo(0x1b80000, -1, 0x1900000, 1);
    Audio_PlayCue(141);
    Actor_SetSpeed(9, 0x19999, 0xcccc);
    Actor_WalkToAndWait(9, 0x1b8, 0x190);
    Camera_SetSpeed(0xc000, 0x1800);
    Camera_MoveTo(0x1b80000, -1, 0x12c0000, 1);
    Actor_SetSpeed(9, 0xcccc, 0x6666);
    Actor_WalkToAndWait(9, 0x1b8, 0x12c);
    Actor_SetAnimation(9, 0);
    Audio_PlayCue(0x121);
    Event_Wait(40);
    Actor_SetSpeed(11, 0xcccc, 0x6666);
    Actor_SetPosition(11, 0x1b70000, 0x1320000);
    Actor_Jump(11, 4, 0);
    Actor_WalkToAndWait(11, 0x1b7, 0x138);
    Actor_WalkToAndWait(11, 0x1a0, 0x138);
    Actor_WalkToAndWait(11, 0x190, 0x100);
    Actor_FaceDirection(11, 0x3000, 40);
    Camera_MoveTo(0x19a0000, -1, 0x1180000, 1);
    Actor_SetSpeed(10, 0x9999, 0x4ccc);
    Actor_SetPosition(10, 0x1b70000, 0x1320000);
    Actor_Jump(10, 4, 0);
    Actor_WalkToAndWait(10, 0x1b7, 0x138);
    Actor_WalkToAndWait(10, 0x1a0, 0x138);
    Actor_WalkToAndWait(10, 0x184, 0x10e);
    Actor_FaceDirection(10, 0xd000, 10);
    Actor_FaceDirection(11, 0x5000, 10);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0xcccc, 0x6666);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0x1b70000, 0x1320000);
    Actor_Jump(ACTOR_PARTY_LEADER, 4, 0);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x1b7, 0x138);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x1a0, 0x138);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x184, 0x12c);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 40);
    fieldActor = Actor_Get(ACTOR_PARTY_LEADER);
    random = Random_Next();
    motionPhase = random * 5;
    motionActions = KareiMachi_Data02;
    fieldActor += 102;
    *(u16 *)fieldActor = motionPhase >> 12;
    Actor_EnableActionCallback(ACTOR_PARTY_LEADER, motionActions);
    Actor_Jump(11, 2, 20);
    Actor_SetAnimationAndWait(11, 3);
    Event_SetMessage((s32)MsgKareiWeveArrivedHammet);
    Event_ShowMessageAndWait(11, 0, 10);
    Actor_SetAnimationAndWait(10, 3);
    object = Actor_Get(ACTOR_PARTY_LEADER);
    if (object != 0) {
        Actor_SetPosition(ACTOR_GERALD, *(s32 *)(object + 8), *(s32 *)(object + 16));
    }
    object = Actor_Get(ACTOR_PARTY_LEADER);
    if (object != 0) {
        Actor_SetPosition(ACTOR_IVAN, *(s32 *)(object + 8), *(s32 *)(object + 16));
    }
    object = Actor_Get(ACTOR_PARTY_LEADER);
    if (object != 0) {
        Actor_SetPosition(ACTOR_MIA, *(s32 *)(object + 8), *(s32 *)(object + 16));
    }
    Actor_SetSpeed(ACTOR_GERALD, 0xcccc, 0x6666);
    Actor_SetSpeed(ACTOR_IVAN, 0xcccc, 0x6666);
    Actor_SetSpeed(ACTOR_MIA, 0xcccc, 0x6666);
    Actor_WalkTo(ACTOR_GERALD, 0x17a, 0x136);
    Actor_WalkTo(ACTOR_IVAN, 0x190, 0x120);
    Actor_WalkToAndWait(ACTOR_MIA, 0x19a, 0x134);
    Actor_SetAnimation(ACTOR_GERALD, 1);
    Actor_SetAnimation(ACTOR_IVAN, 1);
    Actor_FaceDirection(ACTOR_GERALD, 0xc000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0xc000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0xc000, 40);
    fieldActor = Actor_Get(ACTOR_GERALD);
    random = Random_Next();
    fieldActor += 102;
    *(u16 *)fieldActor = ((random * 5) >> 12);
    fieldActor = Actor_Get(ACTOR_IVAN);
    random = Random_Next();
    fieldActor += 102;
    *(u16 *)fieldActor = ((random * 5) >> 12);
    fieldActor = Actor_Get(ACTOR_MIA);
    random = Random_Next();
    fieldActor += 102;
    *(u16 *)fieldActor = ((random * 5) >> 12);
    Actor_EnableActionCallback(ACTOR_GERALD, motionActions);
    Actor_EnableActionCallback(ACTOR_IVAN, motionActions);
    Actor_EnableActionCallback(ACTOR_MIA, motionActions);
    Actor_ShowEmote(ACTOR_IVAN, 0x101, 60);
    Event_ShowMessageAndWait(0x2002, 0, 10);
    Actor_ShowEmote(10, 0x100, 40);
    Actor_FaceDirection(10, 0x3000, 20);
    Event_ShowMessageAndWait(10, 0, 10);
    Actor_Stop(ACTOR_PARTY_LEADER);
    Actor_Stop(ACTOR_GERALD);
    Actor_Stop(ACTOR_IVAN);
    Actor_Stop(ACTOR_MIA);
    Task_Wait(1);
    Actor_FaceDirection(ACTOR_IVAN, 0xc000, 10);
    Actor_SetAnimationAndWait(ACTOR_IVAN, 3);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xc000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0xc000, 20);
    Actor_SetAnimationAndWait(10, 3);
    Event_ShowMessageAndWait(10, 0, 10);
    Actor_ShowEmote(ACTOR_MIA, 0x101, 60);
    Event_ShowMessageAndWait(0x2003, 0, 10);
    Actor_SetAnimationAndWait(10, 3);
    Event_ShowMessageAndWait(10, 0, 10);
    Actor_ShowEmote(ACTOR_GERALD, 0x101, 60);
    Event_ShowMessageAndWait(ACTOR_GERALD, 0, 10);
    Actor_FaceDirection(10, 0x5000, 0);
    Actor_RunRepeatedMotion(11, 2);
    Event_ShowMessageAndWait(11, 0, 10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x6000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xe000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0x2000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0xa000, 20);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimation(ACTOR_GERALD, 3);
    Actor_SetAnimation(ACTOR_IVAN, 3);
    Actor_SetAnimationAndWait(ACTOR_MIA, 3);
    Event_Wait(10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xc000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0xc000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0xc000, 10);
    Actor_FaceDirection(10, 0xd000, 10);
    Actor_SetAnimationAndWait(10, 3);
    Event_ShowMessageAndWait(10, 0, 10);
    Actor_SetAnimationAndWait(11, 3);
    Actor_SetAttachedEffect(ACTOR_IVAN, 0x102);
    Event_Wait(40);
    Actor_StartRepeatedMotion(ACTOR_IVAN, 2);
    Event_ShowMessageAndWait(0x2002, 0, 10);
    Actor_FaceDirection(11, 0x3000, 10);
    Actor_SetAnimation(11, 4);
    Event_ShowMessageAndWait(11, 0, 10);
    Actor_ShowEmote(ACTOR_MIA, 0x101, 40);
    Event_ShowMessageAndWait(0x2003, 0, 10);
    Actor_FaceDirection(10, 0x3000, 10);
    Actor_ShowEmote(10, 0x108, 20);
    Event_ShowMessageAndWait(10, 0, 10);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
    Event_ShowMessageAndWait(ACTOR_GERALD, 0, 10);
    Actor_FaceDirection(11, 0x5000, 0);
    Actor_FaceDirection(10, 0x5000, 10);
    Actor_SetAnimation(11, 4);
    Event_ShowMessageAndWait(11, 0, 10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x6000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xe000, 40);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xc000, 10);
    Actor_SetAnimationAndWait(11, 4);
    Event_ShowMessageAndWait(11, 0, 10);
    Actor_SetAnimationAndWait(10, 4);
    Event_ShowMessageAndWait(10, 0, 10);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x105, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x105, 0);
    Actor_ShowEmote(ACTOR_IVAN, 0x105, 0);
    Actor_ShowEmote(ACTOR_MIA, 0x105, 60);
    Actor_FaceDirection(10, 0x3000, 10);
    Actor_SetAnimation(10, 3);
    Event_OpenMessage(10, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xe000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0x6000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0xa000, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_Wait(20);
        Data_03001ebc->step += 3;
    } else {
        Event_Wait(20);
        Actor_RunRepeatedMotion(11, 2);
        Event_ShowMessageAndWait(11, 0, 40);
        Event_ShowMessageAndWait(11, 0, 10);
        Actor_ShowEmote(ACTOR_MIA, 0x106, 40);
        Actor_FaceDirection(ACTOR_MIA, 0xc000, 20);
        Event_ShowMessageAndWait(0x2003, 0, 10);
    }
    Actor_FaceDirection(ACTOR_GERALD, 0xc000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0xc000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0xc000, 10);
    Actor_SetAnimation(ACTOR_GERALD, 3);
    Actor_SetAnimation(ACTOR_IVAN, 3);
    Actor_SetAnimationAndWait(ACTOR_MIA, 3);
    Event_Wait(20);
    Actor_FaceDirection(8, 0x3000, 0);
    Actor_SetAnimationAndWait(10, 3);
    Camera_SetSpeed(0x10000, 0x2000);
    Camera_MoveTo(0x1180000, -1, 0xc80000, 1);
    Actor_WalkToAndWait(10, 0x14d, 222);
    Actor_WalkToAndWait(10, 0x11c, 198);
    Actor_FaceDirection(10, 0x8000, 10);
    Actor_SetAnimationAndWait(10, 3);
    Actor_FaceDirection(8, 0, 10);
    Actor_SetAnimationAndWait(8, 3);
    Actor_SetSpeed(8, 0xcccc, 0x6666);
    Actor_WalkToAndWait(8, 0x10c, 198);
    Actor_FaceDirection(8, 0xc000, 10);
    Actor_RunRepeatedMotion(8, 2);
    Audio_PlayCue(125);
    Map_CopyCellsTo(71, 60, 76, 11, 2, 1);
    Map_CopyCellAttributes(71, 60, 2, 1, 16, 11);
    Event_Wait(20);
    Actor_WalkToAndWait(8, 246, 198);
    Actor_FaceDirection(8, 0, 20);
    *(u8 *)(Battle_GetWorkObject1e0() + 85) = 0;
    Camera_SetSpeed(0x9999, 0x1333);
    Camera_MoveTo(0xf80000, -1, 0xaa0000, 1);
    Actor_WalkToAndWait(10, 0x10e, 198);
    Actor_WalkToAndWait(10, 0x10e, 174);
    Actor_WalkToAndWait(10, 224, 170);
    Actor_WalkToAndWait(10, 210, 158);
    Actor_WalkToAndWait(10, 246, 148);
    Actor_WalkToAndWait(10, 246, 142);
    Actor_SetPosition(10, 0, 0);
    Data_03001ebc->request = 514;
    Event_CloseScreen();
    Event_WaitForScreen();
    Actor_SetPosition(9, 0x1b80000, 0x1540000);
    object = Actor_Get(9);
    scale = 0x4000;
    *(u16 *)(object + 6) = scale;
    Camera_MoveTo(0x17c0000, -1, 0x1180000, 0);
    Map_Redraw();
    Task_Wait(10);
    Event_OpenScreen();
    Event_WaitForScreen();
    Event_Wait(40);
    Actor_SetAnimationAndWait(11, 3);
    Event_Wait(20);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimation(ACTOR_GERALD, 3);
    Actor_SetAnimation(ACTOR_IVAN, 3);
    ((void (*)())Engine_ActorSetAnimationAndWait)(3, 3);
    Actor_WalkToAndWait(11, 0x1a4, 0x11a);
    Actor_WalkToAndWait(11, 0x1a4, 0x138);
    Actor_WalkToAndWait(11, 0x1b7, 0x138);
    Actor_WalkToAndWait(11, 0x1b7, 0x132);
    Actor_SetPosition(11, 0, 0);
    exitActions = KareiMachi_Data03;
    Actor_EnableActionCallback(ACTOR_GERALD, exitActions);
    Actor_EnableActionCallback(ACTOR_IVAN, (s32)exitActions);
    Call2(Object_SetActionCallbackAndRefreshById, 3, (s32)exitActions);
    Camera_MoveTo(0x19a0000, -1, 0x12c0000, 1);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x1a0, 0x138);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x1b7, 0x138);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x1b7, 0x132);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
    Audio_PlayCue(141);
    Actor_SetSpeed(9, 0xcccc, 0x6666);
    Camera_SetSpeed(0x10000, 0x2000);
    Camera_MoveTo(0x1b80000, -1, 0x1a40000, 1);
    Actor_WalkToAndWait(9, 0x1b8, 0x1a4);
    Camera_SetSpeed(0x20000, scale);
    Camera_MoveTo(0x1b80000, -1, 0x2580000, 1);
    Actor_SetSpeed(9, 0x19999, 0xcccc);
    Actor_WalkToAndWait(9, 0x1b8, 0x1f4);
    Actor_WalkTo(9, 0x1b8, 0x258);
    Audio_PlayCue(0x121);
    work = Data_03001ebc;
    work->setup = 24;
    work->request = 0x100;
    Event_CloseScreen();
    Event_WaitForScreen();
    Event_RequestExit(10);
    Event_End();
}

s32 *SceneActor_FindAtTileXZ(s32 x, s32 z)
{
    extern u8 *Data_03001ebc;

    s32 **tbl = (s32 **)(Data_03001ebc + 0x14);
    u32 i;

    for (i = 8; i <= 65; i++) {
        s32 *p = tbl[i];

        if (x == (p[2] >> 20) && z == (p[4] >> 20)) {
            return p;
        }
    }
    return 0;
}

void FieldScene_RunLateSequence(void)
{
    s32 tmp[3];
    s32 record;
    s32 rec;
    s32 idx;
    s32 w;
    s32 w2;
    s32 a;
    s32 *dst;
    s32 zero;
    s32 k;

    record = Value1(Engine_ActorGet, ACTOR_PARTY_LEADER);
    idx = (s32)((u32)*(u16 *)(record + 6) >> 12);
    a = *(s16 *)(record + 10);
    w = KareiMachi_Data01[idx];
    rec = Value2(SceneActor_FindAtTileXZ,
                 (a + (w >> 16)) >> 4,
                 (*(s16 *)(record + 18) + (s32)(s16)w) >> 4);
    if (rec != 0) {
        zero = 0;
        *(u8 *)(rec + 34) = 2;
        dst = tmp;
        w2 = KareiMachi_Data01[idx];
        dst[0] = *(s32 *)(rec + 8) + (w2 & -0x10000);
        dst[1] = *(s32 *)(rec + 12);
        dst[2] = *(s32 *)(rec + 16) + (w2 << 16);
        if (Value2(Object_CheckMovementCollision, rec, (s32)dst) <= 0) {
            Object_SetAnimation(record, 8);
            k = 0x3333;
            Task_Wait(15);
            Audio_PlayCue(185);
            *(s32 *)(rec + 48) = k;
            *(s32 *)(rec + 52) = k;
            Object_SetPosition(rec, dst[0], dst[1], dst[2]);
            *(s32 *)(record + 48) = k;
            *(s32 *)(record + 52) = k;
            Object_SetPosition(record, dst[0], dst[1], dst[2]);
            Object_CommitPosition(rec);
            BattleFx_PlayQueuedSound();
            *(s32 *)(rec + 8) = dst[0];
            *(s32 *)(rec + 16) = dst[2];
            *(s32 *)(rec + 36) = zero;
            *(s32 *)(rec + 44) = zero;
            Object_SetAnimation(record, 1);
            FieldScene_RunScene3a8SequenceB();
        }
    }
}

void FieldScene_RunScene3a8SequenceB(void)
{
    s32 rec8;
    s32 rec4;
    s32 rec7;
    s32 rec2;
    u32 i;
    s32 v5;
    s32 v6;
    s32 v7;

    rec8 = Value1(Engine_ActorGet, 8);
    rec4 = Value1(Engine_ActorGet, 9);
    rec7 = GameFlag_IsSet(0x302);
    if (rec7 != 0) {
    } else if ((*(s32 *)(rec8 + 8) >> 19) > 29) {
    } else {
        rec2 = Value1(Engine_ActorGet, 11);
        Event_Begin();
        Map_CopyCellAttributes(7, 44, 1, 1, rec7, 1);
        i = 67;
        v7 = 1;
        v6 = 5;
        do {
            Map_CopyCellsTo(i, 58, 78, 41, v7, v6);
            Event_Wait(4);
            if (i == 70) {
                GameFlag_Set(0x302);
            }
            i++;
        } while (i <= 74);
        v5 = 2;
        Map_CopyCellsTo(16, 109, 13, 109, 3, v5);
        Event_Wait(40);
        *(s32 *)(rec2 + 24) = 0x1999;
        *(s32 *)(rec2 + 28) = 0x1999;
        Actor_SetPosition(11, 0x960000, 0x2d80000);
        Value2(Engine_ActorEnableActionCallback, 11, (s32)KareiMachi_ActionScript01);
        v6 = 1;
        Map_CopyCellsTo(67, 64, 71, 44, v6, v5);
        Map_CopyCellsTo(67, 64, 72, 44, v6, v5);
        Map_CopyCellsTo(67, 68, 73, 43, v6, v5);
        Map_CopyCellsTo(67, 68, 74, 43, v6, v5);
        Map_CopyCellsTo(67, 64, 75, 44, v6, v5);
        Map_CopyCellsTo(67, 66, 76, 44, v6, v5);
        Map_CopyCellsTo(67, 64, 77, 44, v6, v5);
        Map_CopyCellsTo(67, 64, 78, 44, v6, v5);
        Map_CopyCellsTo(67, 64, 79, 44, v6, v5);
        Map_CopyCellsTo(67, 66, 80, 44, v6, v5);
        Map_CopyCellsTo(2, 0, 9, 42, v5, v5);
        Event_Wait(40);
        Map_CopyCellsTo(68, 64, 71, 44, v6, v5);
        Map_CopyCellsTo(68, 64, 72, 44, v6, v5);
        Map_CopyCellsTo(68, 68, 73, 43, v6, v5);
        Map_CopyCellsTo(68, 68, 74, 43, v6, v5);
        Map_CopyCellsTo(68, 64, 75, 44, v6, v5);
        Map_CopyCellsTo(68, 66, 76, 44, v6, v5);
        Map_CopyCellsTo(68, 64, 77, 44, v6, v5);
        Map_CopyCellsTo(68, 64, 78, 44, v6, v5);
        Map_CopyCellsTo(68, 64, 79, 44, v6, v5);
        Map_CopyCellsTo(68, 66, 80, 44, v6, v5);
        Map_CopyCellsTo(4, 0, 9, 42, v5, v5);
        Event_Wait(40);
        v5 = 10;
        Map_CopyCellsTo(7, 11, 7, 42, v5, 8);
        Map_CopyCellsTo(71, 12, 71, 43, v5, 13);
        v5 = 44;
        Map_CopyCellAttributes(6, 13, 12, 12, 6, v5);
        Event_Wait(40);
        SceneState_LinkRecordZeroWhenFlag200Clear();
        Map_CopyCellAttributes(0, 1, 1, 1, 7, v5);
        Event_End();
    }
    if (GameFlag_IsSet(0x303) != 0) {
    } else if ((*(s32 *)(rec4 + 8) >> 19) > 87) {
    } else {
        Event_Begin();
        i = 67;
        v7 = 1;
        v6 = 5;
        do {
            Map_CopyCellsTo(i, 58, 107, 41, v7, v6);
            Event_Wait(4);
            if (i == 70) {
                GameFlag_Set(0x303);
            }
            i++;
        } while (i <= 74);
        v6 = 2;
        Map_CopyCellsTo(45, 109, 42, 109, 3, v6);
        Event_Wait(40);
        v5 = 1;
        Map_CopyCellsTo(67, 64, 102, 44, v5, v6);
        Map_CopyCellsTo(67, 64, 103, 44, v5, v6);
        Map_CopyCellsTo(67, 64, 104, 44, v5, v6);
        Map_CopyCellsTo(67, 66, 105, 44, v5, v6);
        Map_CopyCellsTo(67, 64, 106, 44, v5, v6);
        Map_CopyCellsTo(67, 64, 107, 44, v5, v6);
        Map_CopyCellsTo(67, 64, 108, 44, v5, v6);
        Map_CopyCellsTo(67, 66, 109, 44, v5, v6);
        Event_Wait(40);
        Map_CopyCellsTo(68, 64, 102, 44, v5, v6);
        Map_CopyCellsTo(68, 64, 103, 44, v5, v6);
        Map_CopyCellsTo(68, 64, 104, 44, v5, v6);
        Map_CopyCellsTo(68, 66, 105, 44, v5, v6);
        Map_CopyCellsTo(68, 64, 106, 44, v5, v6);
        Map_CopyCellsTo(68, 64, 107, 44, v5, v6);
        Map_CopyCellsTo(68, 64, 108, 44, v5, v6);
        Map_CopyCellsTo(68, 66, 109, 44, v5, v6);
        Event_Wait(40);
        v5 = 8;
        Map_CopyCellsTo(38, 14, 38, 44, v5, 4);
        Map_CopyCellsTo(102, 14, 102, 44, v5, 12);
        Map_CopyCellAttributes(37, 13, 10, 12, 37, 43);
        Event_Wait(40);
        SceneState_LinkRecordZeroWhenFlag200Clear();
        Event_End();
    }
}

void SceneEffect_UpdateMotionWithDamping(struct Obj_020036f8 *p)
{
    s16 *h;
    s32 v;
    s32 a;
    s32 b;

    h = &p->f64;
    v = *h;
    if (v == 0) {
        Object_Destroy();
    } else if (v == 1) {
        p->f24 = 0;
        p->f28 = 0;
        p->f08 = 0;
        p->f0c = 0;
    } else {
        p->f18 += 0x800;
        p->f1c += 0x800;
    }
    p->f08 += p->f24;
    p->f0c += p->f28;
    a = p->f24;
    b = p->f28;
    p->f24 = a - a / 256;
    p->f28 = b - b / 16;
    { s32 t = *(u16 *)h; t -= 1; *(u16 *)h = t; }
}

void FieldScene_RunSupplementalSequenceOne(s32 a0)
{
    s32 rec7;
    s32 rec8;
    s32 record;
    s32 mask;
    s32 c12;
    s32 old;
    u8 *rp;
    u8 *p4;

    rec7 = Value1(Engine_ActorGet, 8);
    rec8 = Actor_Get(9);
    if ((u32)(*(s16 *)(rec7 + 10) + -0x17d) <= 12) {
        if (*(s16 *)(rec7 + 18) <= 0x309) {
            goto L_020037ae;
        }
        record = Value1(Engine_ActorGet, ACTOR_PARTY_LEADER);
        p4 = (u8 *)*(s32 *)(rec7 + 80);
        rp = (u8 *)*(s32 *)(record + 80);
        c12 = 12 & rp[9];
        old = p4[9];
        mask = -13;
        mask &= old;
        p4[9] = (mask | c12);
    } else {
        L_020037ae:;
        if (GameFlag_IsSet(0x302) == 0) {
            if (*(s16 *)(rec7 + 10) <= 245) {
                if ((*(volatile s32 *)&gFrameCount & 1) == 0) {
                    if (GameFlag_IsSet(0x202) == 0) {
                        Call1(BattleFx_SetQueuedSoundAndPlay, -1);
                        Audio_PlayCue(230);
                        GameFlag_Set(0x202);
                    }
                    FieldScene_RunScene3a8SequenceA(*(s32 *)(rec7 + 8), *(s32 *)(rec7 + 12), *(s32 *)(rec7 + 16));
                }
            }
        }
    }
    if (GameFlag_IsSet(0x303) == 0) {
        if (*(s16 *)(rec8 + 10) <= 0x2c5) {
            if ((*(volatile s32 *)&gFrameCount & 1) == 0) {
                if (GameFlag_IsSet(0x203) == 0) {
                    Call1(BattleFx_SetQueuedSoundAndPlay, -1);
                    Audio_PlayCue(230);
                    GameFlag_Set(0x203);
                }
                FieldScene_RunScene3a8SequenceA(*(s32 *)(rec8 + 8), *(s32 *)(rec8 + 12), *(s32 *)(rec8 + 16));
            }
        }
    }
}

void FieldScene_RunScene3a8SequenceA(s32 a0, s32 a1, s32 a2)
{
    s32 p8;
    u8 *rec7;
    s32 value;
    s32 mask;
    u8 *link;

    p8 = a2;
    value = Value0(Engine_RandomNext);
    rec7 = (u8 *)Value4(Object_CreateFar, 222, (a0 + -0x80000), (((((u32)(value << 3) >> 16) << 16) + a1) + 0x100000), p8);
    if ((s32)rec7 != 0) {
        rec7[85] = (mask = 0);
        link = (u8 *)*(s32 *)((s32)rec7 + 80);
        mask -= 13;
        link[9] = ((link[9] & mask) | 8);
        Object_SetPalette((s32)rec7, 9);
        Actor_SetSpriteFlags((s32)rec7, 0);
        value = Random_Next();
        *(s32 *)((s32)rec7 + 36) = ((((u32)(value << 1) >> 16) - 1) << 16);
        value = Value0(Engine_RandomNext);
        *(s32 *)((s32)rec7 + 40) = ((((u32)(((value << 1) + value) << 1) >> 16) - 3) << 16);
        {
            u16 *target = (u16 *)((s32)rec7 + 100);
            s32 shown = 20;

            *target = shown;
            *((u8 *)target - 3) = 1;
        }
        Object_SetAnimation((s32)rec7, 1);
        Engine_ObjectSetScript((s32)rec7, (s32)KareiMachi_Script02);
    }
}

void SceneEffect_UpdateLobeOrbitEffect26(void)
{
    Effect_0200390c *effect = Actor_Get(26);
    RenderData *render = effect->render;
    s32 offset = Math_Sin(effect->angle) * 2;
    s32 first;

    if (offset > 0) {
        offset = -offset;
    }
    effect->x = effect->base_x + Math_Cos(effect->angle) * 2;
    effect->y = effect->base_y + offset;
    render->rotation = Math_Cos(effect->angle + 0x8000) >> 3;
    first = Random_Next();
    effect->angle +=
        ((u32)(first << 9) >> 16)
        + ((u32)(Random_Next() << 9) >> 16)
        + 0x400;
}

void FieldScene_DrawTilesAndRaiseActor11(void)
{
    Effect *effect = Actor_Get(11);

    Map_CopyCellAttributes(0, 0, 1, 1, 9, 14);
    Map_CopyCellAttributes(0, 0, 1, 1, 9, 45);
    if (effect != 0) {
        Actor_SetSpriteFlags(effect, 0);
        effect->y -= 0x200000;
        effect->state23 = 2;
    }
    GameFlag_Set(0x201);
}

s32 SceneEffect_UpdateOrbitingEffect(Effect_0200390c *effect)
{
    RenderData *render = effect->render;
    s32 ofs = Math_Sin(effect->angle) * 2;
    s32 first;

    if (ofs > 0) {
        ofs = -ofs;
    }
    effect->x = effect->base_x + Math_Cos(effect->angle) * 2;
    effect->y = effect->base_y + ofs;
    render->rotation = Math_Cos(effect->angle + 0x8000) / 8;
    first = Random_Next();
    effect->angle +=
        ((u32)(first << 9) >> 16)
        + ((u32)(Random_Next() << 9) >> 16)
        + 0x400;
    return 0;
}

void InitializeOrbitingRenderEffect(void)
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
    Item_LoadIcon(ITEM_NUT);
    transfer += 0x400;
    Vram_Load(sprite->palette, 128, transfer);
    Heap_Release(17);

    actor->orbit_center_x = actor->x;
    actor->orbit_angle = zero;
    actor->orbit_center_y = actor->y;
    actor->active = 1;
    actor->callback = (u32)SceneEffect_UpdateOrbitingEffect;
    actor->state = zero;
}
