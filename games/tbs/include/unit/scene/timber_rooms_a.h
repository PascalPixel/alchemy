#ifndef ALCHEMY_UNIT_MAP_TIMBER_ROOMS_A_H
#define ALCHEMY_UNIT_MAP_TIMBER_ROOMS_A_H

#include "types.h"
#include "map_tables.h"
#include "scene_header.h"

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneExitRegion *Scene_GetExitRegions(void);
const u32 *Scene_GetExits(void);
const struct ObjectEventTemplate *Scene_GetObjectEventTemplates(void);
const struct MapEvent *Scene_GetEvents(void);
s32 Scene_Initialize(void);
extern struct SceneEntrance gTimberRoomsAEntrances[];
extern struct SceneExitRegion gTimberRoomsARegions[];
extern u32 gTimberRoomsAExits[];
extern struct ObjectEventTemplate gTimberRoomsAObjectEventTemplates[];
extern struct ObjectEventTemplate
    gTimberRoomsAObjectEventTemplatesAfterAudience[];
extern struct MapEvent gTimberRoomsAEvents[];
extern struct MapEvent gTimberRoomsAEventsAfterAudience[];

#endif
