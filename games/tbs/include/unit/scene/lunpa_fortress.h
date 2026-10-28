#ifndef ALCHEMY_UNIT_MAP_LUNPA_FORTRESS_H
#define ALCHEMY_UNIT_MAP_LUNPA_FORTRESS_H

#include "types.h"
#include "scene_tables.h"

struct SceneEntrance;
struct SceneExitRegion;
struct Trigger;
struct ActorPlacement;

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneExitRegion *Scene_GetExitRegions(void);
const u32 *Scene_GetExits(void);
const struct ActorPlacement *Scene_GetActorPlacements(void);
const struct Trigger *Scene_GetTriggers(void);
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
extern struct ActorPlacement gLunpaFortressActorPlacementsOther[];
extern struct ActorPlacement gLunpaFortressUnusedWestLunpaCaveActorPlacements[];
extern struct ActorPlacement gLunpaFortressGroundFloorActorPlacements[];
extern struct ActorPlacement gLunpaFortressUpperFloorActorPlacements[];
extern struct ActorPlacement gLunpaFortressCaveActorPlacements[];
extern struct ActorPlacement gLunpaFortressToadonpaActorPlacements[];
extern struct Trigger gLunpaFortressNoTriggers[];
extern struct Trigger gLunpaFortressGroundFloorTriggers[];
extern struct Trigger gLunpaFortressUpperFloorTriggers[];
extern struct Trigger gLunpaFortressCaveTriggers[];
extern struct Trigger gLunpaFortressTriggersOther[];
extern u32 gLunpaFortressToadonpaDoorCells[];
extern u32 gLunpaFortressCaveDoorCells[];
extern u32 gLunpaFortressUpperFloorDoorCells[];
extern u32 gLunpaFortressGroundFloorDoorCells[];

#endif
