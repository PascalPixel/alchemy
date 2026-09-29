/* The log-rolling stage's start: the leader and the competitor walk out onto
 * the logs, face each other and take their places, the result is weighted
 * by the leader's place, and the stage records where the party returns. */
#include "LOG_ROLLING.H"

void ColossoLogRollingStage_NoopSceneHook(void);
s32 ColossoLogRollingStage_PositionActiveActor(s32 slot, s32 frames);
void ColossoLogRollingStage_WaitForBalanceState(void);
void Object_RefreshSelectorById(s32 object);
void BattleFx_SetWeightedResult(s32 kind, s32 weight);
void Party_SetFields1ceAnd1d0(s32 scene, s32 entrance);
void Event_SetPair1d4(s32 scene, s32 entrance);

void KorosseoMaruta_RunStageStart(void)
{
    s32 place;
    s32 i;
    s32 last;

    /* FAKEMATCH: the last place is set at the top as a forced temporary, so
     * the weight stays 4 - place + 1 instead of folding to 5 - place. */
    last = 4;
    ColossoLogRollingStage_NoopSceneHook();
    Engine_EventBegin();
    place = ColossoLogRollingStage_PositionActiveActor(3, 17);
    ColossoLogRollingStage_WaitForBalanceState();
    for (i = 0; i < 10; i++) {
        Object_RefreshSelectorById(8);
    }
    Actor_SetSpeed(8, 0x10000, 0x8000);
    Actor_WalkTo(8, 0x5f8, 192);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x5d8, 192);
    Actor_SetAnimation(8, 1);
    Actor_FaceEachOther(ACTOR_PARTY_LEADER, 8, 0);
    Event_Wait(10);
    Actor_SetAnimation(8, 3);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Event_Wait(20);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x20000, 0x10000);
    Actor_SetSpeed(8, 0x20000, 0x10000);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 0x5e0, 192);
    Actor_WalkToAndWait(8, 0x5f0, 192);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 16);
    Actor_SetAnimation(8, 9);
    Event_Wait(10);
    BattleFx_SetWeightedResult(72, last - place + 1);
    /* FAKEMATCH: the do/while keeps the stage flag store ahead of the
     * scene load that follows it. */
    do {
        gGameState.unknown_1f8[0x22b - 0x1f8] = 3;
    } while (0);
    Party_SetFields1ceAnd1d0((s32)&SceneId_KorosseoMaruta, 4);
    Event_SetPair1d4((s32)&SceneId_KorosseoMaruta, 5);
    Engine_GameFlagSet(0x11a);
}
