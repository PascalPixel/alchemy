#ifndef ALCHEMY_UNIT_MAP_LACQUER_ROOMS_B_H
#define ALCHEMY_UNIT_MAP_LACQUER_ROOMS_B_H

#include "types.h"
#include "map_tables.h"

const struct MapWarpDest *LacquerRoomsB_GetWarpDests(void);
const struct MapWarpRegion *LacquerRoomsB_GetRegions(void);
const u32 *LacquerRoomsB_GetWarps(void);
const struct ObjectEventTemplate *LacquerRoomsB_GetObjectEventTemplates(void);
const struct MapEvent *LacquerRoomsB_GetEvents(void);
s32 LacquerRoomsB_Initialize(void);
extern s32 gFieldEffect_ShortScript[];
extern s32 gFieldEffect_MediumScript[];
extern s32 gFieldEffect_LongScript[];
extern const s32 *gFieldEffect_DurationScripts[];
extern struct MapWarpDest gLacquerRoomsB_WarpDests[];
extern u32 gLacquerRoomsB_Warps[];
extern struct ObjectEventTemplate gLacquerRoomsB_ObjectEventTemplates[];
extern struct ObjectEventTemplate
    gLacquerRoomsB_SchoolRoomObjectEventTemplates[];
extern struct MapEvent gLacquerRoomsB_Events[];
extern struct MapEvent gLacquerRoomsB_SchoolRoomEvents[];

#endif
