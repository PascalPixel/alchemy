#ifndef ALCHEMY_UNIT_MAP_LOG_ROOMS_C_H
#define ALCHEMY_UNIT_MAP_LOG_ROOMS_C_H

#include "types.h"
#include "map_tables.h"

const struct SceneEntrance *LogRoomsC_GetEntrances(void);
const struct SceneExitRegion *LogRoomsC_GetRegions(void);
const u32 *LogRoomsC_GetExits(void);
const struct ObjectEventTemplate *LogRoomsC_GetObjectEventTemplates(void);
const struct MapEvent *LogRoomsC_GetEvents(void);
s32 LogRoomsC_Initialize(void);
extern struct SceneEntrance gLogRoomsC_Entrances[];
extern u32 gLogRoomsC_Exits[];
extern struct ObjectEventTemplate gLogRoomsC_ObjectEventTemplates[];
extern struct MapEvent gLogRoomsC_Events[];

#endif
