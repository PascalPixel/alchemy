#ifndef ALCHEMY_UNIT_MAP_LALIVERO_H
#define ALCHEMY_UNIT_MAP_LALIVERO_H

#include "types.h"
#include "map_tables.h"

struct MapWarpRegion;

const struct MapWarpDest *Lalivero_GetWarpDests(void);
const struct MapWarpRegion *Lalivero_GetRegions(void);
const u32 *Lalivero_GetWarps(void);
const struct ObjectEventTemplate *Lalivero_GetObjectEventTemplates(void);
const struct MapEvent *Lalivero_GetEvents(void);
s32 Lalivero_Initialize(void);
extern s32 gLalivero_CryingScript[];
extern struct MapWarpDest gLaliveroWarpDests[];
extern u32 gLaliveroWarps[];
extern struct ObjectEventTemplate gLaliveroObjectEventTemplates[];
extern struct ObjectEventTemplate
    gLaliveroObjectEventTemplatesAfterLighthouse[];
extern s16 gLaliveroDoorCells[][2];
extern u16 gLaliveroDoorOpenSteps[];
extern u16 gLaliveroGateOpenSteps[];
extern struct MapEvent gLaliveroEvents[];
extern struct MapEvent gLaliveroEventsAfterLighthouse[];

#endif
