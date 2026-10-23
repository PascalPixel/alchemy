#ifndef ALCHEMY_UNIT_MAP_LUNPA_H
#define ALCHEMY_UNIT_MAP_LUNPA_H

#include "types.h"
#include "map_tables.h"

struct MapWarpDest;
struct MapEvent;
struct ObjectEventTemplate;
struct MapWarpRegion;

extern s32 gFieldEffect_ShortScript[];
extern s32 gFieldEffect_MediumScript[];
extern s32 gFieldEffect_LongScript[];
extern const s32 *gFieldEffect_DurationScripts[];
const struct MapWarpDest *Map_GetWarpDests(void);
const struct MapWarpRegion *Map_GetWarpRegions(void);
const u32 *Map_GetWarps(void);
const struct ObjectEventTemplate *Map_GetObjectEventTemplates(void);
const struct MapEvent *Map_GetEvents(void);
s32 Map_Initialize(void);
extern struct MapWarpDest gLunpaWarpDests[];
extern struct MapWarpDest gGateWarpDests[];
extern u32 gLunpaWarps[];
extern u32 gGateWarps[];
extern struct ObjectEventTemplate gLunpaObjectEventTemplates[];
extern struct ObjectEventTemplate gGateObjectEventTemplates[];
extern struct MapEvent gLunpaSealedEvents[];
extern struct MapEvent gLunpaReopenedEvents[];
extern struct MapEvent gGateEvents[];
extern u32 gLunpaUnusedWord;

#endif
