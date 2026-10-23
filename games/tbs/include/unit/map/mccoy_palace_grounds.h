#ifndef ALCHEMY_UNIT_MAP_MCCOY_PALACE_GROUNDS_H
#define ALCHEMY_UNIT_MAP_MCCOY_PALACE_GROUNDS_H

#include "types.h"
#include "map_tables.h"

struct MapEvent;
struct ObjectEventTemplate;
struct MapWarpRegion;

const struct MapWarpDest *Map_GetWarpDests(void);
const struct MapWarpRegion *Map_GetWarpRegions(void);
const u32 *Map_GetWarps(void);
const struct ObjectEventTemplate *Map_GetObjectEventTemplates(void);
const struct MapEvent *Map_GetEvents(void);
s32 Map_Initialize(void);
extern u16 gPalaceGateOpenSteps[];
extern s32 gGuardWatchesParty[];
extern struct MapWarpDest gMcCoyPalaceGroundsWarpDests[];
extern u32 gMcCoyPalaceGroundsWarps[];
extern struct ObjectEventTemplate gMcCoyPalaceGroundsNoObjectEventTemplates[];
extern struct ObjectEventTemplate gMcCoyPalaceGroundsObjectEventTemplates[];
extern struct MapEvent gMcCoyPalaceGroundsNoEvents[];
extern struct MapEvent gMcCoyPalaceGroundsEvents[];

#endif
