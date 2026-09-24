#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

#include "PALETTE_SCENE.H"

enum PaletteMessage {
    MSG_HEALING_WATERS_MERCURY_LIGHTHOUSE_MIGHT = 0x14c8,
    MSG_SILENCE = 0x14c9,
    MSG_CONTROL_TRETS_HEART_SHALL_NOT = 0x14cc,
    MSG_SILENCE_2 = 0x14eb,
    MSG_PEOPLE_KOLIMA_FORGIVE_ME = 0x14ec,
    MSG_WATER_HERMES_SEEPED_INTO_TRET = 0x14ed,
    MSG_FEEL_GREAT_POWER_SPREADING_THROUGH = 0x14ee,
    MSG_SHOULD_DO_PEOPLE_KOLIMA_CURSED = 0x14fb,
    MSG_WAS_INDEED_ANGRY_PEOPLE_HAD = 0x1501,
    MSG_OWE_GREAT_DEBT_HAVE_SAVED = 0x1519,
    MSG_NOW_HAVE_SUCH_POWER_AXE = 0x151c,
    MSG_KNOW_CANNOT_STOP_BUT_PLEASE = 0x151e,
    MSG_MUST_HORRIBLE_BEYOND_RIVER_AM = 0x151f
};


struct PaletteEffectFrame {
    s32 pad00[6];
    s32 progress;
    s32 pad1c[7];
    s32 limit;
    s32 second_limit;
    s32 third_limit;
};

struct EffectSprite { u8 pad00[9]; u8 flags; u8 pad0a[28]; u8 state; };

struct PaletteEffect {
    u8 pad00[0x18];
    s32 progress;
    u8 pad1c[7];
    u8 flags;
    u8 pad24[12];
    s32 rate_x;
    s32 rate_y;
    u8 pad38[24];
    struct EffectSprite *sprite;
    u8 pad54[1];
    u8 mode;
};

struct OrbitingPaletteEffect {
    s32 pad00[2];
    s32 x;
    s32 y;
    s32 z;
    s32 pad14;
    s32 angle_x;
    s32 angle_y;
    s32 pad20[6];
    s32 anchor_x;
    s32 anchor_y;
    s32 anchor_z;
    s32 pad44[3];
    u8 *owner;
    s32 pad54[4];
    s16 step;
    s16 heading;
};

struct PaletteSceneRecord {
    u8 pad00[216];
    u16 values[1];                 /* +216 */
};

extern s32 Data_02009dd4;
extern s32 Data_02009dcc;
extern s32 Data_02009dc0[3];
extern const s32 SceneAction_ActorOneEntry[];
extern const s32 SceneAction_ActorTwoEntry[];
extern const s32 SceneAction_ActorThreeEntry[];
extern const s32 SceneAction_GroupFinish[];
extern s32 Data_03001e40;
extern u8 Data_02009d9c[];
extern u8 Data_03001ebc[];

void Func_02001d58(void);
void Func_020015a0(void);
void Func_020019c8();
s32 Func_02001910();
s32 Func_02001918();
void Func_020012f4();
void Func_020019d0();
struct PaletteEffect *Func_02002b84(s32, s32, s32, s32);
void Func_02002be4(struct PaletteEffect *, s32, s32, s32);
void Func_02002e82();
void Func_02002eb4();
s32 Func_020030b0();
s32 Func_020030b8();
struct PaletteSceneRecord *Func_020030bc();
void Func_02001462();
void Func_02001478();
void Func_0200148c();
s32 Func_020014a2();
s32 Func_02001b2c();
void Func_02001c3a();
void Func_02001cea();
s32 Func_02001d04();
void Func_02001ebc();

/* One symbol per call site, named at the site's decoded address. All three
 * reach the same helper, which scales one channel by the adjustment, and each
 * site still needs its own name. */

void PaletteScene_AdvanceTransition(void);

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

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1_02000158(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call1_020002ec(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

/*
 * Returns the in-image table at 0x02009ba4. The eight-byte owner at 0x02000030
 * includes its one pool word, which holds that address and is returned
 * without being dereferenced.
 */
u8 *PaletteScene_GetScriptData(void)
{
    return (u8 *)0x02009ba4;
}

/*
 * Returns the in-image table at 0x02009c04. The eight-byte owner at 0x02000038
 * includes its one pool word, which holds that address and is returned
 * without being dereferenced.
 */
u8 *PaletteScene_GetMessageData(void)
{
    return (u8 *)0x02009c04;
}

/*
 * Returns the in-image table at 0x02009c24. The eight-byte owner at 0x02000040
 * includes its one pool word, which holds that address and is returned
 * without being dereferenced.
 */
u8 *PaletteScene_GetActorData(void)
{
    return (u8 *)0x02009c24;
}

/*
 * Returns the in-image table at 0x02009c34. The eight-byte owner at 0x02000048
 * includes its one pool word, which holds that address and is returned
 * without being dereferenced.
 */
u8 *PaletteScene_GetEffectData(void)
{
    return (u8 *)0x02009c34;
}

void PaletteScene_Initialize(void)
{
    void *scene;

    scene = *(void **)0x03001EBC;
    Event_Begin();
    Actor_WalkByAndWait(ACTOR_PARTY_LEADER, 0, 0);
    Event_RequestExit(FIELD_AT_OFFSET(scene, s16 *, 0x16C));
    Event_End();
}

/* Returns this overlay's state block. */
u8 *PaletteScene_GetState(void) { return (u8 *)0x02009d3c; }

void FieldScene_RunScene395_02000158(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    if (GameFlag_IsSet(0x845) != 0) {
        Func_02001462(10, 1);
        Event_SetMessage(MSG_NOW_HAVE_SUCH_POWER_AXE);
        Event_ShowMessage(8, 0);
        Func_02001478(10, 0);
    } else {
        if (GameFlag_IsSet(0x844) != 0) {
            Func_0200148c(10, 1);
            Event_SetMessage(MSG_SILENCE_2);
            Event_ShowMessage(8, 0);
            Value2(Func_020014a2, 10, 0);
            record = Func_02001b2c(184);
            if (record == -1) {
                goto L_02000220;
            }
            {
                u16 *target = (u16 *)(*(u8 **)Data_03001ebc + 0x172);
                s32 shown = 1;

                *target = shown;
            }
        } else {
            Event_SetMessage(MSG_SILENCE);
            Event_ShowMessage(8, 0);
            ColorBuffer_ApplyTarget(0x406218, 1);
            ColorBuffer_Interpolate(20);
            Task_Wait(40);
            Event_ShowMessageAndWait(0x200e, 0, 10);
            Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
            Event_ShowMessage(0x200e, 0);
            ColorBuffer_ApplyTarget(0x10000, 1);
            ColorBuffer_Interpolate(20);
            Task_Wait(40);
        }
    }
    L_02000220:;
    Event_End();
}

void PaletteScene_RunActorNineBranch(void)
{
    Event_Begin();
    if (GameFlag_IsSet(0x845) != 0) {
        Event_SetMessage(MSG_MUST_HORRIBLE_BEYOND_RIVER_AM);
    } else {
        Event_SetMessage(MSG_HEALING_WATERS_MERCURY_LIGHTHOUSE_MIGHT);
    }
    Event_ShowMessage(9, 0);
    Event_End();
}

void PaletteScene_RunActorEightBranch(void)
{
    Event_Begin();
    if (GameFlag_IsSet(0x845) != 0) {
        Event_SetMessage(MSG_KNOW_CANNOT_STOP_BUT_PLEASE);
    } else {
        Event_SetMessage(MSG_PEOPLE_KOLIMA_FORGIVE_ME);
    }
    Event_ShowMessage(8, 0);
    Event_End();
}

void PaletteScene_RunFlaggedBranch(void)
{
    Event_Begin();
    Func_02001d58();
    if (GameFlag_IsSet(0x844) == 0) {
        RunEventScript01();
    } else {
        PaletteScene_RunActorTransitionSequence();
    }
    Event_End();
}

void RunEventScript01(void)
{
    extern s32 Data_02009dd0;

    u32 i;
    s32 rec8;

    rec8 = Actor_Get(ACTOR_PARTY_LEADER);
    Value3(Engine_ActorFaceDirection, 0, 0xc000, 0);
    ColorBuffer_ApplyTarget(0x406218, 1);
    ColorBuffer_Interpolate(20);
    Task_Wait(40);
    Audio_PlayCue(17);
    Data_02009dd0 = 1;
    Call2(Func_02001c3a, 0x2009219, 0xc80);
    Task_Wait(30);
    Data_02009dd0 = 0;
    Camera_MoveTo(0x1480000, -1, 0xeb0000, 1);
    Actor_SetSpritePriority(ACTOR_PARTY_LEADER, 1);
    *(u8 *)(Func_02001d04(0) + 90) &= 254;
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 16);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x20000, 0x20000);
    Audio_PlayCue(133);
    *(s32 *)(rec8 + 40) = 0x50000;
    *(s32 *)(rec8 + 72) = 0x4000;
    *(s32 *)(rec8 + 68) = 0xa000;
    Actor_MoveToAndWait(ACTOR_PARTY_LEADER, 0x14f, 0x102);
    while (*(s32 *)(rec8 + 40) >= 0) {
        Task_Wait(1);
    }
    do {
        Task_Wait(1);
    } while (*(s32 *)(rec8 + 40) <= 0);
    Audio_PlayCue(161);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 19);
    Event_Wait(120);
    Call1_020002ec(Func_02001cea, 0x2009219);
    Task_Wait(40);
    *(s32 *)(rec8 + 68) = 0x4000;
    {
        u8 *record = Actor_Get(ACTOR_PARTY_LEADER);
        u8 flags = record[90] | 1;

        record[90] = flags;
    }
    Event_Wait(80);
    Event_SetMessage(MSG_CONTROL_TRETS_HEART_SHALL_NOT);
    Event_ShowMessageAndWait(0x200e, 0, 20);
    Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Event_Wait(20);
    Event_ShowMessage(0x200e, 0);
    Func_02001ebc();
    ColorBuffer_ApplyTarget(0x10000, 1);
    ColorBuffer_Interpolate(20);
    Task_Wait(40);
    {
        s32 shown = 0xc000;

        *(u16 *)(rec8 + 6) = shown;
    }
    *(s32 *)(rec8 + 72) = 0x10000;
    *(s32 *)(rec8 + 68) = 0x4000;
    Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Event_Wait(40);
    Actor_Jump(ACTOR_PARTY_LEADER, 4, 0);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
    Event_Wait(20);
}

/* The scene's actor transition sequence. Actor three takes part only when the
 * saved flag reports it enabled, and the arms that skip it bump the step
 * counter instead. */
void PaletteScene_RunActorTransitionSequence(void)
{
    s32 actorThreeEnabled;
    u8 *object;
    s32 *transitionState;
    s32 cycle;
    s32 sceneWorkSlot;
    s32 effectCallback;
    const s32 *finalActions;

    actorThreeEnabled = GameFlag_IsSet(3);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x148, 212);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 20);
    Audio_PlayCue(17);
    Message_ShowCentered(MSG_WATER_HERMES_SEEPED_INTO_TRET, 1);
    Actor_SetSpeed(ACTOR_GERALD, 0x10000, 0x8000);
    Actor_SetSpeed(ACTOR_IVAN, 0x10000, 0x8000);
    object = Actor_Get(ACTOR_PARTY_LEADER);
    if (object != 0) {
        Actor_SetPosition(ACTOR_GERALD, *(s32 *)(object + 8), *(s32 *)(object + 16));
    }
    object = Actor_Get(ACTOR_PARTY_LEADER);
    if (object != 0) {
        Actor_SetPosition(ACTOR_IVAN, *(s32 *)(object + 8), *(s32 *)(object + 16));
    }
    Actor_EnableActionCallback(ACTOR_GERALD, SceneAction_ActorOneEntry);
    Actor_EnableActionCallback(ACTOR_IVAN, SceneAction_ActorTwoEntry);
    if (actorThreeEnabled != 0) {
        Actor_SetSpeed(ACTOR_MIA, 0x10000, 0x8000);
        object = Actor_Get(ACTOR_PARTY_LEADER);
        if (object != 0) {
            Actor_SetPosition(ACTOR_MIA, *(s32 *)(object + 8), *(s32 *)(object + 16));
        }
        Actor_EnableActionCallback(ACTOR_MIA, SceneAction_ActorThreeEntry);
    }
    Func_020019c8(2);
    Event_Wait(40);
    KorimaPalette_Restore(0);
    ColorBuffer_Interpolate(32);
    Task_Wait(40);
    transitionState = &Data_02009dd4;
    *transitionState = 0;
    Value2(Func_02001910, (s32)PaletteScene_AdvanceTransition, 0xc80);
    Event_Wait(40);
    Actor_FaceDirection(ACTOR_GERALD, 0x6000, 20);
    Camera_SetSpeed(0x33333, 0x6666);
    Camera_MoveTo(0x1000000, -1, 0xfe0000, 1);
    Camera_WaitForMove();
    Audio_PlayCue(246);
    Event_Wait(40);
    Actor_FaceDirection(ACTOR_IVAN, 0x2000, 20);
    Camera_MoveTo(0x19d0000, -1, 0x1050000, 1);
    Camera_WaitForMove();
    Audio_PlayCue(246);
    Event_Wait(40);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0x4000, 20);
    Camera_MoveTo(0x1460000, -1, 0x1800000, 1);
    Camera_WaitForMove();
    Audio_PlayCue(246);
    if (*transitionState != 24) {
        do {
            Task_Wait(1);
        } while (*transitionState != 24);
    }
    Value1(Func_02001918, (s32)PaletteScene_AdvanceTransition);
    Task_Wait(10);
    cycle = 0;
    do {
        KorimaPalette_Restore(0);
        ColorBuffer_Interpolate(6);
        Task_Wait(6);
        KorimaPalette_Restore(1);
        ColorBuffer_Interpolate(6);
        cycle = (cycle + 1);
        Task_Wait(6);
    } while ((u32)cycle <= 3);
    KorimaPalette_Restore(0);
    ColorBuffer_Interpolate(40);
    Task_Wait(80);
    Camera_MoveTo(0x1480000, 0x80000, 0xd40000, 1);
    Camera_WaitForMove();
    Event_Wait(40);
    Func_020012f4(10, 1);
    Event_Wait(40);
    Audio_PlayCue(7);
    Event_SetMessage(MSG_FEEL_GREAT_POWER_SPREADING_THROUGH);
    Event_ShowMessage(8, 0);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Actor_StartRepeatedMotion(ACTOR_GERALD, 2);
    Actor_StartRepeatedMotion(ACTOR_MIA, 2);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xc000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0xc000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0xc000, 20);
    Func_020012f4(10, 2);
    Event_Wait(20);
    Func_020012f4(10, 3);
    Event_Wait(40);
    Func_020012f4(10, 1);
    Event_Wait(20);
    Event_ShowMessage(8, 0);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x105, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x105, 0);
    Actor_ShowEmote(ACTOR_MIA, 0x105, 0);
    Actor_ShowEmote(ACTOR_IVAN, 0x105, 40);
    Camera_MoveTo(0xea0000, 0, 0xe80000, 1);
    Camera_WaitForMove();
    Event_Wait(40);
    Func_020012f4(11, 1);
    Event_Wait(40);
    Func_020012f4(11, 3);
    Event_Wait(20);
    Event_ShowMessageAndWait(0x4009, 0, 20);
    Func_020012f4(11, 2);
    Event_Wait(10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x6000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0x6000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0x6000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0x6000, 20);
    Func_020012f4(11, 3);
    Event_Wait(20);
    Func_020012f4(11, 2);
    Event_Wait(20);
    Func_020012f4(11, 3);
    Event_ShowMessageAndWait(0x4009, 0, 10);
    Func_020012f4(10, 0);
    Event_Wait(20);
    Event_ShowMessage(0x8008, 0);
    Func_020012f4(10, 1);
    Event_Wait(20);
    Event_OpenMessage(0x8008, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xe000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0xa000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0xc000, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_ShowMessage(0x4009, 0);
        Event_ShowMessage(0x8008, 0);
    } else {
        sceneWorkSlot = 0x3001ebc;
        *(u16 *)((*(s32 *)sceneWorkSlot + 0x1d8)) += 2;
        Actor_ShowEmote(ACTOR_MIA, 0x103, 0);
        Actor_ShowEmote(ACTOR_GERALD, 0x103, 0);
        Actor_ShowEmote(ACTOR_IVAN, 0x103, 40);
        Actor_SetAnimation(ACTOR_GERALD, 4);
        Event_ShowMessage(ACTOR_GERALD, 0);
        if (actorThreeEnabled != 0) {
            Actor_RunRepeatedMotion(ACTOR_MIA, 2);
            Event_ShowMessage(ACTOR_MIA, 0);
        } else {
            *(u16 *)((*(s32 *)sceneWorkSlot + 0x1d8)) += 1;
        }
        Actor_SetAnimationAndWait(ACTOR_IVAN, 3);
        Event_ShowMessage(ACTOR_IVAN, 0);
        Event_ShowMessage(0x4009, 0);
        Event_ShowMessage(0x8008, 0);
    }
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimation(ACTOR_GERALD, 3);
    Actor_SetAnimation(ACTOR_MIA, 3);
    Actor_SetAnimationAndWait(ACTOR_IVAN, 3);
    Camera_MoveTo(0x1480000, 0x80000, 0xd40000, 1);
    Camera_WaitForMove();
    Event_Wait(20);
    Func_020012f4(10, 0);
    Event_Wait(20);
    KorimaPalette_Restore(0);
    ColorBuffer_Interpolate(1);
    Task_Wait(1);
    ColorBuffer_ApplyTarget(0x406218, 1);
    ColorBuffer_Interpolate(40);
    Event_Wait(60);
    Data_02009dcc = 0;
    Data_02009dc0[0] = 0x1480000;
    Data_02009dc0[1] = 0x300000;
    effectCallback = (s32)Func_020015a0;
    Data_02009dc0[2] = 0xcd0000;
    Value2(Func_02001910, effectCallback, 0xc80);
    Event_Wait(100);
    Func_02001918(effectCallback);
    ColorBuffer_ApplyTarget(0x7fff, 0);
    ColorBuffer_Interpolate(60);
    Event_Wait(100);
    KorimaPalette_Restore(0);
    ColorBuffer_Interpolate(20);
    Event_Wait(40);
    Func_020012f4(10, 1);
    Event_Wait(10);
    Event_SetMessage(MSG_SHOULD_DO_PEOPLE_KOLIMA_CURSED);
    Event_ShowMessage(0x8008, 0);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimation(ACTOR_GERALD, 3);
    Actor_SetAnimation(ACTOR_MIA, 3);
    Actor_SetAnimationAndWait(ACTOR_IVAN, 3);
    Camera_MoveTo(0xea0000, 0, 0xe80000, 1);
    Camera_WaitForMove();
    Event_Wait(20);
    Event_ShowMessage(0x4009, 0);
    Event_ShowMessageAndWait(0x8008, 0, 10);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x6000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xe000, 10);
    Event_OpenMessage(ACTOR_GERALD, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Actor_ShowEmote(ACTOR_GERALD, 0x102, 40);
    } else {
        Actor_SetAnimationAndWait(ACTOR_GERALD, 4);
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    }
    Event_ShowMessage(ACTOR_GERALD, 0);
    Func_020012f4(10, 4);
    Event_Wait(20);
    Event_SetMessage(MSG_WAS_INDEED_ANGRY_PEOPLE_HAD);
    Event_ShowMessage(0x8008, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xc000, 0);
    Event_ShowMessage(0x8008, 0);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimation(ACTOR_GERALD, 3);
    Actor_SetAnimation(ACTOR_MIA, 3);
    Actor_SetAnimationAndWait(ACTOR_IVAN, 3);
    Func_020012f4(10, 4);
    Event_Wait(20);
    Event_ShowMessageAndWait(0x8008, 0, 20);
    Func_020012f4(11, 0);
    Event_ShowMessageAndWait(0x4009, 0, 20);
    Func_020012f4(11, 3);
    Event_Wait(40);
    Func_020012f4(11, 1);
    Event_Wait(20);
    Event_ShowMessageAndWait(0x4009, 0, 20);
    Func_020012f4(10, 2);
    Event_Wait(20);
    Event_ShowMessage(0x8008, 0);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x102, 0);
    Actor_ShowEmote(ACTOR_MIA, 0x102, 0);
    Actor_ShowEmote(ACTOR_IVAN, 0x102, 80);
    Func_020012f4(11, 5);
    Event_Wait(60);
    Func_020012f4(11, 3);
    Event_Wait(20);
    Event_ShowMessageAndWait(0x4009, 0, 20);
    Func_020012f4(10, 5);
    Event_Wait(40);
    Func_020012f4(10, 2);
    Event_Wait(20);
    Event_ShowMessageAndWait(0x4008, 0, 20);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
    Actor_FaceDirection(ACTOR_GERALD, 0x8000, 10);
    Event_ShowMessage(ACTOR_GERALD, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0x8000, 20);
    Event_ShowMessage(0x8002, 0);
    Func_020012f4(11, 4);
    Event_Wait(20);
    Event_ShowMessageAndWait(0x4009, 0, 20);
    Actor_FaceDirection(ACTOR_GERALD, 0xe000, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x6000, 10);
    Event_ShowMessageAndWait(ACTOR_GERALD, 0, 20);
    Func_020012f4(10, 1);
    Event_ShowMessageAndWait(0x8008, 0, 10);
    Func_020012f4(10, 2);
    Event_Wait(20);
    Func_020012f4(11, 3);
    Event_Wait(40);
    Func_020012f4(11, 0);
    Event_Wait(20);
    KorimaPalette_Restore(0);
    ColorBuffer_Interpolate(1);
    Task_Wait(1);
    ColorBuffer_ApplyTarget(0x406218, 1);
    ColorBuffer_Interpolate(40);
    Event_Wait(60);
    Data_02009dcc = 0;
    Data_02009dc0[0] = 0x880000;
    Data_02009dc0[1] = 0x140000;
    effectCallback = (s32)Func_020015a0;
    Data_02009dc0[2] = 0x1020000;
    Value2(Func_02001910, effectCallback, 0xc80);
    Event_Wait(100);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x6000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0x6000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0x6000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0x6000, 40);
    Actor_StartRepeatedMotion(ACTOR_IVAN, 1);
    Actor_ShowEmote(ACTOR_IVAN, 0x100, 20);
    Event_ShowMessageAndWait(0x8002, 0, 10);
    Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x2000, 10);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Func_020012f4(10, 4);
    Event_Wait(20);
    Event_ShowMessage(0x8008, 0);
    Actor_ShowEmote(ACTOR_IVAN, 0x101, 60);
    Actor_FaceDirection(ACTOR_IVAN, 0xc000, 10);
    Event_ShowMessageAndWait(0x8002, 0, 10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0xc000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xc000, 20);
    Event_ShowMessageAndWait(0x8008, 0, 10);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimation(ACTOR_GERALD, 3);
    Actor_SetAnimation(ACTOR_MIA, 3);
    Actor_SetAnimationAndWait(ACTOR_IVAN, 3);
    Event_Wait(10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x6000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0x6000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0x6000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0x6000, 120);
    Func_02001918(effectCallback);
    Event_Wait(60);
    KorimaPalette_Restore(0);
    ColorBuffer_Interpolate(40);
    Func_020012f4(10, 2);
    Event_Wait(20);
    Event_ShowMessageAndWait(0x8008, 0, 20);
    Func_020012f4(11, 3);
    Event_ShowMessage(0x4009, 0);
    Event_ShowMessage(0x8008, 0);
    Func_020012f4(11, 4);
    Event_Wait(20);
    Event_ShowMessageAndWait(0x4009, 0, 10);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Actor_StartRepeatedMotion(ACTOR_GERALD, 2);
    Actor_StartRepeatedMotion(ACTOR_MIA, 2);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
    Func_020012f4(10, 1);
    Event_OpenMessage(0x8008, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xe000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0xc000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0xa000, 0);
    if (Event_ChooseYesNo(0, 0) == 1) {
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    }
    Event_Wait(10);
    Func_020012f4(10, 2);
    Event_Wait(20);
    Func_020012f4(11, 3);
    Event_Wait(40);
    Func_020012f4(10, 1);
    Event_Wait(20);
    Event_ShowMessageAndWait(0x8008, 0, 10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0, 0);
    Actor_FaceDirection(ACTOR_MIA, 0xc000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0x8000, 10);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimation(ACTOR_GERALD, 3);
    Actor_SetAnimation(ACTOR_MIA, 3);
    Actor_SetAnimationAndWait(ACTOR_IVAN, 3);
    Audio_PlayCue(17);
    finalActions = SceneAction_GroupFinish;
    Actor_EnableActionCallback(ACTOR_GERALD, finalActions);
    if (actorThreeEnabled != 0) {
        Actor_EnableActionCallback(ACTOR_MIA, finalActions);
    }
    Call2(Func_020019d0, 2, (s32)finalActions);
    Func_020012f4(10, 4);
    Func_020012f4(10, 4);
    Event_Wait(20);
    Event_SetMessage(MSG_OWE_GREAT_DEBT_HAVE_SAVED);
    Event_ShowMessage(0x8008, 0);
    Func_020012f4(11, 4);
    Func_020012f4(11, 4);
    Event_Wait(20);
    Event_ShowMessageAndWait(0x4009, 0, 10);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    GameFlag_Set(0x845);
    Audio_PlayCue(1);
    PaletteScene_SetRecordValue(184, 185);
}

s32 PaletteScene_AdvanceEffectFrame(struct PaletteEffectFrame *frame)
{
    frame->progress += 0x1EB8;
    if (frame->limit == 0x80000000) {
        if (frame->second_limit == frame->limit) {
            if (frame->third_limit == frame->second_limit) {
                Engine_ObjectDispatchRelease(frame);
            }
        }
    }
    return 1;
}

void PaletteScene_SpawnEffect(void)
{
    extern s32 Data_02009dd0;

    struct PaletteEffect *effect;
    struct EffectSprite *sprite;
    s32 phase;
    s32 effect_flags;
    s32 sprite_flags;
    s32 spawn_x = 0x01460000;
    s32 spawn_y = 0x00200000;
    s32 spawn_z = 0x00c00000;
    s32 target_x = 0x01460000;
    s32 target_z = 0x00f00000;

    phase = Data_03001e40 & 3;
    if (phase != 0) return;
    if (Data_02009dd0 != 0) Audio_PlayCue(200);
    effect = Func_02002b84(26, spawn_x, spawn_y, spawn_z);
    if (effect == 0) return;
    sprite = effect->sprite;
    sprite->state = phase;
    effect_flags = 0xfe;
    effect_flags &= effect->flags;
    effect->flags = effect_flags;
    sprite_flags = ~12;
    sprite_flags &= sprite->flags;
    sprite_flags |= 4;
    sprite->flags = sprite_flags;
    effect->progress = 0x1999;
    effect->rate_x = 0x40000;
    effect->rate_y = 0x40000;
    effect->mode = phase;
    Object_SetAnimation(effect, 2);
    Func_02002be4(effect, target_x, 0, target_z);
    Object_SetScript(effect, Data_02009d9c);
}

/* Steps the shared transition counter, firing at 0 and at 20 and wrapping at
 * 30. */
void PaletteScene_AdvanceTransition(void)
{
    s32 step = Data_02009dd4;

    if (step == 0) {
        KorimaPalette_Restore(0);
        ColorBuffer_Interpolate(20);
    } else if (step == 20) {
        KorimaPalette_Restore(1);
        ColorBuffer_Interpolate(8);
    }
    step = Data_02009dd4 + 1;
    Data_02009dd4 = step;
    if (step == 30) {
        Data_02009dd4 = 0;
    }
}

void PaletteScene_AdvanceOrbit(struct OrbitingPaletteEffect *effect)
{
    s32 position[3];
    s32 step = effect->step;
    s32 heading;

    if (step <= 119) {
        position[0] = effect->anchor_x;
        position[1] = effect->anchor_y;
        position[2] = effect->anchor_z;
        heading = effect->heading;
        Func_02002e82(step << 16, step * 768 + heading, position);
        effect->x = position[0];
        effect->y = position[1];
        effect->z = position[2];
        effect->angle_x += 0x147;
        effect->angle_y += 0x147;
        effect->step++;
    } else {
        Func_02002eb4(effect->owner[0x1c]);
        Engine_ObjectDispatchRelease(effect);
    }
}

/* Two lookups, each of which can fail with -1; on success stores the caller's
 * halfword into the table at +216 of the record the first index names. */
void PaletteScene_SetRecordValue(s32 key, s32 value)
{
    s32 slot = Func_020030b0(key);

    if (slot != -1) {
        s32 index = Func_020030b8(slot, key);

        if (index != -1) {
            Func_020030bc(slot)->values[index] = value;
        }
    }
}

/* Applies the adjustment to palette RAM, skipping two protected windows. */
void PaletteScene_AdjustPaletteWindow(s32 adjustment)
{
    volatile u16 *palette = (volatile u16 *)0x05000000;
    u32 phase;
    u32 next_phase;
    KorimaPalette_SaveFirst();
    phase = 0;
    do {
        u32 index = phase >> 16;
        u32 second_window;

        if ((u32)(phase + 0xffef0000) > 0x60000) {
            second_window = (index + 0xff3f) << 16;
            if (second_window > 0x70000)
                palette[index] = PaletteScene_AdjustColor(palette[index], adjustment);
        }
        next_phase = phase + 0x10000;
        phase = next_phase;
    } while (next_phase <= 0x00df0000);
    KorimaPalette_Capture(); KorimaPalette_SaveSecond(); ColorBuffer_ApplyTarget(0x10000, 0);
}

/*
 * Applies the asymmetric RGB555 colour adjustment: red rises, green and blue
 * fall. Control jumps over a mask literal inside the span and rejoins before
 * the common return, so the literal belongs to this owner.
 */
u16 PaletteScene_AdjustColor(u16 color, s32 adjustment)
{
    s16 green = (s16)((color >> 5) & 31);
    s16 red = (s16)(color & 31);
    s16 blue = (s16)((color >> 10) & 31);
    u32 packed;

    red = (s16)(red + Math_Divide(
        red,
        (s32)((u32)adjustment << 2)
    ));
    green = (s16)(green - Math_Divide(green, adjustment));
    blue = (s16)(blue - Math_Divide(blue, adjustment));

    /* Only the increasing channel is explicitly saturated by this owner. */
    if (red > 31)
        red = 31;

    packed = (u32)(s32)red;
    packed |= ((u32)(s32)blue << 10) | ((u32)(s32)green << 5);
    return (u16)packed;
}
