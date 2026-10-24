#ifndef ALCHEMY_UNIT_MAP_XIAN_INTERIORS_H
#define ALCHEMY_UNIT_MAP_XIAN_INTERIORS_H

#include "types.h"
#include "map_tables.h"

const struct MapWarpDest *XianInteriors_GetWarpDests(void);
const struct MapWarpRegion *XianInteriors_GetRegions(void);
const u32 *XianInteriors_GetWarps(void);
const struct ObjectEventTemplate *XianInteriors_GetObjectEventTemplates(void);
const struct MapEvent *XianInteriors_GetEvents(void);
s32 XianInteriors_Initialize(void);
extern s32 gFieldEffect_ShortScript[];
extern s32 gFieldEffect_MediumScript[];
extern s32 gFieldEffect_LongScript[];
extern const s32 *gFieldEffect_DurationScripts[];
extern struct MapWarpDest gXianInteriors_WarpDests[];
extern u32 gXianInteriors_Warps[];
extern struct ObjectEventTemplate gXianInteriors_ObjectEventTemplates[];
extern struct ObjectEventTemplate
    gXianInteriors_SchoolRoomObjectEventTemplates[];
extern struct MapEvent gXianInteriors_Events[];
extern struct MapEvent gXianInteriors_SchoolRoomEvents[];

#endif
