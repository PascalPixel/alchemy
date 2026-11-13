#ifndef ALCHEMY_MODULE_SCENE_SOL_SANCTUM_STATUE_H
#define ALCHEMY_MODULE_SCENE_SOL_SANCTUM_STATUE_H

#include "types.h"
#include "scene_tables.h"

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneExitRegion *Scene_GetExitRegions(void);
const u32 *Scene_GetExits(void);
const struct ActorPlacement *Scene_GetActorPlacements(void);
const struct Trigger *Scene_GetTriggers(void);
s32 Scene_Initialize(void);
extern struct SceneEntrance gSolSanctumStatue_Entrances[];
extern u32 gSolSanctumStatue_Exits[];
extern struct ActorPlacement gSolSanctumStatue_ActorPlacements[];
extern struct Trigger gSolSanctumStatue_Triggers[];
extern s32 gSolSanctumStatue_ScrollPhase;

#endif
