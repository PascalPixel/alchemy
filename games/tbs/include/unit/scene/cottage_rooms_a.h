#ifndef ALCHEMY_UNIT_MAP_COTTAGE_ROOMS_A_H
#define ALCHEMY_UNIT_MAP_COTTAGE_ROOMS_A_H

#include "types.h"
#include "map_tables.h"

const struct MapWarpDest *CottageRoomsA_GetWarpDests(void);
const struct MapWarpRegion *CottageRoomsA_GetRegions(void);
const u32 *CottageRoomsA_GetWarps(void);
const struct ObjectEventTemplate *CottageRoomsA_GetObjectEventTemplates(void);
const struct MapEvent *CottageRoomsA_GetEvents(void);
s32 CottageRoomsA_Initialize(void);
extern s32 gCottageRoomsA_FaceTargetScript[];
extern s32 gCottageRoomsA_FatherPacingScript[];
extern s32 gCottageRoomsA_PartyLeavesScript[];
extern s32 gCottageRoomsA_SleepZScript[];
extern struct MapWarpDest gCottageRoomsA_WarpDests[];
extern u32 gCottageRoomsA_Warps[];
extern u32 gCottageRoomsA_StormWarps[];
extern struct ObjectEventTemplate gCottageRoomsA_ObjectEventTemplates[];
extern struct ObjectEventTemplate
    gCottageRoomsA_ObjectEventTemplatesAfterLeaving[];
extern struct ObjectEventTemplate
    gCottageRoomsA_ObjectEventTemplatesAfterKalay[];
extern struct MapEvent gCottageRoomsA_StormEvents[];
extern struct MapEvent gCottageRoomsA_Events[];
extern struct MapEvent gCottageRoomsA_EventsAfterLeaving[];
extern struct MapEvent gCottageRoomsA_EventsAfterKalay[];

#endif
