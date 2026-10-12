#ifndef ALCHEMY_UNIT_SCENE_SORU_DOU_H
#define ALCHEMY_UNIT_SCENE_SORU_DOU_H

#include "types.h"
#include "field_scene.h"

u8 *State_Run(void);
s32 SoruDou_GetView(void);
u8 *SceneData_GetPrimaryTable37c(void);
u8 *SceneData_GetSecondaryTable37c(void);
u8 *SceneData_GetTertiaryTable37c(void);
s32 Resource37c_Run(void);
extern struct SceneEntrance SoruDou_SceneTable0[];
extern u32 SoruDou_SceneTable1[];
extern struct ScenePlacement SoruDou_SceneTable2[];
extern struct SceneEvent SoruDou_SceneTable3[];

#endif
