#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

void Resource3ba_NoOpCallback(void);
s32 FieldScene_RunFlag211ApproachScene();
void SceneState_WaitUntilWord1000IsNine(void);
void Object_RefreshSelectorById(s32 actor);
s32 BattleFx_SetWeightedResult();
s32 Party_SetFields1ceAnd1d0();
void Event_SetPair1d4(s32 scene, s32 entrance);

/* The river stage's start: after the approach scene and once the stage is
   ready, actor 8 and the leader walk to the start line and face each
   other. The approach's outcome goes to the weighted result, the river's
   scene is stored with entrance 4 and 5 in the two scene pairs, the stage
   byte at 0x22b becomes 3 and flag 0x11a is set. */
void KorosseoKawa_RunStageStart(void)
{
    s32 approach;
    s32 n;
    s32 base;
    u8 *state;

    base = 0;
    Resource3ba_NoOpCallback();
    Engine_EventBegin();
    approach = FieldScene_RunFlag211ApproachScene(120, 127);
    SceneState_WaitUntilWord1000IsNine();
    n = 9;
    do {
        Object_RefreshSelectorById(8);
        n = n - 1;
    } while (n >= 0);
    Actor_SetSpeed(8, 0x10000, 0x8000);
    Actor_WalkTo(8, 0x528, 192);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x508, 192);
    Engine_ActorSetAnimation(8, 1);
    Engine_ActorFaceEachOther(0, 8, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimation(8, 3);
    Engine_ActorSetAnimationAndWait(0, 3);
    Engine_EventWait(20);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x20000, 0x10000);
    Actor_SetSpeed(8, 0x20000, 0x10000);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 0x510, 192);
    Actor_WalkToAndWait(8, 0x520, 192);
    Engine_ActorSetAnimation(0, 16);
    Engine_ActorSetAnimation(8, 9);
    Engine_EventWait(10);
    /* FAKEMATCH: base is 0 from the top of the function, so 0 - approach
     * + 1 is not folded into 1 - approach. */
    BattleFx_SetWeightedResult(72, base - approach + 1);
    state = (u8 *)&gGameState;
    /* FAKEMATCH: the do-while keeps the stage flag store ahead of the
     * scene's pool load. */
    do {
        state[0x22b] = 3;
    } while (0);
    Party_SetFields1ceAnd1d0((s32)&SceneId_KorosseoKawa, 4);
    Event_SetPair1d4((s32)&SceneId_KorosseoKawa, 5);
    GameFlag_Set(0x11a);
}
