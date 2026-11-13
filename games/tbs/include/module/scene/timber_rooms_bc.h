#ifndef ALCHEMY_MODULE_SCENE_TIMBER_ROOMS_BC_H
#define ALCHEMY_MODULE_SCENE_TIMBER_ROOMS_BC_H

#include "types.h"
#include "scene_tables.h"
#include "scene.h"

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneExitRegion *Scene_GetExitRegions(void);
const u32 *Scene_GetExits(void);
const struct ActorPlacement *Scene_GetActorPlacements(void);
const struct Trigger *Scene_GetTriggers(void);
extern s32 gTimberRoomsBC_InnWorkerChores[];
extern struct SceneEntrance gTimberRoomsBC_Entrances[];
extern struct SceneExitRegion gTimberRoomsB_Regions[];
extern struct SceneExitRegion gTimberRoomsC_Regions[];
extern u32 gTimberRoomsBC_Exits[];
extern struct ActorPlacement gTimberRoomsB_ActorPlacements[];
extern struct ActorPlacement gTimberRoomsC_ActorPlacements[];
extern struct ActorPlacement gTimberRoomsC_ActorPlacementsAfterLighthouse[];
extern struct Trigger gTimberRoomsB_Triggers[];
extern struct Trigger gTimberRoomsB_TriggersAfterLighthouse[];
extern struct Trigger gTimberRoomsC_Triggers[];
extern struct Trigger gTimberRoomsC_TriggersAfterLighthouse[];

#endif
