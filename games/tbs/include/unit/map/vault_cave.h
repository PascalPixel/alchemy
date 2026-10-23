#ifndef ALCHEMY_UNIT_MAP_VAULT_CAVE_H
#define ALCHEMY_UNIT_MAP_VAULT_CAVE_H

#include "types.h"
#include "map_tables.h"
#include "map_header.h"

struct MapWarpDest;
struct MapEvent;
struct ObjectEventTemplate;

const struct MapWarpDest *Map_GetWarpDests(void);
const struct MapWarpRegion *Map_GetWarpRegions(void);
const u32 *Map_GetWarps(void);
const struct ObjectEventTemplate *Map_GetObjectEventTemplates(void);
const struct MapEvent *Map_GetEvents(void);
s32 Map_Initialize(void);
extern const s16 gVaultCave_DirectionSteps[];
extern const s32 gVaultCave_PuffScript[];
extern struct MapWarpDest gVaultCave_FallbackWarpDests[];
extern struct MapWarpDest gVaultCavePassage_WarpDests[];
extern struct MapWarpDest gVaultCaveVambrace_WarpDests[];
extern struct MapWarpDest gVaultCaveMouth_WarpDests[];
extern struct MapWarpRegion gVaultCave_Regions[];
extern u32 gVaultCave_FallbackWarps[];
extern u32 gVaultCavePassage_Warps[];
extern u32 gVaultCaveVambrace_Warps[];
extern u32 gVaultCaveMouth_Warps[];
extern struct ObjectEventTemplate gVaultCave_FallbackObjectEventTemplates[];
extern struct ObjectEventTemplate gVaultCavePassage_ObjectEventTemplates[];
extern struct ObjectEventTemplate gVaultCaveVambrace_ObjectEventTemplates[];
extern struct ObjectEventTemplate gVaultCaveMouth_ObjectEventTemplates[];
extern struct MapEvent gVaultCave_FallbackEvents[];
extern struct MapEvent gVaultCavePassage_Events[];
extern struct MapEvent gVaultCaveVambrace_Events[];
extern struct MapEvent gVaultCaveMouth_Events[];
extern u32 gVaultCave_ScheduleFrames[];
extern s32 gVaultCave_ScheduleTimer;
extern s32 gVaultCave_ScheduleIndex;
extern s32 gVaultCave_TickCounter;
extern s32 gVaultCave_TickValue;

#endif
