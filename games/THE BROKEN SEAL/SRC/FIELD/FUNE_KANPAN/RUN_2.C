#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
extern u8 MsgFuneAyeCaptainSeaMonsters[];
extern u8 MsgFunePreparationsReady[];


union Slot {
    s32 w;
    s16 h[2];
};

extern u8 FuneKanpan_CrewScript[];

s32 BuildMotionCountdown(s32, s16);
void FieldScene_RunScene3af_02001c14();
s32 Object_GetById();
void Event_CallWithLastActiveObjectId();
void FieldScene_CallPairWith10();
void Event_CallWithLastActiveObjectId(s32);
s32 Object_GetById(s32);

/* Phase/status word at 0x1c0 of the shared scene work record. */

/* The step value differs in the localized scene data. */

/* Offset of a flag byte on an actor record, cleared and set below. */

/* Pointer, held at fixed address 0x03001ebc, to the shared scene work
 * record. The phase/status word lives at offset 0x1c0 of that record. */

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

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)&gEventWork;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

static __inline__ void Call1_020029d4(void (*f)(), s32 a0)
{

    f(a0);
}

static __inline__ void Call1_02003a0c(void (*f)(), s32 a0)
{

    f(a0);
}

#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
#endif

#if defined(TBS_EDITION_JA)
#elif defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
#else
#endif

#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
#endif
#if defined(TBS_EDITION_DE)
#endif

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
void FieldScene_RunScene3af_02000bb8(void);

void FieldScene_RunScene3af_02001a98(void)
{
    s32 record;

    Event_Begin();
    Camera_MoveTo(-1, -1, -1, 0);
    Task_Wait(1);
    Actor_SetPosition(20, 0, 0);
    Actor_SetPosition(22, 0, 0);
    Actor_SetPosition(24, 0, 0);
    Actor_SetPosition(25, 0, 0);
    Actor_SetPosition(26, 0, 0);
    Actor_SetPosition(27, 0, 0);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
    Actor_SetPosition(23, 0, 0);
    record = Object_GetById(23);
    {
        s32 shown = 0x3000;

        *(u16 *)(record + 6) = shown;
    }
    Actor_SetPosition(21, 0xe80000, 0x28a0000);
    record = Object_GetById(21);
    {
        s32 shown = 0xb000;

        *(u16 *)(record + 6) = shown;
    }
    Camera_MoveTo(0xe80000, -1, 0x27c0000, 0);
    Map_Redraw();
    Task_Wait(1);
    FieldScene_RunScene3af_02001c14(23, 21);
}

void FieldScene_RunScene3af_02001b58(void)
{
    s32 record;

    Event_Begin();
    Call1(Event_CallWithLastActiveObjectId, (u32)FuneKanpan_CrewScript);
    Task_Wait(1);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0xe80000, 0x27c0000);
    Actor_SetChildValue(ACTOR_PARTY_LEADER, 15);
    record = Object_GetById(0);
    Actor_SetSpriteFlags(record, 0);
    Task_Wait(1);
    Camera_FollowActor(ACTOR_PARTY_LEADER, 0);
    Map_Redraw();
    Task_Wait(1);
    Actor_Stop(22);
    Actor_Stop(21);
    Task_Wait(1);
    Actor_SetPosition(22, 0, 0);
    Actor_SetPosition(21, 0, 0);
    Actor_SetPosition(20, 0, 0);
    record = Object_GetById(20);
    {
        s32 shown = 0x3000;

        *(u16 *)(record + 6) = shown;
    }
    Actor_SetPosition(23, 0xe80000, 0x28a0000);
    record = Object_GetById(23);
    {
        s32 shown = 0xb000;

        *(u16 *)(record + 6) = shown;
    }
    Task_Wait(1);
    FieldScene_RunScene3af_02001c14(20, 23);
}

void FieldScene_RunScene3af_02001c14(s32 a0, s32 a1)
{
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    Event_OpenScreen();
    Event_WaitForScreen();
    Event_Wait(20);
    FieldScene_RunScene3af_02000bb8();
    Actor_SetPosition(a0, 0xd80000, 0x24c0000);
    Actor_SetSpeed(a0, 0xcccc, 0x6666);
    Actor_WalkToAndWait(a0, 216, 0x258);
    Actor_WalkToAndWait(a0, 218, 0x25c);
    Actor_WalkToAndWait(a0, 234, 0x25c);
    Actor_WalkToAndWait(a0, 236, 0x26a);
    Actor_FaceDirection(a0, 0x5000, 20);
    Actor_SetAnimationAndWait(a0, 3);
    Event_Wait(20);
    Call2(FieldScene_CallPairWith10, a1, 0x5000);
    Actor_Jump(a1, 4, 40);
    Actor_StartRepeatedMotion(a1, 2);
    Event_SetMessage((s32)MsgFunePreparationsReady);
    Event_ShowMessageAndWait(a1, 0, 20);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
    Event_CloseScreen();
    Event_WaitForScreen();
    Event_RequestExit(10);
}

void FieldScene_RunActorTwentyDialogueSequence(void)
{
    extern s32 *Data_03001ebc;

    Event_Begin();
    Event_CallWithLastActiveObjectId((s32)FuneKanpan_CrewScript);
    Task_Wait(1);
    Actor_SetChildValue(ACTOR_PARTY_LEADER, 15);
    Actor_SetSpriteFlags(Object_GetById(0), 0);
    Data_03001ebc[0x70] = 0x202;
    Event_OpenScreen();
    Event_WaitForScreen();
    Event_Wait(20);
    Actor_RunRepeatedMotion(20, 1);
    Event_SetMessage((s32)MsgFuneAyeCaptainSeaMonsters);
    Event_ShowMessageAndWait(20, 0, 10);
    FieldScene_CallPairWith10(22, 0x5000);
    Actor_Jump(22, 4, 20);
    Actor_StartRepeatedMotion(22, 2);
    Event_ShowMessageAndWait(0x6016, 0, 20);
    Event_CloseScreen();
    Event_WaitForScreen();
    Event_RequestExit(11);
}
void FieldScene_CallPairWith10(s32 a, s32 b);
