#ifndef ALCHEMY_MODULE_SCENE_BILIBIN_CAVE_H
#define ALCHEMY_MODULE_SCENE_BILIBIN_CAVE_H

#include "types.h"
#include "scene_tables.h"

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneExitRegion *Scene_GetExitRegions(void);
const u32 *Scene_GetExits(void);
const struct ActorPlacement *Scene_GetActorPlacements(void);
const struct Trigger *Scene_GetTriggers(void);
s32 Scene_Initialize(void);
extern s32 gBilibinCave_ClearSpriteScript[];
extern struct SceneEntrance gBilibinCaveEntrancesOther[];
extern struct SceneEntrance gBilibinCaveEntranceEntrances[];
extern struct SceneEntrance gBilibinCaveRapierEntrances[];
extern struct SceneEntrance gBilibinCaveIceEntrances[];
extern u32 gBilibinCaveExits[];
extern struct ActorPlacement gBilibinCaveActorPlacementsOther[];
extern struct ActorPlacement gBilibinCaveEntranceActorPlacements[];
extern struct ActorPlacement gBilibinCaveRapierActorPlacements[];
extern struct ActorPlacement gBilibinCaveIceActorPlacements[];
extern struct Trigger gBilibinCaveTriggersOther[];
extern struct Trigger gBilibinCaveEntranceTriggers[];
extern struct Trigger gBilibinCaveRapierTriggers[];
extern struct Trigger gBilibinCaveIceTriggers[];
extern s32 gPushedActor_DirectionSteps[];

#endif
