#ifndef ALCHEMY_UNIT_SCENE_SOL_CORRIDOR_H
#define ALCHEMY_UNIT_SCENE_SOL_CORRIDOR_H

#include "types.h"
#include "field_scene.h"

u8 *SceneData_GetScriptTable(void);
s32 SceneData_ReturnZero(void);
u8 *SceneData_GetMessageTable(void);
u8 *SceneData_GetActorTable(void);
u8 *SceneData_GetEffectTable(void);
extern struct SceneEntrance SolCorridor_SceneTable0[];
extern u32 SolCorridor_SceneTable1[];
extern struct ScenePlacement SolCorridor_SceneTable2[];
extern struct SceneEvent SolCorridor_SceneTable3[];

#endif
