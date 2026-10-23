#ifndef ALCHEMY_UNIT_MAP_HAMMET_PALACE_H
#define ALCHEMY_UNIT_MAP_HAMMET_PALACE_H

#include "types.h"
#include "map_tables.h"

struct MapWarpDest;
struct MapEvent;
struct ObjectEventTemplate;
struct MapWarpRegion;

const struct MapWarpDest *Map_GetWarpDests(void);
const struct MapWarpRegion *Map_GetWarpRegions(void);
const u32 *Map_GetWarps(void);
const struct ObjectEventTemplate *Map_GetObjectEventTemplates(void);
const struct MapEvent *Map_GetEvents(void);
s32 Map_Initialize(void);
extern s32 gHammetPalace_PartyActions[];
extern struct MapWarpDest gHammetPalaceWarpDestsOther[];
extern struct MapWarpDest gHammetPalaceWarpDests[];
extern u32 gHammetPalaceWarps[];
extern struct ObjectEventTemplate gHammetPalaceObjectEventTemplatesOther[];
extern struct ObjectEventTemplate gHammetPalaceObjectEventTemplates[];
extern struct MapEvent gHammetPalaceEventsOther[];
extern struct MapEvent gHammetPalaceEvents[];

#endif
