/* Land ho: the ensemble's seven actors drift in, then each walks its way
 * ashore while the lookout cheers, and the party leaves the mast for the
 * ship's cabin; the exit depends on which way the voyage went. */
#include "FUNE.H"

extern u8 MsgFuneLandHo[];

/* Each actor's walk ashore, where the overlay's data lies. */
extern u8 FuneHobashira_EnsembleWalk9[];
extern u8 FuneHobashira_EnsembleWalk10[];
extern u8 FuneHobashira_EnsembleWalk11[];
extern u8 FuneHobashira_EnsembleWalk12[];
extern u8 FuneHobashira_EnsembleWalk13[];
extern u8 FuneHobashira_EnsembleWalk14[];
extern u8 FuneHobashira_EnsembleWalk15[];

void OverlayObject_InitWithRandomFields(s32 object);
s32 SceneData_GetDifferenceOfPairSums(void);

void FieldScene_RunSevenActorEnsemble(void)
{
    Event_Begin();
    Actor_SetChildValue(ACTOR_PARTY_LEADER, 15);
    Actor_SetSpriteFlags(Engine_ActorGet(ACTOR_PARTY_LEADER), 0);
    Event_CallWithLastActiveObjectId((s32)FuneHobashira_EnsembleObjects);
    Task_Wait(1);
    OverlayObject_InitWithRandomFields(9);
    OverlayObject_InitWithRandomFields(10);
    OverlayObject_InitWithRandomFields(11);
    OverlayObject_InitWithRandomFields(12);
    OverlayObject_InitWithRandomFields(13);
    OverlayObject_InitWithRandomFields(14);
    OverlayObject_InitWithRandomFields(15);
    Actor_EnableActionCallback(8, FuneHobashira_LookoutActions);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 3);
    Event_OpenScreen();
    Event_WaitForScreen();
    Event_Wait(400);
    Actor_Stop(9);
    Actor_Stop(10);
    Actor_Stop(11);
    Actor_Stop(12);
    Actor_Stop(13);
    Actor_Stop(14);
    Actor_Stop(15);
    Actor_SetSpeed(9, 0x30000, 0x18000);
    Actor_SetSpeed(10, 0x30000, 0x18000);
    Actor_SetSpeed(11, 0x30000, 0x18000);
    Actor_SetSpeed(12, 0x30000, 0x18000);
    Actor_SetSpeed(13, 0x30000, 0x18000);
    Actor_SetSpeed(14, 0x30000, 0x18000);
    Actor_SetSpeed(15, 0x30000, 0x18000);
    Actor_EnableActionCallback(9, FuneHobashira_EnsembleWalk9);
    Actor_EnableActionCallback(10, FuneHobashira_EnsembleWalk10);
    Actor_EnableActionCallback(11, FuneHobashira_EnsembleWalk11);
    Actor_EnableActionCallback(12, FuneHobashira_EnsembleWalk12);
    Actor_EnableActionCallback(13, FuneHobashira_EnsembleWalk13);
    Actor_EnableActionCallback(14, FuneHobashira_EnsembleWalk14);
    Actor_EnableActionCallback(15, FuneHobashira_EnsembleWalk15);
    Event_Wait(40);
    Actor_StartRepeatedMotion(8, 3);
    Actor_SetAttachedEffect(8, 258);
    Event_Wait(120);
    Actor_StartRepeatedMotion(8, 1);
    Actor_ShowEmote(8, 256, 60);
    Actor_SetSpeed(8, 0x10000, 0x8000);
    Actor_WalkToAndWait(8, 164, 344);
    Actor_Jump(8, 4, 10);
    Actor_Jump(8, 6, 20);
    Event_SetMessage((s32)MsgFuneLandHo);
    Event_ShowMessageAndWait(8, 0, 20);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
    Event_CloseScreen();
    Event_WaitForScreen();
    gGameState.saved_scene = (s32)&SceneId_FuneHeya;
    gGameState.saved_entrance = 2;
    if (SceneData_GetDifferenceOfPairSums() == 11) {
        Event_RequestExit(15);
    } else {
        Event_RequestExit(14);
    }
    Event_End();
}
