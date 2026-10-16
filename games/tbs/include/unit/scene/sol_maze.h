#ifndef ALCHEMY_UNIT_SCENE_SOL_MAZE_H
#define ALCHEMY_UNIT_SCENE_SOL_MAZE_H

#include "types.h"
#include "field_scene.h"

struct SceneRegion;

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneRegion *Scene_GetRegions(void);
const u32 *Scene_GetExits(void);
const struct ScenePlacement *Scene_GetPlacements(void);
const struct SceneEvent *Scene_GetEvents(void);
extern struct SceneEntrance gSolMazeEntrances[];
extern u32 gSolMazeExits[];
extern struct ScenePlacement gSolMazePlacements[];
extern struct SceneEvent gSolMazeEvents[];

#endif
