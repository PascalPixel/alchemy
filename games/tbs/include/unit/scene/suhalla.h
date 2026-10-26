#ifndef ALCHEMY_UNIT_MAP_SUHALLA_H
#define ALCHEMY_UNIT_MAP_SUHALLA_H

#include "types.h"
#include "map_tables.h"

struct MapWarpRegion;

const struct MapWarpDest *Scene_GetWarpDests(void);
const struct MapWarpRegion *Scene_GetWarpRegions(void);
const u32 *Scene_GetWarps(void);
const struct ObjectEventTemplate *Scene_GetObjectEventTemplates(void);
const struct MapEvent *Scene_GetEvents(void);
s32 Scene_Initialize(void);
extern struct MapWarpDest gSuhallaWarpDests[];
extern u32 gSuhallaWarps[];
extern struct ObjectEventTemplate gSuhallaObjectEventTemplates[];
extern struct ObjectEventTemplate gSuhallaObjectEventTemplatesLater[];
extern s16 gSuhallaDoorCells[][2];
extern u16 gSuhallaDoorOpenSteps[];
extern u16 gSuhallaWideDoorOpenSteps[];
extern struct MapEvent gSuhallaEvents[];
extern struct MapEvent gSuhallaEventsLater[];

#endif
