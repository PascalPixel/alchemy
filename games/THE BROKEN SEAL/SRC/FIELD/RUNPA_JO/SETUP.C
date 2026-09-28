/* The Lunpa fortress: the scene tasks and the actors restored from the story
 * flags. */
#include "FORTRESS.H"

void FieldScene_InstallSceneTasks(void)
{
    FieldScene_ActivateThreeActorGroup();
    switch (gGameState.entrance) {
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
        gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
        Value2(Engine_TaskAddCallback, (s32)TriggerSceneStage95FromActor12, 3200);
        Value2(Engine_TaskAddCallback, (s32)FieldScene_RunScene3bfSequenceB, 3200);
        Value2(Engine_TaskAddCallback, (s32)FieldScene_RunScene3bfSequenceC, 3200);
        Map_SetWorkFlagBits9To11(0xe00);
        break;
    case 12:
    case 19:
        gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
        Map_SetWorkFlagBits9To11(0xc00);
        break;
    case 16:
    case 17:
    case 18:
        gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
        Value2(Engine_TaskAddCallback, (s32)FieldScene_UpdateActorEighteenInteraction, 3200);
        Value2(Engine_TaskAddCallback, (s32)TriggerScene41AtVillagePath, 3200);
        Task_Wait(1);
        Map_Redraw();
        Task_Wait(1);
        Map_CopyCells(101, 9, 10, 8, 110, 9);
        Map_SetWorkFlagBits9To11(0xe00);
        break;
    case 13:
    case 14:
    case 15:
        gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
        Value2(Engine_TaskAddCallback, (s32)FieldScene_RunScene3bfSequenceA, 3200);
        break;
    default:
        gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
        Map_SetWorkFlagBits9To11(0xe00);
        break;
    }
    Actor_SetChildValue(18, 1);
    Actor_SetChildValue(17, 1);
    Actor_SetChildValue(21, 1);
    Actor_SetChildValue(12, 1);
    Actor_SetChildValue(13, 1);
    Task_Wait(1);
}

void FieldScene_SetupActorsForScene(void)
{
    struct ObjectRuntime *actor;

    FieldScene_ActivateTwoActorGroup();
    Actor_SetChildValue(9, 1);
    Actor_SetChildValue(10, 1);
    Actor_SetChildValue(17, 1);
    if (GameFlag_IsSet(0x94c)) {
        Actor_SetPosition(15, 0, 0);
    }
    if (GameFlag_IsSet(0x949)) {
        Actor_SetPosition(11, 0, 0);
    }
    if (GameFlag_IsSet(0x94b)) {
        Actor_SetPosition(16, 0, 0);
    }
    if (GameFlag_IsSet(0xf2e)) {
        Actor_SetPosition(8, 0, 0);
    }
    switch (gGameState.entrance) {
    case 1:
    case 2:
    case 3:
        gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
        Map_SetWorkFlagBits9To11(0xe00);
        Engine_TaskAddCallback(FieldScene_UpdateActorPairInteraction, 3200);
        Task_Wait(1);
        Map_Redraw();
        Task_Wait(1);
        break;
    case 10:
    case 13:
    case 20:
    case 23:
    case 24:
        gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
        Map_SetWorkFlagBits9To11(0xc00);
        Actor_SetSpriteFlags(Object_GetById(24), 0);
        if (GameFlag_IsSet(0x314)) {
            Actor_SetPosition(25, 0x3680000, 0x780000);
        }
        break;
    case 21:
    case 22:
        gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
        Map_SetWorkFlagBits9To11(0xe00);
        Engine_TaskAddCallback(FieldScene_UpdateActorSeventeenInteraction, 3200);
        Task_Wait(1);
        Map_Redraw();
        Task_Wait(1);
        break;
    case 11:
    case 12:
        gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
        if (GameFlag_IsSet(0x94a)) {
            FieldScene_RunSequenceTail();
        }
        break;
    case 31:
        gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
        FieldScene_RunSequenceTail();
        break;
    case 14:
    case 15:
    case 16:
        Engine_TaskAddCallback(TriggerScene40AtVillagePath, 3200);
        break;
    default:
        gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
        Map_SetWorkFlagBits9To11(0xe00);
        break;
    }
    actor = Object_GetById(8);
    Actor_SetSpriteFlags(Object_GetById(8), 0);
    Actor_SetSpritePriority(8, 1);
    *(s32 *)&actor->unknown_18[0] = 0xc000;
    *(s32 *)&actor->unknown_18[4] = 0xc000;
}

void FieldScene_RestoreActorsFromFlags(void)
{
    struct ObjectRuntime *actor;

    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
    FieldScene_ActivateAlternateActorGroup();
    if (GameFlag_IsSet(0x943)) {
        PlaceActorTwelveAndFinishScene();
    }
    GameFlag_Set(0x217);
    GameFlag_Set(0x218);
    if (GameFlag_IsSet(0x944)) {
        Actor_SetPosition(8, 0, 0);
        GameFlag_Clear(0x217);
    }
    if (GameFlag_IsSet(0x945)) {
        Actor_SetPosition(9, 0, 0);
        ConfigureInteractionRegionC();
    }
    if (GameFlag_IsSet(0x946)) {
        Actor_SetPosition(10, 0, 0);
        GameFlag_Clear(0x218);
    }
    if (GameFlag_IsSet(0x947)) {
        ConfigureInteractionRegionA();
    }
    if (GameFlag_IsSet(0x948)) {
        ConfigureInteractionRegionB();
    }
    Event_Begin();
    actor = Object_GetById(8);
    if (actor != 0) {
        actor->unknown_23 = 2;
    }
    actor = Object_GetById(9);
    if (actor != 0) {
        actor->unknown_23 = 2;
    }
    actor = Object_GetById(10);
    if (actor != 0) {
        actor->unknown_23 = 2;
    }
    actor = Object_GetById(11);
    if (actor != 0) {
        Actor_SetSpriteFlags(actor, 0);
    }
    actor->unknown_23 = 2;
    actor = Object_GetById(12);
    if (actor != 0) {
        actor->unknown_56[3] |= 0x10;
    }
    Actor_SetSpriteFlags(Object_GetById(11), 0);
    Event_End();
    Map_SetWorkFlagBits9To11(0xe00);
}

void FieldScene_ActivateThreeActorGroup(void)
{
    if (GameFlag_IsSet(0x35a)) {
        PlaceSceneObjectPairFromTableC(0);
    }
    if (GameFlag_IsSet(0x35b)) {
        PlaceSceneObjectPairFromTableC(1);
    }
    if (GameFlag_IsSet(0x35c)) {
        PlaceSceneObjectPairFromTableC(2);
    }
}

void FieldScene_ActivateTwoActorGroup(void)
{
    if (GameFlag_IsSet(0x358)) {
        PlaceSceneObjectPairFromTableB(0);
    }
    if (GameFlag_IsSet(0x359)) {
        PlaceSceneObjectPairFromTableB(1);
    }
}

void FieldScene_ActivateAlternateActorGroup(void)
{
    if (GameFlag_IsSet(0x355)) {
        FieldScene_SetPositionPairs(0);
    }
    if (GameFlag_IsSet(0x356)) {
        FieldScene_SetPositionPairs(1);
    }
    if (GameFlag_IsSet(0x357)) {
        FieldScene_SetPositionPairs(2);
    }
}

void ActivateFiveActorGroupFromFlags(void)
{
    if (GameFlag_IsSet(0x350)) {
        PlaceSceneObjectPairFromTableA(0);
    }
    if (GameFlag_IsSet(0x351)) {
        PlaceSceneObjectPairFromTableA(1);
    }
    if (GameFlag_IsSet(0x352)) {
        PlaceSceneObjectPairFromTableA(2);
    }
    if (GameFlag_IsSet(0x353)) {
        PlaceSceneObjectPairFromTableA(3);
    }
    if (GameFlag_IsSet(0x354)) {
        PlaceSceneObjectPairFromTableA(4);
    }
}
