#ifndef ALCHEMY_UNIT_MAP_MCCOY_PALACE_H
#define ALCHEMY_UNIT_MAP_MCCOY_PALACE_H

#include "types.h"
#include "map_tables.h"

struct MapEvent;
struct ObjectEventTemplate;

const struct MapWarpDest *Scene_GetWarpDests(void);
const struct MapWarpRegion *Scene_GetWarpRegions(void);
const u32 *Scene_GetWarps(void);
const struct ObjectEventTemplate *Scene_GetObjectEventTemplates(void);
const struct MapEvent *Scene_GetEvents(void);
s32 Scene_Initialize(void);
extern s32 gPalace_FacingActions[];
extern s32 gPalace_PacingActions[];
extern struct MapWarpDest gMcCoyPalaceWarpDests[];
extern u32 gMcCoyPalaceWarps[];
extern struct ObjectEventTemplate gMcCoyPalaceNoObjectEventTemplates[];
extern struct ObjectEventTemplate gMcCoyPalaceObjectEventTemplates[];
extern struct MapEvent gMcCoyPalaceNoEvents[];
extern struct MapEvent gMcCoyPalaceEvents[];

#endif
