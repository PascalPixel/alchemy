#ifndef ALCHEMY_UNIT_FIELD_BIRIBINO_MURA_MURA_H
#define ALCHEMY_UNIT_FIELD_BIRIBINO_MURA_MURA_H

#include "types.h"
#include "field_scene.h"

struct FieldActor;
struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;
struct SceneRegion;

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
void BiribinoMura_PushFacedBlock(void);
void BiribinoMura_UpdateCornerSpawn(void);
void OverlayObject_SpawnKind24AtActor(struct FieldActor *actor);
void FieldScene_DrawTilesByActor8Row(void);
extern s32 Mura_VillagerActions[];
extern u16 gBiribinoMuraEntrancesOther[];
extern struct SceneEntrance gBiribinoMuraEntrances1[];
extern struct SceneEntrance gBiribinoMuraEntrances3[];
extern struct SceneEntrance gBiribinoMuraEntrances2[];
extern s16 gBiribinoMuraRegions2[];
extern u32 Placement_Messages[];
extern struct ScenePlacement gBiribinoMuraPlacementsOther[];
extern struct ScenePlacement gBiribinoMuraPlacements1[];
extern struct ScenePlacement gBiribinoMuraPlacements3[];
extern struct ScenePlacement gBiribinoMuraPlacements2[];
extern struct SceneEvent gBiribinoMuraEventsOther[];
extern struct SceneEvent gBiribinoMuraEvents1[];
extern struct SceneEvent gBiribinoMuraEvents3[];
extern struct SceneEvent gBiribinoMuraEvents2[];
extern u16 Mura_DoorCellStep6[];
extern u16 Mura_DoorCellStep4[];
extern u16 Mura_DoorCellStep5[];
extern u16 Mura_DoorCellStep1[];
extern u16 Mura_DoorCellStep0[];
extern u16 Mura_DoorCellStep2[];
extern u16 Mura_DoorCellStep3[];
extern u16 *Mura_DoorCellSteps[];
extern u32 Mura_DoorCellOrigins[];
extern u32 Mura_RepaintCells[];
extern s16 BiribinoMura_FacingCellSteps[];
extern s32 Mura_SpawnScript[];

#endif
