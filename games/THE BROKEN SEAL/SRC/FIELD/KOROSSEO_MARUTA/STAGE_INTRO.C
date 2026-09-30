#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "COLOSSO_LOG_ROLLING_STAGE.H"
#include "CALL.H"
extern u8 MsgKorosseoOperatorWallsCheer[];
extern u8 MsgKorosseoPlaceCalledWall[];

void Korosseo_FinishSoloRound();
s32 Korosseo_FadeInCompetitor();
void Korosseo_RestoreCompetitor();
s32 FieldScene_RunMiddleSequence();

/* Colosso log stage: unless the stage is already cleared, show the
 * introduction, wait up to 240 frames for the log to settle and hand over to
 * the stage. */
void KorosseoMaruta_RunStageIntro(s32 a0)
{
    s32 p8;
    s32 rec8;

    if (gGameState.entrance == 2) {
        Korosseo_FinishSoloRound();
    } else {
        Engine_EventBegin();
        rec8 = ColossoLogRollingStage_RunStateInteraction(a0, 3);
        if (rec8 == 0) {
            p8 = *(s32 *)&gEventWork;
            Engine_EventSetMessage((s32)MsgKorosseoPlaceCalledWall);
            ColossoLogRollingStage_ResetAndRunSceneTask();
            Engine_CameraSetSpeed(0x30000, 0x6000);
            Engine_CameraMoveTo(0x2680000, -1, 0xb80000, 1);
            Engine_CameraWaitForMove();
            Engine_EventWait(30);
            Engine_EventShowMessage(a0, 0);
            ColossoLogRollingStage_StartSceneTask();
            Engine_EventWait(60);
            Engine_EventShowMessage(a0, 0);
            Korosseo_FadeInCompetitor(0, 0x1f8, 200);
            Value3(Engine_ActorFaceDirection, 0, 0, 0);
            ColossoLogRollingStage_WaitForSceneTask();
            Call3(Engine_ActorSetSpeed, 0, 0x18000, 0xc000);
            ColossoLogRollingStage_PositionScaledObject(0, 0x2a8, 200);
            if (*(s16 *)(p8 + 0x182) != 5) {
                do {
                    Engine_TaskWait(1);
                    if (++rec8 > 239) {
                        break;
                    }
                } while (*(s16 *)(p8 + 0x182) != 5);
            }
            ColossoLogRollingStage_ClampAndOffsetActiveActor();
            Call3(Engine_ActorFaceDirection, 0, 0xc000, 20);
            Call3(Engine_ActorShowEmote, 0, 0x103, 60);
            Engine_EventShowMessage(a0, 0);
            Korosseo_RestoreCompetitor(0);
            Engine_CameraFollowActor(0, 0);
            ColossoLogRollingStage_InitializeStateInteraction(a0, 3);
            {
                /* FAKEMATCH: the slot pointer and word temporary order the address before the zero */
                u8 *slot = (u8 *)(p8 + 0x182);
                s32 shown = 0;

                *(u16 *)slot = shown;
            }
        } else {
            if (rec8 == 1) {
                Engine_EventSetMessage((s32)MsgKorosseoOperatorWallsCheer);
                Engine_EventShowMessage(a0, 0);
            }
        }
        FieldScene_RunMiddleSequence(rec8, a0, 3);
        Engine_EventEnd();
    }
}
