#ifndef ALCHEMY_UNIT_MAP_MCCOY_PALACE_GROUNDS_H
#define ALCHEMY_UNIT_MAP_MCCOY_PALACE_GROUNDS_H

#include "types.h"
#include "map_tables.h"

struct MapEvent;
struct ObjectEventTemplate;
struct MapWarpRegion;

const struct MapWarpDest *Scene_GetWarpDests(void);
const struct MapWarpRegion *Scene_GetWarpRegions(void);
const u32 *Scene_GetWarps(void);
const struct ObjectEventTemplate *Scene_GetObjectEventTemplates(void);
const struct MapEvent *Scene_GetEvents(void);
s32 Scene_Initialize(void);
extern u16 gPalaceGateOpenSteps[];
extern s32 gGuardWatchesParty[];
extern struct MapWarpDest gMcCoyPalaceGroundsWarpDests[];
extern u32 gMcCoyPalaceGroundsWarps[];
extern struct ObjectEventTemplate gMcCoyPalaceGroundsNoObjectEventTemplates[];
extern struct ObjectEventTemplate gMcCoyPalaceGroundsObjectEventTemplates[];
extern struct MapEvent gMcCoyPalaceGroundsNoEvents[];
extern struct MapEvent gMcCoyPalaceGroundsEvents[];

#endif
