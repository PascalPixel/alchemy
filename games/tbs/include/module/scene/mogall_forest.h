#ifndef ALCHEMY_MODULE_SCENE_MOGALL_FOREST_H
#define ALCHEMY_MODULE_SCENE_MOGALL_FOREST_H

#include "types.h"
#include "scene_tables.h"

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneExitRegion *Scene_GetExitRegions(void);
const u32 *Scene_GetExits(void);
const struct ActorPlacement *Scene_GetActorPlacements(void);
const struct Trigger *Scene_GetTriggers(void);
s32 Scene_Initialize(void);
extern s32 gMogallForest_RockScript[];
extern s32 gMogallForest_ActorScript[];
extern s32 gMogallForest_HidingPlaceScript[];
extern struct SceneEntrance gMogallForestSign_Entrances[];
extern struct SceneEntrance gMogallForestQuartz_Entrances[];
extern struct SceneEntrance gMogallForestKillerApe_Entrances[];
extern struct SceneEntrance gMogallForestSidePaths_Entrances[];
extern u32 gMogallForest_Exits[];
extern struct ActorPlacement gMogallForestSign_ActorPlacements[];
extern struct ActorPlacement gMogallForestQuartz_ActorPlacements[];
extern struct ActorPlacement gMogallForestKillerApe_ActorPlacements[];
extern struct ActorPlacement gMogallForestSidePaths_ActorPlacements[];
extern struct Trigger gMogallForestSign_Triggers[];
extern struct Trigger gMogallForestQuartz_Triggers[];
extern struct Trigger gMogallForestKillerApe_Triggers[];
extern struct Trigger gMogallForestSidePaths_Triggers[];

#endif
