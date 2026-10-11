#ifndef ALCHEMY_UNIT_FIELD_BIRIBINO_MURA_MURA_H
#define ALCHEMY_UNIT_FIELD_BIRIBINO_MURA_MURA_H

#include "types.h"

struct FacingObject;
struct FieldActor;
struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;
struct SceneRegion;

s32 SceneActor_UpdateFacingTowardTarget(struct FacingObject *object);
const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneRegion *Scene_GetRegions(void);
u8 *SceneData_GetMessageTable(void);
void SceneState_SetValues9_3_0(void);
const struct ScenePlacement *Scene_GetPlacements(void);
void FieldScene_RunScriptedSteps947And29DD(void);
const struct SceneEvent *Scene_GetEvents(void);
void FieldScene_RunScriptedStep1472(void);
void FieldScene_RunScriptedStep146E(void);
void SceneDialogue_RunLine1470(void);
void BiribinoMura_TalkAboutHumanTree(void);
void SceneDialogue_RunActorTwelveDialogue(void);
void SceneDialogue_RunActorFourteenDialogue(void);
void SceneDialogue_ShowLine16BF(void);
void SceneDialogue_RunActorSixteenDialogue(void);
void SceneDialogue_ShowLine16CC(void);
void FieldScene_RunEarlySequence(void);
void FieldScene_RunScene38bSequenceC(void);
void BiribinoMura_CheckLeaderAhead(void);
void BiribinoMura_ReceiveJillsGift(void);
s32 Scene_Initialize(void);
void BiribinoMura_RestoreVillageActors(void);
void Scene_UpdatePuzzleActors(void);
void FieldScene_RunScene38bSequenceA(void);
void ActorPresentation_RepaintTenCellsAndActorEightCell(void);
void BiribinoMura_RestoreRaisedEntrance(void);
struct FieldActor *SceneActor_FindActorAtCell(s32 x, s32 z);
void BiribinoMura_PushFacedBlock(void);
void BiribinoMura_UpdateCornerSpawn(void);
void OverlayObject_SpawnKind24AtActor(struct FieldActor *actor);
void FieldScene_DrawTilesByActor8Row(void);

#endif
