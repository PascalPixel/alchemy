/* The party comes aboard and hears there are monsters belowdecks. */
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "STAGED_ACTOR.H"
extern u8 MsgFuneMonstersBelowdecks[];

extern u8 FuneHeya_ProgressTableA[];
void UiText_ShowCenteredMessage();
void Ui_SetRenderResultFromObject();
void Object_SetActionCallbackAndRefreshById();

/* FAKEMATCH: call sites spelled through these wrappers pass their constants
 * straight into the argument registers; a direct call precomputes a costly
 * constant into a pseudo that the compiler then shares with later uses in
 * the block. A value-returning call also sets r0 last of its arguments. */
static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

void FieldScene_RunStepThen10(s32 a);
void FieldScene_CallPairWith10(s32 a, u16 b);
void ConfigureSceneMotionFlags(s32 x, s32 y, s32 z, u32 flags);
void FieldScene_RunSceneStep(s32 step, u32 arg, u32 opt);

/* The leader walks in and the three companions take their places beside
 * him; the shout about monsters comes up, actor 8 answers it and the
 * companions set off. */
void FieldScene_RunPositionTransferPresentation(void)
{
    s32 record;
    s32 message;
    s32 actions;

    ConfigureSceneMotionFlags(0x1b80000, -1, 0xb00000, 0x1000001);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0x1b80000, 0x860000);
    Event_OpenScreen();
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x19999, 0xcccc);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 5);
    Actor_MoveToAndWait(ACTOR_PARTY_LEADER, 0x198, 134);
    Actor_MoveToAndWait(ACTOR_PARTY_LEADER, 0x198, 152);
    Actor_MoveToAndWait(ACTOR_PARTY_LEADER, 0x1b0, 166);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 0);
    record = Value1((s32 (*)())Engine_ActorGet, 0);
    if (record != 0) {
        Actor_SetPosition(ACTOR_IVAN, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    record = Value1((s32 (*)())Engine_ActorGet, 0);
    if (record != 0) {
        Actor_SetPosition(ACTOR_GERALD, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    record = Value1((s32 (*)())Engine_ActorGet, 1);
    if (record != 0) {
        Actor_SetPosition(ACTOR_MIA, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Task_Wait(1);
    Actor_SetSpeed(ACTOR_IVAN, 0x19999, 0xcccc);
    Actor_WalkTo(ACTOR_IVAN, 0x1a8, 152);
    Actor_SetSpeed(ACTOR_GERALD, 0x19999, 0xcccc);
    Actor_WalkTo(ACTOR_GERALD, 0x1c0, 168);
    Actor_SetSpeed(ACTOR_MIA, 0x20000, 0x10000);
    Actor_WalkToAndWait(ACTOR_MIA, 0x1ca, 152);
    Actor_SetAnimation(ACTOR_GERALD, 1);
    Actor_SetAnimation(ACTOR_IVAN, 1);
    Actor_FaceDirection(ACTOR_IVAN, 0x4000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0x4000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0x4000, 40);
    Ui_SetRenderResultFromObject(10);
    /* The shout is shown centred, and the line after it is spoken. */
    message = (s32)MsgFuneMonstersBelowdecks;
    UiText_ShowCenteredMessage(message, 1, 10);
    Event_Wait(10);
    FieldScene_RunSceneStep(0, 0, 40);
    Value3(FieldScene_RunSceneStep, 1, 0x4000, 20);
    Camera_SetSpeed(0x39999, 0x7333);
    Call4(ConfigureSceneMotionFlags, 0x1b80000, -1, 0x1400000, 0x10000014);
    Actor_RunRepeatedMotion(8, 2);
    Call2(FieldScene_CallPairWith10, 8, 0xd000);
    Event_SetMessage(message + 1);
    FieldScene_RunStepThen10(8);
    Actor_FaceDirection(8, 0, 20);
    Call4(ConfigureSceneMotionFlags, 0x1b80000, -1, 0x860000, 0x10000000);
    actions = (s32)FuneHeya_ProgressTableA;
    Actor_EnableActionCallback(ACTOR_GERALD, actions);
    Value2(Engine_ActorEnableActionCallback, 2, actions);
    Object_SetActionCallbackAndRefreshById(3, actions);
    Event_Wait(40);
    GameFlag_Set(0x301);
    FieldScene_RunSceneStep(23, 0, 0);
    GameFlag_Clear(FLAG_ARRIVAL_EVENT_PENDING);
}
