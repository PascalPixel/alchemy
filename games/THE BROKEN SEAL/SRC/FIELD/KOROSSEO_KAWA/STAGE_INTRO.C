#include "TYPES.H"
#include "FIELD_EVENT.H"
extern u8 MsgKorosseoCallRockChallenge[];
extern u8 MsgKorosseoObjectiveStageClear[];

void Korosseo_FinishSoloRound();
void Engine_EventBegin();
s32 SceneDialogue_RunFlagGatedPromptInteraction();
void Engine_EventSetMessage();
void Engine_CameraSetSpeed();
void Engine_CameraMoveTo();
void Engine_CameraWaitForMove();
void Engine_EventShowMessage();
s32 Korosseo_FadeInCompetitor();
void Engine_ActorSetSpeed();
void Engine_ActorWalkToAndWait();
void Engine_EventWait();
void Engine_ActorShowEmote();
void Engine_ActorFaceDirection();
s32 OverlayObject_PlaceWithScale14000();
void StagedActor_PushActorAhead();
void OverlayObject_ResetMotionFields();
void Engine_ActorSetAnimation();
void Korosseo_RestoreCompetitor();
void Engine_CameraFollowActor();
void Engine_ActorSetPosition();
void SceneState_SendIdBySceneId();
s32 FieldScene_RunMiddleSequence();
void Engine_EventEnd();

/* Colosso river stage: unless the stage is already cleared, walk the player
 * through the introduction, push the pillars and hand over to the stage. */
void KorosseoKawa_RunStageIntro(s32 a0)
{
    s32 rec;
    s32 record;

    if (gGameState.entrance == 2) {
        Korosseo_FinishSoloRound();
    } else {
        Engine_EventBegin();
        rec = SceneDialogue_RunFlagGatedPromptInteraction(a0, 1);
        if (rec != 0) {
        } else {
            Event_SetMessage((s32)MsgKorosseoCallRockChallenge);
            Camera_SetSpeed(0x30000, 0x6000);
            Camera_MoveTo(0x1480000, -1, 0xa80000, 1);
            Engine_CameraWaitForMove();
            Engine_EventShowMessage(a0, 0);
            Korosseo_FadeInCompetitor(0, 0x118, 200);
            Actor_SetSpeed(0, 0x10000, 0x8000);
            Actor_WalkToAndWait(0, 0x168, 200);
            Engine_EventWait(30);
            Actor_ShowEmote(0, 0x102, 60);
            Engine_EventShowMessage(a0, 0);
            Actor_WalkToAndWait(0, 0x138, 200);
            Engine_EventWait(30);
            Actor_FaceDirection(0, 0xc000, 10);
            Actor_ShowEmote(0, 0x106, 60);
            Actor_SetSpeed(0, 0x18000, 0xc000);
            OverlayObject_PlaceWithScale14000(0, 0x128, 184);
            OverlayObject_PlaceWithScale14000(0, 0x128, 152);
            OverlayObject_PlaceWithScale14000(0, 0x138, 152);
            Actor_FaceDirection(0, 0x4000, 15);
            StagedActor_PushActorAhead();
            OverlayObject_ResetMotionFields(0);
            StagedActor_PushActorAhead();
            OverlayObject_ResetMotionFields(0);
            Actor_SetSpeed(0, 0x18000, 0xc000);
            Actor_WalkToAndWait(0, 0x130, 184);
            Engine_ActorWalkToAndWait(0, 0x128, 192);
            Engine_ActorWalkToAndWait(0, 0x128, 200);
            Engine_ActorFaceDirection(0, 0, 15);
            StagedActor_PushActorAhead();
            OverlayObject_ResetMotionFields(0);
            StagedActor_PushActorAhead();
            OverlayObject_ResetMotionFields(0);
            Engine_ActorSetAnimation(0, 1);
            Engine_EventShowMessage(a0, 0);
            Korosseo_RestoreCompetitor(0);
            Engine_CameraFollowActor(0, 0);
            Actor_SetPosition(9, 0x1380000, 0xa80000);
            SceneState_SendIdBySceneId(a0, 1);
            goto L_020013c2;
        }
        if (rec == 1) {
            Event_SetMessage((s32)MsgKorosseoObjectiveStageClear);
            Engine_EventShowMessage(a0, 0);
        }
        L_020013c2:;
        FieldScene_RunMiddleSequence(rec, a0, 1);
        Engine_EventEnd();
    }
}
