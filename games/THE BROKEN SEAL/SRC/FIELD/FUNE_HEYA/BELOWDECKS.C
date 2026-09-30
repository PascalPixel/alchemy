/* The party comes aboard and hears there are monsters belowdecks. */
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "HEYA.H"
#include "STAGED_ACTOR.H"
extern u8 MsgFuneMonstersBelowdecks[];

extern u8 FuneHeya_ProgressTableA[];
void UiText_ShowCenteredMessage();
void Ui_SetRenderResultFromObject();
void Object_SetActionCallbackAndRefreshById();

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
    record = ((s32 (*)())Object_GetById)(0);
    if (record != 0) {
        Actor_SetPosition(ACTOR_IVAN, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    record = ((s32 (*)())Object_GetById)(0);
    if (record != 0) {
        Actor_SetPosition(ACTOR_GERALD, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    record = ((s32 (*)())Object_GetById)(1);
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
    FieldScene_RunSceneStep(1, 0x4000, 20);
    Camera_SetSpeed(0x39999, 0x7333);
    ConfigureSceneMotionFlags(0x1b80000, -1, 0x1400000, 0x10000014);
    Actor_RunRepeatedMotion(8, 2);
    FieldScene_CallPairWith10(8, 0xd000);
    Event_SetMessage(message + 1);
    FieldScene_RunStepThen10(8);
    Actor_FaceDirection(8, 0, 20);
    ConfigureSceneMotionFlags(0x1b80000, -1, 0x860000, 0x10000000);
    Actor_EnableActionCallback(ACTOR_GERALD, (s32)FuneHeya_ProgressTableA);
    Engine_ActorEnableActionCallback(2, (s32)FuneHeya_ProgressTableA);
    Object_SetActionCallbackAndRefreshById(3, (s32)FuneHeya_ProgressTableA);
    Event_Wait(40);
    GameFlag_Set(0x301);
    FieldScene_RunSceneStep(23, 0, 0);
    GameFlag_Clear(FLAG_ARRIVAL_EVENT_PENDING);
}
