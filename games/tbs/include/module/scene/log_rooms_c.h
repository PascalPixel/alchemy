#ifndef ALCHEMY_MODULE_MAP_LOG_ROOMS_C_H
#define ALCHEMY_MODULE_MAP_LOG_ROOMS_C_H

#include "types.h"
#include "scene_tables.h"

const struct SceneEntrance *LogRoomsC_GetEntrances(void);
const struct SceneExitRegion *LogRoomsC_GetRegions(void);
const u32 *LogRoomsC_GetExits(void);
const struct ActorPlacement *LogRoomsC_GetActorPlacements(void);
const struct Trigger *LogRoomsC_GetTriggers(void);
s32 LogRoomsC_Initialize(void);
extern struct SceneEntrance gLogRoomsC_Entrances[];
extern u32 gLogRoomsC_Exits[];
extern struct ActorPlacement gLogRoomsC_ActorPlacements[];
extern struct Trigger gLogRoomsC_Triggers[];

#endif
