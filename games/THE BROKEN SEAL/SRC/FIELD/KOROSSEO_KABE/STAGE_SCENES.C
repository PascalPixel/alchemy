#include "TASK.H"

void FieldScene_RunSecondActorInteraction(s32 a0)
{
    u32 i;
    s32 rec;
    s32 record;

    if (gGameState.entrance == 2) {
        Korosseo_FinishSoloRound();
    } else {
        Event_Begin();
        rec = Value2(KorosseoKabe_RunStateInteraction, a0, 2);
        if (rec == 0) {
            Event_SetMessage(MSG_SHIFTING_FLOOR_STAGE);
            FieldScene_PlaceSpectatorRow();
            Camera_SetSpeed(0x30000, 0x6000);
            Camera_MoveTo(0x3d80000, -1, 0xe80000, 1);
            Camera_WaitForMove();
            Value2(Engine_EventShowMessage, a0, 0);
            SceneState_ApplyTable8715AndValue104();
            Event_ShowMessage(a0, 0);
            Korosseo_FadeInCompetitor(0, 0x438, 0x108);
            Event_Wait(15);
            Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x18000, 0xc000);
            Value3(SceneActor_PlaceWithScale14000, 0, 0x438, 216);
            Value3(SceneActor_PlaceWithScale14000, 0, 0x428, 216);
            SceneState_WaitForStatusWords();
            Leader_CheckAhead();
            Camera_MoveTo(-1, -1, -1, 0);
            Event_ShowMessage(a0, 0);
            Korosseo_RestoreCompetitor(0);
            Camera_FollowActor(ACTOR_PARTY_LEADER, 0);
            KorosseoKabe_ShowFollowUpPrompt(a0, 2);
        } else {
            if (rec == 1) {
                Event_SetMessage(MSG_OPERATOR_LIFTS_WILL_CHEER_FOR);
                Event_ShowMessage(a0, 0);
            }
        }
        Value3(FieldScene_RunMiddleSequence, rec, a0, 2);
        ((void (*)())Engine_EventEnd)();
    }
}

void FieldScene_RunSceneThreeCoordinator(s32 a0)
{
    u32 i;
    s32 rec2;
    s32 record;

    if (gGameState.entrance == 2) {
        Korosseo_FinishSoloRound();
    } else {
        Event_Begin();
        rec2 = Value2(KorosseoKabe_RunStateInteraction, a0, 3);
        if (rec2 != 0) {
        } else {
            Event_SetMessage(MSG_LOG_ROLLING_STAGE);
            Camera_SetSpeed(0x30000, 0x6000);
            Camera_MoveTo(0x2f00000, -1, 0xc00000, 1);
            Camera_WaitForMove();
            Event_Wait(60);
            Camera_SetSpeed(0x10000, 0x2000);
            Camera_MoveTo(0x2f00000, -1, 0xe00000, 1);
            Camera_WaitForMove();
            Event_ShowMessage(a0, 0);
            Korosseo_FadeInCompetitor(0, 0x358, 0x108);
            Event_Wait(10);
            Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x18000, 0xc000);
            Value3(SceneActor_PlaceWithScale14000, 0, 0x358, 0x108);
            Value3(SceneActor_PlaceWithScale14000, 0, 0x358, 232);
            Event_ShowMessage(a0, 0);
            Call3(SceneActor_PlaceWithScale14000, 0, 0x348, 232);
            Event_Wait(10);
            Value3(SceneActor_MovePairByTileOffset, 33, -64, 0);
            Camera_MoveTo(0x2f00000, -1, 0xd80000, 1);
            Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
            Event_Wait(10);
            Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
            Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x2f8, 232);
            Event_Wait(10);
            Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 30);
            Event_ShowMessage(a0, 0);
            Korosseo_RestoreCompetitor(0);
            Camera_FollowActor(ACTOR_PARTY_LEADER, 0);
            Actor_SetPosition(33, 0x3480000, 0xe80000);
            KorosseoKabe_ShowFollowUpPrompt(a0, 3);
            goto L_020016b0;
        }
        if (rec2 == 1) {
            Event_SetMessage(MSG_HERE_YOUR_OBJECTIVE_RIDE_LOGS);
            Event_ShowMessage(a0, 0);
        }
        L_020016b0:;
        Value3(FieldScene_RunMiddleSequence, rec2, a0, 3);
        Event_End();
    }
}
