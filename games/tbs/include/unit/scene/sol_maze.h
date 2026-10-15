#ifndef ALCHEMY_UNIT_SCENE_SOL_MAZE_H
#define ALCHEMY_UNIT_SCENE_SOL_MAZE_H

#include "types.h"
#include "field_scene.h"

u8 *SceneData_GetScriptTable(void);
s32 SceneData_ReturnZero(void);
u8 *SceneData_GetMessageTable(void);
u8 *SceneData_GetActorTable(void);
u8 *SceneData_GetEffectTable(void);
extern struct SceneEntrance gSolMaze_SceneTable0[];
extern u32 gSolMaze_SceneTable1[];
extern struct ScenePlacement gSolMaze_SceneTable2[];
extern struct SceneEvent gSolMaze_SceneTable3[];

#endif
