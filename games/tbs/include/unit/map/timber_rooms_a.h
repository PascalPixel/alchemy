#ifndef ALCHEMY_UNIT_MAP_TIMBER_ROOMS_A_H
#define ALCHEMY_UNIT_MAP_TIMBER_ROOMS_A_H

#include "types.h"
#include "map_tables.h"
#include "scene_header.h"

const struct MapWarpDest *Scene_GetWarpDests(void);
const struct MapWarpRegion *Scene_GetWarpRegions(void);
const u32 *Scene_GetWarps(void);
const struct ObjectEventTemplate *Scene_GetObjectEventTemplates(void);
const struct MapEvent *Scene_GetEvents(void);
s32 Scene_Initialize(void);
extern struct MapWarpDest gTimberRoomsAWarpDests[];
extern struct MapWarpRegion gTimberRoomsARegions[];
extern u32 gTimberRoomsAWarps[];
extern struct ObjectEventTemplate gTimberRoomsAObjectEventTemplates[];
extern struct ObjectEventTemplate
    gTimberRoomsAObjectEventTemplatesAfterAudience[];
extern struct MapEvent gTimberRoomsAEvents[];
extern struct MapEvent gTimberRoomsAEventsAfterAudience[];

#endif
