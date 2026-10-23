#ifndef ALCHEMY_UNIT_MAP_GOMA_CAVE_ENTRANCE_H
#define ALCHEMY_UNIT_MAP_GOMA_CAVE_ENTRANCE_H

#include "types.h"
#include "map_tables.h"

const struct MapWarpDest *Map_GetWarpDests(void);
const struct MapWarpRegion *Map_GetWarpRegions(void);
const u32 *Map_GetWarps(void);
const struct ObjectEventTemplate *Map_GetObjectEventTemplates(void);
const struct MapEvent *Map_GetEvents(void);
s32 Map_Initialize(void);
extern s32 gShortEffectScript[];
extern s32 gMediumEffectScript[];
extern s32 gLongEffectScript[];
extern const s32 *gEffectScripts[];
extern s32 gGomaCaveEntranceTallIvyScript[];
extern struct MapWarpDest gGomaCaveEntranceWarpDests[];
extern u32 gGomaCaveEntranceWarps[];
extern struct ObjectEventTemplate gGomaCaveEntranceObjectEventTemplates[];
extern struct MapEvent gGomaCaveEntranceEvents[];

#endif
