#ifndef ALCHEMY_UNIT_FIELD_SORU_SEKIZO_SEKIZO_H
#define ALCHEMY_UNIT_FIELD_SORU_SEKIZO_SEKIZO_H

#include "types.h"
#include "field_scene.h"

u8 *SceneEventRuntime_GetScriptData(void);
s32 SceneEventRuntime_ReturnZero(void);
u8 *SceneEventRuntime_GetMessageData(void);
u8 *SceneEventRuntime_GetActorData(void);
u8 *SceneEventRuntime_GetEffectData(void);
s32 SceneEventRuntime_SelectInitialSceneByFlags(void);
void Scene_OpenTheHole(void);
void FieldScene_SetupStagedActors(void);
void FieldScene_RunStagedActorScene(void);
void SoruSekizo_RunSealOpenedSequence(void);
void SceneEffect_UpdateScrollingSpriteRows(void);
void SceneState_RunWhenSlotZeroFacingC000(void);
void SceneState_RunWhenActorZeroFacing4000(void);
void FieldScene_RunPrimarySequenceHead(void);
s32 Scene_RunGuardSequenceB(void);
void Scene_RunGuardSequenceC(void);
void FieldScene_CallWhenCheck9_31_9(void);
void FieldScene_RunGuardedStep11(void);
void FieldScene_RunGuardedStep13(void);
void FieldScene_RunGuardedStep15(void);
void ConfigureSceneAndCheckActors(void);
void ConfigureAlternateSceneAndCheckActors(void);
void FieldScene_RunClosingSequence(void);
void SoruSekizo_BlockLeftTriggerCell(void);
void SoruSekizo_BlockRightTriggerCell(void);
void SoruSekizo_RunStatueDropScene(void);
void FieldScene_RunScene37bSequenceA(void);
void FieldScene_RunFiveValueStep9(void);
void FieldScene_RunFiveValueStep11(void);
void FieldScene_ApplyRect13_31_12_30_12(void);
void FieldScene_RunFiveValueStep15(void);
void FieldScene_ApplyRect10_14_7_13_7(void);
void SceneActor_UseActorTenCellAndNext(void);
void SceneActor_MoveActor10ByRow(void);
void FieldScene_ApplyRect12_21_7_22_7(void);
void SceneActor_ApplyActorTwelveZCellPair(void);
void SceneActor_RunSlot12ColumnStep(void);
void SoruSekizo_RunEventSequence(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4);
s32 SceneActor_IsActorAtTile(s32 no, s32 x, s32 z);
void SoruSekizo_OpenNorthwestSeal(void);
void SoruSekizo_OpenNortheastSeal(void);
void SoruSekizo_OpenSouthwestSeal(void);
void SoruSekizo_OpenSoutheastSeal(void);
void SoruSekizo_OpenSeal(void);
void Scene_ShineLeftBeam(void);
void Scene_ShineRightBeam(void);
void Event_SayThenWait(s32 speaker, s32 frames);
extern s32 SoruSekizo_SpriteRowsGfx[];
extern struct SceneEntrance SceneEventRuntime_ScriptData[];
extern u32 SceneEventRuntime_MessageData[];
extern struct ScenePlacement SceneEventRuntime_ActorData[];
extern struct SceneEvent SceneEventRuntime_EffectData[];
extern s32 SoruSekizo_ScrollPhase;

#endif
