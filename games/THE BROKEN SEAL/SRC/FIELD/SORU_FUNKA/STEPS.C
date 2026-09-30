/* Small scene steps. */
#include "FUNKA.H"

void SceneState_SetValue140Mode0(void)
{
    Psynergy_Begin(140, 0);
}

void FieldScene_CallHelper6620(void)
{
    BattleEffect_CleanupSceneObjects();
}

void FieldScene_RunActor15TwoStep(void)
{
    SceneState_ForwardByRuntimeWordBits((s32)Object_GetById(15));
}
