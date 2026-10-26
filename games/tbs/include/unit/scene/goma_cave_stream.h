#ifndef ALCHEMY_UNIT_MAP_GOMA_CAVE_STREAM_H
#define ALCHEMY_UNIT_MAP_GOMA_CAVE_STREAM_H

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
extern s32 gFieldEffect_ShortScript[];
extern s32 gFieldEffect_MediumScript[];
extern s32 gFieldEffect_LongScript[];
extern const s32 *gFieldEffect_DurationScripts[];
extern struct MapWarpDest gGomaCaveStreamWarpDests[];
extern struct MapWarpDest gGomaCaveBridgeWarpDests[];
extern u32 gGomaCaveStreamAndBridgeWarps[];
extern struct ObjectEventTemplate gGomaCaveStreamObjectEventTemplates[];
extern struct ObjectEventTemplate gGomaCaveBridgeObjectEventTemplates[];
extern struct MapEvent gGomaCaveStreamEvents[];
extern struct MapEvent gGomaCaveBridgeEvents[];

#endif
