#ifndef ALCHEMY_MODULE_MAP_SOL_SANCTUM_MAZE_H
#define ALCHEMY_MODULE_MAP_SOL_SANCTUM_MAZE_H

#include "types.h"
#include "scene_tables.h"

struct SceneExitRegion;

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneExitRegion *Scene_GetExitRegions(void);
const u32 *Scene_GetExits(void);
const struct ActorPlacement *Scene_GetActorPlacements(void);
const struct Trigger *Scene_GetTriggers(void);
extern struct SceneEntrance gSolSanctumMazeEntrances[];
extern u32 gSolSanctumMazeExits[];
extern struct ActorPlacement gSolSanctumMazeActorPlacements[];
extern struct Trigger gSolSanctumMazeTriggers[];

#endif
