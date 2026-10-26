#ifndef ALCHEMY_UNIT_MAP_STONE_ROOMS_H
#define ALCHEMY_UNIT_MAP_STONE_ROOMS_H

#include "types.h"
#include "map_tables.h"

struct MapWarpDest;
struct MapEvent;
struct ObjectEventTemplate;
struct MapWarpRegion;

const struct MapWarpDest *Scene_GetWarpDests(void);
const struct MapWarpRegion *Scene_GetWarpRegions(void);
const u32 *Scene_GetWarps(void);
const struct ObjectEventTemplate *Scene_GetObjectEventTemplates(void);
const struct MapEvent *Scene_GetEvents(void);
s32 Scene_Initialize(void);
extern const struct MapWarpDest gStoneRooms_WarpDests[];
extern const u32 gStoneRooms_Warps[];
extern const struct ObjectEventTemplate gStoneRooms_ThiefObjectEventTemplates[];
extern const struct ObjectEventTemplate
    gStoneRooms_PrisonerObjectEventTemplates[];
extern const struct MapEvent gStoneRooms_ThiefEvents[];
extern const struct MapEvent gStoneRooms_PrisonerEvents[];

#endif
