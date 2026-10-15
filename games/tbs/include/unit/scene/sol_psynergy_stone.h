#ifndef ALCHEMY_UNIT_SCENE_SOL_PSYNERGY_STONE_H
#define ALCHEMY_UNIT_SCENE_SOL_PSYNERGY_STONE_H

#include "types.h"
#include "field_scene.h"

u8 *State_Run(void);
s32 SolPsynergyStone_GetView(void);
u8 *SceneData_GetPrimaryTable37c(void);
u8 *SceneData_GetSecondaryTable37c(void);
u8 *SceneData_GetTertiaryTable37c(void);
s32 Resource37c_Run(void);
extern struct SceneEntrance gSolPsynergyStone_SceneTable0[];
extern u32 gSolPsynergyStone_SceneTable1[];
extern struct ScenePlacement gSolPsynergyStone_SceneTable2[];
extern struct SceneEvent gSolPsynergyStone_SceneTable3[];

#endif
