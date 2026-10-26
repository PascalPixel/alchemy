#ifndef ALCHEMY_UNIT_MAP_COTTAGE_ROOMS_B_H
#define ALCHEMY_UNIT_MAP_COTTAGE_ROOMS_B_H

#include "types.h"
#include "map_tables.h"

const struct MapWarpDest *CottageRoomsB_GetWarpDests(void);
const struct MapWarpRegion *CottageRoomsB_GetRegions(void);
const u32 *CottageRoomsB_GetWarps(void);
const struct ObjectEventTemplate *CottageRoomsB_GetObjectEventTemplates(void);
const struct MapEvent *CottageRoomsB_GetEvents(void);
s32 CottageRoomsB_Initialize(void);
extern s32 gCottageRoomsB_LeaveHouseScript[];
extern s32 gCottageRoomsB_DoraLeavesScript[];
extern s32 gCottageRoomsB_IsaacLeavesScript[];
extern s32 gCottageRoomsB_DoraRunsAheadScript[];
extern s32 gCottageRoomsB_IsaacRunsAheadScript[];
extern s32 gCottageRoomsB_DoraRunsBackScript[];
extern s32 gCottageRoomsB_IsaacRunsBackScript[];
extern s32 gCottageRoomsB_RunToPlazaScript[];
extern s32 gCottageRoomsB_FlameShakesScript[];
extern s32 gCottageRoomsB_FlameRisesScript[];
extern s32 gCottageRoomsB_DoraFacesIsaacScript[];
extern s32 gCottageRoomsB_MythrilBagRisesScript[];
extern s32 gCottageRoomsB_MythrilBagWobblesScript[];
extern struct MapWarpDest gCottageRoomsB_WarpDests[];
extern u32 gCottageRoomsB_Warps[];
extern u32 gCottageRoomsB_StormWarps[];
extern struct ObjectEventTemplate gCottageRoomsB_ObjectEventTemplates[];
extern struct ObjectEventTemplate
    gCottageRoomsB_ObjectEventTemplatesAfterLeaving[];
extern struct ObjectEventTemplate
    gCottageRoomsB_ObjectEventTemplatesAfterKalay[];
extern struct ObjectEventTemplate
    gCottageRoomsB_BabiScholarsObjectEventTemplates[];
extern struct MapEvent gCottageRoomsB_StormEvents[];
extern struct MapEvent gCottageRoomsB_Events[];
extern struct MapEvent gCottageRoomsB_EventsAfterLeaving[];
extern struct MapEvent gCottageRoomsB_EventsAfterKalay[];
extern struct MapEvent gCottageRoomsB_BabiScholarsEvents[];
extern struct MapEvent gCottageRoomsB_BabiScholarsLaterEvents[];

#endif
