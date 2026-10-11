#ifndef ALCHEMY_UNIT_FIELD_ARUTIN_MURA_MURA_H
#define ALCHEMY_UNIT_FIELD_ARUTIN_MURA_MURA_H

#include "types.h"
#include "field_event.h"
#include "field_scene.h"

struct Actor02000c0c;
struct SceneEntrance;
struct SceneEvent;
struct SceneMotion;
struct ScenePlacement;

const struct SceneEntrance *Scene_GetEntrances(void);
s32 SceneData_ReturnZero(void);
u8 *SceneData_GetMessageTable(void);
const struct ScenePlacement *Scene_GetPlacements(void);
const struct SceneEvent *Scene_GetEvents(void);
void FieldScene_RunActorEightPromptDialogue(void);
void SceneDialogue_ShowLine1918(void);
void FieldScene_RunOpeningAuxiliarySequence(void);
void SceneDialogue_RunActor17Message1924(void);
void SceneDialogue_RunActor9Message1932(void);
void SceneDialogue_RunActor10Message18d9(void);
void SceneDialogue_RunActor14Message18e1(void);
void SceneDialogue_RunActor21Message194a(void);
s32 SceneActor_IsSlotZeroAngleInRange(void);
void FieldScene_RunActorFifteenFlagBranch(void);
void FieldScene_RunActorTwentyFlagBranch(void);
void FieldScene_RunActorTwentyOneFlagBranch(void);
void FieldScene_RunFacingGatedDialogue18(void);
void FieldScene_RunEarlySequence(void);
void FieldScene_RunScene3a3SequenceB(void);
void SceneMotion_UpdateTimedActor(struct SceneMotion *work);
void FieldScene_RunScene3a3SequenceC(void);
void SceneState_SetFlag906ByActorNineteenX(void);
s32 Scene_Initialize(void);
void FieldScene_RunMiddleSequence(void);
void FieldScene_RunScene3a3SequenceD(void);
void SceneActor_ResetStateAndSpan(struct Actor02000c0c *actor);
void SceneEffect_UpdateDriftingParticle(struct SceneMotion *work);
void SceneEffect_SpawnDriftingParticle(void);
void ArutinMura_RunDriftEndScene(void);
extern s32 gArutinMuraPlacements2_Data[];
extern struct SceneEntrance gArutinMuraEntrancesOther[];
extern struct SceneEntrance gArutinMuraEntrances1[];
extern struct SceneEntrance gArutinMuraEntrances2[];
extern u32 Placement_Messages[];
extern struct ScenePlacement gArutinMuraPlacementsOther[];
extern struct ScenePlacement gArutinMuraPlacements1[];
extern struct ScenePlacement gArutinMuraPlacements2[];
extern struct SceneEvent gArutinMuraEventsOther[];
extern struct SceneEvent gArutinMuraEvents1[];
extern struct SceneEvent gArutinMuraEvents2[];
extern u16 ArutinMura_TriggerTable1[];
extern u16 ArutinMura_TriggerTable0[];
extern u16 ArutinMura_TriggerTable5[];
extern u16 ArutinMura_TriggerTable6[];
extern u16 ArutinMura_TriggerTable2[];
extern u16 ArutinMura_TriggerTable3[];
extern u16 ArutinMura_TriggerTable4[];
extern struct MapCellAnim ArutinMura_TriggerTable[];

#endif
