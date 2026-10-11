#ifndef ALCHEMY_UNIT_FIELD_SORU_FUNKA_FUNKA_H
#define ALCHEMY_UNIT_FIELD_SORU_FUNKA_FUNKA_H

#include "types.h"

struct Actor_02002e0c;
struct Actor_02002e5c;
union PairObject;

u8 *SceneData_GetScriptTable(void);
s32 SceneData_ReturnZero(void);
u8 *SceneData_GetMessageTable(void);
u8 *SceneData_GetActorTable(void);
u8 *SceneData_GetEffectTable(void);
void Scene_SaturosTakesHostages(void);
void SoruFunka_RaiseAndShrinkActor(s32 a0);
void Resource381_NoOpCallbackA(void);
void Resource381_NoOpCallbackB(void);
s32 SoruFunka_RunHostageSceneOnEntranceTen(void);
void SoruFunka_ThrowEruptionRing(void);
void SceneState_InitStateWordsAndSlots(void);
void SoruFunka_StepEmbers(void);
void SceneState_UpdateRandomTimerLevel(void);
void Scene_RunExtendedEffectPresentation(void);
void SoruFunka_PlaceBeforeStar(s32 actor);
void SceneState_ApplyRectsByCondition(s32 a);
void SceneState_ApplyRectPairByFlag(s32 a);
void FieldScene_RunRandomHalfBranch(void);
void FieldScene_RunLateRandomHalfBranch(void);
void FieldScene_RunFourWayEffectSequence(u32 mode);
void SceneEffect_AdvanceTenEntryTimers(void);
void SceneActor_PlaceAtTileAndRunSteps(s32 a, s32 b);
void SceneState_ConfigureEightCornerRegions(void);
void FieldScene_RunVariantStep(s32 a, s32 b, s32 c);
void FieldScene_RunStepByRuntimeBits(s32 a);
void SceneState_ForwardByRuntimeWordBits(s32 a);
void SceneEffect_UpdateArcOverAnchor(struct Actor_02002e0c *self);
void SceneEffect_UpdateAnchoredRiseArc(struct Actor_02002e5c *obj);
void SoruFunka_SpawnEffectPair(union PairObject *parent);
void SceneState_SetValue140Mode0(void);
void SoruFunka_CleanupSceneObjects(void);
void FieldScene_RunActor15TwoStep(void);

#endif
