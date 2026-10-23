#ifndef ALCHEMY_UNIT_SCENE_LUNPA_FORTRESS_H
#define ALCHEMY_UNIT_SCENE_LUNPA_FORTRESS_H

#include "types.h"
#include "map_tables.h"

struct SceneEntrance;
struct SceneRegion;
struct SceneEvent;
struct ScenePlacement;

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneRegion *Scene_GetRegions(void);
const u32 *Scene_GetExits(void);
const struct ScenePlacement *Scene_GetPlacements(void);
const struct SceneEvent *Scene_GetEvents(void);
s32 Scene_Initialize(void);
extern s32 gLunpaFortressRandomPick;
extern s32 gSecondPatrolRoute[];
extern s32 gFirstPatrolRoute[];
extern s32 gDonpaSleeps[];
extern s32 gGroundFloorPatrolRoute[];
extern s32 gHallPatrolRoute[];
extern struct SceneEntrance gLunpaFortressGroundFloorEntrances[];
extern struct SceneEntrance gLunpaFortressUpperFloorEntrances[];
extern struct SceneEntrance gLunpaFortressCaveEntrances[];
extern struct SceneEntrance gLunpaFortressEntrancesOther[];
extern u32 gLunpaFortressExitsOther[];
extern u32 gLunpaFortressUpperFloorExits[];
extern u32 gLunpaFortressCaveAndToadonpaExits[];
extern struct ScenePlacement gLunpaFortressPlacementsOther[];
extern struct ScenePlacement gLunpaFortressUnusedWestLunpaCavePlacements[];
extern struct ScenePlacement gLunpaFortressGroundFloorPlacements[];
extern struct ScenePlacement gLunpaFortressUpperFloorPlacements[];
extern struct ScenePlacement gLunpaFortressCavePlacements[];
extern struct ScenePlacement gLunpaFortressToadonpaPlacements[];
extern struct SceneEvent gLunpaFortressNoEvents[];
extern struct SceneEvent gLunpaFortressGroundFloorEvents[];
extern struct SceneEvent gLunpaFortressUpperFloorEvents[];
extern struct SceneEvent gLunpaFortressCaveEvents[];
extern struct SceneEvent gLunpaFortressEventsOther[];
extern u32 gLunpaFortressToadonpaDoorCells[];
extern u32 gLunpaFortressCaveDoorCells[];
extern u32 gLunpaFortressUpperFloorDoorCells[];
extern u32 gLunpaFortressGroundFloorDoorCells[];

#endif
