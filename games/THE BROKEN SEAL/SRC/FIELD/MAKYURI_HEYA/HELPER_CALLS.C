#include "PROBE.H"

void FieldScene_CallHelper67e8(void)
{
    Makyuri_TickSpawnTimer();
}

void FieldScene_Forward646c(void)
{
    SceneState_ClearCurrentRecordAndReleaseTarget();
}

void FieldScene_RunSingleStep(void)
{
    Makyuri_RunActorMove();
}

void FieldScene_CallHelper6364(void)
{
    MakyuriIriguchi_RaisePillar();
}

void SceneDialogue_RunLine1637(void)
{
    Event_Begin();
    Message_ShowCentered(MSG_STRANGE_FORCES_AT_WORK_SEEMS, 1);
    Event_End();
}
