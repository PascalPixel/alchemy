#ifndef ALCHEMY_UNIT_SCENE_VALE_INTERIORS_INN_H
#define ALCHEMY_UNIT_SCENE_VALE_INTERIORS_INN_H

#include "types.h"
#include "field_scene.h"

const struct SceneEntrance *ValeInteriorsInn_GetEntrances(void);
const struct SceneRegion *ValeInteriorsInn_GetRegions(void);
const u32 *ValeInteriorsInn_GetExits(void);
const struct ScenePlacement *ValeInteriorsInn_GetPlacements(void);
const struct SceneEvent *ValeInteriorsInn_GetEvents(void);
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
extern struct SceneEntrance gValeInteriorsInn_Entrances[];
extern u32 gValeInteriorsInn_Exits[];
extern u32 gValeInteriorsInn_StormExits[];
extern struct ScenePlacement gValeInteriorsInn_Placements[];
extern struct ScenePlacement gValeInteriorsInn_PlacementsAfterLeaving[];
extern struct ScenePlacement gValeInteriorsInn_PlacementsAfterKalay[];
extern struct ScenePlacement gValeInteriorsInn_ScholarsPlacements[];
extern struct SceneEvent gValeInteriorsInn_StormEvents[];
extern struct SceneEvent gValeInteriorsInn_Events[];
extern struct SceneEvent gValeInteriorsInn_EventsAfterLeaving[];
extern struct SceneEvent gValeInteriorsInn_EventsAfterKalay[];
extern struct SceneEvent gValeInteriorsInn_ScholarsEvents[];
extern struct SceneEvent gValeInteriorsInn_ScholarsLaterEvents[];

#endif
