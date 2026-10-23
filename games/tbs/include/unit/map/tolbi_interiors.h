#ifndef ALCHEMY_UNIT_MAP_TOLBI_INTERIORS_H
#define ALCHEMY_UNIT_MAP_TOLBI_INTERIORS_H

#include "types.h"
#include "script_layer.h"
#include "map_tables.h"

const struct MapWarpDest *TolbiInteriors_GetWarpDests(void);
const struct MapWarpRegion *TolbiInteriors_GetRegions(void);
const u32 *TolbiInteriors_GetWarps(void);
const struct ObjectEventTemplate *
TolbiInteriors_SelectObjectEventTemplates(void);
const struct MapEvent *TolbiInteriors_SelectEvents(void);
s32 TolbiInteriors_ApplyEntryState(void);
extern s32 gTolbiInteriorsSquashStretch[];
extern struct MapWarpDest gTolbiInteriorsWarpDests[];
extern u32 gTolbiInteriorsWarps[];
extern struct ObjectEventTemplate gTolbiInteriorsObjectEventTemplates[];
extern struct ObjectEventTemplate
    gTolbiInteriorsObjectEventTemplatesAfterColosso[];
extern struct MapEvent gTolbiInteriorsEventsFestival[];
extern struct MapEvent gTolbiInteriorsEventsFinalsDay[];
extern struct MapEvent gTolbiInteriorsEventsAfterColosso[];
extern u16 gTolbiInteriorsDoorOpening[];
extern struct MapGridCellAnimation gTolbiInteriorsDoors[];

#endif
