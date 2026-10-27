#ifndef ALCHEMY_UNIT_MAP_MOGALL_FOREST_H
#define ALCHEMY_UNIT_MAP_MOGALL_FOREST_H

#include "types.h"
#include "scene_tables.h"

struct SceneEntrance;
struct Trigger;
struct ActorTemplate;

const struct SceneEntrance *MogallForest_GetEntrances(void);
const struct SceneExitRegion *MogallForest_GetRegions(void);
const u32 *MogallForest_GetExits(void);
const struct ActorTemplate *MogallForest_GetActorTemplates(void);
const struct Trigger *MogallForest_GetTriggers(void);
s32 MogallForest_Initialize(void);
extern s32 gPushedActor_DirectionSteps[];
extern u32 gPushedActor_FootprintKinds[];
extern s32 gPushedActor_FootprintBounds[];
extern s32 gFieldEffect_ShortScript[];
extern s32 gFieldEffect_MediumScript[];
extern s32 gFieldEffect_LongScript[];
extern const s32 *gFieldEffect_DurationScripts[];
extern s32 gMogallForest_RockScript[];
extern s32 gMogallForest_ActorScript[];
extern s32 gMogallForest_HidingPlaceScript[];
extern struct SceneEntrance gMogallForestSign_Entrances[];
extern struct SceneEntrance gMogallForestQuartz_Entrances[];
extern struct SceneEntrance gMogallForestKillerApe_Entrances[];
extern struct SceneEntrance gMogallForestSidePaths_Entrances[];
extern u32 gMogallForest_Exits[];
extern struct ActorTemplate gMogallForestSign_ActorTemplates[];
extern struct ActorTemplate gMogallForestQuartz_ActorTemplates[];
extern struct ActorTemplate gMogallForestKillerApe_ActorTemplates[];
extern struct ActorTemplate gMogallForestSidePaths_ActorTemplates[];
extern struct Trigger gMogallForestSign_Triggers[];
extern struct Trigger gMogallForestQuartz_Triggers[];
extern struct Trigger gMogallForestKillerApe_Triggers[];
extern struct Trigger gMogallForestSidePaths_Triggers[];

#endif
