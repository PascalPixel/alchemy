#ifndef ALCHEMY_UNIT_MAP_LOG_ROOMS_C_H
#define ALCHEMY_UNIT_MAP_LOG_ROOMS_C_H

#include "types.h"
#include "scene_tables.h"

const struct SceneEntrance *LogRoomsC_GetEntrances(void);
const struct SceneExitRegion *LogRoomsC_GetRegions(void);
const u32 *LogRoomsC_GetExits(void);
const struct ActorTemplate *LogRoomsC_GetActorTemplates(void);
const struct Trigger *LogRoomsC_GetTriggers(void);
s32 LogRoomsC_Initialize(void);
extern struct SceneEntrance gLogRoomsC_Entrances[];
extern u32 gLogRoomsC_Exits[];
extern struct ActorTemplate gLogRoomsC_ActorTemplates[];
extern struct Trigger gLogRoomsC_Triggers[];

#endif
