#ifndef ALCHEMY_UNIT_SCENE_VALE_INTERIORS_1_H
#define ALCHEMY_UNIT_SCENE_VALE_INTERIORS_1_H

#include "types.h"
#include "field_scene.h"

const struct SceneEntrance *ValeInteriors1_GetEntrances(void);
const struct SceneRegion *ValeInteriors1_GetRegions(void);
const u32 *ValeInteriors1_GetExits(void);
const struct ScenePlacement *ValeInteriors1_GetPlacements(void);
const struct SceneEvent *ValeInteriors1_GetEvents(void);
s32 ValeInteriors1_Initialize(void);
extern s32 gValeInteriors1_FaceTargetScript[];
extern s32 gValeInteriors1_FatherPacingScript[];
extern s32 gValeInteriors1_PartyLeavesScript[];
extern s32 gValeInteriors1_DriftingSpriteScript[];
extern struct SceneEntrance gValeInteriors1_Entrances[];
extern u32 gValeInteriors1_Exits[];
extern u32 gValeInteriors1_StormExits[];
extern struct ScenePlacement gValeInteriors1_Placements[];
extern struct ScenePlacement gValeInteriors1_PlacementsAfterLeaving[];
extern struct ScenePlacement gValeInteriors1_PlacementsAfterKalay[];
extern struct SceneEvent gValeInteriors1_StormEvents[];
extern struct SceneEvent gValeInteriors1_Events[];
extern struct SceneEvent gValeInteriors1_EventsAfterLeaving[];
extern struct SceneEvent gValeInteriors1_EventsAfterKalay[];

#endif
