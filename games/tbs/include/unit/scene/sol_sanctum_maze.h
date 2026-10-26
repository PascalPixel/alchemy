#ifndef ALCHEMY_UNIT_MAP_SOL_SANCTUM_MAZE_H
#define ALCHEMY_UNIT_MAP_SOL_SANCTUM_MAZE_H

#include "types.h"
#include "map_tables.h"

struct SceneExitRegion;

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneExitRegion *Scene_GetExitRegions(void);
const u32 *Scene_GetExits(void);
const struct ObjectEventTemplate *Scene_GetObjectEventTemplates(void);
const struct MapEvent *Scene_GetEvents(void);
extern struct SceneEntrance gSolSanctumMazeEntrances[];
extern u32 gSolSanctumMazeExits[];
extern struct ObjectEventTemplate gSolSanctumMazeObjectEventTemplates[];
extern struct MapEvent gSolSanctumMazeEvents[];

#endif
