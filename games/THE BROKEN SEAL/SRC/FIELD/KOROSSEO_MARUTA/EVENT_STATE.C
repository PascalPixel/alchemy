/* The scene event's state hooks. */
#include "LOG_ROLLING.H"

void ColossoLogRollingStage_RunSceneEventIfReady(void)
{
    if (ColossoLogRollingStage_FindActorAhead() == 0) {
        Engine_LeaderCheckAhead();
    }
}

void ColossoLogRollingStage_FinishOrContinueSceneEvent(void)
{
    if (ColossoLogRollingStage_FindActorAhead() == 0) {
        Engine_LeaderCheckAhead();
    } else {
        ColossoLogRollingStage_RunSetupCompletionHooks();
    }
}

s32 ColossoLogRollingStage_GetSceneEventState(void)
{
    return (s32)gKorosseoMarutaEvents;
}
