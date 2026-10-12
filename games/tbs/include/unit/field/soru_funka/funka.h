#ifndef ALCHEMY_UNIT_FIELD_SORU_FUNKA_FUNKA_H
#define ALCHEMY_UNIT_FIELD_SORU_FUNKA_FUNKA_H

#include "types.h"
#include "field_scene.h"

union FieldObject;
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
void Effect_AnimateVerticalPositive(union FieldObject *object);
void Effect_AnimateVerticalNegative(union FieldObject *object);
void SoruFunka_SpawnEffectPair(union PairObject *parent);
void SceneState_SetValue140Mode0(void);
void SoruFunka_CleanupSceneObjects(void);
void FieldScene_RunActor15TwoStep(void);
extern s32 Funka_ArcOriginsHead[];
extern s32 Funka_ArcOrigins[];
extern struct SceneEntrance Placement_Scripts[];
extern u32 Placement_Messages[];
extern struct ScenePlacement Placement_Actors[];
extern struct SceneEvent Placement_Effects[];
extern u16 SoruFunka_StarCells[];
extern s32 Funka_EmberScript[];
extern u8 Soru_RingDrift[][3];
extern u8 Soru_RingSwing[][3];
extern s8 Soru_RingDirection[][3];
extern s32 Soru_RingOffsetX[];
extern s32 Soru_RingOffsetZ[];

#endif
