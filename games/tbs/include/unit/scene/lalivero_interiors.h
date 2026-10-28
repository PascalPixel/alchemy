#ifndef ALCHEMY_UNIT_MAP_LALIVERO_INTERIORS_H
#define ALCHEMY_UNIT_MAP_LALIVERO_INTERIORS_H

#include "types.h"
#include "scene_tables.h"
#include "scene.h"

const struct SceneEntrance *LaliveroInteriors_GetEntrances(void);
const struct SceneExitRegion *LaliveroInteriors_GetRegions(void);
const u32 *LaliveroInteriors_GetExits(void);
const struct ActorPlacement *LaliveroInteriors_GetActorPlacements(void);
const struct Trigger *LaliveroInteriors_GetTriggers(void);
s32 Scene_Initialize(void);
extern s32 gLaliveroInteriors_InnWorkerChores[];
extern struct SceneEntrance gLaliveroInteriors_Entrances[];
extern struct SceneExitRegion gTimberRoomsB_Regions[];
extern struct SceneExitRegion gTimberRoomsC_Regions[];
extern u32 gLaliveroInteriors_Exits[];
extern struct ActorPlacement gTimberRoomsB_ActorPlacements[];
extern struct ActorPlacement gTimberRoomsC_ActorPlacements[];
extern struct ActorPlacement gTimberRoomsC_ActorPlacementsAfterLighthouse[];
extern struct Trigger gTimberRoomsB_Triggers[];
extern struct Trigger gTimberRoomsB_TriggersAfterLighthouse[];
extern struct Trigger gTimberRoomsC_Triggers[];
extern struct Trigger gTimberRoomsC_TriggersAfterLighthouse[];

#endif
