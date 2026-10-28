#ifndef ALCHEMY_MODULE_MAP_BABI_LIGHTHOUSE_H
#define ALCHEMY_MODULE_MAP_BABI_LIGHTHOUSE_H

#include "types.h"
#include "scene_tables.h"
#include "scene.h"

struct SceneEntrance;
struct Trigger;
struct ActorPlacement;
struct SceneExitRegion;

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneExitRegion *Scene_GetExitRegions(void);
const u32 *BabiLighthouse_GetExits(void);
const struct ActorPlacement *Scene_GetActorPlacements(void);
const struct Trigger *Scene_GetTriggers(void);
s32 BabiLighthouse_SetupMap(void);
extern s32 gPushedActor_DirectionSteps[];
extern u32 gPushedActor_FootprintKinds[];
extern s32 gPushedActor_FootprintBounds[];
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
extern struct ActorPlacement gBabiLighthouse_NoActorPlacements[];
extern struct ActorPlacement gBabiLighthouse_EntryActorPlacements[];
extern struct ActorPlacement gBabiLighthouse_LuffActorPlacements[];
extern struct ActorPlacement gBabiLighthouse_StoneDoorActorPlacements[];
extern struct ActorPlacement gBabiLighthouse_RoadActorPlacements[];
extern struct Trigger gBabiLighthouse_EntryTriggers[];
extern struct Trigger gBabiLighthouse_LuffTriggers[];
extern struct Trigger gBabiLighthouse_StoneDoorTriggers[];
extern struct Trigger gBabiLighthouse_RoadTriggers[];

#endif
