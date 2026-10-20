#ifndef ALCHEMY_UNIT_SCENE_VALE_INTERIORS_H
#define ALCHEMY_UNIT_SCENE_VALE_INTERIORS_H

#include "types.h"
#include "field_scene.h"

const struct SceneEntrance *ValeInteriors_GetEntrances(void);
const struct SceneRegion *ValeInteriors_GetRegions(void);
const u32 *ValeInteriors_GetExits(void);
const struct ScenePlacement *ValeInteriors_GetPlacements(void);
const struct SceneEvent *ValeInteriors_GetEvents(void);
s32 ValeInteriors_Initialize(void);
extern s32 gValeInteriors_FaceTargetScript[];
extern s32 gValeInteriors_FatherPacingScript[];
extern s32 gValeInteriors_PartyLeavesScript[];
extern s32 gValeInteriors_DriftingSpriteScript[];
extern struct SceneEntrance gValeInteriors_Entrances[];
extern u32 gValeInteriors_Exits[];
extern u32 gValeInteriors_StormExits[];
extern struct ScenePlacement gValeInteriors_Placements[];
extern struct ScenePlacement gValeInteriors_PlacementsAfterLeaving[];
extern struct ScenePlacement gValeInteriors_PlacementsAfterKalay[];
extern struct SceneEvent gValeInteriors_StormEvents[];
extern struct SceneEvent gValeInteriors_Events[];
extern struct SceneEvent gValeInteriors_EventsAfterLeaving[];
extern struct SceneEvent gValeInteriors_EventsAfterKalay[];

#endif
