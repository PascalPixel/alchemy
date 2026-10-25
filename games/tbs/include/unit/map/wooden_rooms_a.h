#ifndef ALCHEMY_UNIT_MAP_WOODEN_ROOMS_A_H
#define ALCHEMY_UNIT_MAP_WOODEN_ROOMS_A_H

#include "types.h"
#include "map_tables.h"

const struct MapWarpDest *WoodenRoomsA_GetWarpDests(void);
const struct MapWarpRegion *WoodenRoomsA_GetRegions(void);
const u32 *WoodenRoomsA_GetWarps(void);
const struct ObjectEventTemplate *WoodenRoomsA_GetObjectEventTemplates(void);
const struct MapEvent *WoodenRoomsA_GetEvents(void);
s32 WoodenRoomsA_Initialize(void);
extern s32 gWoodenRoomsA_LeaveHouseScript[];
extern s32 gWoodenRoomsA_DoraLeavesScript[];
extern s32 gWoodenRoomsA_IsaacLeavesScript[];
extern s32 gWoodenRoomsA_DoraRunsAheadScript[];
extern s32 gWoodenRoomsA_IsaacRunsAheadScript[];
extern s32 gWoodenRoomsA_DoraRunsBackScript[];
extern s32 gWoodenRoomsA_IsaacRunsBackScript[];
extern s32 gWoodenRoomsA_RunToPlazaScript[];
extern s32 gWoodenRoomsA_FlameShakesScript[];
extern s32 gWoodenRoomsA_FlameRisesScript[];
extern s32 gWoodenRoomsA_DoraFacesIsaacScript[];
extern s32 gWoodenRoomsA_MythrilBagRisesScript[];
extern s32 gWoodenRoomsA_MythrilBagWobblesScript[];
extern struct MapWarpDest gWoodenRoomsA_WarpDests[];
extern u32 gWoodenRoomsA_Warps[];
extern u32 gWoodenRoomsA_StormWarps[];
extern struct ObjectEventTemplate gWoodenRoomsA_ObjectEventTemplates[];
extern struct ObjectEventTemplate
    gWoodenRoomsA_ObjectEventTemplatesAfterLeaving[];
extern struct ObjectEventTemplate
    gWoodenRoomsA_ObjectEventTemplatesAfterKalay[];
extern struct ObjectEventTemplate
    gWoodenRoomsA_BabiScholarsObjectEventTemplates[];
extern struct MapEvent gWoodenRoomsA_StormEvents[];
extern struct MapEvent gWoodenRoomsA_Events[];
extern struct MapEvent gWoodenRoomsA_EventsAfterLeaving[];
extern struct MapEvent gWoodenRoomsA_EventsAfterKalay[];
extern struct MapEvent gWoodenRoomsA_BabiScholarsEvents[];
extern struct MapEvent gWoodenRoomsA_BabiScholarsLaterEvents[];

#endif
