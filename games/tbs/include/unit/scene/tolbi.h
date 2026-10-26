#ifndef ALCHEMY_UNIT_MAP_TOLBI_H
#define ALCHEMY_UNIT_MAP_TOLBI_H

#include "types.h"
#include "script_layer.h"
#include "map_tables.h"

const struct MapWarpDest *Tolbi_GetWarpDests(void);
const struct MapWarpRegion *Tolbi_GetRegions(void);
const u32 *Tolbi_GetWarps(void);
const struct ObjectEventTemplate *Tolbi_GetObjectEventTemplates(void);
const struct MapEvent *Tolbi_SelectEvents(void);
s32 Tolbi_ApplyEntryState(s32 unused);
extern s32 gTolbiMirroredObjectEvent[];
extern s32 gTolbiOlderBrotherRun[];
extern s32 gTolbiYoungerSisterRun[];
extern struct MapWarpDest gTolbiWarpDests[];
extern u32 gTolbiWarps[];
extern struct ObjectEventTemplate gTolbiObjectEventTemplates[];
extern struct MapEvent gTolbiEventsFestival[];
extern struct MapEvent gTolbiEventsFinalsDay[];
extern struct MapEvent gTolbiEventsAfterColosso[];
extern u16 gTolbiDoorOpening[];
extern struct MapGridCellAnimation gTolbiDoors[];

#endif
