/* NONMATCHING: resource_3b0 0x02008e78, FieldScene_RunSevenActorEnsemble, from
 * FIELD/FUNE_HOBASHIRA/OBJECT_SCALE.C (2026-09-28).
 * Stores scene 0x6f into the game state from a literal load (a link-time
 * value; a plain constant becomes movs #0x6f). Remaining: the scene id. */
#include "FUNE.H"
extern u8 MsgFuneLandHo[];

void FieldScene_RunSevenActorEnsemble(void)
{
    s32 ensemble;
    s32 selector;

    Event_Begin();
    Actor_SetChildValue(ACTOR_PARTY_LEADER, 15);
    ensemble = Func_020020be(0);
    Actor_SetSpriteFlags(ensemble, 0);
    Call1(Func_020020ba, (s32)FuneHobashira_EnsembleObjects);
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
    Actor_SetSpeed(9, 196608, 98304);
    Actor_SetSpeed(10, 196608, 98304);
    Actor_SetSpeed(11, 196608, 98304);
    Actor_SetSpeed(12, 196608, 98304);
    Actor_SetSpeed(13, 196608, 98304);
    Actor_SetSpeed(14, 196608, 98304);
    Actor_SetSpeed(15, 196608, 98304);
    Value2(Engine_ActorEnableActionCallback, 9, 33592400);
    Value2(Engine_ActorEnableActionCallback, 10, 33592448);
    Value2(Engine_ActorEnableActionCallback, 11, 33592496);
    Value2(Engine_ActorEnableActionCallback, 12, 33592544);
    Value2(Engine_ActorEnableActionCallback, 13, 33592592);
    Value2(Engine_ActorEnableActionCallback, 14, 33592640);
    Value2(Engine_ActorEnableActionCallback, 15, 33592688);
    Event_Wait(40);
    Actor_StartRepeatedMotion(8, 3);
    Actor_SetAttachedEffect(8, 258);
    Event_Wait(120);
    Actor_StartRepeatedMotion(8, 1);
    Actor_ShowEmote(8, 256, 60);
    Actor_SetSpeed(8, 65536, 32768);
    Actor_WalkToAndWait(8, 164, 344);
    Actor_Jump(8, 4, 10);
    Actor_Jump(8, 6, 20);
    Event_SetMessage((s32)MsgFuneLandHo);
    Event_ShowMessageAndWait(8, 0, 20);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
    Event_CloseScreen();
    Event_WaitForScreen();
    Data_02000240[226] = (s32)&Value_0000006f;
    Data_02000240[227] = 2;
    selector = SceneData_GetDifferenceOfPairSums();
    if (selector == 11) {
        Event_RequestExit(15);
    } else {
        Event_RequestExit(14);
    }
    Event_End();
}
