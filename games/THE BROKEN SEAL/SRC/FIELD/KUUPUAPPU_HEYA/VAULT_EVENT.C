#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

extern u8 MsgKuupuappuWontGoAway[];

void SceneState_SetWord1c0To209AndRun(void);
void SceneActor_SetModeZeroAndValue(s32 actor, s32 frames);
void SceneActor_SetPairZeroAndValue(s32 actor, s32 other, s32 frames);
void SceneEffect_ApplyThreeValuesAndFinish(s32 actor, s32 animation, s32 frames);
void Party_SetFields1ceAnd1d0(s32 scene, s32 entrance);
void Event_SetPair1d4(s32 scene, s32 entrance);
void BattleFx_SetWeightedResult(s32 first, s32 second);

/* The Vault house scene in which the three villagers will not go away:
   they and the party are placed and turned, the screen opens, the three
   argue in turn, and the scene closes back into the same house, entrance
   17, with entrance 16 kept as the one to return to. */
void KuupuappuHeya_RunVaultEvent(void)
{
    u8 *state;

    Actor_SetPosition(10, 0x3180000, 0x1a00000);
    Actor_SetPosition(11, 0x3200000, 0x1900000);
    Actor_SetPosition(12, 0x3080000, 0x1a00000);
    Actor_FaceDirection(10, 0x3000, 0);
    Actor_FaceDirection(11, 0x3000, 0);
    Actor_FaceDirection(12, 0x3000, 0);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0x3180000, 0x1b80000);
    Actor_SetPosition(1, 0x3280000, 0x1b00000);
    Actor_SetPosition(2, 0x3180000, 0x1c80000);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Actor_FaceDirection(1, 0xb000, 0);
    Actor_FaceDirection(2, 0xb000, 0);
    Actor_FaceActor(8, 10, 0);
    gEventWork->start_transition = 0x209;
    Camera_FollowActor(0, 0);
    Camera_WaitForMove();
    Map_Redraw();
    Task_Wait(1);
    gEventWork->transition_frames = 32;
    SceneState_SetWord1c0To209AndRun();
    Event_Wait(60);
    Event_SetMessage((s32)MsgKuupuappuWontGoAway);
    Actor_RunRepeatedMotion(11, 1);
    Event_Wait(20);
    SceneActor_SetModeZeroAndValue(11, 30);
    Actor_RunRepeatedMotion(12, 1);
    Event_Wait(20);
    Event_ShowMessage(12, 0);
    SceneActor_SetPairZeroAndValue(10, 11, 30);
    Actor_SetAnimation(10, 3);
    SceneEffect_ApplyThreeValuesAndFinish(11, 3, 30);
    SceneActor_SetPairZeroAndValue(10, 12, 30);
    Actor_SetAnimation(10, 3);
    SceneEffect_ApplyThreeValuesAndFinish(12, 3, 40);
    Actor_FaceDirection(10, 0x3000, 0);
    Actor_FaceDirection(11, 0x3000, 0);
    Actor_FaceDirection(12, 0x3000, 0);
    Event_Wait(20);
    SceneEffect_ApplyThreeValuesAndFinish(10, 4, 20);
    Event_ShowMessage(10, 0);
    gEventWork->start_transition = 0x200;
    Party_SetFields1ceAnd1d0((s32)&SceneId_KuupuappuHeya, 17);
    Event_SetPair1d4((s32)&SceneId_KuupuappuHeya, 16);
    state = (u8 *)&gGameState;
    /* FAKEMATCH: the do-while puts the state's address in a block of its
       own, so it is loaded ahead of the byte's offset. */
    do {
        state[0x22b] = 3;
        BattleFx_SetWeightedResult(12, 5);
    } while (0);
}
