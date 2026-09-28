#include "HAIDIA_BABI.H"

/* The Psynergy steps the scene's events run. */

void BattleEffect_CleanupSceneObjects(void);
void OverlayObject_UpdateOnFrameParity();
void OverlayObject_ApplyRandomSlotOnOddFrames();

void SceneState_SetValue140Mode0(void)
{
    Psynergy_Begin(0x8C, 0);
}

void FieldScene_RunSingleStep(void)
{
    BattleEffect_CleanupSceneObjects();
}

void FieldScene_RunActor8TwoStep(void)
{
    OverlayObject_UpdateOnFrameParity(Engine_ActorGet(8));
}

void FieldScene_RunStep17(void)
{
    OverlayObject_ApplyRandomSlotOnOddFrames(Engine_ActorGet(17));
}

void FieldScene_RunSixStepSequence17e4(void)
{
    Psynergy_Begin(0x94, 1);
    Psynergy_SetTarget(8, 0x11);
    Psynergy_RaiseHands();
    Psynergy_PlayEffect(1);
    Psynergy_LowerHands();
    BattleEffect_CleanupSceneObjects();
}
