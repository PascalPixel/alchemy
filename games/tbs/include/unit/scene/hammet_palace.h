#ifndef ALCHEMY_UNIT_MAP_HAMMET_PALACE_H
#define ALCHEMY_UNIT_MAP_HAMMET_PALACE_H

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
extern s32 gHammetPalace_PartyActions[];
extern struct MapWarpDest gHammetPalaceWarpDestsOther[];
extern struct MapWarpDest gHammetPalaceWarpDests[];
extern u32 gHammetPalaceWarps[];
extern struct ObjectEventTemplate gHammetPalaceObjectEventTemplatesOther[];
extern struct ObjectEventTemplate gHammetPalaceObjectEventTemplates[];
extern struct MapEvent gHammetPalaceEventsOther[];
extern struct MapEvent gHammetPalaceEvents[];

#endif
