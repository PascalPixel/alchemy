#ifndef ALCHEMY_UNIT_SCENE_VALE_INTERIORS_2_H
#define ALCHEMY_UNIT_SCENE_VALE_INTERIORS_2_H

#include "types.h"
#include "field_scene.h"

const struct SceneEntrance *ValeInteriors2_GetEntrances(void);
const struct SceneRegion *ValeInteriors2_GetRegions(void);
const u32 *ValeInteriors2_GetExits(void);
const struct ScenePlacement *ValeInteriors2_GetPlacements(void);
const struct SceneEvent *ValeInteriors2_GetEvents(void);
s32 ValeInteriors2_Initialize(void);
extern s32 gValeInteriors2_LeaveHouseScript[];
extern s32 gValeInteriors2_DoraLeavesScript[];
extern s32 gValeInteriors2_IsaacLeavesScript[];
extern s32 gValeInteriors2_DoraRunsAheadScript[];
extern s32 gValeInteriors2_IsaacRunsAheadScript[];
extern s32 gValeInteriors2_DoraRunsBackScript[];
extern s32 gValeInteriors2_IsaacRunsBackScript[];
extern s32 gValeInteriors2_RunToPlazaScript[];
extern s32 gValeInteriors2_FlameShakesScript[];
extern s32 gValeInteriors2_FlameRisesScript[];
extern s32 gValeInteriors2_DoraFacesIsaacScript[];
extern s32 gValeInteriors2_MythrilBagRisesScript[];
extern s32 gValeInteriors2_MythrilBagWobblesScript[];
extern struct SceneEntrance gValeInteriors2_Entrances[];
extern u32 gValeInteriors2_Exits[];
extern u32 gValeInteriors2_StormExits[];
extern struct ScenePlacement gValeInteriors2_Placements[];
extern struct ScenePlacement gValeInteriors2_PlacementsAfterLeaving[];
extern struct ScenePlacement gValeInteriors2_PlacementsAfterKalay[];
extern struct ScenePlacement gValeInteriors2_ScholarsPlacements[];
extern struct SceneEvent gValeInteriors2_StormEvents[];
extern struct SceneEvent gValeInteriors2_Events[];
extern struct SceneEvent gValeInteriors2_EventsAfterLeaving[];
extern struct SceneEvent gValeInteriors2_EventsAfterKalay[];
extern struct SceneEvent gValeInteriors2_ScholarsEvents[];
extern struct SceneEvent gValeInteriors2_ScholarsLaterEvents[];

#endif
