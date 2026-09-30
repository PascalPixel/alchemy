#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

extern u8 MsgKuupuappuRobinTakeLead[];
extern u8 MsgKuupuappuTheyreActingSuspiciousSomethingsNot[];
extern u8 KuupuappuHeya_PairScriptF[];
extern u8 KuupuappuHeya_PairScriptG[];
extern u8 KuupuappuHeya_PairScriptK[];

void SceneActor_FaceActors24And25TowardActorZero(void);
void Scheduler_RemoveCallback();
void SceneActor_SetModeZeroAndValue(s32 a, s32 b);
void SceneEffect_ApplyThreeValuesAndFinish();
void SceneActor_SetPairZeroAndValue();
void FieldScene_RunSplitTripleSteps(s32 a, s32 b, s32 c);

/* Moves the next dialogue line on by amount messages. */
static __inline__ void bump_step(s32 amount)
{
    gEventWork->message += amount;
}

/* Ivan finds actors 24 and 25 suspicious and the party agrees to follow
 * them; the pair then take their places by the far wall. */
void FieldScene_RunScene383SequenceB(void)
{
    s32 actor24;
    s32 actor25;
    s32 base;

    actor24 = ((s32 (*)())Engine_ActorGet)(24);
    actor25 = ((s32 (*)())Engine_ActorGet)(25);
    Event_Begin();
    Scheduler_RemoveCallback((s32)SceneActor_FaceActors24And25TowardActorZero);
    GameFlag_Clear(0x300);
    if (*(s16 *)(actor24 + 100) <= 3) {
        Engine_ActorEnableActionCallback(24, KuupuappuHeya_PairScriptG);
    } else {
        Engine_ActorEnableActionCallback(24, KuupuappuHeya_PairScriptF);
    }
    if (*(s16 *)(actor25 + 100) <= 2) {
        Engine_ActorEnableActionCallback(25, KuupuappuHeya_PairScriptK);
    } else {
        Engine_ActorEnableActionCallback(25, KuupuappuHeya_PairScriptF);
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
    FieldScene_RunSplitTripleSteps(2, 0, 30);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
    base = (s32)MsgKuupuappuTheyreActingSuspiciousSomethingsNot;
    Event_SetMessage(base);
    Event_ShowMessage(ACTOR_IVAN, 0);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
    FieldScene_RunSplitTripleSteps(1, 2, 20);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    SceneEffect_ApplyThreeValuesAndFinish(1, 3, 20);
    SceneActor_SetPairZeroAndValue(0, 1, 10);
    Event_OpenMessage(ACTOR_GERALD, 0);
    if (Event_ChooseYesNo(0, 0) != 0) {
        bump_step(1);
    }
    SceneActor_SetModeZeroAndValue(1, 30);
    Event_SetMessage((base + 4));
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
    FieldScene_RunSplitTripleSteps(1, 2, 50);
    Actor_ShowEmote(ACTOR_IVAN, 0x100, 0);
    Event_Wait(60);
    SceneActor_SetPairZeroAndValue(0, 1, 50);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
    FieldScene_RunSplitTripleSteps(1, 2, 30);
    SceneEffect_ApplyThreeValuesAndFinish(2, 3, 10);
    SceneActor_SetModeZeroAndValue(2, 20);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x102, 0);
    Event_Wait(60);
    SceneEffect_ApplyThreeValuesAndFinish(2, 3, 20);
    SceneActor_SetModeZeroAndValue(2, 30);
    Event_SetMessage((s32)MsgKuupuappuRobinTakeLead);
    Event_ShowMessage(ACTOR_GERALD, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 0);
    Event_Wait(40);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimation(ACTOR_GERALD, 3);
    SceneEffect_ApplyThreeValuesAndFinish(2, 3, 50);
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
