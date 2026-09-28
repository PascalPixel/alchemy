/* Small scene steps. */
#include "CHOJO.H"

void SceneState_ApplyPair140And0(void)
{
    Psynergy_Begin(140, 0);
}

void FieldScene_ForwardValue81fc(s32 a)
{
    BattleEffect_CleanupSceneObjects(a);
}

void FieldScene_RunStep6(void)
{
    SceneState_ForwardByRuntimeWordBits((s32)Actor_Get(6));
}
