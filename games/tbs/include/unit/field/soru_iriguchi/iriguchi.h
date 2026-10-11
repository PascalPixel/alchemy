#ifndef ALCHEMY_UNIT_FIELD_SORU_IRIGUCHI_IRIGUCHI_H
#define ALCHEMY_UNIT_FIELD_SORU_IRIGUCHI_IRIGUCHI_H

#include "types.h"

struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;
struct SceneRegion;

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneRegion *Scene_GetRegions(void);
const u32 *Scene_GetExits(void);
const struct ScenePlacement *Scene_GetPlacements(void);
const struct SceneEvent *Scene_GetEvents(void);
void FieldScene_RunScene37fSequenceA(void);
void SceneDialogue_RunFlag81aMessageBranch(void);
void FieldScene_RunOpeningAuxiliarySequence(void);
void FieldScene_RunFlag821Dialogue(void);
void SoruIriguchi_SetSmallGem(void);
void Scene_UpdateOuterActor9Flags(void);
void Scene_UpdateOuterActor10Flags(void);
void Scene_UpdateFormationActor9Flags(void);
void Scene_UpdateFormationActor10Flags(void);
void Scene_UpdateFormationActor11Flags(void);
void Scene_UpdateFormationActor12Flags(void);
void Scene_UpdateFormationActor13Flags(void);
void Scene_UpdateFormationActor14Flags(void);
s32 *SceneActor_FindSlotByTilePosition(s32 x, s32 z);
void SoruIriguchi_PushFacedBlock(void);
s32 Scene_Initialize(void);
void SoruIriguchi_ApplyEntryState(void);
void FieldScene_RunSceneEntryHook(void);
void Scene_EnterSolSanctum(void);
void Scene_SukuretaSuspectsHiddenPassage(void);
void Scene_RunTransitionCue(void);
void Scene_RunActorFormation(s32 a0);
void FieldScene_RunScriptedStep953(void);
void Scene_UpdateCueTimer(s32 a0, s32 a1, s32 a2);

#endif
