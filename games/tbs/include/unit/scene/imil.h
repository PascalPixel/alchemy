#ifndef ALCHEMY_UNIT_SCENE_IMIL_H
#define ALCHEMY_UNIT_SCENE_IMIL_H

#include "types.h"
#include "field_scene.h"

struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;

const struct SceneEntrance *Imil_GetEntrances(void);
struct SceneRegion *Imil_GetRegions(void);
u32 *Imil_GetExits(void);
const struct ScenePlacement *Imil_GetPlacements(void);
const struct SceneEvent *Imil_GetEvents(void);
s32 Imil_Initialize(void);
extern s16 gImil_KeyHeadings[];
extern s16 gImil_SwayHeadings[];
extern s32 gImil_ColdVillagerScript[];
extern s32 gImil_ShiverScript[];
extern s32 gImil_CaretakerHurryScript[];
extern s32 gImil_LeaderStepAsideScript[];
extern s32 gImil_BobScript[];
extern s32 gImil_SparkScript[];
extern s32 gImil_MiaCheckRoomScript[];
extern s32 gImil_MiaRunOutScript[];
extern s32 gImil_FaceTargetScript[];
extern s32 gImil_FollowMiaOutScript[];
extern struct SceneEntrance gImil_TownEntrances[];
extern struct SceneEntrance gImil_InteriorEntrances[];
extern u32 gImil_Exits[];
extern struct ScenePlacement gImil_TownPlacements[];
extern struct ScenePlacement gImil_TownPlacementsCured[];
extern struct ScenePlacement gImil_InteriorPlacements[];
extern struct SceneEvent gImil_TownEvents[];
extern struct SceneEvent gImil_InteriorEvents[];
extern u16 gImil_DoorCellSteps5[];
extern u16 gImil_DoorCellSteps4[];
extern u16 gImil_DoorCellSteps1[];
extern u16 gImil_DoorCellSteps0[];
extern u16 gImil_DoorCellSteps2[];
extern u16 gImil_DoorCellSteps3[];
extern u16 *gImil_DoorCellSteps[];
extern s16 gImil_DoorCellPoints[][2];
extern u16 gImil_CellStepsA[];
extern u16 gImil_CellStepsB[];

#endif
