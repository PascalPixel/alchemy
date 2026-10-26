#ifndef ALCHEMY_UNIT_MAP_GOMA_CAVE_ENTRANCE_H
#define ALCHEMY_UNIT_MAP_GOMA_CAVE_ENTRANCE_H

#include "types.h"
#include "map_tables.h"

const struct MapWarpDest *Scene_GetWarpDests(void);
const struct MapWarpRegion *Scene_GetWarpRegions(void);
const u32 *Scene_GetWarps(void);
const struct ObjectEventTemplate *Scene_GetObjectEventTemplates(void);
const struct MapEvent *Scene_GetEvents(void);
s32 Scene_Initialize(void);
extern s32 gFieldEffect_ShortScript[];
extern s32 gFieldEffect_MediumScript[];
extern s32 gFieldEffect_LongScript[];
extern const s32 *gFieldEffect_DurationScripts[];
extern s32 gGomaCaveEntranceTallIvyScript[];
extern struct MapWarpDest gGomaCaveEntranceWarpDests[];
extern u32 gGomaCaveEntranceWarps[];
extern struct ObjectEventTemplate gGomaCaveEntranceObjectEventTemplates[];
extern struct MapEvent gGomaCaveEntranceEvents[];

#endif
