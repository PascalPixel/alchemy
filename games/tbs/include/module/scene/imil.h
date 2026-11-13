#ifndef ALCHEMY_MODULE_SCENE_IMIL_H
#define ALCHEMY_MODULE_SCENE_IMIL_H

#include "types.h"
#include "scene_tables.h"

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneExitRegion *Scene_GetExitRegions(void);
const u32 *Scene_GetExits(void);
const struct ActorPlacement *Scene_GetActorPlacements(void);
const struct Trigger *Scene_GetTriggers(void);
s32 Scene_Initialize(void);
extern s16 gImil_KeyHeadings[];
extern s16 gImil_SwayHeadings[];
extern s32 gImil_FreezingYoungManScript[];
extern s32 gImil_ShiverScript[];
extern s32 gImil_OldWomanHurryScript[];
extern s32 gImil_LeaderStepAsideScript[];
extern s32 gImil_BobScript[];
extern s32 gImil_SparkScript[];
extern s32 gImil_MiaCheckRoomScript[];
extern s32 gImil_MiaRunOutScript[];
extern s32 gImil_FaceTargetScript[];
extern s32 gImil_FollowMiaOutScript[];
extern struct SceneEntrance gImil_TownEntrances[];
extern struct SceneEntrance gImil_RoomEntrances[];
extern u32 gImil_Exits[];
extern struct ActorPlacement gImil_TownActorPlacements[];
extern struct ActorPlacement gImil_TownActorPlacementsCured[];
extern struct ActorPlacement gImil_RoomActorPlacements[];
extern struct Trigger gImil_TownTriggers[];
extern struct Trigger gImil_RoomTriggers[];
extern u16 gImil_SanctumDoorSteps[];
extern u16 gImil_RoomsEastDoorSteps[];
extern u16 gImil_InnDoorSteps[];
extern u16 gImil_HouseSouthwestDoorSteps[];
extern u16 gImil_HouseWestDoorSteps[];
extern u16 gImil_HouseNortheastDoorSteps[];
extern u16 *gImil_DoorCellSteps[];
extern s16 gImil_DoorCellPoints[][2];
extern u16 gImil_CellStepsA[];
extern u16 gImil_CellStepsB[];
/// The position (x, y, z) the unused arc and sway effects move around.
extern s32 gImil_ArcOrigin[];

#endif
