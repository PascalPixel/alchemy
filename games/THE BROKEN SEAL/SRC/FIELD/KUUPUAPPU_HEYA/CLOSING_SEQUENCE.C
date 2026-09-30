#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

extern u8 MsgKuupuappuVaultCutFree[];
extern u8 MsgKuupuappuVaultThievesCaught[];

void SceneActor_SetModeZeroAndValue();
void SceneActor_SetPairZeroAndValue();
void SceneEffect_ApplyThreeValuesAndFinish();
void FieldScene_RunSplitTripleSteps();
void Object_RefreshSelectorById();
void Party_SetFields1ceAnd1d0();
void Event_SetPair1d4();
void BattleFx_SetWeightedResult();

/* The Vault's closing scene after the thieves are caught: the party is cut
 * free, the mayor and the villagers talk it over, one question decides who
 * speaks next, and the scene closes back into the house at entrance 17,
 * with entrance 16 kept as the one to return to. The two runs of lines
 * start at MsgKuupuappuVaultCutFree and MsgKuupuappuVaultThievesCaught. */
void FieldScene_RunVaultClosingSequence(void)
{
    s32 base;
    u8 *state;

    Engine_EventBegin();
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 52428, 26214);
    Actor_SetSpeed(ACTOR_GERALD, 52428, 26214);
    Actor_SetSpeed(ACTOR_IVAN, 52428, 26214);
    Engine_ActorRunRepeatedMotion(0, 3);
    Engine_EventWait(20);
    base = (s32)MsgKuupuappuVaultCutFree;
    Engine_MessageShowCentered(base, 1);
    base = base + 1;
    Engine_EventSetMessage(base);
    Engine_EventWait(30);
    Engine_ActorSetAnimation(8, 1);
    Event_Wait(60);
    SceneEffect_ApplyThreeValuesAndFinish(8, 3, 40);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 792, 440);
    FieldScene_RunSplitTripleSteps(0, 8, 20);
    Actor_SetPosition(ACTOR_GERALD, 51904512, 28835840);
    Actor_SetPosition(ACTOR_IVAN, 51904512, 28835840);
    Actor_WalkTo(ACTOR_GERALD, 808, 432);
    Actor_WalkTo(ACTOR_IVAN, 792, 456);
    Engine_ActorWaitForMove(1);
    Engine_ActorFaceActor(1, 8, 0);
    Engine_ActorWaitForMove(2);
    FieldScene_RunSplitTripleSteps(2, 8, 60);
    Actor_ShowEmote(8, 258, 0);
    Engine_EventWait(60);
    SceneActor_SetModeZeroAndValue(8, 20);
    Engine_ActorSetAnimation(2, 3);
    SceneEffect_ApplyThreeValuesAndFinish(1, 3, 30);
    Engine_ActorRunRepeatedMotion(8, 2);
    Engine_EventWait(20);
    SceneActor_SetModeZeroAndValue(8, 20);
    Actor_ShowEmote(ACTOR_IVAN, 258, 0);
    Actor_ShowEmote(ACTOR_GERALD, 258, 0);
    Engine_EventWait(60);
    Actor_FaceDirection(8, 12288, 0);
    Engine_EventWait(10);
    SceneActor_SetModeZeroAndValue(8, 30);
    Actor_FaceDirection(8, 53248, 0);
    Engine_EventWait(30);
    Actor_ShowEmote(8, 256, 0);
    Engine_EventWait(60);
    Actor_FaceDirection(8, 45056, 0);
    Engine_EventWait(40);
    Actor_FaceDirection(8, 53248, 0);
    Engine_EventWait(40);
    FieldScene_RunSplitTripleSteps(8, 0, 20);
    SceneEffect_ApplyThreeValuesAndFinish(8, 4, 30);
    SceneActor_SetModeZeroAndValue(8, 20);
    Engine_ActorRunRepeatedMotion(2, 1);
    SceneActor_SetModeZeroAndValue(2, 40);
    Actor_SetPosition(10, 48758784, 26738688);
    Engine_AudioPlayCue(61);
    SceneActor_SetModeZeroAndValue(10, 20);
    Engine_ActorStartRepeatedMotion(0, 1);
    Engine_ActorStartRepeatedMotion(1, 1);
    Engine_ActorStartRepeatedMotion(2, 1);
    Engine_ActorRunRepeatedMotion(8, 1);
    Engine_EventWait(30);
    Engine_ActorFaceActor(0, 10, 0);
    Engine_ActorFaceActor(1, 10, 0);
    Engine_ActorFaceActor(2, 10, 0);
    FieldScene_RunSplitTripleSteps(8, 10, 40);
    Actor_SetSpeed(10, 52428, 26214);
    Actor_SetSpeed(11, 98304, 49152);
    Actor_SetSpeed(12, 98304, 49152);
    Actor_SetPosition(11, 48758784, 26738688);
    Actor_SetPosition(12, 48758784, 26738688);
    Actor_WalkToAndWait(10, 792, 416);
    Actor_FaceDirection(10, 12288, 0);
    Engine_EventWait(70);
    Engine_ActorFaceActor(0, 10, 0);
    Actor_FaceActor(ACTOR_GERALD, 10, 0);
    Engine_ActorFaceActor(2, 10, 0);
    Engine_ActorFaceActor(8, 10, 0);
    Actor_EnableActionCallback(11, 33608264);
    Engine_EventWait(40);
    Actor_EnableActionCallback(12, 33608364);
    Object_RefreshSelectorById(12);
    Actor_FaceDirection(11, 8192, 0);
    Actor_FaceDirection(12, 8192, 0);
    Event_Wait(40);
    SceneEffect_ApplyThreeValuesAndFinish(10, 4, 20);
    SceneActor_SetModeZeroAndValue(10, 20);
    Engine_ActorRunRepeatedMotion(11, 1);
    Engine_EventWait(10);
    SceneActor_SetModeZeroAndValue(11, 20);
    Engine_ActorRunRepeatedMotion(12, 1);
    Engine_EventWait(10);
    SceneActor_SetModeZeroAndValue(12, 40);
    Actor_ShowEmote(ACTOR_IVAN, 257, 0);
    Engine_EventWait(60);
    SceneEffect_ApplyThreeValuesAndFinish(10, 4, 20);
    SceneActor_SetModeZeroAndValue(10, 20);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 257, 0);
    Actor_ShowEmote(ACTOR_GERALD, 257, 0);
    Event_Wait(60);
    Actor_ShowEmote(8, 256, 0);
    Engine_EventWait(40);
    Engine_ActorRunRepeatedMotion(8, 2);
    Engine_EventWait(20);
    SceneActor_SetModeZeroAndValue(8, 20);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 8, 0);
    Engine_ActorFaceActor(1, 8, 0);
    FieldScene_RunSplitTripleSteps(2, 8, 20);
    SceneEffect_ApplyThreeValuesAndFinish(8, 4, 30);
    SceneActor_SetModeZeroAndValue(8, 20);
    Engine_ActorRunRepeatedMotion(2, 2);
    Engine_EventWait(20);
    Engine_ActorFaceActor(0, 2, 0);
    FieldScene_RunSplitTripleSteps(1, 2, 30);
    SceneActor_SetModeZeroAndValue(1, 20);
    Actor_ShowEmote(8, 258, 0);
    Engine_EventWait(60);
    SceneActor_SetModeZeroAndValue(8, 20);
    Engine_ActorRunRepeatedMotion(10, 1);
    Engine_EventWait(10);
    SceneActor_SetModeZeroAndValue(10, 20);
    Engine_ActorFaceActor(0, 10, 0);
    Actor_FaceActor(ACTOR_GERALD, 10, 0);
    FieldScene_RunSplitTripleSteps(2, 10, 20);
    SceneEffect_ApplyThreeValuesAndFinish(11, 3, 20);
    SceneActor_SetModeZeroAndValue(11, 20);
    SceneEffect_ApplyThreeValuesAndFinish(12, 4, 20);
    SceneActor_SetModeZeroAndValue(12, 20);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 258, 0);
    Actor_ShowEmote(ACTOR_GERALD, 258, 0);
    Actor_ShowEmote(ACTOR_IVAN, 258, 0);
    Engine_EventWait(60);
    SceneEffect_ApplyThreeValuesAndFinish(10, 3, 20);
    Engine_EventOpenMessage(10, 0);
    Engine_EventWait(50);
    Engine_ActorFaceActor(2, 0, 0);
    FieldScene_RunSplitTripleSteps(1, 0, 30);
    if (Engine_UiWorkWaitThenFinalizeCapacity(0, 0) == 0) {
        Engine_EventWait(40);
        Engine_ActorFaceActor(1, 10, 0);
        Actor_FaceActor(ACTOR_IVAN, 10, 0);
        Actor_RunRepeatedMotion(10, 2);
        Engine_EventWait(20);
        Engine_EventShowMessage(10, 0);
    } else {
        Event_Wait(40);
        Engine_ActorFaceActor(1, 10, 0);
        Engine_ActorFaceActor(2, 10, 0);
        base = (s32)MsgKuupuappuVaultThievesCaught;
        Engine_EventSetMessage(base);
        Engine_ActorRunRepeatedMotion(10, 2);
        Engine_EventWait(20);
        Engine_EventShowMessage(10, 0);
        base = base - 3;
        Engine_EventSetMessage(base);
    }
    Actor_ShowEmote(11, 259, 0);
    Engine_EventWait(60);
    SceneActor_SetModeZeroAndValue(11, 20);
    SceneEffect_ApplyThreeValuesAndFinish(12, 4, 20);
    SceneActor_SetModeZeroAndValue(12, 30);
    SceneActor_SetPairZeroAndValue(10, 11, 30);
    Engine_ActorSetAnimation(10, 3);
    SceneEffect_ApplyThreeValuesAndFinish(11, 3, 30);
    SceneActor_SetPairZeroAndValue(10, 12, 30);
    Engine_ActorSetAnimation(10, 3);
    SceneEffect_ApplyThreeValuesAndFinish(12, 3, 40);
    Actor_FaceDirection(10, 12288, 0);
    Actor_FaceDirection(11, 12288, 0);
    Actor_FaceDirection(12, 12288, 0);
    Engine_EventWait(20);
    SceneEffect_ApplyThreeValuesAndFinish(10, 4, 20);
    Engine_EventShowMessage(10, 0);
    GameFlag_Set(2132);
    gEventWork->start_transition = 0x200;
    base = (s32)&SceneId_KuupuappuHeya;
    Party_SetFields1ceAnd1d0(base, 17);
    Event_SetPair1d4(base, 16);
    state = (u8 *)&gGameState;
    /* FAKEMATCH: the do-while puts the state's address in a block of its
       own, so it is loaded ahead of the byte's offset. */
    do {
        state[0x22b] = 3;
    } while (0);
    BattleFx_SetWeightedResult(12, 5);
    Engine_EventEnd();
}
