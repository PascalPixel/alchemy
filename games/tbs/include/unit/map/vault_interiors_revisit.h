#ifndef ALCHEMY_UNIT_MAP_VAULT_INTERIORS_REVISIT_H
#define ALCHEMY_UNIT_MAP_VAULT_INTERIORS_REVISIT_H

#include "types.h"
#include "map_tables.h"

const struct MapWarpDest *Map_GetWarpDests(void);
const struct MapWarpRegion *Map_GetWarpRegions(void);
const u32 *Map_GetWarps(void);
const struct ObjectEventTemplate *Map_GetObjectEventTemplates(void);
const struct MapEvent *Map_GetEvents(void);
s32 Map_Initialize(void);
extern struct MapWarpDest gVaultInteriorsRevisit_WarpDests[];
extern u32 gVaultInteriorsRevisit_Warps[];
extern struct ObjectEventTemplate gVaultInteriorsRevisit_ObjectEventTemplates[];
extern struct MapEvent gVaultInteriorsRevisit_Events[];

#endif
