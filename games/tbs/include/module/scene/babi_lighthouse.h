#ifndef ALCHEMY_MODULE_SCENE_BABI_LIGHTHOUSE_H
#define ALCHEMY_MODULE_SCENE_BABI_LIGHTHOUSE_H

#include "types.h"
#include "scene_tables.h"
#include "scene.h"

const u32 *BabiLighthouse_GetExits(void);
s32 BabiLighthouse_SetupMap(void);
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
