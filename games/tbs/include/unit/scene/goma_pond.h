#ifndef ALCHEMY_UNIT_SCENE_GOMA_POND_H
#define ALCHEMY_UNIT_SCENE_GOMA_POND_H

#include "types.h"
#include "field_scene.h"

u8 *SceneData_GetPrimaryTable(void);
s32 SceneData_ReturnZero(void);
u8 *SceneData_GetSecondaryTable(void);
u8 *SceneData_GetTertiaryTable(void);
u8 *SceneData_GetQuaternaryTable(void);
s32 Scene_PlaceActor8OnGate300(void);
extern struct SceneEntrance gGomaPond_SceneTable0[];
extern u32 gGomaPond_SceneTable1[];
extern struct ScenePlacement gGomaPond_SceneTable2[];
extern struct SceneEvent gGomaPond_SceneTable3[];

#endif
