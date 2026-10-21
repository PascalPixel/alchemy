#ifndef ALCHEMY_UNIT_SCENE_VALE_INTERIORS_SHOPS_H
#define ALCHEMY_UNIT_SCENE_VALE_INTERIORS_SHOPS_H

#include "types.h"
#include "field_scene.h"

const struct SceneEntrance *ValeInteriorsShops_GetEntrances(void);
const struct SceneRegion *ValeInteriorsShops_GetRegions(void);
const u32 *ValeInteriorsShops_GetExits(void);
const struct ScenePlacement *ValeInteriorsShops_GetPlacements(void);
const struct SceneEvent *ValeInteriorsShops_GetEvents(void);
s32 ValeInteriorsShops_Initialize(void);
extern s32 gValeInteriorsShops_FaceTargetScript[];
extern s32 gValeInteriorsShops_FatherPacingScript[];
extern s32 gValeInteriorsShops_PartyLeavesScript[];
extern s32 gValeInteriorsShops_DriftingSpriteScript[];
extern struct SceneEntrance gValeInteriorsShops_Entrances[];
extern u32 gValeInteriorsShops_Exits[];
extern u32 gValeInteriorsShops_StormExits[];
extern struct ScenePlacement gValeInteriorsShops_Placements[];
extern struct ScenePlacement gValeInteriorsShops_PlacementsAfterLeaving[];
extern struct ScenePlacement gValeInteriorsShops_PlacementsAfterKalay[];
extern struct SceneEvent gValeInteriorsShops_StormEvents[];
extern struct SceneEvent gValeInteriorsShops_Events[];
extern struct SceneEvent gValeInteriorsShops_EventsAfterLeaving[];
extern struct SceneEvent gValeInteriorsShops_EventsAfterKalay[];

#endif
