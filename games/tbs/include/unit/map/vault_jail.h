#ifndef ALCHEMY_UNIT_MAP_VAULT_JAIL_H
#define ALCHEMY_UNIT_MAP_VAULT_JAIL_H

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
extern const struct MapWarpDest gVaultJail_WarpDests[];
extern const u32 gVaultJail_Warps[];
extern const struct ObjectEventTemplate gVaultJail_ThiefObjectEventTemplates[];
extern const struct ObjectEventTemplate
    gVaultJail_PrisonerObjectEventTemplates[];
extern const struct MapEvent gVaultJail_ThiefEvents[];
extern const struct MapEvent gVaultJail_PrisonerEvents[];

#endif
