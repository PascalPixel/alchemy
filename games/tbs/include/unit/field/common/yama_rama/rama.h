#ifndef ALCHEMY_UNIT_FIELD_COMMON_YAMA_RAMA_RAMA_H
#define ALCHEMY_UNIT_FIELD_COMMON_YAMA_RAMA_RAMA_H

#include "types.h"

struct FieldActor;
struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;
struct SceneRegion;

s32 EventScript_PrepareActorRenderFlags(struct FieldActor *actor);
s32 OverlayObject_SetFacingTowardObject10(void *self);
const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneRegion *Scene_GetRegions(void);
const u32 *Scene_GetExits(void);
const struct ScenePlacement *Scene_GetPlacements(void);
void SceneDialogue_RunMessage1958Step(void);
void SceneDialogue_RunActor11Message195d(void);
void SceneDialogue_RunActor13Message1961(void);
void FieldScene_RunPrimaryScript(void);
void FieldScene_RunScene3a2SequenceA(void);
void SceneDialogue_RunLine1956(void);
void ConfigureAndPlaceActorFourteen(void);
void YamaRama_ClearActorPlacement(void);
void SceneDialogue_RunActorFifteenByLeaderHeading(void);
void Scene_RunEventTransition(void);
void Scene_RunActorCue(void);
void Scene_RunActorExchange(void);
void Scene_RunActorSequence(void);
void FieldScene_RunScriptedSteps0And1A12(void);
void FieldScene_RunPairedLayoutStepsThenSetOne(void);
void SceneState_RunRect6x28Step(void);
s32 SceneActor_SetFlagBitByRankAgainstActorZero(struct FieldActor *actor);
void SceneActor_UpdateActorFourteenByDepth(void);
void ActorPresentation_PrepareActorFourteenWithCallback(void);
void YamaRama_DisableActorCollision(void);
const struct SceneEvent *Scene_GetEvents(void);
s32 YamaRama_ApplyEntryState(void);

#endif
