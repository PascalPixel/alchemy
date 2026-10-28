#ifndef ALCHEMY_MODULE_MAP_SUHALLA_GATE_H
#define ALCHEMY_MODULE_MAP_SUHALLA_GATE_H

#include "types.h"
#include "scene_tables.h"

struct SceneExitRegion;

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneExitRegion *Scene_GetExitRegions(void);
const u32 *Scene_GetExits(void);
const struct ActorPlacement *Scene_GetActorPlacements(void);
const struct Trigger *Scene_GetTriggers(void);
s32 Scene_Initialize(void);
extern s32 gSuhallaGateWoundedNoCollision[];
extern s32 gSuhallaGateWounded[];
extern s32 gSuhallaGateWoundedFlippedNoCollision[];
extern s32 gSuhallaGateWoundedFlipped[];
extern struct SceneEntrance gSuhallaGateEntrancesOther[];
extern struct SceneEntrance gSuhallaGateDewEntrances[];
extern struct SceneEntrance gGatewayCaveEntrances[];
extern u32 gSuhallaGateExits[];
extern struct ActorPlacement gSuhallaGateActorPlacementsOther[];
extern struct ActorPlacement gSuhallaGateDewActorPlacements[];
extern struct ActorPlacement gSuhallaGateCheckpointActorPlacements[];
extern struct ActorPlacement
    gSuhallaGateCheckpointActorPlacementsAfterAudience[];
extern struct Trigger gSuhallaGateTriggersOther[];
extern struct Trigger gSuhallaGateDewTriggers[];
extern struct Trigger gGatewayCaveTriggers[];

#endif
