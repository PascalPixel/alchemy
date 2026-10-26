#ifndef ALCHEMY_UNIT_MAP_BABI_LIGHTHOUSE_H
#define ALCHEMY_UNIT_MAP_BABI_LIGHTHOUSE_H

#include "types.h"
#include "map_tables.h"
#include "scene_header.h"

struct SceneEntrance;
struct MapEvent;
struct ActorTemplate;
struct SceneExitRegion;

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneExitRegion *Scene_GetExitRegions(void);
const u32 *BabiLighthouse_GetExits(void);
const struct ActorTemplate *Scene_GetActorTemplates(void);
const struct MapEvent *Scene_GetEvents(void);
s32 BabiLighthouse_SetupMap(void);
extern s32 gPushedObject_DirectionSteps[];
extern u32 gPushedObject_FootprintKinds[];
extern s32 gPushedObject_FootprintBounds[];
extern s32 gFieldEffect_ShortScript[];
extern s32 gFieldEffect_MediumScript[];
extern s32 gFieldEffect_LongScript[];
extern const s32 *gFieldEffect_DurationScripts[];
extern s32 gBabiLighthouse_TrembleScript[];
extern struct SceneEntrance gBabiLighthouse_EntryEntrances[];
extern struct SceneEntrance gBabiLighthouse_LuffEntrances[];
extern struct SceneEntrance gBabiLighthouse_StoneDoorEntrances[];
extern struct SceneEntrance gBabiLighthouse_RoadEntrances[];
extern struct SceneExitRegion gBabiLighthouse_EntryRegions[];
extern u32 gBabiLighthouse_Exits[];
extern struct ActorTemplate gBabiLighthouse_NoActorTemplates[];
extern struct ActorTemplate gBabiLighthouse_EntryActorTemplates[];
extern struct ActorTemplate gBabiLighthouse_LuffActorTemplates[];
extern struct ActorTemplate gBabiLighthouse_StoneDoorActorTemplates[];
extern struct ActorTemplate gBabiLighthouse_RoadActorTemplates[];
extern struct MapEvent gBabiLighthouse_EntryEvents[];
extern struct MapEvent gBabiLighthouse_LuffEvents[];
extern struct MapEvent gBabiLighthouse_StoneDoorEvents[];
extern struct MapEvent gBabiLighthouse_RoadEvents[];

#endif
