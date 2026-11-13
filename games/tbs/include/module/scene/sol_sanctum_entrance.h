#ifndef ALCHEMY_MODULE_SCENE_SOL_SANCTUM_ENTRANCE_H
#define ALCHEMY_MODULE_SCENE_SOL_SANCTUM_ENTRANCE_H

#include "types.h"
#include "scene_tables.h"

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneExitRegion *Scene_GetExitRegions(void);
const u32 *Scene_GetExits(void);
const struct ActorPlacement *Scene_GetActorPlacements(void);
const struct Trigger *Scene_GetTriggers(void);
s32 Scene_Initialize(void);
extern struct SceneEntrance gSolSanctumEntrance_FallbackEntrances[];
extern struct SceneEntrance gSolEntrance_Entrances[];
extern struct SceneEntrance gSolSanctumReliefs_Entrances[];
extern u32 gSolSanctumEntrance_Exits[];
extern struct ActorPlacement gSolSanctumEntrance_FallbackActorPlacements[];
extern struct ActorPlacement gSolSanctumReliefs_ActorPlacements[];
extern struct ActorPlacement
    gSolSanctumReliefs_FirstReliefRoomActorPlacements[];
extern struct ActorPlacement
    gSolSanctumReliefs_SecondReliefRoomActorPlacements[];
extern struct ActorPlacement gSolEntrance_ActorPlacements[];
extern struct Trigger gSolSanctumEntrance_FallbackTriggers[];
extern struct Trigger gSolEntrance_Triggers[];
extern struct Trigger gSolSanctumReliefs_Triggers[];
extern struct Trigger gSolSanctumReliefs_FirstReliefRoomTriggers[];
extern struct Trigger gSolSanctumReliefs_SecondReliefRoomTriggers[];
extern s32 gSolSanctumPushSteps[];

#endif
