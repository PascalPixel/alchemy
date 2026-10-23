#ifndef ALCHEMY_UNIT_MAP_GOMA_CAVE_WATERWAY_H
#define ALCHEMY_UNIT_MAP_GOMA_CAVE_WATERWAY_H

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
extern s32 gEffectScript0[];
extern s32 gEffectScript1[];
extern s32 gEffectScript2[];
extern const s32 *gEffectScripts[];
extern struct MapWarpDest gGomaCaveStreamWarpDests[];
extern struct MapWarpDest gGomaCaveBridgeWarpDests[];
extern u32 gGomaCaveStreamAndBridgeWarps[];
extern struct ObjectEventTemplate gGomaCaveStreamObjectEventTemplates[];
extern struct ObjectEventTemplate gGomaCaveBridgeObjectEventTemplates[];
extern struct MapEvent gGomaCaveStreamEvents[];
extern struct MapEvent gGomaCaveBridgeEvents[];

#endif
