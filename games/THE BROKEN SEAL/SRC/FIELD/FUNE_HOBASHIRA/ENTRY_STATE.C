/* The mast on arrival: record the visit, start the deck's sway while the
 * voyage flags allow it, stand actor 8 by the rail, then run the scene the
 * entrance names. */
#include "FUNE.H"

void battle_owner_69(void);
void FuneHobashira_UpdateSway(void);
void FieldScene_RunScene3b0_0200040c(void);
void FieldScene_RunScene3b0_02000468(void);
void Scene_RunFourActorStagingSequence(void);
void FieldScene_RunActorNinePresentationCycles(void);
void FieldScene_RunPrimarySequence(void);
void FieldScene_RunSevenActorEnsemble(void);

s32 FuneHobashira_ApplyEntryState(void)
{
    struct FieldActor *leader;

    Engine_GameFlagSet(0x144);
    gEventWork->start_transition = 0x209;
    if ((Engine_GameFlagIsSet(0x927) != 0 || Engine_GameFlagIsSet(0x928) != 0)
        && Engine_GameFlagIsSet(0x93e) == 0 && Engine_GameFlagIsSet(0x8a0) == 0) {
        FuneHobashira_SwayX = (u16)Random16();
        FuneHobashira_SwayY = (u16)Random16();
        Engine_TaskAddCallback(FuneHobashira_UpdateSway, 0xc80);
    }
    if (Engine_GameFlagIsSet(0x925) != 0 && Engine_GameFlagIsSet(0x93e) == 0) {
        Actor_SetPosition(8, 0xa40000, 0x1480000);
    }
    switch (gGameState.entrance) {
    case 1:
        if (Engine_GameFlagIsSet(0x109) == 0) {
            leader = Engine_ActorGet(ACTOR_PARTY_LEADER);
            Engine_EventBegin();
            battle_owner_69();
            leader->y.fixed = 0x380000;
            Camera_MoveTo(-1, -1, -1, 0);
            Engine_TaskWait(1);
            Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 0);
            Engine_MapRedraw();
            Engine_TaskWait(1);
            Engine_EventEnd();
        }
        break;
    case 10:
        if (Engine_GameFlagIsSet(0x928) != 0) {
            FieldScene_RunScene3b0_02000468();
        } else {
            FieldScene_RunScene3b0_0200040c();
        }
        break;
    case 11:
        gGameState.saved_scene = (s32)&SceneId_FuneHeya;
        gGameState.saved_entrance = 30;
        Scene_RunFourActorStagingSequence();
        break;
    case 12:
        gGameState.saved_scene = (s32)&SceneId_FuneHeya;
        gGameState.saved_entrance = 30;
        FieldScene_RunActorNinePresentationCycles();
        break;
    case 13:
        gGameState.saved_scene = (s32)&SceneId_FuneHeya;
        gGameState.saved_entrance = 30;
        FieldScene_RunPrimarySequence();
        break;
    case 14:
        FieldScene_RunSevenActorEnsemble();
        break;
    }
    return 0;
}
