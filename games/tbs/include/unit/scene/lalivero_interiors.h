#ifndef ALCHEMY_UNIT_MAP_LALIVERO_INTERIORS_H
#define ALCHEMY_UNIT_MAP_LALIVERO_INTERIORS_H

#include "types.h"
#include "map_tables.h"
#include "scene_header.h"

const struct SceneEntrance *LaliveroInteriors_GetEntrances(void);
const struct SceneExitRegion *LaliveroInteriors_GetRegions(void);
const u32 *LaliveroInteriors_GetExits(void);
const struct ObjectEventTemplate *
LaliveroInteriors_GetObjectEventTemplates(void);
const struct MapEvent *LaliveroInteriors_GetEvents(void);
s32 Scene_Initialize(void);
extern s32 gLaliveroInteriors_InnWorkerChores[];
extern struct SceneEntrance gLaliveroInteriors_Entrances[];
extern struct SceneExitRegion gTimberRoomsB_Regions[];
extern struct SceneExitRegion gTimberRoomsC_Regions[];
extern u32 gLaliveroInteriors_Exits[];
extern struct ObjectEventTemplate gTimberRoomsB_ObjectEventTemplates[];
extern struct ObjectEventTemplate gTimberRoomsC_ObjectEventTemplates[];
extern struct ObjectEventTemplate
    gTimberRoomsC_ObjectEventTemplatesAfterLighthouse[];
extern struct MapEvent gTimberRoomsB_Events[];
extern struct MapEvent gTimberRoomsB_EventsAfterLighthouse[];
extern struct MapEvent gTimberRoomsC_Events[];
extern struct MapEvent gTimberRoomsC_EventsAfterLighthouse[];

#endif
