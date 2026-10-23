#ifndef ALCHEMY_UNIT_MAP_VALE_INTERIORS_INN_H
#define ALCHEMY_UNIT_MAP_VALE_INTERIORS_INN_H

#include "types.h"
#include "map_tables.h"

const struct MapWarpDest *ValeInteriorsInn_GetWarpDests(void);
const struct MapWarpRegion *ValeInteriorsInn_GetRegions(void);
const u32 *ValeInteriorsInn_GetWarps(void);
const struct ObjectEventTemplate *
ValeInteriorsInn_GetObjectEventTemplates(void);
const struct MapEvent *ValeInteriorsInn_GetEvents(void);
s32 ValeInteriorsInn_Initialize(void);
extern s32 gValeInteriorsInn_LeaveHouseScript[];
extern s32 gValeInteriorsInn_DoraLeavesScript[];
extern s32 gValeInteriorsInn_IsaacLeavesScript[];
extern s32 gValeInteriorsInn_DoraRunsAheadScript[];
extern s32 gValeInteriorsInn_IsaacRunsAheadScript[];
extern s32 gValeInteriorsInn_DoraRunsBackScript[];
extern s32 gValeInteriorsInn_IsaacRunsBackScript[];
extern s32 gValeInteriorsInn_RunToPlazaScript[];
extern s32 gValeInteriorsInn_FlameShakesScript[];
extern s32 gValeInteriorsInn_FlameRisesScript[];
extern s32 gValeInteriorsInn_DoraFacesIsaacScript[];
extern s32 gValeInteriorsInn_MythrilBagRisesScript[];
extern s32 gValeInteriorsInn_MythrilBagWobblesScript[];
extern struct MapWarpDest gValeInteriorsInn_WarpDests[];
extern u32 gValeInteriorsInn_Warps[];
extern u32 gValeInteriorsInn_StormWarps[];
extern struct ObjectEventTemplate gValeInteriorsInn_ObjectEventTemplates[];
extern struct ObjectEventTemplate
    gValeInteriorsInn_ObjectEventTemplatesAfterLeaving[];
extern struct ObjectEventTemplate
    gValeInteriorsInn_ObjectEventTemplatesAfterKalay[];
extern struct ObjectEventTemplate
    gValeInteriorsInn_ScholarsObjectEventTemplates[];
extern struct MapEvent gValeInteriorsInn_StormEvents[];
extern struct MapEvent gValeInteriorsInn_Events[];
extern struct MapEvent gValeInteriorsInn_EventsAfterLeaving[];
extern struct MapEvent gValeInteriorsInn_EventsAfterKalay[];
extern struct MapEvent gValeInteriorsInn_ScholarsEvents[];
extern struct MapEvent gValeInteriorsInn_ScholarsLaterEvents[];

#endif
