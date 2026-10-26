#ifndef ALCHEMY_UNIT_MAP_LOG_ROOMS_B_H
#define ALCHEMY_UNIT_MAP_LOG_ROOMS_B_H

#include "types.h"
#include "map_tables.h"

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneExitRegion *Scene_GetExitRegions(void);
const u32 *Scene_GetExits(void);
const struct ObjectEventTemplate *Scene_GetObjectEventTemplates(void);
const struct MapEvent *Scene_GetEvents(void);
s32 Scene_Initialize(void);
extern struct SceneEntrance gLogRoomsB_Entrances[];
extern u32 gLogRoomsB_Exits[];
extern struct ObjectEventTemplate gLogRoomsB_ObjectEventTemplates[];
extern struct MapEvent gLogRoomsB_Events[];

#endif
