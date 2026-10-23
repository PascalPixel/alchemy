#ifndef ALCHEMY_UNIT_MAP_VALE_INN_H
#define ALCHEMY_UNIT_MAP_VALE_INN_H

#include "types.h"
#include "map_tables.h"

const struct MapWarpDest *ValeInn_GetWarpDests(void);
const struct MapWarpRegion *ValeInn_GetRegions(void);
const u32 *ValeInn_GetWarps(void);
const struct ObjectEventTemplate *ValeInn_GetObjectEventTemplates(void);
const struct MapEvent *ValeInn_GetEvents(void);
s32 ValeInn_Initialize(void);
extern s32 gValeInn_LeaveHouseScript[];
extern s32 gValeInn_DoraLeavesScript[];
extern s32 gValeInn_IsaacLeavesScript[];
extern s32 gValeInn_DoraRunsAheadScript[];
extern s32 gValeInn_IsaacRunsAheadScript[];
extern s32 gValeInn_DoraRunsBackScript[];
extern s32 gValeInn_IsaacRunsBackScript[];
extern s32 gValeInn_RunToPlazaScript[];
extern s32 gValeInn_FlameShakesScript[];
extern s32 gValeInn_FlameRisesScript[];
extern s32 gValeInn_DoraFacesIsaacScript[];
extern s32 gValeInn_MythrilBagRisesScript[];
extern s32 gValeInn_MythrilBagWobblesScript[];
extern struct MapWarpDest gValeInn_WarpDests[];
extern u32 gValeInn_Warps[];
extern u32 gValeInn_StormWarps[];
extern struct ObjectEventTemplate gValeInn_ObjectEventTemplates[];
extern struct ObjectEventTemplate gValeInn_ObjectEventTemplatesAfterLeaving[];
extern struct ObjectEventTemplate gValeInn_ObjectEventTemplatesAfterKalay[];
extern struct ObjectEventTemplate gValeInn_ScholarsObjectEventTemplates[];
extern struct MapEvent gValeInn_StormEvents[];
extern struct MapEvent gValeInn_Events[];
extern struct MapEvent gValeInn_EventsAfterLeaving[];
extern struct MapEvent gValeInn_EventsAfterKalay[];
extern struct MapEvent gValeInn_ScholarsEvents[];
extern struct MapEvent gValeInn_ScholarsLaterEvents[];

#endif
